# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set_directory_properties(PROPERTIES LABELS "ru")

add_executable(ofh_ru_cplane_scheduling_handler_test ofh_ru_cplane_scheduling_handler_test.cpp)
target_link_libraries(ofh_ru_cplane_scheduling_handler_test ocudu_ofh_ru ocudu_phy_support ocudu_support GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_cplane_scheduling_handler_test)

add_executable(ofh_ru_downlink_rx_window_handler_test ofh_ru_downlink_rx_window_handler_test.cpp)
target_link_libraries(ofh_ru_downlink_rx_window_handler_test ocudu_ofh_ru ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_downlink_rx_window_handler_test)

add_executable(ofh_ru_uplink_scheduling_recorder_test ofh_ru_uplink_scheduling_recorder_test.cpp)
target_link_libraries(ofh_ru_uplink_scheduling_recorder_test ocudu_ofh_ru ocudu_ofh_message_serdes ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_uplink_scheduling_recorder_test)

add_executable(ofh_ru_uplink_symbol_responder_test ofh_ru_uplink_symbol_responder_test.cpp)
target_link_libraries(ofh_ru_uplink_symbol_responder_test ocudu_ofh_ru ocudu_ofh_transmitter ocudu_ofh_message_serdes ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_uplink_symbol_responder_test)

add_executable(ofh_ru_prach_window_responder_test ofh_ru_prach_window_responder_test.cpp)
target_link_libraries(ofh_ru_prach_window_responder_test ocudu_ofh_ru ocudu_ofh_transmitter ocudu_ofh_message_serdes ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_prach_window_responder_test)

add_executable(ofh_ru_message_receiver_test ofh_ru_message_receiver_test.cpp)
target_link_libraries(ofh_ru_message_receiver_test ocudu_ofh_ru ocudu_ofh_ethernet ocudu_ofh_ecpri ocudu_support GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_message_receiver_test)

add_executable(ofh_ru_sector_test ofh_ru_sector_test.cpp)
target_link_libraries(ofh_ru_sector_test ocudu_ofh_ru ocudu_ofh_transmitter ocudu_ofh_receiver ocudu_ofh_message_serdes
        ocudu_ofh_compression ocudu_ofh_ecpri ocudu_ofh_ethernet ocudu_ofh_timing ocudu_phy_support ocudu_support
        ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_sector_test)

add_executable(ru_emulator_upper_phy_test ru_emulator_upper_phy_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_upper_phy.cpp)
target_include_directories(ru_emulator_upper_phy_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_upper_phy_test ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_upper_phy_test)

add_executable(ru_emulator_rx_symbol_adapter_test ru_emulator_rx_symbol_adapter_test.cpp)
target_include_directories(ru_emulator_rx_symbol_adapter_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_rx_symbol_adapter_test ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_rx_symbol_adapter_test)

add_executable(ru_emulator_sdr_upper_phy_test ru_emulator_sdr_upper_phy_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_sdr_upper_phy.cpp)
target_include_directories(ru_emulator_sdr_upper_phy_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_sdr_upper_phy_test ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_sdr_upper_phy_test)

add_executable(ru_emulator_sdr_config_test ru_emulator_sdr_config_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_sdr.cpp)
target_include_directories(ru_emulator_sdr_config_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_sdr_config_test ocudu_ran ocudu_support GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_sdr_config_test)

add_executable(ru_emulator_cli11_schema_test ru_emulator_cli11_schema_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_cli11_schema.cpp)
target_include_directories(ru_emulator_cli11_schema_test PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(ru_emulator_cli11_schema_test ocudulog ocudu_ran ocudu_support ocudu_yaml_util
        GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_cli11_schema_test)

add_executable(ru_emulator_example_configs_test ru_emulator_example_configs_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_cli11_schema.cpp)
target_include_directories(ru_emulator_example_configs_test PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/external)
# This target validates the DPDK configuration schema without initializing a DPDK runtime.
target_compile_definitions(ru_emulator_example_configs_test PRIVATE
        OCUDU_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
        DPDK_FOUND)
target_link_libraries(ru_emulator_example_configs_test ocudulog ocudu_ran ocudu_support ocudu_yaml_util
        GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_example_configs_test)

add_executable(ru_emulator_tti_orchestrator_test ru_emulator_tti_orchestrator_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_tti_orchestrator.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_sdr_upper_phy.cpp)
target_include_directories(ru_emulator_tti_orchestrator_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_tti_orchestrator_test ocudu_phy_support ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_tti_orchestrator_test)

add_executable(ru_emulator_gps_slot_aligner_test ru_emulator_gps_slot_aligner_test.cpp)
target_include_directories(ru_emulator_gps_slot_aligner_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ru_emulator_gps_slot_aligner_test ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_gps_slot_aligner_test)

add_executable(ofh_ru_message_transmitter_test ofh_ru_message_transmitter_test.cpp)
target_link_libraries(ofh_ru_message_transmitter_test ocudu_ofh_ru ocudu_ofh_ethernet ocudu_support ocudu_ran GTest::gtest GTest::gtest_main)
gtest_discover_tests(ofh_ru_message_transmitter_test)

add_executable(ru_emulator_appconfig_yaml_writer_test ru_emulator_appconfig_yaml_writer_test.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_appconfig_yaml_writer.cpp
        ${CMAKE_SOURCE_DIR}/apps/examples/ofh/ru_emulator_cli11_schema.cpp)
target_include_directories(ru_emulator_appconfig_yaml_writer_test PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(ru_emulator_appconfig_yaml_writer_test ocudulog ocudu_ran ocudu_support ocudu_yaml_util
        GTest::gtest GTest::gtest_main)
gtest_discover_tests(ru_emulator_appconfig_yaml_writer_test)
