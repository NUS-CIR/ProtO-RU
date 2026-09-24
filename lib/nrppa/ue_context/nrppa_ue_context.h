// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "nrppa_ue_logger.h"
#include "ocudu/nrppa/nrppa.h"
#include "ocudu/nrppa/nrppa_e_cid.h"
#include "ocudu/support/timers.h"
#include <unordered_map>

namespace ocudu::ocucp {

struct nrppa_ue_ids {
  cu_cp_ue_index_t ue_index;
  ran_ue_meas_id_t ran_ue_meas_id;
  lmf_ue_meas_id_t lmf_ue_meas_id;
};

struct nrppa_ue_context {
  nrppa_ue_ids             ue_ids;
  nrppa_cu_cp_ue_notifier* ue = nullptr;
  unique_timer             meas_report_timer;

  std::optional<std::chrono::milliseconds> meas_periodicity_ms;
  std::vector<nrppa_meas_quantities_item>  meas_quantities;
  /// Set while the gNB-DU runs a periodic E-CID measurement for this UE.
  bool du_e_cid_meas_active = false;

  nrppa_ue_logger logger;

  nrppa_ue_context(cu_cp_ue_index_t         ue_index_,
                   ran_ue_meas_id_t         ran_ue_meas_id_,
                   lmf_ue_meas_id_t         lmf_ue_meas_id_,
                   nrppa_cu_cp_ue_notifier& ue_notifier_,
                   timer_manager&           timers_,
                   task_executor&           task_exec_) :
    ue_ids({ue_index_, ran_ue_meas_id_, lmf_ue_meas_id_}),
    ue(&ue_notifier_),
    logger("NRPPA", {ue_index_, ran_ue_meas_id_, lmf_ue_meas_id_})
  {
    meas_report_timer = timers_.create_unique_timer(task_exec_);
  }

  [[nodiscard]] nrppa_cu_cp_ue_notifier* get_cu_cp_ue() const { return ue; }
};

class nrppa_ue_context_list
{
public:
  nrppa_ue_context_list(ocudulog::basic_logger& logger_);

  /// \brief Checks whether a UE with the given UE index exists.
  /// \param[in] ue_index The UE index used to find the UE.
  /// \return True when a UE for the given UE index exists, false otherwise.
  bool contains(cu_cp_ue_index_t ue_index) const;

  nrppa_ue_context& operator[](cu_cp_ue_index_t ue_index);

  nrppa_ue_context& add_ue(cu_cp_ue_index_t         ue_index,
                           ran_ue_meas_id_t         ran_ue_meas_id,
                           lmf_ue_meas_id_t         lmf_ue_meas_id,
                           nrppa_cu_cp_ue_notifier& ue_notifier,
                           timer_manager&           timers,
                           task_executor&           task_exec);

  void update_ue_index(cu_cp_ue_index_t         new_ue_index,
                       cu_cp_ue_index_t         old_ue_index,
                       nrppa_cu_cp_ue_notifier& new_ue_notifier,
                       timer_manager&           timers,
                       task_executor&           task_exec);

  void remove_ue_context(cu_cp_ue_index_t ue_index);

  size_t size() const;

  /// \brief Get the next available RAN UE MEAS ID.
  expected<ran_ue_meas_id_t, std::string> allocate_ran_ue_meas_id();

protected:
  ran_ue_meas_id_t next_ran_ue_meas_id = ran_ue_meas_id_t::min;

private:
  void increase_next_ran_ue_meas_id();

  ocudulog::basic_logger& logger;
  // Indexed by ue_index.
  std::unordered_map<cu_cp_ue_index_t, nrppa_ue_context> ues;
};

} // namespace ocudu::ocucp
