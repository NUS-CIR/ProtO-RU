// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pxsch_bler_test_channel_emulator.h"
#include "pxsch_bler_test_factories.h"
#include "pxsch_bler_test_parse.h"
#include "ocudu/adt/format.h"
#include "ocudu/phy/support/resource_grid.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/phy/upper/channel_processors/pusch/pusch_decoder_result.h"
#include "ocudu/phy/upper/channel_processors/pusch/pusch_processor_result_notifier.h"
#include "ocudu/phy/upper/rx_buffer_pool.h"
#include "ocudu/phy/upper/unique_rx_buffer.h"
#include "ocudu/ran/precoding/precoding_codebooks.h"
#include "ocudu/ran/pusch/pusch_dmrs_symbol_mask.h"
#include "ocudu/ran/resource_allocation/rb_interval.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/sch/sch_dmrs_power.h"
#include "ocudu/ran/sch/sch_mcs.h"
#include "ocudu/ran/sch/sch_segmentation.h"
#include "ocudu/ran/sch/tbs_calculator.h"
#include "ocudu/support/executors/task_worker_pool.h"
#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>

using namespace ocudu;

static constexpr subcarrier_spacing  scs                         = subcarrier_spacing::kHz30;
static constexpr rnti_t              rnti                        = to_rnti(0x1234);
static constexpr unsigned            bwp_start_rb                = 0;
static constexpr unsigned            nof_ofdm_symbols            = 14;
static constexpr unsigned            start_symbol_index          = 0;
static constexpr dmrs_typeA_position dmrs_typeA_pos              = dmrs_typeA_position::pos2;
static constexpr dmrs_config_type    dmrs                        = dmrs_config_type::type1;
static constexpr unsigned            nof_cdm_groups_without_data = 2;
static constexpr cyclic_prefix       cp_prefix                   = cyclic_prefix::NORMAL;
static constexpr unsigned            n_id                        = 0;
static constexpr unsigned            scrambling_id               = 0;
static constexpr bool                n_scid                      = false;
static constexpr bool                use_early_stop              = true;
/// [Implementation-defined] Number of HARQ processes, which sets the number of slots between retransmissions.
static constexpr unsigned nof_harq_processes = 16;

class pxsch_bler_test
{
public:
  explicit pxsch_bler_test(const pxsch_bler_test_configuration& cfg_) : cfg(cfg_)
  {
    ocudulog::init();
    setup();
  }

  void run() { loop(); }

  ~pxsch_bler_test() { teardown(); }

private:
  static std::shared_ptr<resource_grid_factory> create_grid_factory()
  {
    std::shared_ptr<channel_precoder_factory> precod_factory = create_channel_precoder_factory("auto");
    report_fatal_error_if_not(precod_factory, "Failed to create channel precoding factory.");

    return create_resource_grid_factory();
  }

  class pdsch_processor_notifier_adaptor : public pdsch_processor_notifier
  {
  public:
    void on_finish_processing() override { completed = true; }

    void wait_for_completion()
    {
      while (!completed.load()) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
    }

  private:
    std::atomic<bool> completed = {false};
  };

  /// Implements a PUSCH decoder notifier adaptor for synchronizing the decoder results.
  class pusch_processor_notifier_adaptor : public pusch_processor_result_notifier
  {
  public:
    void on_uci(const pusch_processor_result_control& uci_) override
    {
      std::unique_lock<std::mutex> lock(mutex);
      uci = uci_;
    }

    void on_sch(const pusch_processor_result_data& sch_) override
    {
      std::unique_lock<std::mutex> lock(mutex);
      completed = true;
      sch       = sch_;
      cvar.notify_all();
    }

    const pusch_processor_result_data& wait_for_completion()
    {
      std::unique_lock<std::mutex> lock(mutex);
      while (!completed) {
        cvar.wait(lock);
      }

      return sch;
    }

    bool                           completed = false;
    pusch_processor_result_control uci;
    pusch_processor_result_data    sch;
    std::mutex                     mutex;
    std::condition_variable        cvar;
  };

  void setup()
  {
    // Prepare executors.
    worker_pool =
        std::make_unique<task_worker_pool<concurrent_queue_policy::locking_mpmc>>("thread", cfg.max_nof_threads, 1024);
    executor = std::make_unique<task_worker_pool_executor<concurrent_queue_policy::locking_mpmc>>(*worker_pool);

    // Prepare logging.
    auto log_level = ocudulog::str_to_basic_level(cfg.log_level).value_or(ocudulog::basic_levels::warning);
    ocudulog::fetch_basic_logger("ALL").set_level(log_level);
    ocudulog::fetch_basic_logger("PHY", true).set_level(log_level);

    // Compute DM-RS symbol mask from higher-layer parameters.
    dmrs_symbol_mask dmrs_symbols = pusch_dmrs_symbol_mask_mapping_type_A_single_get({
        .typeA_pos           = dmrs_typeA_pos,
        .additional_position = cfg.dmrs_additional_pos,
        .last_symbol         = static_cast<uint8_t>(start_symbol_index + nof_ofdm_symbols - 1),
    });

    // Compute modulation and code scheme.
    sch_mcs_description mcs_descr = pdsch_mcs_get_config(cfg.mcs_table, cfg.mcs_index);

    // Frequency allocation equal to bandwidth part.
    static prb_interval freq_allocation = {bwp_start_rb, cfg.bwp_size_rb};

    // Calculate transport block size.
    tbs_calculator_configuration tbs_config = {};
    tbs_config.mcs_descr                    = mcs_descr;
    tbs_config.n_prb                        = freq_allocation.length();
    tbs_config.nof_layers                   = cfg.nof_layers;
    tbs_config.nof_symb_sh                  = nof_ofdm_symbols;
    tbs_config.nof_dmrs_prb = get_nof_re_per_prb(dmrs) * dmrs_symbols.count() * nof_cdm_groups_without_data;
    units::bytes tbs        = tbs_calculator_calculate(tbs_config);

    // Select LDPC base graph.
    ldpc_base_graph_type ldpc_base_graph =
        get_ldpc_base_graph(mcs_descr.get_normalised_target_code_rate(), tbs.to_bits());

    // Generate frequency allocation.
    rb_allocation freq_alloc =
        rb_allocation::make_type1(freq_allocation.start(), freq_allocation.length(), std::nullopt);

    // Create PDSCH processor factory.
    std::shared_ptr<pdsch_processor_factory> pdsch_proc_factory =
        create_sw_pdsch_processor_factory(*executor, cfg.max_nof_threads + 1, "pxsch_bler_test", cfg.pxsch_type);
    report_fatal_error_if_not(pdsch_proc_factory, "Failed to create PDSCH processor factory.");

    // Create a PDSCH processor pool factory - creates a PDSCH processor for each retransmission.
    pdsch_proc_factory = create_pdsch_processor_pool(pdsch_proc_factory, cfg.rep_rv_sequence.size());
    report_fatal_error_if_not(pdsch_proc_factory, "Failed to create PDSCH processor pool factory.");

    // Create PUSCH processor factory.
    std::shared_ptr<pusch_processor_factory> pusch_proc_factory =
        create_sw_pusch_processor_factory(*executor,
                                          cfg.rep_rv_sequence.size() * cfg.max_nof_threads + 1,
                                          cfg.nof_ldpc_iterations,
                                          use_early_stop,
                                          cfg.pxsch_type,
                                          cfg.td_interpolation_strategy,
                                          cfg.fd_smoothing_strategy,
                                          cfg.equalizer_algorithm);
    report_fatal_error_if_not(pusch_proc_factory, "Failed to create PUSCH processor factory.");

    // Create a PUSCH processor pool factory - creates a PUSCH processor for each retransmission.
    pusch_processor_pool_factory_config pusch_proc_pool_config{
        .factory                = pusch_proc_factory,
        .uci_factory            = pusch_proc_factory,
        .nof_regular_processors = static_cast<unsigned>(cfg.rep_rv_sequence.size()),
        .nof_uci_processors     = static_cast<unsigned>(cfg.rep_rv_sequence.size()),
    };
    pusch_proc_factory = create_pusch_processor_pool(pusch_proc_pool_config);
    report_fatal_error_if_not(pusch_proc_factory, "Failed to create PUSCH processor pool factory.");

    // Create resource grid factory.
    std::shared_ptr<resource_grid_factory> grid_factory = create_grid_factory();
    report_fatal_error_if_not(grid_factory, "Failed to create resource grid factory.");

    // Create PDSCH processors.
    for ([[maybe_unused]] unsigned rep_idx : cfg.rep_rv_sequence) {
      transmitter = pdsch_proc_factory->create();
      report_fatal_error_if_not(transmitter, "Failed to create PDSCH processor.");
      tx_processors.push_back(std::move(transmitter));
    }

    // Create PUSCH processor.
    receiver = pusch_proc_factory->create(ocudulog::fetch_basic_logger("PHY"));
    report_fatal_error_if_not(receiver, "Failed to create PUSCH processor.");

    // Create resource grids.
    for ([[maybe_unused]] unsigned rep_idx : cfg.rep_rv_sequence) {
      tx_grids.emplace_back(grid_factory->create(cfg.nof_layers, MAX_NSYMB_PER_SLOT, MAX_NOF_SUBCARRIERS));
      rx_grids.emplace_back(grid_factory->create(cfg.nof_rx_ports, MAX_NSYMB_PER_SLOT, MAX_NOF_SUBCARRIERS));
    }

    // Calculate number of codeblocks.
    nof_codeblocks = compute_nof_codeblocks(units::bits(tbs), ldpc_base_graph);

    // Prepare receive soft buffer pool.
    rx_buffer_pool_config buffer_pool_config;
    buffer_pool_config.max_codeblock_size   = ldpc::MAX_CODEBLOCK_SIZE;
    buffer_pool_config.nof_buffers          = nof_buffers;
    buffer_pool_config.nof_codeblocks       = nof_buffers * nof_codeblocks;
    buffer_pool_config.expire_timeout_slots = 1;
    buffer_pool_config.external_soft_bits   = false;

    // Create buffer pool.
    buffer_pool = create_rx_buffer_pool(buffer_pool_config);
    report_error_if_not(buffer_pool, "Failed to create buffer pool.");

    // Each layer is mapped to a DM-RS port, starting at port 1000.
    dmrs_port_mask dmrs_ports(DMRS_MAX_NPORTS);
    dmrs_ports.fill(0, cfg.nof_layers);

    // Prepare PDSCH processor configuration.
    pdsch_config.reserve(cfg.rep_rv_sequence.size());
    for (unsigned i_rep = 0, rep_end = cfg.rep_rv_sequence.size(); i_rep != rep_end; ++i_rep) {
      unsigned rep_rv = cfg.rep_rv_sequence[i_rep];

      pdsch_config.push_back(pdsch_processor::pdu_t{
          .context          = std::nullopt,
          .slot             = slot_point(to_numerology_value(scs), 0),
          .rnti             = rnti,
          .bwp_size_rb      = cfg.bwp_size_rb,
          .bwp_start_rb     = bwp_start_rb,
          .cp               = cp_prefix,
          .codewords        = {pdsch_processor::codeword_description{mcs_descr.modulation, rep_rv, ldpc_base_graph}},
          .n_id             = n_id,
          .ref_point        = pdsch_processor::pdu_t::PRB0,
          .dmrs_symbol_mask = dmrs_symbols,
          .dmrs             = dmrs,
          .scrambling_id    = scrambling_id,
          .n_scid           = n_scid,
          .nof_cdm_groups_without_data = nof_cdm_groups_without_data,
          .dmrs_ports                  = dmrs_ports,
          .freq_alloc                  = freq_alloc,
          .start_symbol_index          = 0,
          .nof_symbols                 = nof_ofdm_symbols,
          .tbs_lbrm                    = tbs_lbrm_default,
          .reserved                    = {},
          .ratio_pdsch_dmrs_to_sss_dB  = get_sch_to_dmrs_ratio_dB(nof_cdm_groups_without_data),
          .ratio_pdsch_data_to_sss_dB  = 0.0F,
          .precoding_and_beamforming =
              precoding_beamforming_configuration::make_wideband(make_identity(cfg.nof_layers)),
      });

      static_vector<uint8_t, MAX_PORTS> rx_ports(cfg.nof_rx_ports);
      std::iota(rx_ports.begin(), rx_ports.end(), 0U);

      pusch_processor::dmrs_configuration dmrs_config = {
          .dmrs                        = dmrs,
          .scrambling_id               = scrambling_id,
          .n_scid                      = n_scid,
          .nof_cdm_groups_without_data = nof_cdm_groups_without_data,
      };

      pusch_processor::codeword_description cw_descr = {rep_rv, ldpc_base_graph, i_rep == 0, i_rep == rep_end - 1};

      // Prepare PUSCH processor configuration.
      pusch_config.push_back(pusch_processor::pdu_t{
          .harq_id            = INVALID_HARQ_ID,
          .slot               = slot_point(to_numerology_value(scs), 0) + cfg.ul_slot_offset,
          .slot_offset        = cfg.ul_slot_offset,
          .rnti               = rnti,
          .bwp_size_rb        = cfg.bwp_size_rb,
          .bwp_start_rb       = bwp_start_rb,
          .cp                 = cp_prefix,
          .mcs_descr          = mcs_descr,
          .codeword           = cw_descr,
          .uci                = {},
          .n_id               = n_id,
          .nof_tx_layers      = cfg.nof_layers,
          .rx_ports           = rx_ports,
          .dmrs_symbol_mask   = dmrs_symbols,
          .dmrs               = dmrs_config,
          .freq_alloc         = freq_alloc,
          .start_symbol_index = 0,
          .nof_symbols        = nof_ofdm_symbols,
          .tbs_lbrm           = tbs_lbrm_default,
          .dc_position =
              cfg.enable_dc_position ? std::optional(cfg.bwp_size_rb * NOF_SUBCARRIERS_PER_RB / 2) : std::nullopt,
          .n_rapid = std::nullopt,
      });
    }

    // Resize data to accomodate the transport block.
    tx_data.resize(tbs.value());
    rx_data.resize(tbs.value());

    emulator = std::make_unique<channel_emulator>(cfg.channel_delay_profile,
                                                  cfg.channel_fading_distribution,
                                                  cfg.sinr_dB,
                                                  cfg.cfo_Hz,
                                                  cfg.doppler_Hz,
                                                  cfg.mimo_correlation,
                                                  cfg.nof_corrupted_re_per_ofdm_symbol,
                                                  cfg.nof_layers,
                                                  cfg.nof_rx_ports,
                                                  MAX_NOF_SUBCARRIERS,
                                                  nof_ofdm_symbols,
                                                  cfg.max_nof_threads,
                                                  scs,
                                                  *executor);
  }

  void teardown()
  {
    worker_pool->stop();
    ocudulog::flush();
  }

  void print_stats(double completion_percent)
  {
    double crc_bler        = static_cast<double>(crc_error_count) / static_cast<double>(count);
    double data_bler       = static_cast<double>(data_error_count) / static_cast<double>(count);
    double mean_iterations = static_cast<double>(count_iterations) /
                             static_cast<double>(count * nof_codeblocks * cfg.rep_rv_sequence.size());
    // Each transmission occupies a slot, so the throughput is relative to the transmissions, not the transport blocks.
    double throughput_percent = 100.0 * (1.0 - crc_bler) / repetitions_stats.get_mean();

    fmt::print("\r[{:>5.1f}%] "
               "Iterations={{{:<2} {:<2} {:<3.1f}}}; "
               "Repetitions={{{:<2} {:<2} {:<3.1f}}}; "
               "BLER={:.10f}/{:.10f}; "
               "Throughput={:.2f}%; "
               "SINR={{{:+.2f} {:+.2f} {:+.2f}}}; "
               "EVM={{{:.3f} {:.3f} {:.3f}}}; "
               "TA={{{:.2f} {:.2f} {:.2f}}}us; "
               "CFO={{{:.2f} {:.2f} {:.2f}}}Hz; "
               "pxsch={}",
               completion_percent,
               min_iterations,
               max_iterations,
               mean_iterations,
               repetitions_stats.get_min(),
               repetitions_stats.get_max(),
               repetitions_stats.get_mean(),
               crc_bler,
               data_bler,
               throughput_percent,
               sinr_stats.get_min(),
               sinr_stats.get_max(),
               sinr_stats.get_mean(),
               evm_stats.get_min(),
               evm_stats.get_max(),
               evm_stats.get_mean(),
               ta_stats_us.get_min(),
               ta_stats_us.get_max(),
               ta_stats_us.get_mean(),
               cfo_stats_Hz.get_min(),
               cfo_stats_Hz.get_max(),
               cfo_stats_Hz.get_mean(),
               cfg.pxsch_type);
  }

  void loop()
  {
    std::mt19937 rgen(0);

    // Iterate different seeds.
    for (unsigned n = 0; n != cfg.nof_repetitions; ++n) {
      // Generate random data.
      std::generate(tx_data.begin(), tx_data.end(), [&rgen]() { return static_cast<uint8_t>(rgen() & 0xff); });
      // Select a HARQ identifier for this run.
      harq_id_t harq_id = to_harq_id(n % nof_buffers);

      // Process PDSCH.
      std::vector<pdsch_processor_notifier_adaptor> tx_notifiers(cfg.rep_rv_sequence.size());
      for (unsigned i_rep = 0, rep_end = cfg.rep_rv_sequence.size(); i_rep != rep_end; ++i_rep) {
        // Select notifier.
        pdsch_processor_notifier_adaptor& tx_notifier = tx_notifiers[i_rep];

        // Process PDSCH
        tx_processors[i_rep]->process(
            tx_grids[i_rep]->get_writer(), tx_notifier, {shared_transport_block(tx_data)}, pdsch_config[i_rep]);
      }

      // Wait for PDSCH processing to complete.
      for (pdsch_processor_notifier_adaptor& tx_notifier : tx_notifiers) {
        tx_notifier.wait_for_completion();
      }

      // Run channel emulator for retransmission. Consecutive transport blocks occupy consecutive HARQ processes, so the
      // retransmissions of a transport block are nof_harq_processes slots apart and every transmission gets a distinct
      // slot.
      for (unsigned i_rep = 0, rep_end = cfg.rep_rv_sequence.size(); i_rep != rep_end; ++i_rep) {
        uint64_t i_slot = static_cast<uint64_t>(n / nof_harq_processes) * nof_harq_processes * rep_end +
                          i_rep * nof_harq_processes + n % nof_harq_processes;
        emulator->run(rx_grids[i_rep]->get_writer(), tx_grids[i_rep]->get_reader(), i_slot);
      }

      // Process PUSCH.
      std::vector<pusch_processor_notifier_adaptor> rx_notifiers(cfg.rep_rv_sequence.size());
      for (unsigned i_rep = 0, rep_end = cfg.rep_rv_sequence.size(); i_rep != rep_end; ++i_rep) {
        bool new_data = (i_rep == 0);

        // Get a receive buffer.
        unique_rx_buffer buffer = buffer_pool->get_pool().reserve(
            pusch_config[i_rep].slot, trx_buffer_identifier(rnti, harq_id), nof_codeblocks, new_data);
        report_error_if_not(buffer, "Invalid buffer.");

        // Fork PUSCH reception.
        receiver->process(
            rx_data, std::move(buffer), rx_notifiers[i_rep], rx_grids[i_rep]->get_reader(), pusch_config[i_rep]);
      }

      // Wait for all PUSCH processing to complete.
      std::vector<pusch_processor_result_data> sch_results;
      sch_results.reserve(cfg.rep_rv_sequence.size());
      for (pusch_processor_notifier_adaptor& rx_notifier : rx_notifiers) {
        sch_results.push_back(rx_notifier.wait_for_completion());
      }

      // Extract the latest SCH result.
      const auto sch_result_it =
          std::find_if(sch_results.begin(), sch_results.end(), [](const pusch_processor_result_data& data) {
            return data.data.tb_crc_ok;
          });
      repetitions_stats.update(std::min(std::distance(sch_results.begin(), sch_result_it) + 1UL, sch_results.size()));

      // Accumulate counters.
      ++count;
      if (sch_result_it == sch_results.end()) {
        ++crc_error_count;
      }
      if (tx_data != rx_data) {
        ++data_error_count;
      }

      for (const pusch_processor_result_data& sch_result : sch_results) {
        max_iterations = std::max(sch_result.data.ldpc_decoder_stats.get_max(), max_iterations);
        min_iterations = std::min(sch_result.data.ldpc_decoder_stats.get_min(), min_iterations);
        count_iterations += static_cast<uint64_t>(sch_result.data.ldpc_decoder_stats.get_nof_observations() *
                                                  sch_result.data.ldpc_decoder_stats.get_mean());
        if (sch_result.csi.get_total_evm().has_value()) {
          evm_stats.update(sch_result.csi.get_total_evm().value());
        }
        if (sch_result.csi.get_sinr_dB().has_value()) {
          sinr_stats.update(sch_result.csi.get_sinr_dB().value());
        }
        if (sch_result.csi.get_time_alignment().has_value()) {
          ta_stats_us.update(sch_result.csi.get_time_alignment()->to_seconds() * 1e6);
        }
        if (sch_result.csi.get_cfo_Hz().has_value()) {
          cfo_stats_Hz.update(*sch_result.csi.get_cfo_Hz());
        }
      }

      // Increment slots.
      for (unsigned i_rep = 0, rep_end = cfg.rep_rv_sequence.size(); i_rep != rep_end; ++i_rep) {
        ++pdsch_config[i_rep].slot;
        ++pusch_config[i_rep].slot;
      }

      // Set following line to 1 for printing partial results.
      if (cfg.show_stats && (n % 100 == 0)) {
        print_stats(static_cast<double>(n) / static_cast<double>(cfg.nof_repetitions) * 100.0);
      }
    }

    // Print final results.
    print_stats(100.0);
  }

  unsigned                    nof_codeblocks;
  static constexpr unsigned   nof_buffers      = 4;
  uint64_t                    count            = 0;
  uint64_t                    crc_error_count  = 0;
  uint64_t                    data_error_count = 0;
  unsigned                    max_iterations   = std::numeric_limits<unsigned>::min();
  unsigned                    min_iterations   = std::numeric_limits<unsigned>::max();
  uint64_t                    count_iterations = 0;
  sample_statistics<unsigned> repetitions_stats;
  sample_statistics<float>    sinr_stats;
  sample_statistics<float>    evm_stats;
  sample_statistics<float>    ta_stats_us;
  sample_statistics<float>    cfo_stats_Hz;

  const pxsch_bler_test_configuration& cfg;

  std::unique_ptr<pdsch_processor>              transmitter;
  std::unique_ptr<pusch_processor>              receiver;
  std::vector<std::unique_ptr<resource_grid>>   tx_grids;
  std::vector<std::unique_ptr<resource_grid>>   rx_grids;
  std::vector<std::unique_ptr<pdsch_processor>> tx_processors;
  std::unique_ptr<rx_buffer_pool_controller>    buffer_pool;

  std::vector<pdsch_processor::pdu_t> pdsch_config;
  std::vector<pusch_processor::pdu_t> pusch_config;

  std::vector<uint8_t> tx_data;
  std::vector<uint8_t> rx_data;

  std::unique_ptr<task_worker_pool<concurrent_queue_policy::locking_mpmc>>          worker_pool;
  std::unique_ptr<task_worker_pool_executor<concurrent_queue_policy::locking_mpmc>> executor;

  std::unique_ptr<channel_emulator> emulator;
};

int main(int argc, char** argv)
{
  auto cfg = parse_configuration(argc, argv);
  if (!cfg.has_value()) {
    return cfg.error();
  }

  pxsch_bler_test test(cfg.value());
  test.run();

  return 0;
}
