# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        ofh_cplane_message_builder_dynamic_compression_impl.cpp
        ofh_cplane_message_builder_impl.cpp
        ofh_cplane_message_builder_static_compression_impl.cpp
        ofh_cplane_message_decoder_impl.cpp
        ofh_serdes_factories.cpp
        ofh_uplane_message_builder_impl.cpp
        ofh_uplane_message_builder_dynamic_compression_impl.cpp
        ofh_uplane_message_builder_static_compression_impl.cpp
        ofh_uplane_message_decoder_impl.cpp
        ofh_uplane_message_decoder_dynamic_compression_impl.cpp
        ofh_uplane_message_decoder_static_compression_impl.cpp)

add_library(ocudu_ofh_message_serdes STATIC ${SOURCES})
target_link_libraries(ocudu_ofh_message_serdes ocudulog)
