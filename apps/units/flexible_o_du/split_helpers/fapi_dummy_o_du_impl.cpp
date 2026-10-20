// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "fapi_dummy_o_du_impl.h"
#include "ocudu/fapi_adaptor/mac/mac_fapi_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/mac_fapi_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/p5/mac_fapi_p5_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/mac/p7/mac_fapi_p7_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p5/phy_fapi_p5_sector_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p7/phy_fapi_p7_sector_adaptor.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;

fapi_dummy_o_du_impl::fapi_dummy_o_du_impl(unsigned                                        nof_cells_,
                                           std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> fapi_dummy_,
                                           std::unique_ptr<odu::o_du_high>                 odu_hi_) :
  nof_cells(nof_cells_), odu_hi(std::move(odu_hi_)), fapi_dummy(std::move(fapi_dummy_))
{
  ocudu_assert(odu_hi, "Invalid O-DU high");
  ocudu_assert(fapi_dummy, "Invalid FAPI dummy");

  for (unsigned i = 0; i != nof_cells; ++i) {
    auto& mac_sector = odu_hi->get_mac_fapi_fastpath_adaptor().get_sector_adaptor(i);
    auto& phy_sector = fapi_dummy->get_sector_adaptor(i);

    auto& p5_mac_adaptor = mac_sector.get_p5_sector_fastpath_adaptor();
    auto& p5_phy_adaptor = phy_sector.get_p5_sector_adaptor();
    p5_phy_adaptor.set_p5_responses_notifier(p5_mac_adaptor.get_p5_responses_notifier());
    p5_phy_adaptor.set_error_indication_notifier(p5_mac_adaptor.get_error_indication_notifier());

    auto& p7_mac_adaptor = mac_sector.get_p7_sector_adaptor();
    auto& p7_phy_adaptor = phy_sector.get_p7_sector_adaptor();
    p7_phy_adaptor.set_p7_slot_indication_notifier(p7_mac_adaptor.get_p7_slot_indication_notifier());
    p7_phy_adaptor.set_error_indication_notifier(p7_mac_adaptor.get_error_indication_notifier());
    p7_phy_adaptor.set_p7_indications_notifier(p7_mac_adaptor.get_p7_indications_notifier());
  }
}

void fapi_dummy_o_du_impl::start()
{
  odu_hi->get_operation_controller().start();
}

void fapi_dummy_o_du_impl::stop()
{
  odu_hi->get_operation_controller().stop();
}
