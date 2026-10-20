// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/du/du.h"
#include "ocudu/du/du_high/o_du_high.h"
#include "ocudu/du/du_operation_controller.h"
#include "ocudu/fapi_adaptor/phy/phy_fapi_adaptor.h"
#include <memory>

namespace ocudu {

/// O-RAN DU made of an O-DU high connected to a FAPI dummy.
class fapi_dummy_o_du_impl : public odu::du, public du_operation_controller
{
public:
  fapi_dummy_o_du_impl(unsigned                                        nof_cells_,
                       std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> fapi_dummy_,
                       std::unique_ptr<odu::o_du_high>                 odu_hi_);

  // See interface for documentation.
  du_operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  void start() override;

  // See interface for documentation.
  void stop() override;

private:
  const unsigned                                  nof_cells;
  std::unique_ptr<odu::o_du_high>                 odu_hi;
  std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> fapi_dummy;
};

} // namespace ocudu
