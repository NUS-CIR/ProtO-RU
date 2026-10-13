# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI

set(SOURCES_DIFI
        radio_config_difi_validator.cpp
        radio_difi_context_packet.cpp
        radio_difi_data_packet.cpp
        radio_difi_stream_config.cpp
        radio_difi_udp_socket.cpp
        radio_factory_difi_impl.cpp
        radio_session_difi_impl.cpp
        radio_difi_rx_channel.cpp
        radio_difi_rx_stream.cpp
        radio_difi_tx_channel.cpp
        radio_difi_tx_stream.cpp)

add_library(ocudu_radio_difi STATIC ${SOURCES_DIFI})

target_link_libraries(ocudu_radio_difi fmt ocudulog ocudu_support ocuduvec)

add_to_exported_libs(ocudu_radio_difi)
