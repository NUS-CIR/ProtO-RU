# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_executable(cg_scheduler_test configured_grant_scheduler_test.cpp configured_grant_type2_scheduler_test.cpp)
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

add_executable(cg_slot_wheel_test cg_slot_wheel_test.cpp)
target_include_directories(cg_slot_wheel_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(cg_slot_wheel_test ocudu_sched ocudulog gtest ocudu_gtest_main)
gtest_discover_tests(cg_slot_wheel_test)

add_executable(cg_type2_resource_manager_test cg_type2_resource_manager_test.cpp)
target_include_directories(cg_type2_resource_manager_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(cg_type2_resource_manager_test
        ocudu_sched
        ocudulog
        mac_configuration_helpers
        sched_config
        scheduler_test_suite
        sched_test_doubles
        scheduler_test_utils
        gtest
        ocudu_gtest_main)
gtest_discover_tests(cg_type2_resource_manager_test)
