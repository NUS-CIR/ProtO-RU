# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_executable(pdcch_type0_css_occassions_test pdcch/pdcch_type0_css_occassions_test.cpp)
target_link_libraries(pdcch_type0_css_occassions_test ocudu_sched ocudulog gtest gtest_main)
add_test(pdcch_type0_css_occassions_test pdcch_type0_css_occassions_test)

add_executable(pdsch_default_time_allocation_test pdsch/pdsch_default_time_allocation_test.cpp)
target_link_libraries(pdsch_default_time_allocation_test ocudu_sched ocudulog gtest gtest_main)
add_test(pdsch_default_time_allocation_test pdsch_default_time_allocation_test)

add_executable(pucch_support_test
        pucch/pucch_collision_info_test.cpp
        pucch/pucch_default_resource_test.cpp)
target_link_libraries(pucch_support_test ocudu_sched ocudulog gtest gtest_main)
gtest_discover_tests(pucch_support_test)

add_executable(pusch_default_time_allocation_test pusch/pusch_default_time_allocation_test.cpp)
target_link_libraries(pusch_default_time_allocation_test ocudu_sched ocudulog gtest gtest_main)
add_test(pusch_default_time_allocation_test pusch_default_time_allocation_test)

add_executable(pusch_td_resource_indices_test pusch/pusch_td_resource_indices_test.cpp)
target_link_libraries(pusch_td_resource_indices_test ocudu_sched sched_test_doubles scheduler_test_utils ocudulog gtest gtest_main)
add_test(pusch_td_resource_indices_test pusch_td_resource_indices_test)

add_executable(prbs_calculator_test prbs_calculator_test.cpp)
target_link_libraries(prbs_calculator_test ocudu_sched ocudulog gtest gtest_main)
add_test(prbs_calculator_test prbs_calculator_test)

add_executable(csi_rs_helpers_test csi_rs_helpers_test.cpp)
target_link_libraries(csi_rs_helpers_test ocudu_sched ocudulog GTest::gtest ocudu_gtest_main)
gtest_discover_tests(csi_rs_helpers_test)

add_executable(dmrs_helpers_test dmrs_helpers_test.cpp)
target_link_libraries(dmrs_helpers_test ocudu_sched ocudulog GTest::gtest ocudu_gtest_main)
gtest_discover_tests(dmrs_helpers_test)

add_executable(mcs_calculator_test mcs_calculator_test.cpp)
target_link_libraries(mcs_calculator_test ocudu_sched gtest gtest_main)
add_test(mcs_calculator_test.cpp mcs_calculator_test)

add_executable(mcs_tbs_calculator_test mcs_tbs_calculator_test.cpp)
target_link_libraries(mcs_tbs_calculator_test
        ocudu_sched
        scheduler_test_suite
        mac_configuration_helpers
        ocudu_du_manager
        gtest
        gtest_main)
add_test(mcs_tbs_calculator_test mcs_tbs_calculator_test)

add_executable(pdcch_aggregation_level_calculator_test pdcch_aggregation_level_calculator_test.cpp)
target_link_libraries(pdcch_aggregation_level_calculator_test ocudu_sched gtest gtest_main)
gtest_discover_tests(pdcch_aggregation_level_calculator_test)
