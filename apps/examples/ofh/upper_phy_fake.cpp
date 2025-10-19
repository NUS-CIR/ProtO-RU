 #include "upper_phy_fake.h"
 #include "srsran/phy/support/prach_buffer.h"
 #include "srsran/phy/support/prach_buffer_context.h"
 #include "srsran/phy/support/resource_grid_mapper.h"
 #include "srsran/phy/support/resource_grid_reader.h"
 #include "srsran/phy/support/resource_grid_writer.h"
 #include "srsran/phy/support/shared_resource_grid.h"
 #include "srsran/phy/support/support_factories.h"
 #include "srsran/phy/upper/channel_processors/channel_processor_factories.h"
 #include "srsran/phy/upper/channel_processors/ssb_processor.h"
 #include "srsran/ran/precoding/precoding_codebooks.h"
 #include "srsran/srsvec/bit.h"
 #include "srsran/adt/gps_clock.h"
 #include <condition_variable>
 #include <mutex>
 #include <random>
 
 using namespace srsran;
 using namespace ofh;
 
 namespace {
 
 class upper_phy_fake_sw : public upper_phy_fake
 {
 private:
   srslog::basic_logger&                 logger;
   std::mutex                            mutex;
   std::condition_variable               cvar_tti_boundary;
   bool                                  tti_boundary = false;
   bool                                  quit         = false;
   std::shared_ptr<downlink_context_repository> dl_cp_repo;
   std::shared_ptr<prach_context_repository> prach_context_repo;
   std::unique_ptr<resource_grid_pool>   ul_rg_pool;
   std::unique_ptr<resource_grid_pool>   dl_rg_pool;
   std::vector<shared_resource_grid>     current_grids;
   upper_phy_rg_gateway*                 gateway;
   upper_phy_rx_symbol_request_notifier* rx_symb_req_notifier;
   bool                                  enable_ul_processing;
   bool                                  enable_prach_processing;
   unsigned                              nof_subcs;
   unsigned                              nof_ports;
 
   // Pseudo-random data and symbol buffers.
   static constexpr unsigned MAX_NRE_PER_SLOT = MAX_NSYMB_PER_SLOT * MAX_RB * NRE;
   static_bit_buffer<MAX_NRE_PER_SLOT * MODULATION_MAX_BITS_PER_SYMBOL> data;
   static_re_buffer<1, MAX_NRE_PER_SLOT>                                data_symbols;
 
 public:
   upper_phy_fake_sw(srslog::basic_logger&                 logger_,
                        std::shared_ptr<downlink_context_repository>   dl_cp_repo_,
                        std::shared_ptr<prach_context_repository>  prach_context_repo_,
                        std::unique_ptr<resource_grid_pool>   ul_rg_pool_,
                        std::unique_ptr<resource_grid_pool>   dl_rg_pool_,
                        upper_phy_rg_gateway*                 gateway_,
                        upper_phy_rx_symbol_request_notifier* rx_symb_req_notifier_,
                        bool                                  enable_ul_processing_,
                        bool                                  enable_prach_processing_,
                        unsigned                              nof_subcs_,
                        unsigned                              nof_ports_
                        ) :
     logger(logger_),
     dl_cp_repo(std::move(dl_cp_repo_)),
     prach_context_repo(std::move(prach_context_repo_)),
     ul_rg_pool(std::move(ul_rg_pool_)),
     dl_rg_pool(std::move(dl_rg_pool_)),
     gateway(gateway_),
     rx_symb_req_notifier(rx_symb_req_notifier_),
     enable_ul_processing(enable_ul_processing_),
     enable_prach_processing(enable_prach_processing_),
     nof_subcs(nof_subcs_),
     nof_ports(nof_ports_)
   {
     
     srsran_assert(dl_cp_repo, "Invalid DL RG pool.");
     srsran_assert(ul_rg_pool, "Invalid UL RG pool.");
     srsran_assert(dl_rg_pool, "Invalid UL RG pool.");
     srsran_assert(gateway, "Invalid RG gateway.");
     srsran_assert(rx_symb_req_notifier, "Invalid receive symbol request notifier.");
     srsran_assert(nof_subcs != 0, "Number of OFDM subcarriers cannot be zero.");
     srsran_assert(nof_ports_ != 0, "Number of antenna ports cannot be zero.");
   }

   void handle_tti_boundary(const upper_phy_timing_context& context) override
   {
    // fmt::print("tti : slot {}\n", context.slot);
     std::unique_lock<std::mutex> lock(mutex);
     srsran_assert(gateway, "Upper PHY is not connected to a gateway.");
 
     // Set logger context.
     logger.set_context(context.slot.sfn(), context.slot.slot_index());
    //  logger.warning("New TTI boundary : {}", context.slot);
 
     // Wait for TTI boundary to be cleared.
     cvar_tti_boundary.wait(lock, [this]() { return ((!tti_boundary) || quit); });
     
     // Request RX symbol if UL processing is enabled.
     if (enable_ul_processing) {
       resource_grid_context rx_symb_context;
       rx_symb_context.sector = 0;
       rx_symb_context.slot   = context.slot;
 
       // Try to allocate a resource grid.
       shared_resource_grid rg = ul_rg_pool->allocate_resource_grid(context.slot);
 
       // If the resource grid allocation fails, it aborts the slot request.
       if (rg) {
         rx_symb_req_notifier->on_uplink_slot_request(rx_symb_context, rg);
       }
     }
 
     // Request PRACH capture if PRACH processing is enabled.
     if (enable_prach_processing) {
       const auto ctx = prach_context_repo->pop_prach_buffer(context.slot);
       if(ctx.has_value()){
        prach_buffer_context prach_context = ctx.value().context;
        rx_symb_req_notifier->on_prach_capture_request(prach_context, *ctx.value().buffer);
       }
     }
     // Get a resource grid from the transmission rg pool.
     shared_resource_grid dl_rg = dl_rg_pool->allocate_resource_grid(context.slot);
  
     auto merged_rg = dl_cp_repo->pop_and_merge_slot_resource_grid(context.slot, dl_rg);
     // Abort slot processing if the grid is not valid.
     if (!merged_rg) {
       logger.warning("Invalid resource grid for slot {}.", context.slot);
       // Raise TTI boundary and notify.
       tti_boundary = true;
       cvar_tti_boundary.notify_all();
      return;
    }

    resource_grid_context rg_context;
    rg_context.sector = 0;
    rg_context.slot   = context.slot;
    gateway->send(rg_context, std::move(merged_rg));
    
    // Raise TTI boundary and notify.
    tti_boundary = true;
    cvar_tti_boundary.notify_all();
   }
 
   void handle_ul_half_slot_boundary(const upper_phy_timing_context& context) override
   {
     logger.debug("UL half slot boundary.");
   }
 
   void handle_ul_full_slot_boundary(const upper_phy_timing_context& context) override
   {
     logger.debug("UL full slot boundary.");
   }
 
   void wait_tti_boundary() override
   {
     std::unique_lock<std::mutex> lock(mutex);
 
     // Wait for TTI boundary to be raised.
     cvar_tti_boundary.wait(lock, [this]() { return (tti_boundary || quit); });
 
     // Clear TTI boundary and notify
     tti_boundary = false;
     cvar_tti_boundary.notify_all();
   }
 
   void stop() override
   {
     std::unique_lock<std::mutex> lock(mutex);
     quit = true;
     cvar_tti_boundary.notify_all();
   }
 };
 
 } // namespace
 
 #define ASSERT_FACTORY(FACTORY_PTR)                                                                                    \
   do {                                                                                                                 \
     if (!FACTORY_PTR) {                                                                                                \
       logger.error("{}:{}: {}: Error creating {}.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #FACTORY_PTR);            \
       return nullptr;                                                                                                  \
     }                                                                                                                  \
   } while (false)

 std::unique_ptr<upper_phy_fake> srsran::upper_phy_fake::create(const configuration& config)
 {
   srslog::basic_logger& logger = srslog::fetch_basic_logger("UpperPHY_fake", false);
   logger.set_level(config.log_level);
  
   std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
   ASSERT_FACTORY(rg_factory);
 
   // Determine the number of subcarriers of the resource grid.
   unsigned nof_subcs = config.max_nof_prb * NRE;
   
  // Create DL and UL resource grid pool configuration.
  std::unique_ptr<resource_grid_pool> dl_rg_pool = nullptr;
  std::unique_ptr<resource_grid_pool> ul_rg_pool = nullptr;
  {
    unsigned                                    nof_sectors = 1;
    unsigned                                    nof_slots   = config.rg_pool_size;
    std::vector<std::unique_ptr<resource_grid>> dl_grids;
    std::vector<std::unique_ptr<resource_grid>> ul_grids;
    dl_grids.reserve(nof_sectors * nof_slots);
    ul_grids.reserve(nof_sectors * nof_slots);

    for (unsigned sector_id = 0; sector_id != nof_sectors; ++sector_id) {
      for (unsigned slot_id = 0; slot_id != nof_slots; ++slot_id) {
        dl_grids.push_back(rg_factory->create(config.max_nof_ports, MAX_NSYMB_PER_SLOT, nof_subcs));
        ASSERT_FACTORY(dl_grids.back());
        ul_grids.push_back(rg_factory->create(config.max_nof_ports, MAX_NSYMB_PER_SLOT, nof_subcs));
        ASSERT_FACTORY(ul_grids.back());
      }
    }

    // Create DL resource grid pool.
    dl_rg_pool = create_generic_resource_grid_pool(std::move(dl_grids));
    ASSERT_FACTORY(dl_rg_pool);

    // Create UL resource grid pool.
    ul_rg_pool = create_generic_resource_grid_pool(std::move(ul_grids));
    ASSERT_FACTORY(ul_rg_pool);
 
   return std::make_unique<upper_phy_fake_sw>(logger,
                                                 config.dl_slot_repo,
                                                 config.prach_context_repo,
                                                 std::move(ul_rg_pool),
                                                 std::move(dl_rg_pool),
                                                 config.gateway,
                                                 config.rx_symb_req_notifier,
                                                 config.enable_ul_processing,
                                                 config.enable_prach_processing,
                                                 nof_subcs,
                                                 config.max_nof_ports
                                                 );
  }
 }
 