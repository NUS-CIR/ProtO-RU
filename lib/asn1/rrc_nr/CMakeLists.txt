# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

# RRC NR ASN1
add_library(rrc_nr_asn1 STATIC
        common.cpp
        serving_cell.cpp
        ue_cap.cpp
        bwp_cfg.cpp
        sys_info.cpp
        radio_bearer_cfg.cpp
        dl_ccch_msg_ies.cpp
        dl_dcch_msg_ies.cpp
        ul_ccch_msg_ies.cpp
        ul_dcch_msg_ies.cpp
        pcch_msg_ies.cpp
        dl_dcch_msg.cpp
        ul_dcch_msg.cpp
        nr_ue_variables.cpp
        pc5_rrc_definitions.cpp
        nr_sidelink_preconf.cpp
        cell_group_config.cpp
        bcch_bch_msg.cpp
        bcch_dl_sch_msg.cpp
        pcch_msg.cpp
        ul_ccch_msg.cpp
        ul_ccch1_msg.cpp
        dl_ccch_msg.cpp
        rrc_nr.cpp
        rrc_nr_lists.cpp)
target_compile_options(rrc_nr_asn1 PRIVATE -Os -fno-exceptions)
target_link_libraries(rrc_nr_asn1 asn1_utils)
