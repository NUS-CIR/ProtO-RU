// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "scheduler_test_doubles.h"
#include "tests/test_doubles/scheduler/cell_config_builder_profiles.h"
#include "tests/test_doubles/scheduler/sched_result_copy.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "ocudu/adt/circular_vector.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/sched_cell_config_helpers.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/rrm/pucch_resource_manager.h"
#include "ocudu/scheduler/scheduler_factory.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"
#include "ocudu/support/benchmark_utils.h"
#include <chrono>
#include <getopt.h>

using namespace ocudu;

struct bench_params {
  unsigned    nof_repetitions        = 100;
  unsigned    nof_ues                = 32;
  unsigned    dl_bs                  = 1000000;
  unsigned    ul_bs                  = 0;
  unsigned    max_dl_grants_per_slot = 100;
  duplex_mode dplx_mode              = duplex_mode::TDD;
  bool        ntn_harq_modes         = false;
  unsigned    ntn_koffset_ms         = 5;
  bool        debug                  = false;
};

static void usage(const char* prog, const bench_params& params)
{
  fmt::print("Usage: {} [-R repetitions] [-u UEs] [-b DL buffer] [-B UL buffer] [-m Max DL UEs per slot] "
             "[-D duplex mode] [-n split HARQ modes] [-k K-offset] [-d]\n",
             prog);
  fmt::print("\t-R Repetitions [Default {}]\n", params.nof_repetitions);
  fmt::print("\t-u Number of UEs [Default {}]\n", params.nof_ues);
  fmt::print("\t-b Average RLC DL buffer state [Default {}]\n", params.dl_bs);
  fmt::print("\t-B Buffer status reported per LCG. Setting this value to 0 disables UL traffic [Default {}]\n",
             params.ul_bs);
  fmt::print("\t-m Maximum number of DL UE grants per slot [Default {}]\n", params.max_dl_grants_per_slot);
  fmt::print("\t-D Duplex mode (FDD/TDD) [Default {}]\n", to_string(params.dplx_mode));
  fmt::print("\t-n Split the HARQ processes of each UE between the two NTN HARQ modes, and bind each bearer to one "
             "of them [Default {}]\n",
             params.ntn_harq_modes);
  fmt::print("\t-k NTN cell-specific K-offset in ms, which sets how long a mode A process waits for its feedback "
             "[Default {}]\n",
             params.ntn_koffset_ms);
  fmt::print("\t-d Debug mode [Default {}]\n", params.debug);
  fmt::print("\t-h Show this message\n");
}

static void parse_args(int argc, char** argv, bench_params& params)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "R:u:b:B:m:D:n:k:d:h")) != -1) {
    switch (opt) {
      case 'R':
        params.nof_repetitions = std::strtol(optarg, nullptr, 10);
        break;
      case 'u':
        params.nof_ues = std::strtol(optarg, nullptr, 10);
        break;
      case 'b':
        params.dl_bs = std::strtol(optarg, nullptr, 10);
        break;
      case 'B':
        params.ul_bs = std::strtol(optarg, nullptr, 10);
        break;
      case 'm':
        params.max_dl_grants_per_slot = std::strtol(optarg, nullptr, 10);
        break;
      case 'D': {
        if (std::string(optarg) == "FDD") {
          params.dplx_mode = duplex_mode::FDD;
        } else if (std::string(optarg) == "TDD") {
          params.dplx_mode = duplex_mode::TDD;
        } else {
          usage(argv[0], params);
          std::exit(1);
        }
        break;
      }
      case 'n':
        params.ntn_harq_modes = std::strtol(optarg, nullptr, 10) > 0;
        break;
      case 'k':
        params.ntn_koffset_ms = std::strtol(optarg, nullptr, 10);
        break;
      case 'd':
        params.debug = std::strtol(optarg, nullptr, 10) > 0;
        break;
      case 'h':
      default:
        usage(argv[0], params);
        std::exit(0);
    }
  }
}

/// \brief Splits the HARQ processes of a UE evenly between the two NTN HARQ modes and binds each bearer to one of them.
///
/// A cell whose processes all operate in one mode never picks them by mode, so the grants a scheduler builds for it
/// carry no restriction. Splitting them is what makes the restrictions, and the work of deriving them, apply.
static void split_ue_harq_modes(sched_ue_config_request& cfg)
{
  serving_cell_config& serv_cell = (*cfg.cells)[0].ded->serv_cell_cfg;

  // A bit set to one disables the DL HARQ feedback of that process, so the second half owes no HARQ-ACK.
  pdsch_serving_cell_config& pdsch_cfg = serv_cell.pdsch_serv_cell_cfg.value();
  pdsch_cfg.nof_harq_proc              = pdsch_serving_cell_config::nof_harq_proc_for_pdsch::n32;
  const unsigned nof_dl                = static_cast<unsigned>(pdsch_cfg.nof_harq_proc);
  pdsch_cfg.dl_harq_feedback_disabled.fill(false);
  pdsch_cfg.dl_harq_feedback_disabled.fill(nof_dl / 2, nof_dl);

  // A bit set to one identifies a process in mode A, so the first half retransmits and the second does not.
  pusch_serving_cell_config& pusch_cfg = serv_cell.ul_config->pusch_serv_cell_cfg.value();
  pusch_cfg.nof_harq_proc              = pusch_serving_cell_config::nof_harq_proc_for_pusch::n32;
  const unsigned nof_ul                = static_cast<unsigned>(pusch_cfg.nof_harq_proc);
  pusch_cfg.ul_harq_mode.fill(false);
  pusch_cfg.ul_harq_mode.fill(0, nof_ul / 2);

  // The SRBs and the first DRB take the mode that retransmits, the second DRB the one that does not. Both bearers
  // carry data in either direction, so a grant of either mode has channels asking for it.
  for (logical_channel_config& lc_cfg : *cfg.lc_config_list) {
    const bool retransmits          = is_srb(lc_cfg.lcid) or lc_cfg.lcid == lcid_t::LCID_MIN_DRB;
    lc_cfg.allowed_harq_mode        = retransmits ? ul_harq_mode::mode_a : ul_harq_mode::mode_b;
    lc_cfg.allowed_dl_harq_feedback = retransmits ? dl_harq_feedback::enabled : dl_harq_feedback::disabled;
    if (not retransmits) {
      // A channel bound to mode B reports its buffer in the group held for that mode, so the grants of either mode
      // are asked for separately.
      lc_cfg.lc_group = NON_GBR_DRB_MODE_B_LCG_ID;
    }
  }
}

class multi_ue_sched_simulator
{
public:
  multi_ue_sched_simulator(const scheduler_expert_config&    expert_cfg_,
                           const cell_config_builder_params& builder_params_,
                           bool                              ntn_harq_modes_ = false,
                           unsigned                          ntn_koffset_ms_ = 5) :
    ntn_harq_modes(ntn_harq_modes_),
    ntn_koffset_ms(ntn_koffset_ms_),
    expert_cfg(expert_cfg_),
    builder_params(builder_params_),
    logger(ocudulog::fetch_basic_logger("SCHED")),
    pucch_res_mng(expert_cfg_.ue.max_pucchs_per_slot),
    sch(create_scheduler(scheduler_config{expert_cfg, cfg_notif})),
    next_sl_tx(builder_params.scs_common, 0)
  {
    sched_cell_configuration_request_message cell_cfg_msg =
        sched_config_helper::make_default_sched_cell_configuration_request(builder_params);
    cell_cfg_msg.ran      = config_helpers::make_default_ran_cell_config(builder_params);
    auto& pucch_resources = cell_cfg_msg.ran.init_bwp.pucch.resources;
    std::get<pucch_f2_params>(pucch_resources.f2_or_f3_or_f4_params).max_code_rate = max_pucch_code_rate::dot_35;
    pucch_resources.nof_cell_csi_resources                                         = 4;
    pucch_resources.nof_cell_sr_resources                                          = 2;
    pucch_resources.res_set_size                                                   = 6;

    if (ntn_harq_modes) {
      // Only an NTN cell allowing mode B holds a process across a round trip, and only then does a scheduler pick its
      // processes by mode.
      cell_cfg_msg.ran.ntn_params.emplace();
      cell_cfg_msg.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{ntn_koffset_ms};
      cell_cfg_msg.ran.ntn_params->ul_harq_mode_b                = true;
    }

    cell_cfgs = {cell_cfg_msg.ran};

    sch->handle_cell_configuration_request(cell_cfg_msg);

    pucch_res_mng.add_cell(to_du_cell_index(0), cell_cfgs[0]);

    logger.set_context(next_sl_tx.sfn(), next_sl_tx.slot_index());
  }

  void add_ue()
  {
    sched_ue_creation_request_message ue_cfg_msg = sched_config_helper::create_default_sched_ue_creation_request(
        cell_cfgs[0], {lcid_t::LCID_SRB2, lcid_t::LCID_MIN_DRB, static_cast<lcid_t>(lcid_t::LCID_MIN_DRB + 1)});
    ue_cfg_msg.ue_index           = to_du_ue_index(ue_count);
    ue_cfg_msg.crnti              = to_rnti(0x4601 + ue_count);
    ue_cfg_msg.starts_in_fallback = false;

    auto& pcell = *(*ue_cfg_msg.cfg.cells)[0].ded;

    // Generate PUCCH resources for the UE.
    bool success = pucch_res_mng.alloc_resources(pcell);
    ocudu_assert(success, "Failed to allocate resources for UE={}", ue_count);

    if (ntn_harq_modes) {
      // A UE creation leaves every process in the retransmitting mode, as only a configuration update assigns
      // them. The same configuration goes out again as an update once the creation has been applied.
      split_ue_harq_modes(ue_cfg_msg.cfg);
      pending_splits.push_back(ue_cfg_msg);
    }
    sch->handle_ue_creation_request(ue_cfg_msg);
    ue_count++;
  }

  /// Sends the configuration update that assigns the HARQ modes of each UE added so far.
  void apply_harq_mode_splits()
  {
    for (const sched_ue_creation_request_message& ue_cfg_msg : pending_splits) {
      sch->handle_ue_reconfiguration_request(sched_ue_reconfiguration_message{
          .ue_index = ue_cfg_msg.ue_index, .crnti = ue_cfg_msg.crnti, .cfg = ue_cfg_msg.cfg});
      sch->handle_ue_config_applied(ue_cfg_msg.ue_index);
    }
    pending_splits.clear();
  }

  void push_dl_bs(unsigned dl_bs_val)
  {
    for (unsigned ue_index = 0; ue_index != ue_count; ++ue_index) {
      for (unsigned offset = 0; offset != 2; ++offset) {
        dl_buffer_state_indication_message dl_bs{};
        dl_bs.ue_index = to_du_ue_index(ue_index);
        dl_bs.lcid     = static_cast<lcid_t>(lcid_t::LCID_MIN_DRB + offset);
        dl_bs.bs       = dl_bs_val;
        sch->handle_dl_buffer_state_indication(dl_bs);
      }
    }
  }

  /// Reports the given buffer for the UEs of the half-open range, which a caller keeps inside the event pool.
  void push_ul_bsr(unsigned ul_bs_val, unsigned first_ue, unsigned last_ue)
  {
    for (unsigned ue_index = first_ue; ue_index < std::min(last_ue, ue_count); ++ue_index) {
      ul_bsr_indication_message bsr{};
      bsr.cell_index = to_du_cell_index(0);
      bsr.ue_index   = to_du_ue_index(ue_index);
      bsr.crnti      = to_rnti(0x4601 + ue_index);
      bsr.type       = bsr_format::LONG_BSR;
      // A reported buffer stands until the next report, and a grant is restricted to mode A wherever a channel of
      // that mode still holds one. The two modes therefore go to different UEs in a split, so that the run keeps
      // asking for grants of both rather than for mode A alone. The signalling bearers stay out of it, as their
      // slice outranks the one carrying the data and would serve one half of the UEs ahead of the other.
      if (not ntn_harq_modes) {
        bsr.reported_lcgs.push_back(ul_bsr_lcg_report{SRB_LCG_ID, ul_bs_val});
        bsr.reported_lcgs.push_back(ul_bsr_lcg_report{NON_GBR_DRB_LCG_ID, ul_bs_val});
      } else if (ue_index % 2 == 0) {
        bsr.reported_lcgs.push_back(ul_bsr_lcg_report{NON_GBR_DRB_LCG_ID, ul_bs_val});
      } else {
        // Only the split binds a bearer to this group. Reporting it otherwise asks for grants no channel fills.
        bsr.reported_lcgs.push_back(ul_bsr_lcg_report{NON_GBR_DRB_MODE_B_LCG_ID, ul_bs_val});
      }
      sch->handle_ul_bsr_indication(bsr);
    }
  }

  void run_slot() { result = sch->slot_indication(next_sl_tx, to_du_cell_index(0)); }

  void process_results()
  {
    slot_point_extended result_sl_tx = next_sl_tx;
    ++next_sl_tx;

    // Store previous results if any.
    if (result.valid()) {
      copy_result(result, sched_results[result_sl_tx.count()]);
    }

    // Process past UCI results.
    slot_point               sl_rx = next_sl_tx.without_hyper_sfn() - dl_pipeline_delay - uci_process_delay;
    const sched_result_copy& res   = sched_results[sl_rx.count()];
    if (res.success) {
      uci_indication ind;
      ind.slot_rx    = sl_rx;
      ind.cell_index = to_du_cell_index(0);
      for (const auto& pucch : res.ul.pucchs) {
        uci_indication::uci_pdu pdu;
        pdu.ue_index = to_du_ue_index(static_cast<unsigned>(pucch.crnti) - 0x4601);
        pdu.crnti    = pucch.crnti;
        if (pucch.format() == pucch_format::FORMAT_1) {
          uci_indication::uci_pdu::uci_pucch_f0_or_f1_pdu f1{};
          f1.sr_detected = false;
          if (pucch.uci_bits.harq_ack_nof_bits > 0 and pucch.uci_bits.sr_bits == sr_nof_bits::no_sr) {
            f1.ul_sinr_dB = 10;
            f1.harqs.resize(pucch.uci_bits.harq_ack_nof_bits, mac_harq_ack_report_status::ack);
          } else if (pucch.uci_bits.harq_ack_nof_bits > 0) {
            // ACK+SR
            f1.ul_sinr_dB = -10;
            f1.harqs.resize(pucch.uci_bits.harq_ack_nof_bits, mac_harq_ack_report_status::dtx);
          } else {
            f1.ul_sinr_dB  = -10;
            f1.sr_detected = false;
          }
          pdu.pdu = f1;
          ind.ucis.push_back(pdu);
        } else if (pucch.format() == pucch_format::FORMAT_2) {
          uci_indication::uci_pdu::uci_pucch_f2_or_f3_or_f4_pdu f2{};
          f2.ul_sinr_dB = 10;
          if (pucch.uci_bits.harq_ack_nof_bits > 0) {
            f2.harqs.resize(pucch.uci_bits.harq_ack_nof_bits, mac_harq_ack_report_status::ack);
          }
          if (pucch.csi_rep_cfg.has_value()) {
            f2.csi = csi_report_data{.ri                    = 4,
                                     .pmi                   = precoding_matrix_indicator{pmi_typeI_single_panel{
                                                           {pmi_codebook_single_panel_config::two_one, pmi_codebook_typeI_mode::one},
                                         0,
                                         std::nullopt,
                                         std::nullopt,
                                         0}},
                                     .first_tb_wideband_cqi = 15,
                                     .valid                 = true};
          }
          pdu.pdu = f2;
          ind.ucis.push_back(pdu);
        }
      }
      for (const ul_sched_info& ul_grant : res.ul.puschs) {
        if (ul_grant.uci.has_value()) {
          uci_indication::uci_pdu pdu;
          pdu.ue_index = to_du_ue_index(static_cast<unsigned>(ul_grant.pusch_cfg.rnti) - 0x4601);
          pdu.crnti    = ul_grant.pusch_cfg.rnti;
          uci_indication::uci_pdu::uci_pusch_pdu pusch_pdu{};
          if (ul_grant.uci.value().harq.has_value()) {
            pusch_pdu.harqs.resize(ul_grant.uci->harq.value().harq_ack_nof_bits, mac_harq_ack_report_status::ack);
          }
          if (ul_grant.uci->csi.has_value()) {
            pusch_pdu.csi =
                csi_report_data{.ri                    = 4,
                                .pmi                   = precoding_matrix_indicator{pmi_typeI_single_panel{
                                                      {pmi_codebook_single_panel_config::two_one, pmi_codebook_typeI_mode::one},
                                    0,
                                    std::nullopt,
                                    std::nullopt,
                                    0}},
                                .first_tb_wideband_cqi = 15,
                                .valid                 = true};
          }
          pdu.pdu = pusch_pdu;
          ind.ucis.push_back(pdu);
        }
      }

      // Forward UCI indication to scheduler.
      sch->handle_uci_indication(ind);

      // Report every PUSCH as decoded, so that its process is freed rather than held until it times out.
      if (not res.ul.puschs.empty()) {
        ul_crc_indication crc_ind;
        crc_ind.cell_index = to_du_cell_index(0);
        crc_ind.sl_rx      = sl_rx;
        for (const ul_sched_info& ul_grant : res.ul.puschs) {
          crc_ind.crcs.push_back(
              ul_crc_pdu_indication{.rnti     = ul_grant.pusch_cfg.rnti,
                                    .ue_index = to_du_ue_index(static_cast<unsigned>(ul_grant.pusch_cfg.rnti) - 0x4601),
                                    .harq_id  = ul_grant.pusch_cfg.harq_id,
                                    .tb_crc_success = true,
                                    .ul_sinr_dB     = 10});
        }
        sch->handle_crc_indication(crc_ind);
      }
    }

    logger.set_context(next_sl_tx.sfn(), next_sl_tx.slot_index());
  }

  mac_scheduler& sched() const { return *sch; }

private:
  const unsigned dl_pipeline_delay = 4;
  const unsigned uci_process_delay = 2;

  const bool     ntn_harq_modes;
  const unsigned ntn_koffset_ms;

  std::vector<sched_ue_creation_request_message> pending_splits;

  sched_cfg_dummy_notifier     cfg_notif;
  sched_dummy_metric_notifier  metric_notif;
  scheduler_expert_config      expert_cfg;
  cell_config_builder_params   builder_params;
  std::vector<ran_cell_config> cell_cfgs;
  ocudulog::basic_logger&      logger;
  pucch_resource_manager       pucch_res_mng;

  std::unique_ptr<mac_scheduler> sch;

  unsigned                 ue_count = 0;
  slot_point_extended      next_sl_tx;
  sched_slot_result_reader result;

  static constexpr size_t                  nof_stored_sched_results = 64;
  circular_vector<sched_result_copy, true> sched_results{nof_stored_sched_results};
};

void benchmark_sched(benchmarker& bm, const bench_params& params)
{
  scheduler_expert_config sched_cfg              = config_helpers::make_default_scheduler_expert_config();
  sched_cfg.ue.max_pdcch_alloc_attempts_per_slot = params.max_dl_grants_per_slot;

  const bool                 tdd            = params.dplx_mode == duplex_mode::TDD;
  cell_config_builder_params builder_params = cell_config_builder_profiles::create(
      params.dplx_mode,
      frequency_range::FR1,
      tdd ? std::optional<bs_channel_bandwidth>{bs_channel_bandwidth::MHz100} : std::nullopt);
  if (tdd) {
    builder_params.tdd_ul_dl_cfg_common = tdd_ul_dl_config_common{builder_params.scs_common, {10, 7, 8, 2, 0}};
  }
  builder_params.dl_carrier.nof_ant = 4;

  // Instantiate the simulator on the heap because of huge object size
  std::unique_ptr<multi_ue_sched_simulator> sim = std::make_unique<multi_ue_sched_simulator>(
      sched_cfg, builder_params, params.ntn_harq_modes, params.ntn_koffset_ms);

  // Add UEs.
  for (unsigned ue_count = 0; ue_count != params.nof_ues; ++ue_count) {
    sim->add_ue();
  }

  // Let the scheduler apply the UE creations, then the HARQ mode of each of their processes.
  const auto run_slots = [&sim](unsigned nof_slots) {
    for (unsigned count = 0; count != nof_slots; ++count) {
      sim->run_slot();
      sim->process_results();
    }
  };
  static constexpr unsigned nof_warmup_slots = 20;
  run_slots(nof_warmup_slots);
  if (params.ntn_harq_modes) {
    sim->apply_harq_mode_splits();
    run_slots(nof_warmup_slots);
  }

  // Update UE buffer states. The event pool holds a bounded number of reports per slot, so the UL ones go in over
  // several slots, a batch at a time.
  sim->push_dl_bs(params.dl_bs);
  if (params.ul_bs > 0) {
    static constexpr unsigned max_bsrs_per_slot = 32;
    for (unsigned first_ue = 0; first_ue < params.nof_ues; first_ue += max_bsrs_per_slot) {
      sim->push_ul_bsr(params.ul_bs, first_ue, first_ue + max_bsrs_per_slot);
      sim->run_slot();
      sim->process_results();
      sim->push_dl_bs(params.dl_bs);
    }
    run_slots(nof_warmup_slots);
  }

  // Run benchmark. The DL buffer is drained as it is allocated, so it is refilled every slot. A reported UL buffer
  // stands until the next report, so the one above keeps the UL busy for the whole run.
  bm.new_measure(
      fmt::format("{} scheduling {} UEs{}",
                  to_string(params.dplx_mode),
                  params.nof_ues,
                  params.ntn_harq_modes ? ", split HARQ modes" : ""),
      1,
      [&sim]() mutable { sim->run_slot(); },
      [&]() {
        sim->process_results();
        sim->push_dl_bs(params.dl_bs);
      });
}

int main(int argc, char** argv)
{
  ocudulog::init();

  bench_params params{};
  parse_args(argc, argv, params);

  ocudulog::fetch_basic_logger("SCHED", true)
      .set_level(params.debug ? ocudulog::basic_levels::debug : ocudulog::basic_levels::warning);

  std::unique_ptr<benchmarker> bm = std::make_unique<benchmarker>("scheduler", params.nof_repetitions);

  benchmark_sched(*bm, params);

  // Output results.
  bm->print_percentiles_time();
}
