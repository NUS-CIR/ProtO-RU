# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.


set_directory_properties(PROPERTIES LABELS "ofh")

set (SOURCES
        ru_emulator_appconfig_yaml_writer.cpp
        ru_emulator_cli11_schema.cpp
        ru_emulator.cpp
        ru_emulator_rx_window_checker.cpp
        ru_emulator_sdr.cpp
        ru_emulator_sdr_upper_phy.cpp
        ru_emulator_stats_collector.cpp
        ru_emulator_timing_notifier.cpp
        ru_emulator_tti_orchestrator.cpp
        ru_emulator_upper_phy.cpp)

add_executable(ru_emulator ${SOURCES})
target_include_directories(ru_emulator PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external)
target_link_libraries(ru_emulator ocudulog ocudu_ofh ocudu_ofh_ru ocudu_phy_support ocudu_ru_sdr ocudu_radio
        ocudu_lower_phy ocudu_support ocudu_yaml_util)

if (DPDK_FOUND)
    target_compile_options(ru_emulator PRIVATE ${DPDK_CFLAGS})
    target_link_libraries(ru_emulator hal_dpdk ${DPDK_LIBRARIES})
    add_definitions(-DDPDK_FOUND)
endif (DPDK_FOUND)

install(TARGETS ru_emulator
        RUNTIME)
