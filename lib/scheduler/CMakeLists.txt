# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_subdirectory(config)
add_subdirectory(rrm)
add_subdirectory(support)
add_subdirectory(logging)
add_subdirectory(cell)
add_subdirectory(common_scheduling)
add_subdirectory(ue_context)
add_subdirectory(ue_scheduling)
if (ENABLE_SCHEDTRACE)
    add_subdirectory(trace)
endif (ENABLE_SCHEDTRACE)

set(SOURCES
        pdcch_scheduling/pdcch_resource_allocator_impl.cpp
        pdcch_scheduling/pdcch_slot_resource_allocator.cpp
        policy/scheduler_policy_factory.cpp
        policy/scheduler_time_rr.cpp
        policy/scheduler_time_qos.cpp
        pucch_scheduling/pucch_allocator_impl.cpp
        pucch_scheduling/pucch_allocator_helpers.cpp
        pucch_scheduling/pucch_collision_manager.cpp
        uci_scheduling/uci_scheduler_impl.cpp
        uci_scheduling/uci_allocator_impl.cpp
        uci_scheduling/uci_indication_selector.cpp
        slicing/inter_slice_scheduler.cpp
        slicing/ran_slice_instance.cpp
        slicing/slice_ue_repository.cpp
        srs/srs_scheduler_impl.cpp
        cell_event_manager.cpp
        cell_scheduler.cpp
        scheduler_factory.cpp
        scheduler_impl.cpp
        support/csi_rs_helper.cpp
        srs/srs_allocator_impl.cpp
        configured_grant/cg_sched_helpers.cpp
        configured_grant/configured_grant_scheduler_base.cpp
        configured_grant/configured_grant_type1_sched_impl.cpp
        configured_grant/configured_grant_type2_sched_impl.cpp
        configured_grant/cg_type2_resource_manager.cpp)

add_library(ocudu_sched STATIC ${SOURCES}
        $<TARGET_OBJECTS:sched_config_manager>
        $<TARGET_OBJECTS:sched_cell>
        $<TARGET_OBJECTS:common_sched>
        $<TARGET_OBJECTS:sched_ue_context>
        $<TARGET_OBJECTS:ue_sched>)
target_link_libraries(ocudu_sched ocudu_ran sched_config sched_ue_context sched_support scheduler_logger)
