
#pragma once

#include "srsran/phy/lower/lower_phy_timing_notifier.h"
#include "srsran/phy/support/shared_resource_grid.h"
#include "srsran/phy/upper/upper_phy_rx_symbol_handler.h"
#include "srsran/adt/expected.h"
#include "ru_emulator_transceiver.h"
#include "data_flow_uplane_uplink_dispatcher.h"
#include "helpers/uplink_data_flow_config.h"
#include "helpers/data_flow_uplane_uplink_factory.h"
#include "./support/uplink_context_repository.h"
#include <mutex>

using namespace ofh;

namespace srsran {

class ru_rx_symbol_handler : public upper_phy_rx_symbol_handler
{
private:
  srslog::basic_logger&                             logger;
  std::mutex                                        mutex;
  unsigned                                          sector;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC>   ul_eaxc;
  std::optional<tdd_ul_dl_config_common>            tdd_config;
  cyclic_prefix                                     cp;
  unsigned                                          nof_symbols_per_slot;
  ru_emulator_transceiver&                          transceiver;
  std::shared_ptr<uplink_cplane_context_repository> ul_context_repo;
  // Uplink user-plane data flow towards ethernet gateway.
  std::unique_ptr<data_flow_uplane_uplink_data> ul_up_data_flow;
  
  void enqueue_message(const ul_cplane_context& ul_context, const shared_resource_grid& grid){
    const resource_grid_reader& reader = grid.get_reader();

    data_flow_uplane_resource_grid_context context;
    context.slot         = ul_context.radio_hdr.slot;
    context.sector       = sector;
    context.symbol_range = tdd_config
                                      ? get_active_tdd_ul_symbols(tdd_config.value(), context.slot.slot_index(), cp)
                                      : ofdm_symbol_range(0, reader.get_nof_symbols());

    for (unsigned port_id=0, e = reader.get_nof_ports(); port_id != e; port_id++){
      context.port = port_id;
      context.eaxc = ul_eaxc[port_id];
      ul_up_data_flow->enqueue_section_type_1_message(context, grid);
    }
    return;
  }

public:
  ru_rx_symbol_handler(srslog::basic_levels log_level, 
                       ru_emulator_transceiver& transceiver_, 
                       std::shared_ptr<uplink_cplane_context_repository> ul_context_repo_,
                       const uplink_data_flow_config& tx_config,
                       std::shared_ptr<ether::eth_frame_pool> frame_pool,
                       task_executor& executor_) : 
  logger(srslog::fetch_basic_logger("RuRX_Han")),
  sector(tx_config.sector),
  ul_eaxc(tx_config.ul_eaxc),
  tdd_config(tx_config.tdd_config),
  cp(tx_config.cp),
  nof_symbols_per_slot(get_nsymb_per_slot(tx_config.cp)),
  transceiver(transceiver_),
  ul_context_repo(ul_context_repo_)
  {
    logger.set_level(log_level);
    // Create uplink uplane data flow.
    ul_up_data_flow = std::make_unique<data_flow_uplane_uplink_task_dispatcher>(
      create_data_flow_uplane(tx_config, logger, frame_pool), executor_, tx_config.sector);
  }

  void handle_rx_symbol(const upper_phy_rx_symbol_context& context, const shared_resource_grid& grid) override
  {
    fmt::print("111\n");
    std::unique_lock<std::mutex> lock(mutex);
    logger.debug(context.slot.sfn(),
                 context.slot.slot_index(),
                 "Rx symbol {} received for sector {}",
                 context.symbol,
                 context.sector);
    
    try{
      fmt::print("getting ul_context for slot {} symbol {}\n", context.slot, context.symbol);
      const auto& ul_context = ul_context_repo->get(context.slot, context.symbol, context.sector).value();
      if(context.symbol == nof_symbols_per_slot-1){
        //  fmt::print("Handler on slot {}\n", ul_context.radio_hdr.slot);
         enqueue_message(ul_context, grid);
      }
    }
    catch(const tl::bad_expected_access<default_error_t>& e){}
  }  

  void handle_rx_prach_window(const prach_buffer_context& context, const prach_buffer& buffer) override
  {
    std::unique_lock<std::mutex> lock(mutex);
    logger.debug(context.slot.sfn(),
                 context.slot.slot_index(),
                 "PRACH symbol {} received for sector {}",
                 context.start_symbol,
                 context.sector);
  }
};

} // namespace srsran
