# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        ofh_data_flow_cplane_scheduling_commands_impl.cpp
        ofh_data_flow_uplane_data_impl.cpp
        ofh_downlink_handler_impl.cpp
        ofh_message_transmitter_impl.cpp
        ofh_uplane_fragment_size_calculator.cpp
        ofh_uplink_request_handler_impl.cpp
        ofh_uplink_request_handler_task_dispatcher.cpp
        ofh_transmitter_factories.cpp
        ofh_transmitter_impl.cpp)

add_library(ocudu_ofh_transmitter STATIC ${SOURCES})
target_link_libraries(ocudu_ofh_transmitter
        ocudu_ofh_ecpri
        ocudu_ofh_compression
        ocudu_ofh_message_serdes
        ocudu_ofh_ethernet
        ocudu_ran
        ocudu_instrumentation
        ocudulog
        ocuduvec)
