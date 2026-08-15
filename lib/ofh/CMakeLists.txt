# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


add_subdirectory(compression)
add_subdirectory(ecpri)
add_subdirectory(ethernet)
add_subdirectory(receiver)
add_subdirectory(ru)
add_subdirectory(serdes)
add_subdirectory(timing)
add_subdirectory(transmitter)

add_library(ocudu_ofh ofh_factories.cpp ofh_metrics_collector_impl.cpp ofh_sector_impl.cpp ofh_sector_controller.cpp)
target_link_libraries(ocudu_ofh ocudu_ofh_receiver ocudu_ofh_timing ocudu_ofh_transmitter ocudu_ofh_ethernet)

if (DPDK_FOUND)
    add_definitions(-DDPDK_FOUND)
endif (DPDK_FOUND)
