# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI

add_executable(radio_difi_validator_test radio_difi_validator_test.cpp)
target_link_libraries(radio_difi_validator_test ocudu_radio_difi ocudu_support gtest gtest_main)
target_include_directories(radio_difi_validator_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi
        ${CMAKE_SOURCE_DIR}/include)
add_test(radio_difi_validator_test radio_difi_validator_test)

add_executable(radio_difi_udp_socket_test radio_difi_udp_socket_test.cpp)
target_link_libraries(radio_difi_udp_socket_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_udp_socket_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi)
add_test(radio_difi_udp_socket_test radio_difi_udp_socket_test)

add_executable(radio_difi_context_packet_test radio_difi_context_packet_test.cpp)
target_link_libraries(radio_difi_context_packet_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_context_packet_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi)
add_test(radio_difi_context_packet_test radio_difi_context_packet_test)

add_executable(radio_difi_data_packet_test radio_difi_data_packet_test.cpp)
target_link_libraries(radio_difi_data_packet_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_data_packet_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi
        ${CMAKE_SOURCE_DIR}/include)
add_test(radio_difi_data_packet_test radio_difi_data_packet_test)

add_executable(radio_difi_tx_stream_loopback_test radio_difi_tx_stream_loopback_test.cpp)
target_link_libraries(radio_difi_tx_stream_loopback_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_tx_stream_loopback_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi
        ${CMAKE_SOURCE_DIR}/include)
add_test(radio_difi_tx_stream_loopback_test radio_difi_tx_stream_loopback_test)

add_executable(radio_difi_rx_stream_loopback_test radio_difi_rx_stream_loopback_test.cpp)
target_link_libraries(radio_difi_rx_stream_loopback_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_rx_stream_loopback_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi
        ${CMAKE_SOURCE_DIR}/include)
add_test(radio_difi_rx_stream_loopback_test radio_difi_rx_stream_loopback_test)

add_executable(radio_difi_rx_timing_stats_test radio_difi_rx_timing_stats_test.cpp)
target_link_libraries(radio_difi_rx_timing_stats_test ocudu_radio_difi gtest gtest_main)
target_include_directories(radio_difi_rx_timing_stats_test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/radio/difi)
add_test(radio_difi_rx_timing_stats_test radio_difi_rx_timing_stats_test)
