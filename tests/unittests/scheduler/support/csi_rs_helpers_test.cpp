// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/support/csi_rs_helpers.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

constexpr unsigned      nof_prbs = 106;
const crb_interval      all_crbs{0, nof_prbs};
const ofdm_symbol_range pdsch_symbols{1, 14};

/// Returns a bitmap of the CRBs of the given intervals.
crb_bitmap make_crbs(std::initializer_list<crb_interval> intervals)
{
  crb_bitmap crbs(MAX_NOF_PRBS);
  for (const crb_interval& interval : intervals) {
    crbs |= interval;
  }
  return crbs;
}

/// Counts the resource elements of a PDSCH occupied by the given CSI-RS, held in a slot of the scheduler result grid.
unsigned get_nof_csi_rs_re(span<const csi_rs_info> csi_rs_list, ofdm_symbol_range symbols, const crb_bitmap& crbs)
{
  sched_result_grid grid(sched_result_grid_config{});
  auto              csi_rs_pdus = grid.get_builder(0).dl().csi_rs();
  for (const csi_rs_info& csi_rs : csi_rs_list) {
    report_fatal_error_if_not(csi_rs_pdus.emplace_back(csi_rs) != nullptr, "The CSI-RS list of the slot is full");
  }
  return csi_helper::get_nof_csi_rs_re(grid.get_reader(0).dl().csi_rs(), symbols, crbs);
}

/// \brief Single-port CSI-RS in one OFDM symbol.
///
/// Row 1 of TS 38.211, Table 7.4.1.5.3-1, is a TRS resource with three resource elements per PRB. Row 2 has one
/// resource element per PRB, or per every other PRB.
csi_rs_info make_single_port_csi_rs(unsigned                 row,
                                    csi_rs_freq_density_type density,
                                    unsigned                 symbol,
                                    unsigned                 fd_bit,
                                    crb_interval             crbs = all_crbs)
{
  csi_rs_info csi_rs{};
  csi_rs.crbs        = crbs;
  csi_rs.type        = csi_rs_type::CSI_RS_NZP;
  csi_rs.row         = row;
  csi_rs.freq_domain = csi_rs::freq_allocation_mask_type(row == 1 ? 4 : 12);
  csi_rs.freq_domain.set(fd_bit);
  csi_rs.symbol0      = symbol;
  csi_rs.symbol1      = 2;
  csi_rs.cdm_type     = csi_rs_cdm_type::no_CDM;
  csi_rs.freq_density = density;
  return csi_rs;
}

/// TRS of one slot: two OFDM symbols with three resource elements per PRB.
std::vector<csi_rs_info> make_trs_slot()
{
  return {make_single_port_csi_rs(1, csi_rs_freq_density_type::three, 4, 0),
          make_single_port_csi_rs(1, csi_rs_freq_density_type::three, 8, 0)};
}

/// Four-port CSI-RS in one OFDM symbol, with FD-CDM2 (TS 38.211, Table 7.4.1.5.3-1, row 4): four resource elements per
/// PRB.
csi_rs_info make_four_port_csi_rs(unsigned symbol, csi_rs_type type)
{
  csi_rs_info csi_rs{};
  csi_rs.crbs        = all_crbs;
  csi_rs.type        = type;
  csi_rs.row         = 4;
  csi_rs.freq_domain = csi_rs::freq_allocation_mask_type(3);
  csi_rs.freq_domain.set(0);
  csi_rs.symbol0      = symbol;
  csi_rs.symbol1      = 2;
  csi_rs.cdm_type     = csi_rs_cdm_type::fd_CDM2;
  csi_rs.freq_density = csi_rs_freq_density_type::one;
  return csi_rs;
}

} // namespace

TEST(csi_rs_helpers_test, no_csi_rs_occupies_no_resource_elements)
{
  ASSERT_EQ(get_nof_csi_rs_re({}, pdsch_symbols, make_crbs({all_crbs})), 0);
}

TEST(csi_rs_helpers_test, trs_occupies_three_resource_elements_per_prb_and_symbol)
{
  ASSERT_EQ(get_nof_csi_rs_re(make_trs_slot(), pdsch_symbols, make_crbs({all_crbs})), nof_prbs * 2 * 3);
}

TEST(csi_rs_helpers_test, all_the_ports_of_a_csi_rs_are_counted)
{
  const std::vector<csi_rs_info> csi_rs_list = {make_four_port_csi_rs(12, csi_rs_type::CSI_RS_NZP)};
  ASSERT_EQ(get_nof_csi_rs_re(csi_rs_list, pdsch_symbols, make_crbs({all_crbs})), nof_prbs * 4);
}

TEST(csi_rs_helpers_test, only_the_pdsch_symbols_and_crbs_are_counted)
{
  // The PDSCH starts after the first TRS symbol.
  ASSERT_EQ(get_nof_csi_rs_re(make_trs_slot(), {5, 14}, make_crbs({all_crbs})), nof_prbs * 3);

  // The PDSCH covers part of the CRBs of a CSI-RS that covers part of the band.
  const std::vector<csi_rs_info> csi_rs_list = {
      make_single_port_csi_rs(1, csi_rs_freq_density_type::three, 4, 0, crb_interval{0, 52})};
  ASSERT_EQ(get_nof_csi_rs_re(csi_rs_list, pdsch_symbols, make_crbs({{40, nof_prbs}})), 12 * 3);
}

TEST(csi_rs_helpers_test, only_the_crbs_of_a_non_contiguous_pdsch_are_counted)
{
  // Resource allocation type 0: the PDSCH occupies whole RBGs, not necessarily contiguous.
  const crb_bitmap crbs = make_crbs({{0, 8}, {24, 32}, {96, nof_prbs}});
  ASSERT_EQ(get_nof_csi_rs_re(make_trs_slot(), pdsch_symbols, crbs), crbs.count() * 2 * 3);

  // With a density of 1/2, only the even CRBs of the allocation carry the CSI-RS.
  const std::vector<csi_rs_info> csi_rs_list = {
      make_single_port_csi_rs(2, csi_rs_freq_density_type::dot5_even_RB, 4, 0)};
  ASSERT_EQ(get_nof_csi_rs_re(csi_rs_list, pdsch_symbols, make_crbs({{3, 8}, {25, 30}})), 2 + 2);
}

TEST(csi_rs_helpers_test, resource_elements_shared_by_several_csi_rs_are_counted_once)
{
  // An NZP-CSI-RS and a ZP-CSI-RS on the same resource elements.
  std::vector<csi_rs_info> csi_rs_list = {make_four_port_csi_rs(12, csi_rs_type::CSI_RS_NZP),
                                          make_four_port_csi_rs(12, csi_rs_type::CSI_RS_ZP)};
  ASSERT_EQ(get_nof_csi_rs_re(csi_rs_list, pdsch_symbols, make_crbs({all_crbs})), nof_prbs * 4);

  // A CSI-RS on other resource elements of the same symbol adds to the count.
  csi_rs_list.push_back(make_single_port_csi_rs(2, csi_rs_freq_density_type::one, 12, 11));
  ASSERT_EQ(get_nof_csi_rs_re(csi_rs_list, pdsch_symbols, make_crbs({all_crbs})), nof_prbs * (4 + 1));
}

TEST(csi_rs_helpers_test, density_one_half_occupies_every_other_prb)
{
  const std::vector<csi_rs_info> even = {make_single_port_csi_rs(2, csi_rs_freq_density_type::dot5_even_RB, 4, 0)};
  ASSERT_EQ(get_nof_csi_rs_re(even, pdsch_symbols, make_crbs({all_crbs})), nof_prbs / 2);

  // An odd allocation start, for both parities.
  const std::vector<csi_rs_info> odd = {make_single_port_csi_rs(2, csi_rs_freq_density_type::dot5_odd_RB, 4, 0)};
  ASSERT_EQ(get_nof_csi_rs_re(even, pdsch_symbols, make_crbs({{1, 6}})), 2);
  ASSERT_EQ(get_nof_csi_rs_re(odd, pdsch_symbols, make_crbs({{1, 6}})), 3);
}
