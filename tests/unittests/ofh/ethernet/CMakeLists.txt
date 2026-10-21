# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set_directory_properties(PROPERTIES LABELS "ethernet")

add_executable(vlan_ethernet_frame_builder_test vlan_ethernet_frame_builder_test.cpp)
target_link_libraries(vlan_ethernet_frame_builder_test ocudu_ofh_ethernet ocudu_support ocudulog GTest::gtest GTest::gtest_main)
gtest_discover_tests(vlan_ethernet_frame_builder_test)

add_executable(vlan_ethernet_frame_decoder_test vlan_ethernet_frame_decoder_test.cpp)
target_link_libraries(vlan_ethernet_frame_decoder_test ocudu_ofh_ethernet ocudu_support ocudulog GTest::gtest GTest::gtest_main)
gtest_discover_tests(vlan_ethernet_frame_decoder_test)

add_executable(ethernet_frame_pool_test ethernet_frame_pool_test.cpp)
target_include_directories(ethernet_frame_pool_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ethernet_frame_pool_test ocudu_ofh_ethernet ocudulog GTest::gtest ocudu_gtest_main)
gtest_discover_tests(ethernet_frame_pool_test)

if (DPDK_FOUND)
    add_subdirectory(dpdk)
endif (DPDK_FOUND)
