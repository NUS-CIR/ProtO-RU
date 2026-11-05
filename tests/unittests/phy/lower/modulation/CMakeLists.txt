# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.


set_directory_properties(PROPERTIES LABELS "phy")

add_executable(ofdm_modulator_unittest ofdm_modulator_unittest.cpp)
target_include_directories(ofdm_modulator_unittest PRIVATE ${CMAKE_SOURCE_DIR}/tests/unittests)
target_link_libraries(ofdm_modulator_unittest ocudulog ocudu_lower_phy_modulation GTest::gtest GTest::gtest_main)
add_test(ofdm_modulator_unittest ofdm_modulator_unittest)

add_executable(ofdm_demodulator_unittest ofdm_demodulator_unittest.cpp)
target_include_directories(ofdm_demodulator_unittest PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(ofdm_demodulator_unittest ocudulog ocudu_lower_phy_modulation GTest::gtest ocudu_gtest_main)
add_test(ofdm_demodulator_unittest ofdm_demodulator_unittest)

add_executable(ofdm_demodulator_pool_unittest ofdm_demodulator_pool_unittest.cpp)
target_include_directories(ofdm_demodulator_pool_unittest PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(ofdm_demodulator_pool_unittest ocudulog ocudu_lower_phy_modulation GTest::gtest ocudu_gtest_main)
add_test(ofdm_demodulator_pool_unittest ofdm_demodulator_pool_unittest)
