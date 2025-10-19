
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

// using namespace ofh;

namespace srsran {

class ru_rx_symbol_handler : public upper_phy_rx_symbol_handler
{
private:
  srslog::basic_logger&                             logger;
  std::mutex                                        mutex;
  unsigned                                          sector;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC>   ul_eaxc;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC>   prach_eaxc;
  cyclic_prefix                                     cp;
  unsigned                                          nof_symbols_per_slot;
  std::shared_ptr<uplink_cplane_context_repository> ul_context_repo;
  std::shared_ptr<uplink_cplane_context_repository> prach_context_repo;
  // Uplink user-plane data flow towards ethernet gateway.
  std::unique_ptr<data_flow_uplane_uplink_data> ul_up_data_flow;
  
  void enqueue_section_type1_message(const ul_cplane_context& ul_context, 
                                     const shared_resource_grid& grid, 
                                     unsigned symbol, 
                                     unsigned port_id)
  {
    const resource_grid_reader& reader = grid.get_reader();

    data_flow_uplane_rg_context context;
    context.slot         = ul_context.radio_hdr.slot;
    context.sector       = sector;
    context.symbol_range = ofdm_symbol_range(ul_context.radio_hdr.start_symbol, reader.get_nof_symbols());
    context.symbol_id    = symbol;
    context.section_id   = ul_context.section_id;
    logger.debug("Enqueueing message for slot {}, symbol [{}, {}]\n", ul_context.radio_hdr.slot, context.symbol_range.start(), context.symbol_range.stop());
    
    context.port = port_id;
    context.eaxc = ul_eaxc[port_id];
    ul_up_data_flow->enqueue_section_type_1_message(context, grid, symbol);
    return;
  }

  void enqueue_section_type1_prach_message(const ul_cplane_context& prach_context, const prach_buffer& buffer, unsigned port_id){
    data_flow_uplane_prach_context context;
    context.slot                       = prach_context.radio_hdr.slot;
    context.start_symbol               = prach_context.radio_hdr.start_symbol;
    context.nof_symbols                = prach_context.nof_symbols;
    context.sector                     = sector;
    context.prb_start                  = prach_context.prb_start;
    context.nof_prb                    = prach_context.nof_prb;
    context.section_id                 = prach_context.section_id;
    logger.debug("Enqueueing message for slot {}, starting symbol {}, total {} symbols\n", prach_context.radio_hdr.slot, 
                    context.start_symbol, context.nof_symbols);
      context.port = port_id;
      context.eaxc = prach_eaxc[port_id];
      ul_up_data_flow->enqueue_prach_message(context, buffer);
  }

public:
  ru_rx_symbol_handler(srslog::basic_levels log_level, 
                       std::shared_ptr<uplink_cplane_context_repository> ul_context_repo_,
                       std::shared_ptr<uplink_cplane_context_repository> prach_context_repo_,
                       const uplink_data_flow_config& tx_config,
                       std::shared_ptr<ether::eth_frame_pool> frame_pool,
                       task_executor& executor_) : 
  logger(srslog::fetch_basic_logger("RuRX_Han")),
  sector(tx_config.sector),
  ul_eaxc(tx_config.ul_eaxc),
  prach_eaxc(tx_config.prach_eaxc),
  cp(tx_config.cp),
  nof_symbols_per_slot(get_nsymb_per_slot(tx_config.cp)),
  ul_context_repo(ul_context_repo_),
  prach_context_repo(prach_context_repo_)
  {
    logger.set_level(log_level);
    // Create uplink uplane data flow.
    ul_up_data_flow = std::make_unique<data_flow_uplane_uplink_task_dispatcher>(
      create_data_flow_uplane(tx_config, logger, frame_pool), executor_, tx_config.sector);
  }

  void handle_rx_symbol(const upper_phy_rx_symbol_context& context, const shared_resource_grid& grid) override
  {
    std::unique_lock<std::mutex> lock(mutex);
    
    const resource_grid_reader& reader = grid.get_reader();
    for (unsigned port_id=0, e = reader.get_nof_ports(); port_id != e; port_id++){
      unsigned eaxc = ul_eaxc[port_id];
      const auto ul_context = ul_context_repo->get(context.slot, context.symbol, eaxc);
      if(ul_context.has_value()){
        enqueue_section_type1_message(ul_context.value(), grid, context.symbol, port_id);
        logger.debug(context.slot.sfn(),
                     context.slot.slot_index(),
                     "Rx symbol {} received for sector {}",
                     context.symbol,
                     context.sector);
      }
      else continue;
    }
  }  

  void handle_rx_prach_window(const prach_buffer_context& context, const prach_buffer& buffer) override
  {
    std::unique_lock<std::mutex> lock(mutex);
    for (unsigned port_id=0, e = buffer.get_max_nof_ports(); port_id != e; port_id++){
      unsigned eaxc = prach_eaxc[port_id];
      const auto prach_context = prach_context_repo->get(context.slot, context.start_symbol, eaxc);
      if(prach_context.has_value()){
        enqueue_section_type1_prach_message(prach_context.value(), buffer, port_id);
        logger.debug(context.slot.sfn(),
                     context.slot.slot_index(),
                     "PRACH symbol {} received for sector {}",
                     context.start_symbol,
                     context.sector);
      }
      else continue;
    }
  }
};

} // namespace srsran
