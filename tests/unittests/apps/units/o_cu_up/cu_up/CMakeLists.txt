# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.


set_directory_properties(PROPERTIES LABELS "cu_up;apps")

add_executable(cu_up_unit_ngu_socket_config_test cu_up_unit_ngu_socket_config_test.cpp)
target_include_directories(cu_up_unit_ngu_socket_config_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(cu_up_unit_ngu_socket_config_test
        ocudu_cu_up_app_unit
        ocudu_app_services
        ocudu_yaml_util
        ocudu_gtest_main
        gtest)
add_test(cu_up_unit_ngu_socket_config_test cu_up_unit_ngu_socket_config_test)
