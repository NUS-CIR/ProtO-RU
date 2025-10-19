/* based on worker_manager.cpp*/
#include <iostream> 
#include "worker_manager.h"
 #include "srsran/du/du_high/du_high_executor_mapper.h"
 
 using namespace srsran;
 
 static const uint32_t task_worker_queue_size = 2048;
 
 worker_manager::worker_manager(const worker_manager_config& worker_cfg) :
   low_prio_affinity_mng({worker_cfg.low_prio_sched_config})
 {
    
   for (const auto& cell_affinities : worker_cfg.config_affinities) {
     affinity_mng.emplace_back(cell_affinities);
   }
   
   create_ru_executors(worker_cfg.nof_emulators);
   
   if (worker_cfg.ru_sdr_cfg) {
     create_lower_phy_executors(worker_cfg.ru_sdr_cfg.value());
   }
 }
 
 void worker_manager::stop()
 {
   exec_mng.stop();
 }
 void worker_manager::create_prio_worker(const std::string&                                    name,
                                         unsigned                                              queue_size,
                                         const std::vector<execution_config_helper::executor>& execs,
                                         const os_sched_affinity_bitmask&                      mask,
                                         os_thread_realtime_priority                           prio)
 {
   using namespace execution_config_helper;
 
   const single_worker worker_desc{
       name, {concurrent_queue_policy::locking_mpsc, queue_size}, execs, std::nullopt, prio, mask};
   if (not exec_mng.add_execution_context(create_execution_context(worker_desc))) {
     report_fatal_error("Failed to instantiate {} execution context", worker_desc.name);
   }
 }

 void worker_manager::create_ru_executors(unsigned nof_emulators){
    using namespace execution_config_helper;
    for (unsigned i = 0; i != nof_emulators; ++i) {
    // Executors for Open Fronthaul messages reception.
    {
        const std::string name      = "ru_rx_#" + std::to_string(i);
        const std::string exec_name = "ru_rx_exec_#" + std::to_string(i);

        // os_sched_affinity_bitmask   ru_rx_affinity;
        // ru_rx_affinity.set(5);
        const single_worker ru_worker{name,
                                    {concurrent_queue_policy::lockfree_spsc, 2},
                                    {{exec_name}},
                                    std::chrono::microseconds{1},
                                    os_thread_realtime_priority::max() - 1,
                                    affinity_mng.front().calcute_affinity_mask(sched_affinity_mask_types::ofh_rx)};
                                    //ru_rx_affinity};
        if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
        }
        ru_rx_exec.push_back(exec_mng.executors().at(exec_name));
    }
    
    // Executors for the RU emulators.
    {
        const std::string   name      = "ru_emu_#" + std::to_string(i);
        const std::string   exec_name = "ru_emu_exec_#" + std::to_string(i);
        // os_sched_affinity_bitmask   ru_emu_affinity;
        // ru_emu_affinity.set(6);
        const single_worker ru_worker{name,
                                    {concurrent_queue_policy::lockfree_spsc, task_worker_queue_size},
                                    {{exec_name}},
                                    std::chrono::microseconds{1},
                                    os_thread_realtime_priority::max() - 1,
                                    //ru_emu_affinity,
                                    affinity_mng.front().calcute_affinity_mask(sched_affinity_mask_types::ofh_rx)};
        if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
        }
        ru_emulators_exec.push_back(exec_mng.executors().at(exec_name));
    }
    }
    // Timing executor.
    {
    const std::string name      = "ru_timing";
    const std::string exec_name = "ru_timing_exec";
    // os_sched_affinity_bitmask   ru_timing_affinity;
    // ru_timing_affinity.set(7);

    const single_worker ru_worker{name,
                                    {concurrent_queue_policy::lockfree_spsc, 4},
                                    {{exec_name}},
                                    std::chrono::microseconds{0},
                                    os_thread_realtime_priority::max() - 0,
                                    //ru_timing_affinity,
                                    affinity_mng.front().calcute_affinity_mask(sched_affinity_mask_types::ru_timing)};
    if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
    }
    ru_timing_exec = exec_mng.executors().at(exec_name);
    }
 }

 void worker_manager::create_lower_phy_executors(const worker_manager_config::ru_sdr_config& config)
 {
   using namespace execution_config_helper;
  //  os_sched_affinity_bitmask ru_affinity_mask;
  //  os_sched_affinity_bitmask low_prio_affinity_mask;
  //  ru_affinity_mask.set(8);
  //  low_prio_affinity_mask.set(9);
   // Radio Unit worker and executor. As the radio is unique per application, use the first cell of the affinity manager.
   create_prio_worker("radio",
                      task_worker_queue_size,
                      {{"radio_exec"}},
                      //ru_affinity_mask);
                      affinity_mng.front().calcute_affinity_mask(sched_affinity_mask_types::ru));
   radio_exec = exec_mng.executors().at("radio_exec");
   // Radio Unit statistics worker and executor.
   create_prio_worker("ru_stats_worker",
                      1,
                      {{"ru_printer_exec"}},
                      //low_prio_affinity_mask);
                      low_prio_affinity_mng.calcute_affinity_mask(sched_affinity_mask_types::low_priority));
   ru_printer_exec = exec_mng.executors().at("ru_printer_exec");

   for (unsigned cell_id = 0; cell_id != config.nof_cells; ++cell_id) {
      // Instantiate dedicated PRACH worker.
      const std::string cell_id_str = std::to_string(cell_id);
      const std::string name_prach = "phy_prach#" + cell_id_str;
      const std::string prach_exec = "prach_exec#" + cell_id_str;
      // os_sched_affinity_bitmask prach_affinity_mask;
      // prach_affinity_mask.set(10 + cell_id);
      create_prio_worker(name_prach,
                        task_worker_queue_size,
                        {{prach_exec}},
                        //prach_affinity_mask,
                        affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                        os_thread_realtime_priority::max() - 2);

     switch (config.profile) {
       case worker_manager_config::ru_sdr_config::lower_phy_thread_profile::blocking: {
         std::string name      = "phy_worker";
         std::string exec_name = "phy_exec";

         create_prio_worker("phy_worker",
                            task_worker_queue_size,
                            {{"phy_exec"}},
                            affinity_mng.front().calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::no_realtime());
 
         task_executor* phy_exec = exec_mng.executors().at(exec_name);
        
         lower_prach_exec.push_back(phy_exec);
         lower_phy_tx_exec.push_back(phy_exec);
         lower_phy_rx_exec.push_back(phy_exec);
         lower_phy_dl_exec.push_back(phy_exec);
         lower_phy_ul_exec.push_back(phy_exec);
         break;
       }
       case worker_manager_config::ru_sdr_config::lower_phy_thread_profile::single: {
         const std::string name      = "lower_phy#" + std::to_string(cell_id);
         const std::string exec_name = "lower_phy_exec#" + std::to_string(cell_id);
 
         create_prio_worker(name,
                            128,
                            {{exec_name}},
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max());
 
         task_executor* phy_exec = exec_mng.executors().at(exec_name);
         lower_phy_tx_exec.push_back(phy_exec);
         lower_phy_rx_exec.push_back(phy_exec);
         lower_phy_dl_exec.push_back(phy_exec);
         lower_phy_ul_exec.push_back(phy_exec);
 
         lower_prach_exec.push_back(exec_mng.executors().at("prach_exec#" + std::to_string(cell_id)));
         break;
       }
       case worker_manager_config::ru_sdr_config::lower_phy_thread_profile::dual: {
         const std::string name_dl = "lower_phy_dl#" + std::to_string(cell_id);
         const std::string exec_dl = "lower_phy_dl_exec#" + std::to_string(cell_id);
         const std::string name_ul = "lower_phy_ul#" + std::to_string(cell_id);
         const std::string exec_ul = "lower_phy_ul_exec#" + std::to_string(cell_id);
 
         create_prio_worker(name_dl,
                            128,
                            {{exec_dl}},
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max());
         create_prio_worker(name_ul,
                            2,
                            {{exec_ul}},
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max() - 1);
 
         lower_phy_tx_exec.push_back(exec_mng.executors().at(exec_dl));
         lower_phy_rx_exec.push_back(exec_mng.executors().at(exec_ul));
         lower_phy_dl_exec.push_back(exec_mng.executors().at(exec_dl));
         lower_phy_ul_exec.push_back(exec_mng.executors().at(exec_ul));
 
         lower_prach_exec.push_back(exec_mng.executors().at("prach_exec#" + std::to_string(cell_id)));
         break;
       }
       case worker_manager_config::ru_sdr_config::lower_phy_thread_profile::quad: {
         const std::string name_dl = "lower_phy_dl#" + std::to_string(cell_id);
         const std::string exec_dl = "lower_phy_dl_exec#" + std::to_string(cell_id);
         const std::string name_ul = "lower_phy_ul#" + std::to_string(cell_id);
         const std::string exec_ul = "lower_phy_ul_exec#" + std::to_string(cell_id);
         const std::string name_tx = "lower_phy_tx#" + std::to_string(cell_id);
         const std::string exec_tx = "lower_phy_tx_exec#" + std::to_string(cell_id);
         const std::string name_rx = "lower_phy_rx#" + std::to_string(cell_id);
         const std::string exec_rx = "lower_phy_rx_exec#" + std::to_string(cell_id);
        //  os_sched_affinity_bitmask   low_rx_affinity;
        //  os_sched_affinity_bitmask   low_tx_affinity;
        //  os_sched_affinity_bitmask   low_ul_affinity;
        //  os_sched_affinity_bitmask   low_dl_affinity;
        //  low_rx_affinity.set(1);
        //  low_tx_affinity.set(2);
        //  low_ul_affinity.set(3);
        //  low_dl_affinity.set(4);
 
         create_prio_worker(name_tx,
                            128,
                            {{exec_tx}},
                            //low_tx_affinity,
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max());
         create_prio_worker(name_rx,
                            1,
                            {{exec_rx}},
                            //low_rx_affinity,
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max() - 2);
         create_prio_worker(name_dl,
                            128,
                            {{exec_dl}},
                            //low_dl_affinity,
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max() - 1);
         create_prio_worker(name_ul,
                            128,
                            {{exec_ul}},
                            //low_ul_affinity,
                            affinity_mng[cell_id].calcute_affinity_mask(sched_affinity_mask_types::ru),
                            os_thread_realtime_priority::max() - 3);
 
         lower_phy_tx_exec.push_back(exec_mng.executors().at(exec_tx));
         lower_phy_rx_exec.push_back(exec_mng.executors().at(exec_rx));
         lower_phy_dl_exec.push_back(exec_mng.executors().at(exec_dl));
         lower_phy_ul_exec.push_back(exec_mng.executors().at(exec_ul));
 
         lower_prach_exec.push_back(exec_mng.executors().at("prach_exec#" + std::to_string(cell_id)));
         break;
       }
     }
   }
 }
 