// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_sector.h"
#include "../receiver/ofh_sequence_id_checker_impl.h"
#include "../transmitter/ofh_data_flow_uplane_data_impl.h"
#include "ofh_ru_ota_symbol_task_dispatcher.h"
#include "ofh_ru_uplink_task_dispatcher.h"
#include "ocudu/ofh/compression/compression_factory.h"
#include "ocudu/ofh/ecpri/ecpri_factories.h"
#include "ocudu/ofh/ethernet/ethernet_factories.h"
#include "ocudu/ofh/serdes/ofh_serdes_factories.h"
#include "ocudu/ran/slot_point.h"

using namespace ocudu;
using namespace ofh;

ru_sector_impl::ru_sector_impl(std::shared_ptr<rx_grid_context_repository>      grid_repo_,
                               std::shared_ptr<ether::eth_frame_pool>           uplink_uplane_pool_,
                               std::shared_ptr<ether::eth_frame_pool>           prach_pool_,
                               std::unique_ptr<data_flow_uplane_data>           uplink_data_flow_,
                               std::unique_ptr<data_flow_uplane_data>           prach_data_flow_,
                               std::unique_ptr<ru_cplane_scheduling_handler>    scheduling_handler_,
                               std::unique_ptr<ru_cplane_scheduling_dispatcher> cplane_dispatcher_,
                               std::unique_ptr<ru_rx_uplane_data_flow>          rx_uplane_data_flow_,
                               std::unique_ptr<ru_rx_cplane_data_flow>          rx_cplane_data_flow_,
                               std::unique_ptr<sequence_id_checker>             uplane_seq_id_checker_,
                               std::unique_ptr<sequence_id_checker>             cplane_dl_seq_id_checker_,
                               std::unique_ptr<sequence_id_checker>             cplane_ul_seq_id_checker_,
                               std::unique_ptr<rx_window_checker>               window_checker_,
                               std::unique_ptr<ru_downlink_rx_window_handler>   dl_rx_window_handler_,
                               std::unique_ptr<ru_message_receiver>             msg_receiver_,
                               std::unique_ptr<ru_message_transmitter>          msg_transmitter_,
                               std::unique_ptr<ru_ota_symbol_task_dispatcher>   msg_tx_dispatcher_,
                               ru_uplink_response_components                    uplink_components_) :
  grid_repo(std::move(grid_repo_)),
  uplink_uplane_pool(std::move(uplink_uplane_pool_)),
  prach_pool(std::move(prach_pool_)),
  uplink_data_flow(std::move(uplink_data_flow_)),
  prach_data_flow(std::move(prach_data_flow_)),
  scheduling_handler(std::move(scheduling_handler_)),
  cplane_dispatcher(std::move(cplane_dispatcher_)),
  rx_uplane_data_flow(std::move(rx_uplane_data_flow_)),
  rx_cplane_data_flow(std::move(rx_cplane_data_flow_)),
  uplane_seq_id_checker(std::move(uplane_seq_id_checker_)),
  cplane_dl_seq_id_checker(std::move(cplane_dl_seq_id_checker_)),
  cplane_ul_seq_id_checker(std::move(cplane_ul_seq_id_checker_)),
  window_checker(std::move(window_checker_)),
  dl_rx_window_handler(std::move(dl_rx_window_handler_)),
  msg_receiver(std::move(msg_receiver_)),
  msg_transmitter(std::move(msg_transmitter_)),
  msg_tx_dispatcher(std::move(msg_tx_dispatcher_)),
  uplink_components(std::move(uplink_components_)),
  ota_notifiers{msg_tx_dispatcher ? static_cast<ota_symbol_boundary_notifier*>(msg_tx_dispatcher.get())
                                  : static_cast<ota_symbol_boundary_notifier*>(msg_transmitter.get()),
                window_checker.get(),
                dl_rx_window_handler.get()}
{
}

namespace {

/// Builds an uplink/PRACH transmit data flow that enqueues into the given frame pool with the given compression,
/// labelling traces with \c eaxc.
std::unique_ptr<data_flow_uplane_data>
create_uplink_data_flow(const ru_sector_config&                                config,
                        ocudulog::basic_logger&                                logger,
                        std::shared_ptr<ether::eth_frame_pool>                 pool,
                        const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC>& eaxc,
                        const ru_compression_params&                           compr_params)
{
  auto compressor = create_iq_compressor(compr_params.type, logger, config.iq_scaling);
  auto up_builder = config.is_ul_static_compr_hdr
                        ? create_static_compr_method_ofh_user_plane_packet_builder(logger, *compressor)
                        : create_dynamic_compr_method_ofh_user_plane_packet_builder(logger, *compressor);

  data_flow_uplane_data_impl_dependencies deps;
  deps.logger         = &logger;
  deps.frame_pool     = std::move(pool);
  deps.compressor_sel = std::move(compressor);
  // Without a VLAN configuration the frames go out untagged, leaving the tagging to the interface (an SR-IOV VF port
  // VLAN, for instance) instead of inserting a second 802.1Q header on top of it.
  deps.eth_builder    = config.vlan_params.vlan_config ? ether::create_vlan_frame_builder(config.vlan_params)
                                                       : ether::create_frame_builder(config.vlan_params);
  deps.ecpri_builder  = ecpri::create_ecpri_packet_builder();
  deps.up_builder     = std::move(up_builder);

  data_flow_uplane_data_impl_config cfg;
  cfg.sector       = config.sector;
  cfg.cp           = config.cp;
  cfg.ru_nof_prbs  = config.ru_nof_prbs;
  cfg.dl_eaxc      = eaxc;
  cfg.compr_params = compr_params;
  cfg.direction    = data_direction::uplink;

  return std::make_unique<data_flow_uplane_data_impl>(cfg, std::move(deps));
}

} // namespace

std::unique_ptr<ru_sector> ofh::create_ru_sector(const ru_sector_config& config, ru_sector_dependencies&& dependencies)
{
  ocudu_assert(dependencies.logger, "Invalid logger");
  ocudu_assert(dependencies.upper_phy, "Invalid upper PHY");
  ocudu_assert(dependencies.eth_transmitter, "Invalid Ethernet transmitter");

  ocudulog::basic_logger& logger = *dependencies.logger;

  // Received-grid repository, sized to hold a couple of frames worth of slots.
  unsigned repo_size = NOF_SUBFRAMES_PER_FRAME * get_nof_slots_per_subframe(config.scs) * 2;
  auto     grid_repo = std::make_shared<rx_grid_context_repository>(repo_size);

  // Transmit frame pools: regular uplink User-Plane and PRACH (kept separate).
  auto uplink_uplane_pool = std::make_shared<ether::eth_frame_pool>(
      logger, config.mtu_size, config.nof_frames_per_symbol, message_type::user_plane, data_direction::uplink);
  auto prach_pool = std::make_shared<ether::eth_frame_pool>(
      logger, config.mtu_size, config.nof_frames_per_symbol, message_type::uplane_prach, data_direction::uplink);

  // Transmit data flows (uplink User-Plane and PRACH, each labelled with its own eAxCs).
  auto uplink_data_flow =
      create_uplink_data_flow(config, logger, uplink_uplane_pool, config.ul_eaxc, config.ul_compr_params);
  // When an OFH encode executor is provided, decouple the uplink build + compression onto it (the captured/test IQ is
  // handed off and the calling baseband / immediate-response thread is freed), mirroring the O-DU's downlink.
  if (dependencies.uplink_encode_executor != nullptr) {
    uplink_data_flow = std::make_unique<ru_uplink_task_dispatcher>(
        logger, std::move(uplink_data_flow), *dependencies.uplink_encode_executor, config.sector);
  }
  auto prach_data_flow =
      create_uplink_data_flow(config, logger, prach_pool, config.prach_eaxc, config.prach_compr_params);

  // Control-Plane scheduling handler (references the transmit data flows, the grid repository and the upper PHY).
  ru_cplane_scheduling_handler_config       handler_cfg{config.sector};
  ru_cplane_scheduling_handler_dependencies handler_deps;
  handler_deps.logger           = &logger;
  handler_deps.upper_phy        = dependencies.upper_phy;
  handler_deps.grid_repo        = grid_repo;
  handler_deps.uplink_data_flow = uplink_data_flow.get();
  handler_deps.prach_data_flow  = prach_data_flow.get();
  auto handler                  = std::make_unique<ru_cplane_scheduling_handler>(handler_cfg, handler_deps);

  // Store-and-respond uplink components, built only in that mode. In immediate mode the handler answers uplink/PRACH
  // directly; in store-and-respond mode the recorder records the request and the responders reply when captured IQ is
  // pushed into the sector's uplink IQ sink.
  ru_uplink_response_components uplink_components;
  ru_uplink_scheduling_handler* uplink_handler = handler.get();
  if (config.uplink_response_mode == ru_uplink_response_mode::store_and_respond) {
    uplink_components.ul_request_repo    = std::make_shared<ru_uplink_request_repository>(repo_size);
    uplink_components.prach_request_repo = std::make_shared<ru_uplink_request_repository>(repo_size);
    ru_uplink_scheduling_recorder_config recorder_cfg{config.sector, config.scs, config.ru_nof_prbs};
    uplink_components.recorder = std::make_unique<ru_uplink_scheduling_recorder>(
        recorder_cfg, logger, uplink_components.ul_request_repo, uplink_components.prach_request_repo);

    ru_uplink_symbol_responder_config ul_resp_cfg{config.sector, config.ul_eaxc};
    uplink_components.ul_responder = std::make_unique<ru_uplink_symbol_responder>(
        ul_resp_cfg, logger, uplink_components.ul_request_repo, *uplink_data_flow);

    ru_prach_window_responder_config prach_resp_cfg{config.sector, config.prach_eaxc};
    uplink_components.prach_responder = std::make_unique<ru_prach_window_responder>(
        prach_resp_cfg, logger, uplink_components.prach_request_repo, *prach_data_flow);

    uplink_handler = uplink_components.recorder.get();
  }

  // Dispatcher routes received Control-Plane to the handlers, dropping commands for eAxCs not configured for the path.
  // Downlink is always handled by the scheduling handler; uplink/PRACH by the handler (immediate) or the recorder.
  ru_cplane_scheduling_dispatcher_config disp_cfg;
  disp_cfg.sector     = config.sector;
  disp_cfg.dl_eaxc    = config.dl_eaxc;
  disp_cfg.ul_eaxc    = config.ul_eaxc;
  disp_cfg.prach_eaxc = config.prach_eaxc;
  auto dispatcher     = std::make_unique<ru_cplane_scheduling_dispatcher>(disp_cfg, logger, *handler, *uplink_handler);

  // Receive Control-Plane data flow notifies the dispatcher.
  auto cplane_decoder = create_ofh_control_plane_message_decoder(logger, config.scs, config.sector);
  auto rx_cplane      = std::make_unique<ru_rx_cplane_data_flow>(logger, std::move(cplane_decoder), *dispatcher);

  // Receive User-Plane data flow writes the received downlink into the grid repository.
  auto                          decompressor   = create_iq_decompressor(config.dl_compr_params.type, logger);
  auto                          uplane_decoder = config.is_dl_static_compr_hdr
                                                     ? create_static_compr_method_ofh_user_plane_packet_decoder(logger,
                                                                                       config.scs,
                                                                                       config.cp,
                                                                                       config.ru_nof_prbs,
                                                                                       config.sector,
                                                                                       std::move(decompressor),
                                                                                       config.dl_compr_params,
                                                                                       data_direction::downlink)
                                                     : create_dynamic_compr_method_ofh_user_plane_packet_decoder(logger,
                                                                                        config.scs,
                                                                                        config.cp,
                                                                                        config.ru_nof_prbs,
                                                                                        config.sector,
                                                                                        std::move(decompressor),
                                                                                        data_direction::downlink);
  ru_rx_uplane_data_flow_config rx_up_cfg;
  rx_up_cfg.sector = config.sector;
  rx_up_cfg.eaxc   = config.dl_eaxc;
  ru_rx_uplane_data_flow_dependencies rx_up_deps;
  rx_up_deps.logger         = &logger;
  rx_up_deps.grid_repo      = grid_repo;
  rx_up_deps.uplane_decoder = std::move(uplane_decoder);
  auto rx_uplane            = std::make_unique<ru_rx_uplane_data_flow>(rx_up_cfg, std::move(rx_up_deps));

  // Sequence-identifier and reception-window checkers (DU + RU symmetric reception checking). Downlink and uplink
  // Control-Plane are independent sequence streams at the O-DU, so each direction gets its own checker.
  auto uplane_seq_id_checker    = std::make_unique<sequence_id_checker_impl>();
  auto cplane_dl_seq_id_checker = std::make_unique<sequence_id_checker_impl>();
  auto cplane_ul_seq_id_checker = std::make_unique<sequence_id_checker_impl>();
  auto window_checker           = std::make_unique<rx_window_checker>(true, config.rx_window);

  // Downlink reception window handler: drains the grid repository each symbol and finalises closed downlink grids.
  ru_downlink_rx_window_handler_config dl_rx_cfg;
  dl_rx_cfg.sector                  = config.sector;
  dl_rx_cfg.finalize_offset_symbols = config.dl_grid_finalize_offset_symbols;
  auto dl_rx_window_handler =
      std::make_unique<ru_downlink_rx_window_handler>(dl_rx_cfg, *dependencies.upper_phy, grid_repo, logger);

  // Message receiver dispatches received frames to the receive data flows, checking sequence id and reception window.
  ru_message_receiver_config mr_cfg;
  mr_cfg.sector               = config.sector;
  mr_cfg.scs                  = config.scs;
  mr_cfg.nof_symbols_per_slot = get_nsymb_per_slot(config.cp);
  // Expected Ethernet parameters of received frames: the reverse of the sector's transmit parameters (the O-DU is the
  // source and this O-RU the destination).
  mr_cfg.vlan_params                 = config.vlan_params;
  mr_cfg.vlan_params.mac_dst_address = config.vlan_params.mac_src_address;
  mr_cfg.vlan_params.mac_src_address = config.vlan_params.mac_dst_address;
  mr_cfg.dl_eaxc                     = config.dl_eaxc;
  ru_message_receiver_dependencies mr_deps;
  mr_deps.logger                   = &logger;
  mr_deps.eth_decoder              = ether::create_vlan_frame_decoder(logger, config.sector);
  mr_deps.ecpri_decoder            = ecpri::create_ecpri_packet_decoder_using_payload_size(logger, config.sector);
  mr_deps.uplane_handler           = rx_uplane.get();
  mr_deps.cplane_handler           = rx_cplane.get();
  mr_deps.uplane_seq_id_checker    = uplane_seq_id_checker.get();
  mr_deps.cplane_dl_seq_id_checker = cplane_dl_seq_id_checker.get();
  mr_deps.cplane_ul_seq_id_checker = cplane_ul_seq_id_checker.get();
  mr_deps.window_checker           = window_checker.get();
  auto msg_receiver                = std::make_unique<ru_message_receiver>(mr_cfg, std::move(mr_deps));

  // Message transmitter drains the transmit pools to the Ethernet transmitter on each OTA symbol.
  ru_message_transmitter_config mt_cfg{config.sector, config.tx_window_start_symbols, config.tx_window_end_symbols};
  ru_message_transmitter_dependencies mt_deps;
  mt_deps.logger             = &logger;
  mt_deps.eth_transmitter    = std::move(dependencies.eth_transmitter);
  mt_deps.uplink_uplane_pool = uplink_uplane_pool;
  mt_deps.prach_pool         = prach_pool;
  auto msg_transmitter       = std::make_unique<ru_message_transmitter>(mt_cfg, std::move(mt_deps));

  // When an OFH transmit executor is provided, decouple the message transmitter's per-symbol Ethernet drain + send onto
  // it, so the timing thread only ticks (the window checker and downlink reception window handler stay on the timing
  // thread). This mirrors the O-DU's transmitter_ota_symbol_task_dispatcher.
  std::unique_ptr<ru_ota_symbol_task_dispatcher> msg_tx_dispatcher;
  if (dependencies.uplink_encode_executor != nullptr) {
    msg_tx_dispatcher = std::make_unique<ru_ota_symbol_task_dispatcher>(
        config.sector, logger, *dependencies.uplink_encode_executor, *msg_transmitter);
  }

  // The message transmitter (or its dispatcher) and the reception-window checker are subscribed to the application's
  // timing source through ru_sector::get_ota_symbol_boundary_notifiers().

  return std::make_unique<ru_sector_impl>(std::move(grid_repo),
                                          std::move(uplink_uplane_pool),
                                          std::move(prach_pool),
                                          std::move(uplink_data_flow),
                                          std::move(prach_data_flow),
                                          std::move(handler),
                                          std::move(dispatcher),
                                          std::move(rx_uplane),
                                          std::move(rx_cplane),
                                          std::move(uplane_seq_id_checker),
                                          std::move(cplane_dl_seq_id_checker),
                                          std::move(cplane_ul_seq_id_checker),
                                          std::move(window_checker),
                                          std::move(dl_rx_window_handler),
                                          std::move(msg_receiver),
                                          std::move(msg_transmitter),
                                          std::move(msg_tx_dispatcher),
                                          std::move(uplink_components));
}
