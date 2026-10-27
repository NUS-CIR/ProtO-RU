// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "split6_o_du_impl.h"
#include "ocudu/fapi_adaptor/mac/mac_fapi_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/mac_fapi_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/p5/mac_fapi_p5_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/p7/mac_fapi_p7_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p5/phy_fapi_p5_sector_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p7/phy_fapi_p7_sector_adaptor.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;

split6_o_du_impl::split6_o_du_impl(unsigned                                        nof_cells_,
                                   std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> adaptor_,
                                   std::unique_ptr<odu::o_du_high>                 odu_hi_) :
  nof_cells(nof_cells_), odu_hi(std::move(odu_hi_)), adaptor(std::move(adaptor_))
{
  ocudu_assert(odu_hi, "Invalid O-DU high");
  ocudu_assert(adaptor, "Invalid PHY-FAPI adaptor");

  slot_adaptors.reserve(nof_cells);

  for (unsigned i = 0; i != nof_cells; ++i) {
    auto& mac_sector_adaptor    = odu_hi->get_mac_fapi_fastpath_adaptor().get_sector_adaptor(i);
    auto& p5_mac_sector_adaptor = mac_sector_adaptor.get_p5_sector_fastpath_adaptor();
    auto& p7_mac_sector_adaptor = mac_sector_adaptor.get_p7_sector_adaptor();
    auto& p5_phy_sector_adaptor = adaptor->get_sector_adaptor(i).get_p5_sector_adaptor();
    auto& p7_phy_sector_adaptor = adaptor->get_sector_adaptor(i).get_p7_sector_adaptor();

    auto& slot_extender_adaptor = slot_adaptors.emplace_back(p7_mac_sector_adaptor.get_p7_slot_indication_notifier());
    p7_phy_sector_adaptor.set_p7_indications_notifier(p7_mac_sector_adaptor.get_p7_indications_notifier());
    p7_phy_sector_adaptor.set_error_indication_notifier(p7_mac_sector_adaptor.get_error_indication_notifier());
    p7_phy_sector_adaptor.set_p7_slot_indication_notifier(slot_extender_adaptor);

    p5_phy_sector_adaptor.set_p5_responses_notifier(p5_mac_sector_adaptor.get_p5_responses_notifier());
    p5_phy_sector_adaptor.set_error_indication_notifier(p5_mac_sector_adaptor.get_error_indication_notifier());
  }
}

void split6_o_du_impl::start()
{
  // Start the PHY adaptor before DU-high can activate cells after F1 setup.
  for (unsigned i = 0; i != nof_cells; ++i) {
    adaptor->get_sector_adaptor(i).start();
  }

  odu_hi->get_operation_controller().start();
}

void split6_o_du_impl::stop()
{
  odu_hi->get_operation_controller().stop();

  for (unsigned i = 0; i != nof_cells; ++i) {
    adaptor->get_sector_adaptor(i).stop();
  }
}
