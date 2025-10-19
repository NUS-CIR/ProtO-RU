/* bsed on worker_manager.h under apps/services/worker_manager/ */
#pragma once

 #include "worker_manager_config.h"
 #include "worker_manager_worker_getter.h"
 #include "srsran/cu_up/cu_up_executor_mapper.h"
 #include "srsran/du/du_high/du_high_executor_mapper.h"
 #include "srsran/support/executors/task_execution_manager.h"
 #include "srsran/support/executors/task_executor.h"
 
 namespace srsran {
 
 /// Manages the workers of the app.
 struct worker_manager : public worker_manager_executor_getter {
   worker_manager(const worker_manager_config& config);
 
   void stop();
 
   std::vector<task_executor*> lower_phy_tx_exec;
   std::vector<task_executor*> lower_phy_rx_exec;
   std::vector<task_executor*> lower_phy_dl_exec;
   std::vector<task_executor*> lower_phy_ul_exec;
   std::vector<task_executor*> lower_prach_exec;

   task_executor*              radio_exec      = nullptr;
   task_executor*              ru_printer_exec = nullptr;
   task_executor*              ru_timing_exec  = nullptr;

   std::vector<task_executor*> ru_rx_exec;
   std::vector<task_executor*> ru_emulators_exec;

   task_executor*              non_rt_low_prio_exec = nullptr;
   task_executor*              non_rt_hi_prio_exec  = nullptr;
 
 
   /// Get executor based on the name.
   task_executor* find_executor(const std::string& name) const
   {
     auto it = exec_mng.executors().find(name);
     return it != exec_mng.executors().end() ? it->second : nullptr;
   }
 
   task_executor& get_executor(const std::string& name) const override { return *exec_mng.executors().at(name); }
 
   worker_manager_executor_getter* get_executor_getter() { return this; }
 
 private:
   
   /// Manager of execution contexts and respective executors instantiated by the application.
   task_execution_manager exec_mng;
   os_sched_affinity_manager low_prio_affinity_mng;
   /// CPU affinity bitmask manager per cell.
   std::vector<os_sched_affinity_manager> affinity_mng;
 
   /// Helper method to create workers with non zero priority.
   void create_prio_worker(const std::string&                                    name,
                           unsigned                                              queue_size,
                           const std::vector<execution_config_helper::executor>& execs,
                           const os_sched_affinity_bitmask&                      mask,
                           os_thread_realtime_priority prio = os_thread_realtime_priority::no_realtime());
   /// Helper method that creates all the executors.
   void create_ru_executors(unsigned nof_emulators);
   /// Helper method that creates the lower PHY executors.
   void create_lower_phy_executors(const worker_manager_config::ru_sdr_config& config);
 
 };
 
 } // namespace srsran
 