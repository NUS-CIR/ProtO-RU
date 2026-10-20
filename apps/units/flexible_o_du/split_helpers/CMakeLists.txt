# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_subdirectory(metrics)

set(SOURCES
        fapi_dummy_o_du_factory.cpp
        fapi_dummy_o_du_impl.cpp
        flexible_o_du_factory.cpp
        flexible_o_du_impl.cpp
        flexible_o_du_metrics_aggregator.cpp
        ntn_ru_doppler_adapter.cpp
        commands/ntn_config_update_remote_command.cpp)

add_library(ocudu_flexible_o_du_helpers STATIC ${SOURCES})
target_include_directories(ocudu_flexible_o_du_helpers PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ocudu_flexible_o_du_helpers
        ocudu_o_du
        ocudu_phy_dummy_fapi_adaptor
        ocudu_pcap
        ocudu_app_services
        ocudu_o_du_low_unit_helpers
        ocudu_o_du_high_unit_helpers
        ocudu_flexible_o_du_unit_metrics_helpers)
