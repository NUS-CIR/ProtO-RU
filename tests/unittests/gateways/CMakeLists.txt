# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set_directory_properties(PROPERTIES LABELS "gateways")

add_executable(sctp_socket_test sctp_socket_test.cpp)
target_link_libraries(sctp_socket_test ocudu_gateway ocudulog gtest gtest_main)
target_include_directories(sctp_socket_test PRIVATE ${CMAKE_SOURCE_DIR})
gtest_discover_tests(sctp_socket_test NO_PRETTY_VALUES PROPERTIES "LABELS;sctp")

add_executable(sctp_network_gateway_test
        sctp_network_server_test.cpp
        sctp_network_server_peer_test.cpp
        sctp_network_client_test.cpp
        sctp_network_link_test.cpp)
target_link_libraries(sctp_network_gateway_test ocudu_gateway ocudu_support ocudulog gtest gtest_main)
target_include_directories(sctp_network_gateway_test PRIVATE ${CMAKE_SOURCE_DIR})
gtest_discover_tests(sctp_network_gateway_test NO_PRETTY_VALUES PROPERTIES "LABELS;sctp")

set(SCTP_CERT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/certs")
target_compile_definitions(sctp_network_gateway_test PRIVATE
    TEST_CERT_DIR="${SCTP_CERT_DIR}"
)
if (OPENSSL_HAS_DTLS_SCTP)
        target_compile_definitions(sctp_network_gateway_test PRIVATE
            OCUDU_HAVE_OPENSSL_DTLS=1
        )

        set(SCTP_CERTS
            "${SCTP_CERT_DIR}/ca.key"
            "${SCTP_CERT_DIR}/ca.crt"
            "${SCTP_CERT_DIR}/link12.key"
            "${SCTP_CERT_DIR}/link12.crt"
            "${SCTP_CERT_DIR}/link21.key"
            "${SCTP_CERT_DIR}/link21.crt"
            "${SCTP_CERT_DIR}/link13.key"
            "${SCTP_CERT_DIR}/link13.crt"
            "${SCTP_CERT_DIR}/link31.key"
            "${SCTP_CERT_DIR}/link31.crt"
            "${SCTP_CERT_DIR}/link23.key"
            "${SCTP_CERT_DIR}/link23.crt"
            "${SCTP_CERT_DIR}/link32.key"
            "${SCTP_CERT_DIR}/link32.crt"
        )
        add_custom_command(
            OUTPUT ${SCTP_CERTS}
            COMMAND bash "${SCTP_CERT_DIR}/gen_certificates.sh"
            DEPENDS "${SCTP_CERT_DIR}/gen_certificates.sh"
            WORKING_DIRECTORY "${SCTP_CERT_DIR}"
            COMMENT "Generating SCTP/DTLS test certificates"
            VERBATIM
        )
        add_custom_target(sctp_dtls_certs DEPENDS ${SCTP_CERTS})
        add_dependencies(sctp_network_gateway_test sctp_dtls_certs)
endif()

add_executable(udp_network_gateway_test udp_network_gateway_test.cpp)
target_link_libraries(udp_network_gateway_test ocudu_gateway ocudu_support ocudulog gtest gtest_main)
gtest_discover_tests(udp_network_gateway_test PROPERTIES "LABELS;tsan")

add_executable(udp_network_gateway_pool_depletion_test udp_network_gateway_pool_depletion_test.cpp)
target_link_libraries(udp_network_gateway_pool_depletion_test ocudu_gateway ocudu_support ocudulog gtest gtest_main)
gtest_discover_tests(udp_network_gateway_pool_depletion_test PROPERTIES "LABELS;tsan")

add_executable(udp_network_gateway_benchmark_test udp_network_gateway_benchmark_test.cpp)
target_link_libraries(udp_network_gateway_benchmark_test ocudu_gateway ocudu_support ocudulog gtest gtest_main)
target_include_directories(udp_network_gateway_benchmark_test PRIVATE ${CMAKE_SOURCE_DIR})
gtest_discover_tests(udp_network_gateway_benchmark_test PROPERTIES "LABELS;tsan")
