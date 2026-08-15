# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        ofh_ru_cplane_scheduling_handler.cpp
        ofh_ru_downlink_rx_window_handler.cpp
        ofh_ru_message_receiver.cpp
        ofh_ru_message_transmitter.cpp
        ofh_ru_prach_window_responder.cpp
        ofh_ru_sector.cpp
        ofh_ru_uplink_scheduling_recorder.cpp
        ofh_ru_uplink_symbol_responder.cpp)

add_library(ocudu_ofh_ru STATIC ${SOURCES})
target_include_directories(ocudu_ofh_ru PRIVATE ${CMAKE_SOURCE_DIR}/lib/ofh)
target_link_libraries(ocudu_ofh_ru PRIVATE ocudu_ran ocudu_ofh_ethernet ocudu_ofh_ecpri ocudu_ofh_compression
        ocudu_ofh_message_serdes ocudu_ofh_transmitter ocudu_ofh_receiver ocudu_ofh_timing ocudulog)
