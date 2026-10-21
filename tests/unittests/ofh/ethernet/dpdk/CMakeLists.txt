# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


add_executable(dpdk_ethernet_port_context_test dpdk_ethernet_port_context_test.cpp)
target_compile_options(dpdk_ethernet_port_context_test PRIVATE ${DPDK_CFLAGS})
target_include_directories(dpdk_ethernet_port_context_test PRIVATE ${CMAKE_SOURCE_DIR}/tests/unittests)
target_link_libraries(dpdk_ethernet_port_context_test ocudu_ofh_dpdk_ethernet hal_dpdk ocudulog GTest::gtest ocudu_gtest_main)
gtest_discover_tests(dpdk_ethernet_port_context_test)
