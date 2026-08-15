# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        ofh_closed_rx_window_handler.cpp
        ofh_data_flow_uplane_uplink_data_impl.cpp
        ofh_data_flow_uplane_uplink_prach_impl.cpp
        ofh_message_receiver_impl.cpp
        ofh_receiver_controller.cpp
        ofh_ru_cplane_scheduling_dispatcher.cpp
        ofh_ru_rx_cplane_data_flow.cpp
        ofh_ru_rx_uplane_data_flow.cpp
        ofh_receiver_factories.cpp
        ofh_receiver_impl.cpp
        ofh_rx_symbol_reorderer.cpp
        ofh_rx_window_checker.cpp
        ofh_uplane_rx_symbol_data_flow_notifier.cpp
        ofh_uplane_rx_symbol_data_flow_writer.cpp
        ofh_uplane_prach_data_flow_notifier.cpp
        ofh_uplane_prach_symbol_data_flow_writer.cpp)

add_library(ocudu_ofh_receiver STATIC ${SOURCES})
target_include_directories(ocudu_ofh_receiver PRIVATE ${CMAKE_SOURCE_DIR}/lib/ofh)
target_link_libraries(ocudu_ofh_receiver PRIVATE ocudu_ran ocudu_ofh_message_serdes ocudu_ofh_compression
        ocudu_ofh_ethernet ocudu_ofh_ecpri ocudulog ocudu_instrumentation)
