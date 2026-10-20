# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


find_package(SCTP REQUIRED)

if(SCTP_FOUND)
    sbom_add(
        PACKAGE            "SCTP"
        DOWNLOAD_LOCATION  "https://www.sctp.org/"
        SUPPLIER           "SCTP project"
        LICENSE            "BSD-3-Clause"
    )
endif (SCTP_FOUND)


add_library(ocudu_gateway
        sctp_socket.cpp
        sctp_network_gateway_common_impl.cpp
        sctp_network_client_impl.cpp
        sctp_network_server_impl.cpp
        sctp_dtls.cpp
        sctp_dtls_ssl.cpp
        udp_network_gateway_impl.cpp
        sctp_network_client_factory.cpp
        sctp_network_server_factory.cpp
        udp_network_gateway_factory.cpp
)

if (OPENSSL_HAS_DTLS_SCTP)
	target_compile_definitions(ocudu_gateway PRIVATE OCUDU_HAVE_OPENSSL_DTLS=1)
	target_link_libraries(ocudu_gateway ocudu_support ocudulog OpenSSL::SSL OpenSSL::Crypto ${SCTP_LIBRARIES})
else ()
	target_link_libraries(ocudu_gateway ocudu_support ocudu_instrumentation ocudulog ${SCTP_LIBRARIES})
endif()
