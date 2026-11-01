// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e2ap/e2ap_ies.h"
#include "ocudu/asn1/e2ap/e2ap_pdu_contents.h"

using namespace asn1;
using namespace asn1::e2ap;

template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::crit_diagnostics_ie_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_addition_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_addition_ack_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_removal_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_removal_ack_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_upd_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_upd_ack_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_functions_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_functions_id_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_functions_id_cause_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_functions_accept_modified_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_load_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_function_load_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_load_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_load_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_load_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_load_request_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_suspended_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_state_confirm_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_function_state_ctrl_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_list_with_cause_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_audit_action_list_l::value_type>::ops;
template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_audit_list_l::value_type>::ops;
