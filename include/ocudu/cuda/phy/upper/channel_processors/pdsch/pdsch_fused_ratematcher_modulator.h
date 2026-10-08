// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/// \file
/// \brief The downlink shared channel transmit chain, from an encoded code block to symbols, in 1
/// kernel.

#pragma once

#include "ocudu/cuda/adt/cuda_error.h"
#include "ocudu/cuda/adt/cuda_stream.h"
#include "ocudu/phy/upper/codeblock_metadata.h"
#include <cstdint>
#include <cuda_runtime.h>
#include <memory>

namespace ocudu {
namespace cuda {

/// \brief Shape of the code blocks a transport block is split into.
///
/// TS 38.212, Section 5.4.2.1 gives the code blocks of a transport block 1 of 2 rate matched
/// lengths, so that their total matches the allocation exactly. The shorter length goes to the
/// first \c nof_short_codeblocks of them.
struct pdsch_fused_transport_block {
  /// Number of code blocks the transport block is split into.
  unsigned nof_codeblocks = 0;
  /// Number of code blocks that carry the shorter rate matched length.
  unsigned nof_short_codeblocks = 0;
  /// Rate matched length of a short code block, in bits.
  unsigned short_length = 0;
  /// Rate matched length of a long code block, in bits.
  unsigned long_length = 0;
  /// Number of 32-bit words between the encoded code blocks at the input.
  unsigned encoded_word_stride = 0;
};

/// \brief Turns encoded code blocks into modulation symbols in 1 kernel.
///
/// A thread walks 1 output symbol back to the bits that make it: it selects them from the circular
/// buffer as TS 38.212, Section 5.4.2 prescribes, places them as the interleaver of Section 5.4.2.2
/// does, applies the scrambling of TS 38.211, Section 7.3.1.1, and maps the result to a
/// constellation point. Fusing the 4 stages spares the buffers that would hold the rate matched
/// bits and the scrambled bits, and the launches that would write them.
///
/// The scrambling bits come from either of 2 places, and which 1 a caller picks is a trade. Passing
/// a sequence initialiser has each thread generate the bits it needs where it stands, so no
/// sequence buffer is ever written; the generator that does so is a second implementation of the
/// one in `ocudu/cuda/phy/upper/sequence_generators/`. Passing a sequence instead reuses what that
/// generator produced, at the cost of the buffer holding it.
///
/// The entry points take and return device memory and queue their work on a stream. None of them
/// synchronises, so a caller reads the output only after the stream has run.
class pdsch_fused_ratematcher_modulator
{
public:
  /// \brief Creates a rate matcher and modulator.
  ///
  /// The first call uploads the jump matrices that generating a sequence reads from constant
  /// memory, so it costs more than the calls that follow.
  ///
  /// \return A rate matcher and modulator, or a description of the error.
  static cuda_expected<pdsch_fused_ratematcher_modulator> create();

  /// Creates an empty rate matcher and modulator that holds no state.
  pdsch_fused_ratematcher_modulator();

  ~pdsch_fused_ratematcher_modulator();

  pdsch_fused_ratematcher_modulator(const pdsch_fused_ratematcher_modulator&)            = delete;
  pdsch_fused_ratematcher_modulator& operator=(const pdsch_fused_ratematcher_modulator&) = delete;

  pdsch_fused_ratematcher_modulator(pdsch_fused_ratematcher_modulator&& other) noexcept;
  pdsch_fused_ratematcher_modulator& operator=(pdsch_fused_ratematcher_modulator&& other) noexcept;

  /// Returns true when it holds state.
  bool is_valid() const;

  /// \brief Sets the shape the next calls map.
  ///
  /// The base graph, the lifting size, the redundancy version, the number of filler bits and the
  /// modulation scheme come from \c cb, which is the same description the software rate matcher
  /// takes. The lengths that vary across the code blocks of a transport block come from \c tb.
  ///
  /// \param[in] cb Code block description.
  /// \param[in] tb Shape of the code blocks of the transport block.
  /// \return A successful result, or a description of the error.
  cuda_result configure(const codeblock_metadata& cb, const pdsch_fused_transport_block& tb);

  /// Number of symbols a transport block produces, as configured.
  unsigned get_nof_symbols() const;

  /// \brief Maps a transport block, generating the scrambling sequence as it goes.
  ///
  /// \param[out] d_symbols Symbols, in device memory. Holds 2 bytes per symbol, the real part
  ///                       first, on the unnormalised constellation.
  /// \param[in]  d_encoded Encoded code blocks, in device memory, at the configured stride.
  /// \param[in]  c_init    Sequence initialiser, as per TS 38.211, Section 7.3.1.1.
  /// \param[in]  stream    Stream the work is queued on.
  /// \return A successful result, or a description of the error.
  cuda_result map_to_symbols(int8_t* d_symbols, const uint32_t* d_encoded, uint32_t c_init, const cuda_stream& stream);

  /// \brief Maps a transport block from a sequence prepared in advance.
  ///
  /// \param[out] d_symbols  Symbols, in device memory. Holds 2 bytes per symbol.
  /// \param[in]  d_encoded  Encoded code blocks, in device memory, at the configured stride.
  /// \param[in]  d_sequence Scrambling sequence, in device memory, packed as the sequence
  ///                        generator writes it.
  /// \param[in]  stream     Stream the work is queued on.
  /// \return A successful result, or a description of the error.
  cuda_result
  map_to_symbols(int8_t* d_symbols, const uint32_t* d_encoded, const uint32_t* d_sequence, const cuda_stream& stream);

  /// \brief Maps several transport blocks of the same shape in 1 launch.
  ///
  /// The sequence restarts at every transport block, so they must share their scrambling
  /// parameters.
  ///
  /// \param[out] d_symbols             Symbols of every transport block, in device memory.
  /// \param[in]  d_encoded             Encoded code blocks of every transport block, in device
  ///                                   memory, 1 transport block after another.
  /// \param[in]  d_sequence            Scrambling sequence for 1 transport block, in device memory.
  /// \param[in]  nof_transport_blocks  Number of transport blocks in the batch.
  /// \param[in]  stream                Stream the work is queued on.
  /// \return A successful result, or a description of the error.
  cuda_result map_to_symbols(int8_t*            d_symbols,
                             const uint32_t*    d_encoded,
                             const uint32_t*    d_sequence,
                             unsigned           nof_transport_blocks,
                             const cuda_stream& stream);

  /// \brief Describes where a transport block's symbols land in the resource grid.
  struct grid_mapping {
    /// Offset of each allocated resource element within 1 port, in device memory.
    const uint32_t* d_resource_element_offsets = nullptr;
    /// Index of the first symbol of the slice this call writes.
    unsigned symbol_offset = 0;
    /// Number of resource elements the slice covers.
    unsigned nof_resource_elements = 0;
    /// Number of ports the grid holds.
    unsigned nof_ports = 0;
    /// Number of layers the transport block is mapped to. Either 1, which replicates to every
    /// port, or the number of ports, which maps layer \c i to port \c i.
    unsigned nof_layers = 0;
    /// Number of resource elements 1 port of the grid holds.
    unsigned nof_grid_resource_elements_per_port = 0;
    /// Amplitude the symbols are scaled by on the way in.
    float weight = 1.0F;
  };

  /// \brief Maps a transport block straight into the resource grid, in bfloat16.
  ///
  /// Writing the grid directly spares the buffer that would hold the 8-bit symbols and the pass
  /// that would map them.
  ///
  /// \param[out] d_grid     Resource grid, in device memory, 2 bytes per component.
  /// \param[in]  d_encoded  Encoded code blocks, in device memory, at the configured stride.
  /// \param[in]  d_sequence Scrambling sequence, in device memory.
  /// \param[in]  mapping    Where the symbols land.
  /// \param[in]  stream     Stream the work is queued on.
  /// \return A successful result, or a description of the error.
  cuda_result map_to_grid(uint16_t*           d_grid,
                          const uint32_t*     d_encoded,
                          const uint32_t*     d_sequence,
                          const grid_mapping& mapping,
                          const cuda_stream&  stream);

private:
  struct impl;

  explicit pdsch_fused_ratematcher_modulator(std::unique_ptr<impl> state_);

  std::unique_ptr<impl> state;
};

} // namespace cuda
} // namespace ocudu
