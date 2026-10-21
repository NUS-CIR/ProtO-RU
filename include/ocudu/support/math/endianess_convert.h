// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/support/ocudu_assert.h"
#include <cstdint>
#include <cstring>
#include <endian.h>

namespace ocudu {

/// Reads a big-endian 32-bit unsigned integer from the four bytes in \c bytes.
inline uint32_t read_u32_be(span<const uint8_t> bytes)
{
  ocudu_assert(bytes.size() == sizeof(uint32_t), "Unexpected input size {}.", bytes.size());
  // Copy to an intermediate variable, so the bytes are not read through a mismatched type.
  uint32_t value;
  std::memcpy(&value, bytes.data(), sizeof(value));
  return be32toh(value);
}

/// Reads a big-endian 64-bit unsigned integer from the eight bytes in \c bytes.
inline uint64_t read_u64_be(span<const uint8_t> bytes)
{
  ocudu_assert(bytes.size() == sizeof(uint64_t), "Unexpected input size {}.", bytes.size());
  // Copy to an intermediate variable, so the bytes are not read through a mismatched type.
  uint64_t value;
  std::memcpy(&value, bytes.data(), sizeof(value));
  return be64toh(value);
}

} // namespace ocudu
