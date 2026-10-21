// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/support/math/endianess_convert.h"
#include <array>
#include <gtest/gtest.h>

using namespace ocudu;

TEST(endianess_convert_test, read_u32_be_orders_most_significant_byte_first)
{
  const std::array<uint8_t, 4> bytes = {0x12, 0x34, 0x56, 0x78};
  ASSERT_EQ(read_u32_be(bytes), 0x12345678U);
}

TEST(endianess_convert_test, read_u32_be_handles_all_bits_set)
{
  const std::array<uint8_t, 4> bytes = {0xff, 0xff, 0xff, 0xff};
  ASSERT_EQ(read_u32_be(bytes), 0xffffffffU);
}

TEST(endianess_convert_test, read_u64_be_orders_most_significant_byte_first)
{
  const std::array<uint8_t, 8> bytes = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
  ASSERT_EQ(read_u64_be(bytes), 0x0123456789abcdefULL);
}

TEST(endianess_convert_test, read_from_subspan_ignores_surrounding_bytes)
{
  const std::array<uint8_t, 12> bytes = {0xaa, 0xaa, 0x00, 0x00, 0x01, 0x00, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb};
  ASSERT_EQ(read_u32_be(span<const uint8_t>(bytes).subspan(2, 4)), 0x100U);
}
