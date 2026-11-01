# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

# ASN1 utils
add_library(asn1_utils STATIC asn1_utils.cpp)
target_compile_options(asn1_utils PRIVATE -fno-exceptions)
target_link_libraries(asn1_utils ocudulog ocudu_support)

# RRC NR ASN1
add_subdirectory(rrc_nr)

# F1AP ASN1
add_library(f1ap_asn1 STATIC f1ap/common.cpp f1ap/f1ap_ies.cpp f1ap/f1ap_pdu_items.cpp f1ap/f1ap_pdu_contents_ue.cpp
        f1ap/f1ap_pdu_contents.cpp f1ap/f1ap.cpp f1ap/f1ap_lists.cpp)
target_compile_options(f1ap_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(f1ap_asn1 asn1_utils)

# E1AP ASN1
add_library(e1ap_asn1 STATIC e1ap/e1ap.cpp e1ap/e1ap_ies.cpp e1ap/e1ap_pdu_contents.cpp e1ap/e1ap_lists.cpp)
target_compile_options(e1ap_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(e1ap_asn1 asn1_utils)

# E2AP ASN1
add_library(e2ap_asn1 STATIC e2ap/common.cpp e2ap/e2ap_ies.cpp e2ap/e2ap_pdu_contents.cpp e2ap/e2ap.cpp e2ap/e2ap_lists.cpp e2sm/common.cpp e2sm/e2sm_common_ies.cpp e2sm/e2sm_kpm_ies.cpp e2sm/e2sm.cpp e2sm/e2sm_lists.cpp e2sm/e2sm_ccc.cpp)
target_compile_options(e2ap_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(e2ap_asn1 asn1_utils)

# NGAP ASN1
add_library(ngap_asn1 STATIC ngap/common.cpp ngap/ngap_ies.cpp ngap/ngap_pdu_contents.cpp ngap/ngap.cpp
        ngap/ngap_lists.cpp)
target_compile_options(ngap_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(ngap_asn1 asn1_utils)

# NRPPA ASN1
add_library(nrppa_asn1 STATIC nrppa/common.cpp nrppa/nrppa_ies.cpp nrppa/nrppa_pdu_contents.cpp nrppa/nrppa.cpp
        nrppa/nrppa_lists.cpp)
target_compile_options(nrppa_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(nrppa_asn1 asn1_utils)

# XnAP ASN1
add_library(xnap_asn1 STATIC xnap/common.cpp xnap/xnap_ies.cpp xnap/xnap_pdu_contents.cpp xnap/xnap.cpp
        xnap/xnap_lists.cpp)
target_compile_options(xnap_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(xnap_asn1 asn1_utils)

# LPP ASN1
add_library(lpp_asn1 STATIC lpp/lpp.cpp)
target_compile_options(lpp_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(lpp_asn1 asn1_utils)
