# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_executable(cg_scheduler_test configured_grant_scheduler_test.cpp configured_grant_type2_scheduler_test.cpp cg_slot_wheel_test.cpp cg_type2_resource_manager_test.cpp)
target_link_libraries(cg_scheduler_test
        ocudu_sched
        ocudulog
        mac_configuration_helpers
        sched_config
        scheduler_test_suite
        sched_test_doubles
        gtest
        ocudu_gtest_main)
gtest_discover_tests(cg_scheduler_test)
