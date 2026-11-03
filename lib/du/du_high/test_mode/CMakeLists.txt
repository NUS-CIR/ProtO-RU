# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_library(ocudu_du_high_adapters
        du_test_mode_controller.cpp
        f1c_du_test_mode_adapter.cpp
        mac_test_mode_adapter.cpp
        mac_test_mode_helpers.cpp)
target_link_libraries(ocudu_du_high_adapters ocudulog ocudu_support ocudu_du_manager ocudu_mac ocudu_f1ap_du)
