// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief PUSCH demodulator implementation definition.

#include "pusch_demodulator_impl.h"
#include "ocudu/adt/format.h"
#include "ocudu/ocuduvec/simd.h"
#include "ocudu/phy/upper/channel_processors/pusch/pusch_codeword_buffer.h"
#include "ocudu/phy/upper/channel_processors/pusch/pusch_demodulator_notifier.h"

#if defined(__SSE3__)
#include <immintrin.h>
#elif defined(__aarch64__)
#include <arm_neon.h>
#endif

using namespace ocudu;

static void
revert_scrambling(span<log_likelihood_ratio> out, span<const log_likelihood_ratio> in, const bit_buffer& sequence)
{
  ocudu_assert(in.size() == out.size(),
               "Input size (i.e., {}) and output size (i.e., {}) must be equal.",
               in.size(),
               out.size());

  unsigned i      = 0;
  unsigned length = in.size();

#if defined(__AVX512F__) && defined(__AVX512BW__)
  // Number of bits that can be processed with an AVX512 register.
  static constexpr unsigned nof_bits_per_avx512 = 64;

  const __mmask64* avx512_sequence_ptr = reinterpret_cast<const __mmask64*>(sequence.get_buffer().data());
  for (unsigned i_end = (length / nof_bits_per_avx512) * nof_bits_per_avx512; i != i_end; i += nof_bits_per_avx512) {
    // Load 64 bits in a go as a mask.
    __mmask64 xor_mask = *(avx512_sequence_ptr++);

    // Convert XOR mask to 0xff (for 1s) and 0x00 (for 0s).
    __m512i mask = _mm512_movm_epi8(xor_mask);

    // Reverses bits within bytes.
    __m512i shuffle_idx = _mm512_set_epi8(
        // clang-format off
        0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        // clang-format on
    );
    mask = _mm512_shuffle_epi8(mask, shuffle_idx);

    // Load 64 soft bits.
    __m512i data = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(in.data() + i));

    // Negate.
    data = _mm512_xor_si512(mask, data);

    // Add one.
    mask = _mm512_and_si512(mask, _mm512_set1_epi8(1));
    data = _mm512_add_epi8(mask, data);

    // Store register.
    _mm512_storeu_si512(reinterpret_cast<__m512i*>(out.data() + i), data);
  }
#endif // defined(__AVX512F__) && defined(__AVX512BW__)

#if defined(__AVX__) && defined(__AVX2__)
  // Number of bits that can be processed with an AVX register.
  static constexpr unsigned nof_bits_per_avx  = 32;
  const uint8_t*            avx2_sequence_ptr = sequence.get_buffer().data();

  for (unsigned i_byte = i / 8, i_end = (length / nof_bits_per_avx) * nof_bits_per_avx; i != i_end;
       i_byte += nof_bits_per_avx / 8, i += nof_bits_per_avx) {
    // Load sequence in different registers.
    __m256i mask = _mm256_setzero_si256();
    mask         = _mm256_insert_epi8(mask, avx2_sequence_ptr[i_byte + 0], 0);
    mask         = _mm256_insert_epi8(mask, avx2_sequence_ptr[i_byte + 1], 8);
    mask         = _mm256_insert_epi8(mask, avx2_sequence_ptr[i_byte + 2], 16);
    mask         = _mm256_insert_epi8(mask, avx2_sequence_ptr[i_byte + 3], 24);

    // Repeats each byte 8 times.
    __m256i shuffle_mask = _mm256_setr_epi8(
        // clang-format off
        0, 0, 0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8, 8, 8,
        0, 0, 0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8, 8, 8
        // clang-format on
    );
    mask = _mm256_shuffle_epi8(mask, shuffle_mask);

    // Selects each of the bits.
    __m256i bit_mask = _mm256_set_epi8(
        // clang-format off
        1, 2, 4, 8, 16, 32, 64, -128,
        1, 2, 4, 8, 16, 32, 64, -128,
        1, 2, 4, 8, 16, 32, 64, -128,
        1, 2, 4, 8, 16, 32, 64, -128
        // clang-format on
    );
    mask = _mm256_and_si256(bit_mask, mask);

    mask = ~_mm256_cmpeq_epi8(mask, _mm256_setzero_si256());

    // Load 64 soft bits.
    __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(in.data() + i));

    // Negate.
    data = _mm256_xor_si256(mask, data);

    // Add one.
    mask = _mm256_and_si256(mask, _mm256_set1_epi8(1));
    data = _mm256_add_epi8(mask, data);

    // Store register.
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(out.data() + i), data);
  }
#endif // defined(__AVX__) && defined(__AVX2__)

#if defined(__SSE2__) && defined(__SSE3__)
  // Number of bits that can be processed with an SSE register.
  static constexpr unsigned nof_bits_per_sse = 16;

  for (unsigned i_byte = i / 8, i_end = (length / nof_bits_per_sse) * nof_bits_per_sse; i != i_end;
       i_byte += 2, i += nof_bits_per_sse) {
    uint8_t byte0 = sequence.get_byte(i_byte);
    uint8_t byte1 = sequence.get_byte(i_byte + 1);
    int32_t c     = static_cast<int32_t>(byte0) + (static_cast<int32_t>(byte1) << 8);

    // Preload bits of interest in the 16 LSB.
    __m128i mask = _mm_set1_epi32(c);
    mask         = _mm_shuffle_epi8(mask, _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1));

    // Mask each bit.
    mask = _mm_and_si128(mask, _mm_set_epi64x(0x0102040810204080, 0x0102040810204080));

    // Get non zero mask.
    mask = _mm_cmpeq_epi8(mask, _mm_set_epi64x(0x0102040810204080, 0x0102040810204080));

    // Load input.
    __m128i v = _mm_loadu_si128(reinterpret_cast<const __m128i*>(in.data() + i));

    // Negate.
    v = _mm_xor_si128(mask, v);

    // Add one.
    mask = _mm_and_si128(mask, _mm_set1_epi8(1));
    v    = _mm_add_epi8(v, mask);

    _mm_storeu_si128(reinterpret_cast<__m128i*>(out.data() + i), v);
  }
#endif // defined(__SSE2__) && defined(__SSE3__)

#ifdef __aarch64__
  // Number of bits that can be processed with a SIMD register.
  static constexpr unsigned nof_bits_per_simd = 16;

  for (unsigned i_byte = 0, i_end = (length / nof_bits_per_simd) * nof_bits_per_simd; i != i_end;
       i_byte += 2, i += nof_bits_per_simd) {
    uint8_t byte0 = sequence.get_byte(i_byte);
    uint8_t byte1 = sequence.get_byte(i_byte + 1);
    int32_t c     = static_cast<int32_t>(byte0) + (static_cast<int32_t>(byte1) << 8);

    // Preload bits of interest in the 16 LSB.
    uint32x2_t c_dup_u32 = vdup_n_u32(c);
    uint8x16_t mask_u8 =
        vcombine_u8(vdup_lane_u8(vreinterpret_u8_u32(c_dup_u32), 0), vdup_lane_u8(vreinterpret_u8_u32(c_dup_u32), 1));

    // Create bit masks.
    const uint8_t    bit_masks[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    const uint8x16_t bit_masks_u8 = vcombine_u8(vcreate_u8(*(reinterpret_cast<const uint64_t*>(bit_masks))),
                                                vcreate_u8(*(reinterpret_cast<const uint64_t*>(bit_masks))));
    // Mask each bit.
    mask_u8 = vandq_u8(mask_u8, bit_masks_u8);

    // Get non zero mask.
    mask_u8 = vceqq_u8(mask_u8, bit_masks_u8);

    // Load input.
    int8x16_t v = vld1q_s8(reinterpret_cast<const int8_t*>(in.data() + i));

    // Negate.
    v = veorq_s8(vreinterpretq_s8_u8(mask_u8), v);

    // Add one.
    int8x16_t one_s8 = vandq_s8(vreinterpretq_s8_u8(mask_u8), vdupq_n_s8(1));
    v                = vaddq_s8(v, one_s8);

    // Store the result.
    vst1q_s8(reinterpret_cast<int8_t*>(out.data() + i), v);
  }
#endif // __aarch64__

  // Apply remaining bits.
  for (; i != length; ++i) {
    out[i] = in[i].to_value_type() * ((sequence.extract(i, 1) == 1) ? -1 : 1);
  }
}

static float filter_infinite_and_accumulate(unsigned& count, span<const float> input)
{
  float    sum = 0;
  unsigned i   = 0;

#if OCUDU_SIMD_F_SIZE
  const simd_f_t simd_infinity     = ocudu_simd_f_set1(std::numeric_limits<float>::infinity());
  const simd_f_t simd_neg_infinity = ocudu_simd_f_set1(-std::numeric_limits<float>::infinity());
  const simd_i_t simd_one          = ocudu_simd_i_set1(1);

  if (input.size() > 2 * OCUDU_SIMD_F_SIZE) {
    simd_i_t simd_count = ocudu_simd_i_set1(0);
    simd_f_t simd_sum   = ocudu_simd_f_set1(0.0);
    for (unsigned i_end = (input.size() / OCUDU_SIMD_F_SIZE) * OCUDU_SIMD_F_SIZE; i != i_end; i += OCUDU_SIMD_F_SIZE) {
      simd_f_t in = ocudu_simd_f_loadu(&input[i]);

      simd_sel_t isnormal_mask =
          ocudu_simd_sel_and(ocudu_simd_f_gt(simd_infinity, in), ocudu_simd_f_lt(simd_neg_infinity, in));

      simd_sum   = ocudu_simd_f_select(simd_sum, ocudu_simd_f_add(simd_sum, in), isnormal_mask);
      simd_count = ocudu_simd_i_select(simd_count, ocudu_simd_i_add(simd_count, simd_one), isnormal_mask);
    }

    std::array<float, OCUDU_SIMD_F_SIZE> temp_sum;
    ocudu_simd_f_storeu(temp_sum.data(), simd_sum);
    sum = std::accumulate(temp_sum.begin(), temp_sum.end(), 0.0F);
    std::array<int, OCUDU_SIMD_I_SIZE> temp_count;
    ocudu_simd_i_storeu(temp_count.data(), simd_count);
    count += std::accumulate(temp_count.begin(), temp_count.end(), 0);
  }
#endif // OCUDU_SIMD_F_SIZE

  for (unsigned i_end = input.size(); i != i_end; ++i) {
    // Exclude outliers with infinite variance. This makes sure that handling of the DC carrier does not skew
    // the SINR results.
    if (std::isinf(input[i])) {
      continue;
    }

    sum += input[i];
    ++count;
  }

  return sum;
}

void pusch_demodulator_impl::demodulate(pusch_codeword_buffer&              codeword_buffer,
                                        pusch_demodulator_notifier&         notifier,
                                        const resource_grid_reader&         grid,
                                        const dmrs_pusch_estimator_results& est_results,
                                        const configuration&                config)
{
  // Number of receive antenna ports.
  auto nof_rx_ports = static_cast<unsigned>(config.rx_ports.size());

  // Initialize scrambling sequence. When msgA is sent over PUSCH, an alternative scrambling sequence is used, as per
  // TS 38.211 Section 6.3.1.1 Release 16.
  unsigned c_init  = to_underlying(config.rnti) * pow2(15) + config.n_id;
  bool     is_msgA = config.n_rapid.has_value();
  if (is_msgA) {
    c_init = to_underlying(config.rnti) * pow2(16) + *config.n_rapid * pow2(10) + config.n_id;
  }

  descrambler->init(c_init);

  // Prepare PRB active RE mask.
  re_prb_mask active_re_per_prb      = ~re_prb_mask();
  re_prb_mask active_re_per_prb_dmrs = ~get_dmrs_prb_mask(config.dmrs_type, config.nof_cdm_groups_without_data);

  // Prepare RE mask.
  re_symbol_mask_type re_mask      = config.rb_mask.kronecker_product<NOF_SUBCARRIERS_PER_RB>(active_re_per_prb);
  re_symbol_mask_type re_mask_dmrs = config.rb_mask.kronecker_product<NOF_SUBCARRIERS_PER_RB>(active_re_per_prb_dmrs);

  // Calculate the number of bits per RE and port.
  unsigned nof_bits_per_re = config.nof_tx_layers * get_bits_per_symbol(config.modulation);
  ocudu_assert(nof_bits_per_re > 0,
               "Invalid combination of transmit layers (i.e., {}) and modulation (i.e., {}).",
               config.nof_tx_layers,
               to_string(config.modulation));

  // Stats accumulators.
  unsigned total_evm_symbol_count     = 0;
  unsigned total_sinr_softbit_count   = 0;
  float    total_noise_var_accumulate = 0.0;
  float    total_evm_accumulate       = 0.0;

  // Process each OFDM symbol.
  for (unsigned i_symbol = config.start_symbol_index, i_symbol_end = config.start_symbol_index + config.nof_symbols;
       i_symbol != i_symbol_end;
       ++i_symbol) {
    // Stats accumulators for the OFDM symbol.
    unsigned symbol_evm_symbol_count     = 0;
    unsigned symbol_sinr_softbit_count   = 0;
    float    symbol_noise_var_accumulate = 0.0;
    float    symbol_evm_accumulate       = 0.0;

    // Select RE mask for the symbol.
    re_symbol_mask_type& symbol_re_mask = config.dmrs_symb_pos.test(i_symbol) ? re_mask_dmrs : re_mask;

    // Count the amount of active RE in the symbol.
    unsigned nof_re_symbol = symbol_re_mask.count();

    // Skip symbol if it does not contain data.
    if (nof_re_symbol == 0) {
      continue;
    }

    // Resize equalizer output buffers.
    span<cf_t>  eq_re         = span<cf_t>(temp_eq_re).first(nof_re_symbol * config.nof_tx_layers);
    span<float> eq_noise_vars = span<float>(temp_eq_noise_vars).first(nof_re_symbol * config.nof_tx_layers);

    // Look for DC (Direct Current) subcarrier only with transform precoding disabled. This step is skipped when
    // transform precoding is used, as forcing the DC to zero in that case may introduce non-linear distortion after the
    // inverse transform. The issue is particularly pronounced for narrowband PUSCH transmissions.
    std::optional<unsigned> dc_position = config.enable_transform_precoding ? std::nullopt : config.dc_position;

    // Extract channel estimates from the resource grid.
    interval<unsigned>      re_interval(config.rb_mask.find_lowest() * NOF_SUBCARRIERS_PER_RB,
                                   (config.rb_mask.find_highest() + 1) * NOF_SUBCARRIERS_PER_RB);
    re_symbol_mask_type     symbol_re_mask_local = symbol_re_mask.slice(re_interval.start(), re_interval.stop());
    std::optional<unsigned> dc_position_local    = std::nullopt;
    if (dc_position.has_value() && (re_interval.contains(*dc_position))) {
      dc_position_local = *dc_position - re_interval.start();
    }

    const channel_equalizer::ch_est_list& ch_estimates = get_ch_data_estimates(
        est_results, i_symbol, config.nof_tx_layers, symbol_re_mask_local, dc_position_local, config.rx_ports);

    // Extract the Rx port noise variances from the channel estimation.
    for (unsigned i_port = 0; i_port != nof_rx_ports; ++i_port) {
      noise_var_estimates[i_port] = est_results.get_noise_variance(i_port);
    }

    // Extract the data symbols, equalize channels and, for each Tx layer, combine contribution from all Rx antenna
    // ports.
    const re_buffer_reader<cbf16_t>& ch_re = get_ch_data_re(grid, i_symbol, symbol_re_mask, config.rx_ports);
    equalizer->equalize(
        eq_re, eq_noise_vars, ch_re, ch_estimates, span<float>(noise_var_estimates).first(nof_rx_ports), 1.0F);

    // Revert transform precoding for the entire OFDM symbol.
    if (config.enable_transform_precoding) {
      ocudu_assert(config.nof_tx_layers == 1,
                   "Transform precoding is only possible with one layer (i.e. {}).",
                   config.nof_tx_layers);
      precoder->deprecode_ofdm_symbol(eq_re, eq_re);
      precoder->deprecode_ofdm_symbol_noise(eq_noise_vars, eq_noise_vars);
    }

    // Estimate post equalization Signal-to-Interference-plus-Noise Ratio.
    if (compute_post_eq_sinr) {
      symbol_noise_var_accumulate += filter_infinite_and_accumulate(symbol_sinr_softbit_count, eq_noise_vars);
    }

    // Counts the number of processed RE for the OFDM symbol.
    unsigned count_re_symbol = 0;

    // Process subcarriers in groups.
    while (count_re_symbol != nof_re_symbol) {
      // Calculate the remainder number of subcarriers to process for the current OFDM symbol.
      unsigned remain_nof_subc = nof_re_symbol - count_re_symbol;

      // Get a view of the codeword buffer destination.
      span<log_likelihood_ratio> codeword = codeword_buffer.get_next_block_view(remain_nof_subc * nof_bits_per_re);

      // Limit block size if the codeword block is smaller.
      ocudu_assert(codeword.size() % nof_bits_per_re == 0,
                   "The codeword block size (i.e., {}) must be multiple of the number of bits per RE (i.e., {}).",
                   codeword.size(),
                   nof_bits_per_re);

      // Select equalizer output.
      unsigned          nof_block_softbits    = codeword.size();
      unsigned          codeword_block_offset = count_re_symbol * config.nof_tx_layers;
      unsigned          codeword_block_size   = nof_block_softbits / get_bits_per_symbol(config.modulation);
      span<const cf_t>  eq_re_block           = eq_re.subspan(codeword_block_offset, codeword_block_size);
      span<const float> eq_noise_vars_block   = eq_noise_vars.subspan(codeword_block_offset, codeword_block_size);

      // Build LLRs from channel symbols.
      demapper->demodulate_soft(codeword, eq_re_block, eq_noise_vars_block, config.modulation);

      // Calculate EVM only if it is available.
      if (evm_calc) {
        symbol_evm_accumulate +=
            static_cast<float>(codeword_block_size) * evm_calc->calculate(codeword, eq_re_block, config.modulation);
        symbol_evm_symbol_count += codeword_block_size;
      }

      // Generate scrambling sequence.
      static_bit_buffer<pusch_constants::MAX_NOF_BITS_PER_OFDM_SYMBOL> scrambling_seq(nof_block_softbits);
      descrambler->generate(scrambling_seq);

      // Revert scrambling.
      revert_scrambling(codeword, codeword, scrambling_seq);

      // Increment the number of processed RE within the OFDM symbol.
      count_re_symbol += nof_block_softbits / nof_bits_per_re;

      // Update and notify statistics if it is the last processed block for the OFDM symbol. The provisional stats must
      // be notified earlier than the new processed block to ensure the stats are available upon the notification of the
      // results.
      if (count_re_symbol == nof_re_symbol) {
        // Prepare OFDM symbol stats and report.
        pusch_demodulator_notifier::demodulation_stats stats;
        if ((symbol_sinr_softbit_count != 0) && (symbol_noise_var_accumulate > 0.0)) {
          float mean_noise_var = symbol_noise_var_accumulate / static_cast<float>(symbol_sinr_softbit_count);
          stats.sinr_dB.emplace(-convert_power_to_dB(mean_noise_var));
        } else {
          stats.sinr_dB.emplace(std::numeric_limits<float>::infinity());
        }
        if (symbol_evm_symbol_count != 0) {
          stats.evm.emplace(symbol_evm_accumulate / static_cast<float>(symbol_evm_symbol_count));
        }
        notifier.on_provisional_stats(i_symbol, stats);

        // Prepare final stats.
        total_evm_symbol_count += symbol_evm_symbol_count;
        total_sinr_softbit_count += symbol_sinr_softbit_count;
        total_noise_var_accumulate += symbol_noise_var_accumulate;
        total_evm_accumulate += symbol_evm_accumulate;
      }

      // Notify a new processed block.
      codeword_buffer.on_new_block(codeword, scrambling_seq);
    }
  }

  pusch_demodulator_notifier::demodulation_stats stats;
  if ((total_sinr_softbit_count != 0) && (total_noise_var_accumulate > 0.0)) {
    float mean_noise_var = total_noise_var_accumulate / static_cast<float>(total_sinr_softbit_count);
    stats.sinr_dB.emplace(-convert_power_to_dB(mean_noise_var));
  } else {
    stats.sinr_dB.emplace(std::numeric_limits<float>::infinity());
  }
  if (total_evm_symbol_count != 0) {
    stats.evm.emplace(total_evm_accumulate / static_cast<float>(total_evm_symbol_count));
  }

  notifier.on_end_stats(stats);
  codeword_buffer.on_end_codeword();
}

const re_buffer_reader<cbf16_t>&
pusch_demodulator_impl::get_ch_data_re(const resource_grid_reader&              grid,
                                       unsigned                                 i_symbol,
                                       const re_symbol_mask_type&               re_mask,
                                       const static_vector<uint8_t, MAX_PORTS>& rx_ports)
{
  // Extract RE boundaries.
  unsigned nof_re = re_mask.count();
  int      begin  = re_mask.find_lowest();
  int      end    = re_mask.find_highest();
  ocudu_assert(begin <= end, "Invalid mask.");

  // Check if the mask is contiguous.
  if (nof_re == static_cast<unsigned>(end + 1 - begin)) {
    // Prepare channel estimates view.
    ch_re_view.resize(rx_ports.size(), nof_re);

    // Iterate over all layers and ports.
    for (unsigned i_port = 0, i_port_end = rx_ports.size(); i_port != i_port_end; ++i_port) {
      // View of the channel estimation for an OFDM symbol.
      span<const cbf16_t> ch_data_re = grid.get_view(i_port, i_symbol);

      // Set the view in the channel estimates.
      ch_re_view.set_slice(i_port, ch_data_re.subspan(begin, nof_re));
    }
    return ch_re_view;
  }

  // Prepare channel estimates copy destination.
  ch_re_copy.resize(rx_ports.size(), nof_re);

  // Extract RE for each port and symbol.
  for (unsigned i_port = 0, i_port_end = rx_ports.size(); i_port != i_port_end; ++i_port) {
    // Get a view of the port data RE.
    span<cbf16_t> re_port_buffer = ch_re_copy.get_slice(i_port);

    // Copy grid data resource elements into the buffer.
    re_port_buffer = grid.get(re_port_buffer, rx_ports[i_port], i_symbol, 0, re_mask);

    // Verify buffer size.
    ocudu_assert(
        re_port_buffer.empty(), "Invalid number of RE read from the grid. {} RE are missing.", re_port_buffer.size());
  }

  return ch_re_copy;
}

const channel_equalizer::ch_est_list&
pusch_demodulator_impl::get_ch_data_estimates(const dmrs_pusch_estimator_results&      est_results,
                                              unsigned                                 i_symbol,
                                              unsigned                                 nof_tx_layers,
                                              const re_symbol_mask_type&               re_mask,
                                              std::optional<unsigned>                  dc_position,
                                              const static_vector<uint8_t, MAX_PORTS>& rx_ports)
{
  // Extract RE boundaries.
  unsigned nof_re = re_mask.count();
  int      begin  = re_mask.find_lowest();
  int      end    = re_mask.find_highest();
  ocudu_assert((begin >= 0) && (end >= 0), "Invalid mask.");

  ch_estimates_copy.resize(nof_re, rx_ports.size(), nof_tx_layers);

  // Extract data RE coefficients from the channel estimation.
  for (unsigned i_layer = 0, i_layer_end = nof_tx_layers; i_layer != i_layer_end; ++i_layer) {
    for (unsigned i_port = 0, i_port_end = rx_ports.size(); i_port != i_port_end; ++i_port) {
      // Get a view of the channel estimates buffer for a single Rx port.
      span<cbf16_t> ch_port_buffer = ch_estimates_copy.get_channel(i_port, i_layer);

      // Store non-DM-RS REs.
      est_results.get_symbol_ch_estimate(ch_port_buffer, i_symbol, i_port, i_layer, re_mask);

      if (dc_position.has_value() && re_mask.test(*dc_position)) {
        // The number of active REs before the DC position gives the offset inside ch_port_buffer.
        re_symbol_mask_type local_mask        = re_mask.slice(begin, *dc_position);
        unsigned            relative_position = local_mask.count();
        ch_port_buffer[relative_position]     = 0;
      }
    }
  }

  return ch_estimates_copy;
}
