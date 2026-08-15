# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        ethernet_factories.cpp
        ethernet_frame_builder_impl.cpp
        ethernet_transmitter_impl.cpp
        ethernet_receiver_impl.cpp
        ethernet_receiver_utils.cpp
        ethernet_rx_buffer_impl.cpp
        vlan_ethernet_frame_builder_impl.cpp
        vlan_ethernet_frame_decoder_impl.cpp)

add_library(ocudu_ofh_ethernet STATIC ${SOURCES})
target_link_libraries(ocudu_ofh_ethernet ocudulog ocudu_instrumentation)

if (DPDK_FOUND)
    add_subdirectory(dpdk)
    target_link_libraries(ocudu_ofh_ethernet ocudu_ofh_dpdk_ethernet)
endif (DPDK_FOUND)
