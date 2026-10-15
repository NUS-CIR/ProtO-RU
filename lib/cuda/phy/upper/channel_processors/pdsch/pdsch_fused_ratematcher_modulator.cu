// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief The downlink shared channel transmit chain, from an encoded code block to symbols, in 1
/// kernel.
///
/// A thread walks 1 output symbol back to the bits that make it: it selects them from the circular
/// buffer as TS 38.212, Section 5.4.2 prescribes, places them as the interleaver of Section 5.4.2.2
/// does, generates the scrambling bits it needs where it stands, and maps the result to a
/// constellation point. Fusing the 4 stages spares the buffers that would hold the rate matched
/// bits and the scrambled bits, and the launches that would write them.
///
/// The sequence generator here is a second implementation of the one in
/// `lib/cuda/phy/upper/sequence_generators/`, because a thread needs the sequence bits at the
/// position it is working on rather than from a buffer prepared in advance.

#include "ocudu/cuda/adt/cuda_error.h"
#include "ocudu/cuda/phy/upper/channel_processors/pdsch/pdsch_fused_ratematcher_modulator.h"
#include <algorithm>
#include <cuda_fp16.h>
#include <mutex>
#include <new>

namespace ocudu {
namespace cuda {

/* ============================================================================
 * LFSR Jump Tables for on-the-fly Gold sequence generation
 * NOTE: These are defined here (not extern) because CUDA __constant__
 * memory is module-local and cannot be linked across compilation units.
 * ============================================================================ */
#define NC_SKIP 1600
#define LFSR_BITS 31
#define MAX_JUMP_POWER 24

__constant__ uint32_t d_x1_jump_pdsch[MAX_JUMP_POWER + 1][LFSR_BITS];
__constant__ uint32_t d_x2_jump_pdsch[MAX_JUMP_POWER + 1][LFSR_BITS];

static std::once_flag g_pdsch_fused_jump_tables_init_flag;

static void matrix_multiply_gf2_pdsch(const uint32_t A[LFSR_BITS], const uint32_t B[LFSR_BITS], uint32_t C[LFSR_BITS])
{
  uint32_t B_T[LFSR_BITS] = {0};
  for (int i = 0; i < LFSR_BITS; i++) {
    for (int j = 0; j < LFSR_BITS; j++) {
      if (B[j] & (1u << i)) {
        B_T[i] |= (1u << j);
      }
    }
  }
  for (int i = 0; i < LFSR_BITS; i++) {
    C[i] = 0;
    for (int j = 0; j < LFSR_BITS; j++) {
      uint32_t dot = A[i] & B_T[j];
      if (__builtin_popcount(dot) & 1) {
        C[i] |= (1u << j);
      }
    }
  }
}

static void init_x1_base_matrix_pdsch(uint32_t M[LFSR_BITS])
{
  for (int i = 0; i < LFSR_BITS; i++)
    M[i] = 0;
  for (int i = 0; i < LFSR_BITS - 1; i++)
    M[i] = 1u << (i + 1);
  M[LFSR_BITS - 1] = (1u << 0) | (1u << 3);
}

static void init_x2_base_matrix_pdsch(uint32_t M[LFSR_BITS])
{
  for (int i = 0; i < LFSR_BITS; i++)
    M[i] = 0;
  for (int i = 0; i < LFSR_BITS - 1; i++)
    M[i] = 1u << (i + 1);
  M[LFSR_BITS - 1] = (1u << 0) | (1u << 1) | (1u << 2) | (1u << 3);
}

static cudaError_t do_initialize_pdsch_jump_tables()
{
  uint32_t h_x1_jump[MAX_JUMP_POWER + 1][LFSR_BITS];
  uint32_t h_x2_jump[MAX_JUMP_POWER + 1][LFSR_BITS];

  init_x1_base_matrix_pdsch(h_x1_jump[0]);
  init_x2_base_matrix_pdsch(h_x2_jump[0]);

  for (int p = 1; p <= MAX_JUMP_POWER; p++) {
    matrix_multiply_gf2_pdsch(h_x1_jump[p - 1], h_x1_jump[p - 1], h_x1_jump[p]);
    matrix_multiply_gf2_pdsch(h_x2_jump[p - 1], h_x2_jump[p - 1], h_x2_jump[p]);
  }

  cudaError_t err;
  err = cudaMemcpyToSymbol(d_x1_jump_pdsch, h_x1_jump, sizeof(h_x1_jump));
  if (err != cudaSuccess)
    return err;

  err = cudaMemcpyToSymbol(d_x2_jump_pdsch, h_x2_jump, sizeof(h_x2_jump));
  return err;
}

static cudaError_t ensure_pdsch_jump_tables_initialized()
{
  static cudaError_t init_result = cudaSuccess;
  std::call_once(g_pdsch_fused_jump_tables_init_flag, [&]() { init_result = do_initialize_pdsch_jump_tables(); });
  return init_result;
}

__device__ __forceinline__ uint32_t apply_jump_matrix_pdsch(uint32_t state, const uint32_t jump[LFSR_BITS])
{
  uint32_t result = 0;
#pragma unroll
  for (int i = 0; i < LFSR_BITS; i++) {
    uint32_t masked = state & jump[i];
    if (__popc(masked) & 1) {
      result |= (1u << i);
    }
  }
  return result;
}

__device__ __forceinline__ uint32_t advance_x1_pdsch(uint32_t x1, int n)
{
  while (n) {
    int p = __ffs(n) - 1;
    x1    = apply_jump_matrix_pdsch(x1, d_x1_jump_pdsch[p]);
    n &= (n - 1);
  }
  return x1;
}

__device__ __forceinline__ uint32_t advance_x2_pdsch(uint32_t x2, int n)
{
  while (n) {
    int p = __ffs(n) - 1;
    x2    = apply_jump_matrix_pdsch(x2, d_x2_jump_pdsch[p]);
    n &= (n - 1);
  }
  return x2;
}

__device__ __forceinline__ uint32_t step_x1_pdsch(uint32_t x1)
{
  uint32_t new_bit = ((x1 >> 3) ^ x1) & 1;
  return (x1 >> 1) | (new_bit << 30);
}

__device__ __forceinline__ uint32_t step_x2_pdsch(uint32_t x2)
{
  uint32_t new_bit = ((x2 >> 3) ^ (x2 >> 2) ^ (x2 >> 1) ^ x2) & 1;
  return (x2 >> 1) | (new_bit << 30);
}

// ============================================================================

// INT8 Constellation Tables (unnormalized, for direct INT8 output)
// ============================================================================

__device__ __constant__ int8_t PDSCH_QPSK_I8[4] = {1, -1, 1, -1};
__device__ __constant__ int8_t PDSCH_QPSK_Q8[4] = {1, 1, -1, -1};

__device__ __constant__ int8_t PDSCH_QAM16_I8[4] = {1, -1, 3, -3};
__device__ __constant__ int8_t PDSCH_QAM16_Q8[4] = {1, -1, 3, -3};

__device__ __constant__ int8_t PDSCH_QAM64_I8[8] = {3, -3, 5, -5, 1, -1, 7, -7};

__device__ __constant__ int8_t PDSCH_QAM256_I8[16] = {5, -5, 11, -11, 3, -3, 13, -13, 7, -7, 9, -9, 1, -1, 15, -15};

// ============================================================================
// Rate Matching Index Calculation (same as rate_matching.cu)
// ============================================================================

/// \brief Calculate circular buffer position with filler skipping
///
/// The circular buffer has a "hole" for filler bits at positions [Kd, Kd+F).
/// k0 is given in actual buffer coordinates (from rate_matcher_compute_k0).
/// We convert to conceptual coordinates (excluding filler), advance by outIdx,
/// wrap at the effective buffer length, then convert back to actual coordinates.
///
/// Conceptual buffer has (Ncb - F) usable bits:
///   - Positions [0, Kd) = info bits (maps directly)
///   - Positions [Kd, Ncb-F) = parity bits (maps to [Kd+F, Ncb) in actual buffer)
///
/// This matches the CPU rate matcher behavior in ldpc_rate_matcher_impl.cpp.
__device__ __forceinline__ int fused_rate_match_index(int outIdx, int Kd, int F, int k0, int Ncb)
{
  // Effective buffer length (excluding filler hole)
  int effective_len = Ncb - F;

  // Convert k0 from actual to conceptual coordinates
  // k0 should never be in the filler region [Kd, Kd+F) per 3GPP spec
  int conceptual_k0;
  if (k0 < Kd) {
    conceptual_k0 = k0;
  } else {
    // k0 is past the filler region, subtract F to get conceptual position
    conceptual_k0 = k0 - F;
  }

  // Advance by outIdx and wrap at effective buffer length
  int conceptual_pos = (conceptual_k0 + outIdx) % effective_len;

  // Map conceptual position back to actual buffer position
  // Positions >= Kd need to skip over the F filler bits
  if (conceptual_pos >= Kd) {
    return conceptual_pos + F;
  } else {
    return conceptual_pos;
  }
}

// ============================================================================
// Fused RM + Interleave + Scramble + Modulate → INT8 Kernels
// ============================================================================

/// \brief Fused 64QAM: LDPC encoded → rate match → interleave → scramble → INT8 symbols
///
/// Eliminates intermediate rate-matched buffer and packed bytes buffer.
/// Each thread outputs one INT8 complex symbol (2 bytes).
__global__ void __launch_bounds__(256, 4) fused_rm_interleave_scramble_64qam_int8_kernel(
    const uint32_t* __restrict__ d_encoded, // LDPC encoded bits
    uint32_t c_init,
    int8_t* __restrict__ d_symbols_int8, // Output INT8 symbols (2 bytes each)
    int N_cb,                            // Circular buffer size
    int N_full,                          // Full codeword size
    int k0,                              // Starting position
    int E,                               // Rate-matched bits per CB
    int Kd,                              // Payload bits in CB coordinates
    int F,                               // Filler bits
    int puncture_offset,                 // Offset for punctured bits (2*Z)
    int num_cbs,                         // Number of code blocks
    int encoded_stride,                  // Words per CB in encoded buffer
    int nof_short,                       // Number of short CBs (E_short)
    int E_short,                         // Bits per short CB
    int E_long,                          // Bits per long CB
    int total_symbols                    // Total output symbols
)
{
  int sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (sym_idx >= total_symbols)
    return;

  // Calculate global output bit position
  int out_bit_base = sym_idx * 6;

  // Determine which CB and local bit position
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  // On-the-fly scrambling: advance LFSR to this thread's bit position
  uint32_t x1 = 1;
  uint32_t x2 = c_init & 0x7FFFFFFF;
  x1          = advance_x1_pdsch(x1, NC_SKIP + out_bit_base);
  x2          = advance_x2_pdsch(x2, NC_SKIP + out_bit_base);

  // Read 6 bits with interleaving, rate matching, and scrambling
  uint32_t scrambled_bits = 0;
  // Number of symbols for this CB
  int K = bits_this_cb / 6;

#pragma unroll
  for (int b = 0; b < 6; b++) {
    // De-interleave: output bit position → rate-matched position
    // Per 3GPP: j = bit_in_cb / Q_m, i = bit_in_cb % Q_m
    // rate_matched_pos = i * K + j
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / 6;
    int i            = bit_in_cb % 6;
    int rm_bit_in_cb = i * K + j;

    // Rate match: rate-matched position → LDPC encoded position
    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    // Read from LDPC encoded buffer (MSB-first within each byte)
    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;

    // Generate scrambling bit on-the-fly
    uint32_t scr_bit = (x1 ^ x2) & 1u;
    x1               = step_x1_pdsch(x1);
    x2               = step_x2_pdsch(x2);

    // XOR to scramble and accumulate
    scrambled_bits |= ((bit ^ scr_bit) << b);
  }

  // Map to INT8 constellation (64QAM)
  int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2);
  int q_idx =
      ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) | (((scrambled_bits >> 1) & 0x10) >> 2);

  d_symbols_int8[sym_idx * 2]     = PDSCH_QAM64_I8[i_idx];
  d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM64_I8[q_idx];
}

/// \brief Fused QPSK: LDPC encoded → rate match → interleave → scramble → INT8 symbols
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_qpsk_int8_kernel(const uint32_t* __restrict__ d_encoded,
                                                  uint32_t c_init,
                                                  int8_t* __restrict__ d_symbols_int8,
                                                  int N_cb,
                                                  int N_full,
                                                  int k0,
                                                  int E,
                                                  int Kd,
                                                  int F,
                                                  int puncture_offset,
                                                  int num_cbs,
                                                  int encoded_stride,
                                                  int nof_short,
                                                  int E_short,
                                                  int E_long,
                                                  int total_symbols)
{
  int sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (sym_idx >= total_symbols)
    return;

  int out_bit_base   = sym_idx * 2;
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  // On-the-fly scrambling: advance LFSR to this thread's bit position
  uint32_t x1 = 1;
  uint32_t x2 = c_init & 0x7FFFFFFF;
  x1          = advance_x1_pdsch(x1, NC_SKIP + out_bit_base);
  x2          = advance_x2_pdsch(x2, NC_SKIP + out_bit_base);

  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / 2;

#pragma unroll
  for (int b = 0; b < 2; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / 2;
    int i            = bit_in_cb % 2;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;

    // Generate scrambling bit on-the-fly
    uint32_t scr_bit = (x1 ^ x2) & 1u;
    x1               = step_x1_pdsch(x1);
    x2               = step_x2_pdsch(x2);

    // XOR to scramble and accumulate
    scrambled_bits |= ((bit ^ scr_bit) << b);
  }

  d_symbols_int8[sym_idx * 2]     = PDSCH_QPSK_I8[scrambled_bits];
  d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QPSK_Q8[scrambled_bits];
}

/// \brief Fused 16QAM: LDPC encoded → rate match → interleave → scramble → INT8 symbols
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_16qam_int8_kernel(const uint32_t* __restrict__ d_encoded,
                                                   uint32_t c_init,
                                                   int8_t* __restrict__ d_symbols_int8,
                                                   int N_cb,
                                                   int N_full,
                                                   int k0,
                                                   int E,
                                                   int Kd,
                                                   int F,
                                                   int puncture_offset,
                                                   int num_cbs,
                                                   int encoded_stride,
                                                   int nof_short,
                                                   int E_short,
                                                   int E_long,
                                                   int total_symbols)
{
  int sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (sym_idx >= total_symbols)
    return;

  int out_bit_base   = sym_idx * 4;
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  // On-the-fly scrambling: advance LFSR to this thread's bit position
  uint32_t x1 = 1;
  uint32_t x2 = c_init & 0x7FFFFFFF;
  x1          = advance_x1_pdsch(x1, NC_SKIP + out_bit_base);
  x2          = advance_x2_pdsch(x2, NC_SKIP + out_bit_base);

  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / 4;

#pragma unroll
  for (int b = 0; b < 4; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / 4;
    int i            = bit_in_cb % 4;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;

    // Generate scrambling bit on-the-fly
    uint32_t scr_bit = (x1 ^ x2) & 1u;
    x1               = step_x1_pdsch(x1);
    x2               = step_x2_pdsch(x2);

    bit ^= scr_bit;
    scrambled_bits |= (bit << b);
  }

  // 16QAM mapping: bits b0,b2 → I index, bits b1,b3 → Q index
  int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1);
  int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1);

  d_symbols_int8[sym_idx * 2]     = PDSCH_QAM16_I8[i_idx];
  d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM16_Q8[q_idx];
}

/// \brief Fused 256QAM: LDPC encoded → rate match → interleave → scramble → INT8 symbols
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_256qam_int8_kernel(const uint32_t* __restrict__ d_encoded,
                                                    uint32_t c_init,
                                                    int8_t* __restrict__ d_symbols_int8,
                                                    int N_cb,
                                                    int N_full,
                                                    int k0,
                                                    int E,
                                                    int Kd,
                                                    int F,
                                                    int puncture_offset,
                                                    int num_cbs,
                                                    int encoded_stride,
                                                    int nof_short,
                                                    int E_short,
                                                    int E_long,
                                                    int total_symbols)
{
  int sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (sym_idx >= total_symbols)
    return;

  int out_bit_base   = sym_idx * 8;
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  // On-the-fly scrambling: advance LFSR to this thread's bit position
  uint32_t x1 = 1;
  uint32_t x2 = c_init & 0x7FFFFFFF;
  x1          = advance_x1_pdsch(x1, NC_SKIP + out_bit_base);
  x2          = advance_x2_pdsch(x2, NC_SKIP + out_bit_base);

  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / 8;

#pragma unroll
  for (int b = 0; b < 8; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / 8;
    int i            = bit_in_cb % 8;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;

    // Generate scrambling bit on-the-fly
    uint32_t scr_bit = (x1 ^ x2) & 1u;
    x1               = step_x1_pdsch(x1);
    x2               = step_x2_pdsch(x2);

    bit ^= scr_bit;
    scrambled_bits |= (bit << b);
  }

  // 256QAM mapping: bits b0,b2,b4,b6 → I index, bits b1,b3,b5,b7 → Q index
  int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2) |
              ((scrambled_bits & 0x40) >> 3);
  int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) |
              (((scrambled_bits >> 1) & 0x10) >> 2) | (((scrambled_bits >> 1) & 0x40) >> 3);

  d_symbols_int8[sym_idx * 2]     = PDSCH_QAM256_I8[i_idx];
  d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM256_I8[q_idx];
}

__device__ __forceinline__ uint32_t pdsch_read_scramble_bit(const uint32_t* __restrict__ d_scramble_seq, int bit_idx)
{
  int word_idx = bit_idx >> 5;
  int bit_pos  = 31 - (bit_idx & 31);
  return (d_scramble_seq[word_idx] >> bit_pos) & 1u;
}

__device__ __forceinline__ uint16_t pdsch_float_to_bf16_bits(float value)
{
  uint32_t as_u32 = __float_as_uint(value);
  as_u32 += 0x7fffU + ((as_u32 >> 16U) & 1U);
  return static_cast<uint16_t>(as_u32 >> 16U);
}

__device__ __forceinline__ void pdsch_modulate_i8(uint32_t scrambled_bits, int q_m, int8_t& real_i8, int8_t& imag_i8)
{
  if (q_m == 2) {
    real_i8 = PDSCH_QPSK_I8[scrambled_bits];
    imag_i8 = PDSCH_QPSK_Q8[scrambled_bits];
  } else if (q_m == 4) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1);
    int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1);
    real_i8   = PDSCH_QAM16_I8[i_idx];
    imag_i8   = PDSCH_QAM16_Q8[q_idx];
  } else if (q_m == 6) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2);
    int q_idx =
        ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) | (((scrambled_bits >> 1) & 0x10) >> 2);
    real_i8 = PDSCH_QAM64_I8[i_idx];
    imag_i8 = PDSCH_QAM64_I8[q_idx];
  } else {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2) |
                ((scrambled_bits & 0x40) >> 3);
    int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) |
                (((scrambled_bits >> 1) & 0x10) >> 2) | (((scrambled_bits >> 1) & 0x40) >> 3);
    real_i8 = PDSCH_QAM256_I8[i_idx];
    imag_i8 = PDSCH_QAM256_I8[q_idx];
  }
}

template <int Q_M>
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_precomputed_int8_kernel(const uint32_t* __restrict__ d_encoded,
                                                         const uint32_t* __restrict__ d_scramble_seq,
                                                         int8_t* __restrict__ d_symbols_int8,
                                                         int N_cb,
                                                         int N_full,
                                                         int k0,
                                                         int E,
                                                         int Kd,
                                                         int F,
                                                         int puncture_offset,
                                                         int num_cbs,
                                                         int encoded_stride,
                                                         int nof_short,
                                                         int E_short,
                                                         int E_long,
                                                         int total_symbols)
{
  int sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (sym_idx >= total_symbols)
    return;

  int out_bit_base   = sym_idx * Q_M;
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / Q_M;

#pragma unroll
  for (int b = 0; b < Q_M; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / Q_M;
    int i            = bit_in_cb % Q_M;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;
    uint32_t scr_bit      = pdsch_read_scramble_bit(d_scramble_seq, out_bit_base + b);

    scrambled_bits |= ((bit ^ scr_bit) << b);
  }

  if (Q_M == 2) {
    d_symbols_int8[sym_idx * 2]     = PDSCH_QPSK_I8[scrambled_bits];
    d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QPSK_Q8[scrambled_bits];
  } else if (Q_M == 4) {
    int i_idx                       = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1);
    int q_idx                       = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1);
    d_symbols_int8[sym_idx * 2]     = PDSCH_QAM16_I8[i_idx];
    d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM16_Q8[q_idx];
  } else if (Q_M == 6) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2);
    int q_idx =
        ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) | (((scrambled_bits >> 1) & 0x10) >> 2);
    d_symbols_int8[sym_idx * 2]     = PDSCH_QAM64_I8[i_idx];
    d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM64_I8[q_idx];
  } else if (Q_M == 8) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2) |
                ((scrambled_bits & 0x40) >> 3);
    int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) |
                (((scrambled_bits >> 1) & 0x10) >> 2) | (((scrambled_bits >> 1) & 0x40) >> 3);
    d_symbols_int8[sym_idx * 2]     = PDSCH_QAM256_I8[i_idx];
    d_symbols_int8[sym_idx * 2 + 1] = PDSCH_QAM256_I8[q_idx];
  }
}

template <int Q_M>
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_precomputed_bf16_grid_kernel(const uint32_t* __restrict__ d_encoded,
                                                              const uint32_t* __restrict__ d_scramble_seq,
                                                              const uint32_t* __restrict__ d_re_offsets,
                                                              uint16_t* __restrict__ d_grid_bf16,
                                                              int   N_cb,
                                                              int   N_full,
                                                              int   k0,
                                                              int   E,
                                                              int   Kd,
                                                              int   F,
                                                              int   puncture_offset,
                                                              int   num_cbs,
                                                              int   encoded_stride,
                                                              int   nof_short,
                                                              int   E_short,
                                                              int   E_long,
                                                              int   total_symbols,
                                                              int   symbol_offset,
                                                              int   nof_re,
                                                              int   nof_ports,
                                                              int   nof_layers,
                                                              int   nof_grid_re_per_port,
                                                              float weight_real)
{
  int mapped_symbol_idx = blockIdx.x * blockDim.x + threadIdx.x;
  int mapped_symbols    = nof_re * nof_layers;
  if (mapped_symbol_idx >= mapped_symbols)
    return;

  int i_re    = mapped_symbol_idx / nof_layers;
  int i_layer = mapped_symbol_idx - i_re * nof_layers;
  int sym_idx = symbol_offset + mapped_symbol_idx;
  if (sym_idx >= total_symbols)
    return;

  int out_bit_base   = sym_idx * Q_M;
  int short_cb_total = nof_short * E_short;
  int cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    cb_idx       = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    cb_idx       = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / Q_M;

#pragma unroll
  for (int b = 0; b < Q_M; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / Q_M;
    int i            = bit_in_cb % Q_M;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;
    uint32_t scr_bit      = pdsch_read_scramble_bit(d_scramble_seq, out_bit_base + b);

    scrambled_bits |= ((bit ^ scr_bit) << b);
  }

  int8_t real_i8 = 0;
  int8_t imag_i8 = 0;
  pdsch_modulate_i8(scrambled_bits, Q_M, real_i8, imag_i8);
  uint16_t real_bf16 = pdsch_float_to_bf16_bits(static_cast<float>(real_i8) * weight_real);
  uint16_t imag_bf16 = pdsch_float_to_bf16_bits(static_cast<float>(imag_i8) * weight_real);
  uint32_t re_offset = d_re_offsets[i_re];

  if (nof_layers == 1) {
    for (int i_port = 0; i_port != nof_ports; ++i_port) {
      int out_index              = 2 * (i_port * nof_grid_re_per_port + static_cast<int>(re_offset));
      d_grid_bf16[out_index]     = real_bf16;
      d_grid_bf16[out_index + 1] = imag_bf16;
    }
  } else {
    int out_index              = 2 * (i_layer * nof_grid_re_per_port + static_cast<int>(re_offset));
    d_grid_bf16[out_index]     = real_bf16;
    d_grid_bf16[out_index + 1] = imag_bf16;
  }
}

template <int Q_M>
__global__ void __launch_bounds__(256, 4)
    fused_rm_interleave_scramble_precomputed_batch_int8_kernel(const uint32_t* __restrict__ d_encoded,
                                                               const uint32_t* __restrict__ d_scramble_seq,
                                                               int8_t* __restrict__ d_symbols_int8,
                                                               int N_cb,
                                                               int N_full,
                                                               int k0,
                                                               int E,
                                                               int Kd,
                                                               int F,
                                                               int puncture_offset,
                                                               int num_cbs,
                                                               int encoded_stride,
                                                               int nof_short,
                                                               int E_short,
                                                               int E_long,
                                                               int symbols_per_tb,
                                                               int num_tbs)
{
  int global_sym_idx = blockIdx.x * blockDim.x + threadIdx.x;
  int total_symbols  = symbols_per_tb * num_tbs;
  if (global_sym_idx >= total_symbols)
    return;

  int tb_idx         = global_sym_idx / symbols_per_tb;
  int local_sym_idx  = global_sym_idx - tb_idx * symbols_per_tb;
  int out_bit_base   = local_sym_idx * Q_M;
  int short_cb_total = nof_short * E_short;
  int local_cb_idx, local_base, bits_this_cb;

  if (out_bit_base < short_cb_total) {
    local_cb_idx = out_bit_base / E_short;
    local_base   = out_bit_base % E_short;
    bits_this_cb = E_short;
  } else {
    int offset   = out_bit_base - short_cb_total;
    local_cb_idx = nof_short + offset / E_long;
    local_base   = offset % E_long;
    bits_this_cb = E_long;
  }

  int      cb_idx         = tb_idx * num_cbs + local_cb_idx;
  uint32_t scrambled_bits = 0;
  int      K              = bits_this_cb / Q_M;

#pragma unroll
  for (int b = 0; b < Q_M; b++) {
    int bit_in_cb    = local_base + b;
    int j            = bit_in_cb / Q_M;
    int i            = bit_in_cb % Q_M;
    int rm_bit_in_cb = i * K + j;

    int cb_pos  = fused_rate_match_index(rm_bit_in_cb, Kd, F, k0, N_cb);
    int enc_pos = cb_pos + puncture_offset;

    int in_word      = enc_pos / 32;
    int bit_in_word  = enc_pos % 32;
    int byte_in_word = bit_in_word / 8;
    int bit_in_byte  = 7 - (bit_in_word % 8);
    int in_bit_pos   = byte_in_word * 8 + bit_in_byte;

    uint32_t encoded_word = d_encoded[cb_idx * encoded_stride + in_word];
    uint32_t bit          = (encoded_word >> in_bit_pos) & 1u;
    uint32_t scr_bit      = pdsch_read_scramble_bit(d_scramble_seq, out_bit_base + b);

    scrambled_bits |= ((bit ^ scr_bit) << b);
  }

  if (Q_M == 2) {
    d_symbols_int8[global_sym_idx * 2]     = PDSCH_QPSK_I8[scrambled_bits];
    d_symbols_int8[global_sym_idx * 2 + 1] = PDSCH_QPSK_Q8[scrambled_bits];
  } else if (Q_M == 4) {
    int i_idx                              = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1);
    int q_idx                              = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1);
    d_symbols_int8[global_sym_idx * 2]     = PDSCH_QAM16_I8[i_idx];
    d_symbols_int8[global_sym_idx * 2 + 1] = PDSCH_QAM16_Q8[q_idx];
  } else if (Q_M == 6) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2);
    int q_idx =
        ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) | (((scrambled_bits >> 1) & 0x10) >> 2);
    d_symbols_int8[global_sym_idx * 2]     = PDSCH_QAM64_I8[i_idx];
    d_symbols_int8[global_sym_idx * 2 + 1] = PDSCH_QAM64_I8[q_idx];
  } else if (Q_M == 8) {
    int i_idx = (scrambled_bits & 0x1) | ((scrambled_bits & 0x4) >> 1) | ((scrambled_bits & 0x10) >> 2) |
                ((scrambled_bits & 0x40) >> 3);
    int q_idx = ((scrambled_bits >> 1) & 0x1) | (((scrambled_bits >> 1) & 0x4) >> 1) |
                (((scrambled_bits >> 1) & 0x10) >> 2) | (((scrambled_bits >> 1) & 0x40) >> 3);
    d_symbols_int8[global_sym_idx * 2]     = PDSCH_QAM256_I8[i_idx];
    d_symbols_int8[global_sym_idx * 2 + 1] = PDSCH_QAM256_I8[q_idx];
  }
}

// ============================================================================

// ============================================================================
// Geometry
// ============================================================================

/// Shape the kernels read, derived from a code block description and a transport block.
struct fused_geometry {
  /// \brief Number of bits the selection wraps around, N_cb of TS 38.212, Section 5.4.2.1.
  ///
  /// What the punctured columns leave, limited to N_ref with limited-buffer rate matching.
  int N_cb            = 0;
  int N_full          = 0;
  int k0              = 0;
  int Kd              = 0;
  int F               = 0;
  int puncture_offset = 0;
  int encoded_stride  = 0;
  int nof_short       = 0;
  int E_short         = 0;
  int E_long          = 0;
  int nof_cbs         = 0;
  int bits_per_symbol = 0;
  int nof_symbols     = 0;
};

/// \brief Bit the selection starts at, as per TS 38.212, Table 5.4.2.1-2.
static int starting_position(int base_graph, int Z, unsigned rv, int N_cb)
{
  switch (rv) {
    case 0:
      return 0;
    case 1:
      return (base_graph == 1) ? ((17 * N_cb) / (66 * Z)) * Z : ((13 * N_cb) / (50 * Z)) * Z;
    case 2:
      return (base_graph == 1) ? ((33 * N_cb) / (66 * Z)) * Z : ((25 * N_cb) / (50 * Z)) * Z;
    default:
      return (base_graph == 1) ? ((56 * N_cb) / (66 * Z)) * Z : ((43 * N_cb) / (50 * Z)) * Z;
  }
}

/// Number of threads a fused kernel runs per block.
static constexpr unsigned block_size = 256;

/// Number of blocks that covers the given number of symbols.
static unsigned blocks_for(unsigned nof_symbols)
{
  return (nof_symbols + block_size - 1) / block_size;
}

// ============================================================================
// Rate matcher and modulator
// ============================================================================

struct pdsch_fused_ratematcher_modulator::impl {
  fused_geometry geometry;
  bool           configured = false;
};

cuda_expected<pdsch_fused_ratematcher_modulator> pdsch_fused_ratematcher_modulator::create()
{
  cuda_result status = check_cuda_error(static_cast<int>(ensure_pdsch_jump_tables_initialized()),
                                        "fused rate matcher and modulator jump table upload");
  if (!status.has_value()) {
    return make_unexpected(status.error());
  }

  std::unique_ptr<impl> created(new (std::nothrow) impl());
  if (created == nullptr) {
    return make_unexpected(std::string("The fused rate matcher and modulator state could not be allocated"));
  }

  return pdsch_fused_ratematcher_modulator(std::move(created));
}

pdsch_fused_ratematcher_modulator::pdsch_fused_ratematcher_modulator() = default;

pdsch_fused_ratematcher_modulator::pdsch_fused_ratematcher_modulator(std::unique_ptr<impl> state_) :
  state(std::move(state_))
{
}

pdsch_fused_ratematcher_modulator::pdsch_fused_ratematcher_modulator(
    pdsch_fused_ratematcher_modulator&& other) noexcept = default;

pdsch_fused_ratematcher_modulator&
pdsch_fused_ratematcher_modulator::operator=(pdsch_fused_ratematcher_modulator&& other) noexcept = default;

pdsch_fused_ratematcher_modulator::~pdsch_fused_ratematcher_modulator() = default;

bool pdsch_fused_ratematcher_modulator::is_valid() const
{
  return state != nullptr;
}

cuda_result pdsch_fused_ratematcher_modulator::configure(const codeblock_metadata&          cb,
                                                         const pdsch_fused_transport_block& tb)
{
  int base_graph = (cb.tb_common.base_graph == ldpc_base_graph_type::BG1) ? 1 : 2;
  int Z          = static_cast<int>(cb.tb_common.lifting_size);
  if (Z <= 0) {
    return make_unexpected(std::string("The code block description carries an invalid lifting size"));
  }

  int bits_per_symbol = static_cast<int>(cb.tb_common.mod);
  if ((bits_per_symbol != 2) && (bits_per_symbol != 4) && (bits_per_symbol != 6) && (bits_per_symbol != 8)) {
    return make_unexpected("Modulation scheme " + to_string(cb.tb_common.mod) +
                           " is not implemented by the fused rate matcher and modulator");
  }

  if ((tb.nof_codeblocks == 0) || (tb.nof_short_codeblocks > tb.nof_codeblocks) || (tb.long_length == 0)) {
    return make_unexpected(std::string("The transport block description is invalid"));
  }

  unsigned nof_bits =
      tb.nof_short_codeblocks * tb.short_length + (tb.nof_codeblocks - tb.nof_short_codeblocks) * tb.long_length;
  if ((nof_bits % static_cast<unsigned>(bits_per_symbol)) != 0) {
    return make_unexpected(
        std::string("The rate matched length of the transport block is not a multiple of the modulation order"));
  }

  int Kb       = (base_graph == 1) ? 22 : 10;
  int nof_cols = (base_graph == 1) ? 68 : 52;

  fused_geometry& g = state->geometry;
  g.N_full          = nof_cols * Z;
  g.puncture_offset = 2 * Z;
  g.N_cb            = (nof_cols - 2) * Z;
  g.F               = static_cast<int>(cb.cb_specific.nof_filler_bits);
  // The kernels count the information bits from the start of the circular buffer, not from the
  // start of the rate matcher and modulator output.
  g.Kd              = Kb * Z - g.F - g.puncture_offset;
  g.encoded_stride  = static_cast<int>(tb.encoded_word_stride);
  g.nof_short       = static_cast<int>(tb.nof_short_codeblocks);
  g.E_short         = static_cast<int>(tb.short_length);
  g.E_long          = static_cast<int>(tb.long_length);
  g.nof_cbs         = static_cast<int>(tb.nof_codeblocks);
  g.bits_per_symbol = bits_per_symbol;
  g.nof_symbols     = static_cast<int>(nof_bits) / bits_per_symbol;

  // A limited buffer shortens the circular buffer the selection wraps around, and the starting
  // position with it. The kernels take the same length, so the selection and k0 agree; the output
  // layout uses N_full and does not change.
  if (cb.tb_common.Nref > 0) {
    g.N_cb = std::min(static_cast<int>(cb.tb_common.Nref), g.N_cb);
  }
  g.k0 = starting_position(base_graph, Z, cb.tb_common.rv, g.N_cb);

  state->configured = true;

  return {};
}

unsigned pdsch_fused_ratematcher_modulator::get_nof_symbols() const
{
  return static_cast<unsigned>(state->geometry.nof_symbols);
}

cuda_result pdsch_fused_ratematcher_modulator::map_to_symbols(int8_t*            d_symbols,
                                                              const uint32_t*    d_encoded,
                                                              uint32_t           c_init,
                                                              const cuda_stream& stream)
{
  ::cudaStream_t native_stream = static_cast<::cudaStream_t>(stream.native());

  if ((d_encoded == nullptr) || (d_symbols == nullptr)) {
    return make_unexpected(std::string("The fused mapping call carries an invalid configuration"));
  }
  if (!state->configured) {
    return make_unexpected(std::string("The fused rate matcher and modulator is not configured"));
  }

  const fused_geometry& g      = state->geometry;
  unsigned              blocks = blocks_for(g.nof_symbols);

#define OCUDU_FUSED_ARGS                                                                                               \
  d_encoded, c_init, d_symbols, g.N_cb, g.N_full, g.k0, g.E_long, g.Kd, g.F, g.puncture_offset, g.nof_cbs,             \
      g.encoded_stride, g.nof_short, g.E_short, g.E_long, g.nof_symbols

  switch (g.bits_per_symbol) {
    case 2:
      fused_rm_interleave_scramble_qpsk_int8_kernel<<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 4:
      fused_rm_interleave_scramble_16qam_int8_kernel<<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 6:
      fused_rm_interleave_scramble_64qam_int8_kernel<<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    default:
      fused_rm_interleave_scramble_256qam_int8_kernel<<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
  }
#undef OCUDU_FUSED_ARGS

  return check_last_cuda_error("fused mapping to symbols");
}

cuda_result pdsch_fused_ratematcher_modulator::map_to_symbols(int8_t*            d_symbols,
                                                              const uint32_t*    d_encoded,
                                                              const uint32_t*    d_sequence,
                                                              const cuda_stream& stream)
{
  ::cudaStream_t native_stream = static_cast<::cudaStream_t>(stream.native());

  if ((d_encoded == nullptr) || (d_sequence == nullptr) || (d_symbols == nullptr)) {
    return make_unexpected(std::string("The fused mapping call carries an invalid configuration"));
  }
  if (!state->configured) {
    return make_unexpected(std::string("The fused rate matcher and modulator is not configured"));
  }

  const fused_geometry& g      = state->geometry;
  unsigned              blocks = blocks_for(g.nof_symbols);

  // A single transport block has its own kernel, which carries no indexing for a batch dimension
  // that is not there.
#define OCUDU_FUSED_ARGS                                                                                               \
  d_encoded, d_sequence, d_symbols, g.N_cb, g.N_full, g.k0, g.E_long, g.Kd, g.F, g.puncture_offset, g.nof_cbs,         \
      g.encoded_stride, g.nof_short, g.E_short, g.E_long, g.nof_symbols

  switch (g.bits_per_symbol) {
    case 2:
      fused_rm_interleave_scramble_precomputed_int8_kernel<2>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 4:
      fused_rm_interleave_scramble_precomputed_int8_kernel<4>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 6:
      fused_rm_interleave_scramble_precomputed_int8_kernel<6>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    default:
      fused_rm_interleave_scramble_precomputed_int8_kernel<8>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
  }
#undef OCUDU_FUSED_ARGS

  return check_last_cuda_error("fused mapping to symbols");
}

cuda_result pdsch_fused_ratematcher_modulator::map_to_symbols(int8_t*            d_symbols,
                                                              const uint32_t*    d_encoded,
                                                              const uint32_t*    d_sequence,
                                                              unsigned           nof_transport_blocks,
                                                              const cuda_stream& stream)
{
  ::cudaStream_t native_stream = static_cast<::cudaStream_t>(stream.native());

  if ((d_encoded == nullptr) || (d_sequence == nullptr) || (d_symbols == nullptr) || (nof_transport_blocks == 0)) {
    return make_unexpected(std::string("The fused mapping call carries an invalid configuration"));
  }
  if (!state->configured) {
    return make_unexpected(std::string("The fused rate matcher and modulator is not configured"));
  }

  const fused_geometry& g      = state->geometry;
  unsigned              blocks = blocks_for(g.nof_symbols * nof_transport_blocks);

#define OCUDU_FUSED_ARGS                                                                                               \
  d_encoded, d_sequence, d_symbols, g.N_cb, g.N_full, g.k0, g.E_long, g.Kd, g.F, g.puncture_offset, g.nof_cbs,         \
      g.encoded_stride, g.nof_short, g.E_short, g.E_long, g.nof_symbols, static_cast<int>(nof_transport_blocks)

  switch (g.bits_per_symbol) {
    case 2:
      fused_rm_interleave_scramble_precomputed_batch_int8_kernel<2>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 4:
      fused_rm_interleave_scramble_precomputed_batch_int8_kernel<4>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 6:
      fused_rm_interleave_scramble_precomputed_batch_int8_kernel<6>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    default:
      fused_rm_interleave_scramble_precomputed_batch_int8_kernel<8>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
  }
#undef OCUDU_FUSED_ARGS

  return check_last_cuda_error("fused mapping to symbols");
}

cuda_result pdsch_fused_ratematcher_modulator::map_to_grid(uint16_t*           d_grid,
                                                           const uint32_t*     d_encoded,
                                                           const uint32_t*     d_sequence,
                                                           const grid_mapping& mapping,
                                                           const cuda_stream&  stream)
{
  ::cudaStream_t native_stream = static_cast<::cudaStream_t>(stream.native());

  if ((d_encoded == nullptr) || (d_sequence == nullptr) || (d_grid == nullptr) ||
      (mapping.d_resource_element_offsets == nullptr) || (mapping.nof_resource_elements == 0)) {
    return make_unexpected(std::string("The fused mapping call carries an invalid configuration"));
  }
  if (!state->configured) {
    return make_unexpected(std::string("The fused rate matcher and modulator is not configured"));
  }
  if ((mapping.nof_layers != 1) && (mapping.nof_layers != mapping.nof_ports)) {
    return make_unexpected(
        std::string("The fused rate matcher and modulator maps either 1 layer to every port, or 1 layer to each port"));
  }

  const fused_geometry& g      = state->geometry;
  unsigned              blocks = blocks_for(mapping.nof_resource_elements);

#define OCUDU_FUSED_ARGS                                                                                               \
  d_encoded, d_sequence, mapping.d_resource_element_offsets, d_grid, g.N_cb, g.N_full, g.k0, g.E_long, g.Kd, g.F,      \
      g.puncture_offset, g.nof_cbs, g.encoded_stride, g.nof_short, g.E_short, g.E_long, g.nof_symbols,                 \
      static_cast<int>(mapping.symbol_offset), static_cast<int>(mapping.nof_resource_elements),                        \
      static_cast<int>(mapping.nof_ports), static_cast<int>(mapping.nof_layers),                                       \
      static_cast<int>(mapping.nof_grid_resource_elements_per_port), mapping.weight

  switch (g.bits_per_symbol) {
    case 2:
      fused_rm_interleave_scramble_precomputed_bf16_grid_kernel<2>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 4:
      fused_rm_interleave_scramble_precomputed_bf16_grid_kernel<4>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    case 6:
      fused_rm_interleave_scramble_precomputed_bf16_grid_kernel<6>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
    default:
      fused_rm_interleave_scramble_precomputed_bf16_grid_kernel<8>
          <<<blocks, block_size, 0, native_stream>>>(OCUDU_FUSED_ARGS);
      break;
  }
#undef OCUDU_FUSED_ARGS

  return check_last_cuda_error("fused mapping to the resource grid");
}

} // namespace cuda
} // namespace ocudu
