# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

set(SOURCES
        gnb_cu_configuration_update_procedure.cpp
        f1ap_du_setup_procedure.cpp
        f1ap_du_removal_procedure.cpp
        f1ap_du_ue_context_setup_procedure.cpp
        f1ap_du_ue_context_modification_procedure.cpp
        f1ap_du_ue_context_release_procedure.cpp
        f1ap_du_reset_procedure.cpp
        f1ap_du_initiated_reset_procedure.cpp
        f1ap_du_gnbdu_config_update_procedure.cpp
        f1ap_du_positioning_information_exchange_procedure.cpp
        f1ap_du_trp_information_exchange_procedure.cpp
        f1ap_du_e_cid_measurement_initiation_procedure.cpp
        f1ap_du_e_cid_measurement_report_procedure.cpp
        f1ap_du_positioning_measurement_procedure.cpp
        f1ap_du_paging_procedure.cpp
        f1ap_du_write_replace_warning_procedure.cpp
        f1ap_du_ref_time_info_report_procedure.cpp
)

add_library(ocudu_f1ap_du_procedures STATIC ${SOURCES})
target_link_libraries(ocudu_f1ap_du_procedures ocudu_f1ap_common ocudu_f1ap_du_helpers f1ap_asn1)
