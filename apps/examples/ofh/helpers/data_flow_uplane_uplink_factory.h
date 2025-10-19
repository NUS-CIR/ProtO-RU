#pragma once

#include "../data_flow_uplane_uplink.h"
#include "../ofh_uplane_fragment_size_calculator.h"
#include "uplink_data_flow_config.h"
#include "srsran/ofh/compression/compression_factory.h"
#include "srsran/ofh/ecpri/ecpri_factories.h"
#include "srsran/ofh/ethernet/ethernet_factories.h"
#include "srsran/ofh/serdes/ofh_serdes_factories.h"

using namespace srsran;
using namespace ofh;

std::shared_ptr<ether::eth_frame_pool> create_eth_frame_pool(const uplink_data_flow_config& tx_config,
                                                                   srslog::basic_logger&     logger)
{
  ether::vlan_frame_params ether_params;
  auto eth_builder   = (tx_config.tci_up || tx_config.tci_cp) ? ether::create_vlan_frame_builder(ether_params)
                                                              : ether::create_frame_builder(ether_params);
  auto ecpri_builder = ecpri::create_ecpri_packet_builder();

  std::array<std::unique_ptr<ofh::iq_compressor>, ofh::NOF_COMPRESSION_TYPES_SUPPORTED> compressors;
  for (unsigned i = 0; i != ofh::NOF_COMPRESSION_TYPES_SUPPORTED; ++i) {
    compressors[i] = create_iq_compressor(ofh::compression_type::none, logger);
  }
  auto compressor_sel = ofh::create_iq_compressor_selector(std::move(compressors));

  std::unique_ptr<uplane_message_builder> uplane_builder =
      (tx_config.is_uplink_static_compr_hdr_enabled)
          ? ofh::create_static_compr_method_ofh_user_plane_packet_builder(logger, *compressor_sel)
          : ofh::create_dynamic_compr_method_ofh_user_plane_packet_builder(logger, *compressor_sel);

  units::bytes headers_size = eth_builder->get_header_size() +
                              ecpri_builder->get_header_size(ecpri::message_type::iq_data) +
                              uplane_builder->get_header_size(tx_config.ul_compr_params);

  unsigned nof_prbs =
      get_max_Nprb(bs_channel_bandwidth_to_MHz(tx_config.ru_working_bw), tx_config.scs, srsran::frequency_range::FR1);

  unsigned nof_frames_per_symbol = ofh_uplane_fragment_size_calculator::calculate_nof_segments(
      tx_config.mtu_size, nof_prbs, tx_config.ul_compr_params, headers_size);

  return std::make_shared<ether::eth_frame_pool>(tx_config.mtu_size, nof_frames_per_symbol);
}

std::unique_ptr<data_flow_uplane_uplink_data>
create_data_flow_uplane(const uplink_data_flow_config&              tx_config,
                             srslog::basic_logger&                  logger,
                             std::shared_ptr<ether::eth_frame_pool> frame_pool)
{
  data_flow_uplane_uplink_data_impl_config config;
  config.ru_nof_prbs =
      get_max_Nprb(bs_channel_bandwidth_to_MHz(tx_config.ru_working_bw), tx_config.scs, srsran::frequency_range::FR1);
  config.sector       = tx_config.sector;
  config.ul_eaxc      = tx_config.ul_eaxc;
  config.compr_params = tx_config.ul_compr_params;
  config.cp           = tx_config.cp;

  ether::vlan_frame_params ether_params;
  ether_params.eth_type        = ether::ECPRI_ETH_TYPE;
  ether_params.tci             = tx_config.tci_up;
  ether_params.mac_dst_address = tx_config.mac_dst_address;
  ether_params.mac_src_address = tx_config.mac_src_address;

  data_flow_uplane_uplink_data_impl_dependencies dependencies;
  dependencies.logger        = &logger;
  dependencies.frame_pool    = std::move(frame_pool);
  dependencies.eth_builder   = (tx_config.tci_up.has_value()) ? ether::create_vlan_frame_builder(ether_params)
                                                              : ether::create_frame_builder(ether_params);
  dependencies.ecpri_builder = ecpri::create_ecpri_packet_builder();

  const unsigned nof_prbs =
      get_max_Nprb(bs_channel_bandwidth_to_MHz(tx_config.bw), tx_config.scs, srsran::frequency_range::FR1);
  const double bw_scaling = 1.0 / (std::sqrt(nof_prbs * NOF_SUBCARRIERS_PER_RB));

  std::array<std::unique_ptr<ofh::iq_compressor>, ofh::NOF_COMPRESSION_TYPES_SUPPORTED> compressors;
  for (unsigned i = 0; i != ofh::NOF_COMPRESSION_TYPES_SUPPORTED; ++i) {
    compressors[i] =
        create_iq_compressor(static_cast<ofh::compression_type>(i), logger, tx_config.iq_scaling * bw_scaling);
  }
  dependencies.compressor_sel = ofh::create_iq_compressor_selector(std::move(compressors));

  dependencies.up_builder =
      (tx_config.is_uplink_static_compr_hdr_enabled)
          ? ofh::create_static_compr_method_ofh_user_plane_packet_builder(logger, *dependencies.compressor_sel)
          : ofh::create_dynamic_compr_method_ofh_user_plane_packet_builder(logger, *dependencies.compressor_sel);

  return std::make_unique<data_flow_uplane_uplink_data_impl>(config, std::move(dependencies));
}