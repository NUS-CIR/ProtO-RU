
 #pragma once

 #include "ru_emulator_appconfig.h"
 #include "./support/downlink_context_repository.h"
 #include "./support/prach_context_repository.h"
 #include "srsran/ran/tdd/tdd_ul_dl_config.h"
 #include "srsran/phy/upper/channel_modulation/channel_modulation_factories.h"
 #include "srsran/phy/upper/upper_phy_rg_gateway.h"
 #include "srsran/phy/upper/upper_phy_rx_symbol_request_notifier.h"
 #include "srsran/phy/upper/upper_phy_timing_handler.h"
 #include "srsran/phy/support/support_factories.h"
 #include "srsran/ran/cyclic_prefix.h"
 #include "srsran/ran/pci.h"
 #include "srsran/ran/ssb_mapping.h"
 #include "srsran/srslog/logger.h"
 #include <memory>
 #include <string>
 
 namespace srsran {
 
 /// \brief A fake upper_phy for lower_phy support
 ///
 /// It shows how to use the upper PHY processor interfaces. It implements the upper PHY timing interface.
 class upper_phy_fake : public upper_phy_timing_handler
 {
 public:
   /// \brief Waits for the TTI boundary.
   ///
   /// Blocks the thread execution upon the notification of a TTI boundary. This is used for throttling the execution in
   /// a TTI basis from an external thread.
   virtual void wait_tti_boundary() = 0;
 
   /// Stops the upper PHY execution.
   virtual void stop() = 0;
 
   /// Collects upper PHY sample configuration parameters.
   struct configuration {
     /// General upper PHY logging level.
     srslog::basic_levels log_level;
     /// Specifies the maximum number of PRBs.
     unsigned max_nof_prb;
     /// Specifies the maximum number of antenna ports.
     unsigned max_nof_ports;
     /// Specifies the resource grid pool size.
     unsigned rg_pool_size;
     /// Specifies the number of PRACH buffers.
     unsigned nof_prach_buffer;
     /// Specifies the LDPC encoder type.
     std::string ldpc_encoder_type;
     /// Resource grid gateway.
     upper_phy_rg_gateway* gateway;
     /// Uplink request processor.
     upper_phy_rx_symbol_request_notifier* rx_symb_req_notifier = nullptr;
     /// Downlink resource grid repo.
     std::shared_ptr<srsran::ofh::downlink_context_repository> dl_slot_repo;
     /// PRACH context repo.
     std::shared_ptr<srsran::ofh::prach_context_repository> prach_context_repo;
     /// Enable uplink processing.
     bool enable_ul_processing = true;
     /// Enable PRACH processing.
     bool enable_prach_processing = true;
   };

   /// Creates an upper PHY sample.
   static std::unique_ptr<upper_phy_fake> create(const configuration& config);
 };

  /// Creates upper PHY configuration.
  inline upper_phy_fake::configuration 
  generate_upper_part_configuraion(const ru_emulator_ofh_appconfig& cfg)
  {
    static unsigned int bw_rb = 
      band_helper::get_n_rbs_from_bw(cfg.bandwidth, cfg.common_scs, band_helper::get_freq_range(cfg.band.value()));
    // fmt::print("bw = {}, scs = {}, bw_rb = {}, fr = {}\n", bs_channel_bandwidth_to_MHz(cfg.bandwidth), to_numerology_value(cfg.common_scs), bw_rb, to_string(band_helper::get_freq_range(cfg.band.value())));
    
    ///TODO: make this parameter configurable.
    static unsigned nof_ports = std::max(cfg.nof_antennas_dl, cfg.nof_antennas_ul); 
    static unsigned prach_pipeline_depth = 1;
    static srslog::basic_levels log_level = srslog::basic_levels::warning;

    upper_phy_fake::configuration config;
    config.log_level                    = log_level;
    config.max_nof_prb                  = bw_rb;
    config.max_nof_ports                = nof_ports;
    config.rg_pool_size                 = 1024 * cfg.max_proc_delay;
    config.nof_prach_buffer             = prach_pipeline_depth * get_nof_slots_per_subframe(cfg.common_scs);
    config.ldpc_encoder_type            = "generic";

    return config;
  }

 inline std::unique_ptr<resource_grid_pool> 
 create_rg_pool(const upper_phy_fake::configuration& config, srslog::basic_logger& logger)
 {
  std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
  // Determine the number of subcarriers of the resource grid.
  unsigned nof_subcs = config.max_nof_prb * NRE;
  // Create DL and UL resource grid pool configuration.
  std::unique_ptr<resource_grid_pool> rg_pool = nullptr;
  unsigned                                    nof_sectors = 1;
  unsigned                                    nof_slots   = config.rg_pool_size;
  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.reserve(nof_sectors * nof_slots);

  for (unsigned sector_id = 0; sector_id != nof_sectors; ++sector_id) {
    for (unsigned slot_id = 0; slot_id != nof_slots; ++slot_id) {
      grids.push_back(rg_factory->create(config.max_nof_ports, MAX_NSYMB_PER_SLOT, nof_subcs));
    }
  }
  rg_pool = create_generic_resource_grid_pool(std::move(grids));
  
  return rg_pool;
 }

 inline  std::unique_ptr<prach_buffer_pool>  
 create_prach_pool(const upper_phy_fake::configuration& config)
 {
  std::vector<std::unique_ptr<prach_buffer>> prach_mem;
  prach_mem.reserve(config.nof_prach_buffer);

  for (unsigned i = 0, e = config.nof_prach_buffer; i != e; ++i) {
    std::unique_ptr<prach_buffer> buffer;
    ///TODO: make the parameters configurable
    // buffer = create_prach_buffer_short(
    //     config.nof_rx_ports, config.max_nof_td_prach_occasions, config.max_nof_fd_prach_occasions);
    buffer = create_prach_buffer_short(config.max_nof_ports, 1, 1);
    report_fatal_error_if_not(buffer, "Invalid PRACH buffer.");
    prach_mem.push_back(std::move(buffer));
  }

  return create_prach_buffer_pool(std::move(prach_mem));
 }


} // namespace srsran
 