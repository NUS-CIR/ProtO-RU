# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI

add_executable(ru_ofh_config_cli11_schema_test ru_ofh_config_cli11_schema_test.cpp)
target_include_directories(ru_ofh_config_cli11_schema_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_ofh_config_cli11_schema_test PRIVATE
        ocudu_split_7_2_app_unit_helpers
        ocudu_cpu_affinities_helper
        ocudu_support
        ocudu_yaml_util
        GTest::gtest
        GTest::gtest_main)
gtest_discover_tests(ru_ofh_config_cli11_schema_test)
