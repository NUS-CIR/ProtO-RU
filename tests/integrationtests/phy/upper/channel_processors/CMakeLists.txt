# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.


set_directory_properties(PROPERTIES LABELS "phy;tsan;NO_MEMCHECK")

add_executable(pxsch_bler_test pxsch_bler_test.cpp)

set(PXSCH_BLER_TEST_LIBRARIES ocudu_ran
        ocudu_upper_phy
        ocudu_upper_phy_support
        ocudu_channel_precoder
        ocudulog
        ocuduvec)

if (ENABLE_PDSCH_HWACC AND ENABLE_PUSCH_HWACC)
    set_source_files_properties(pxsch_bler_test.cpp pxsch_bler_test_factories.cpp PROPERTIES COMPILE_DEFINITIONS "HWACC_PDSCH_ENABLED; HWACC_PUSCH_ENABLED")
    list(APPEND PXSCH_BLER_TEST_LIBRARIES ocudu_hal_pusch
            ocudu_hal_pdsch
            ocudu_hal_bbdev)
endif (ENABLE_PDSCH_HWACC AND ENABLE_PUSCH_HWACC)

add_library(ocudu_pxsch_bler_test_helpers STATIC
        pxsch_bler_test_channel_emulator.cpp
        pxsch_bler_test_factories.cpp
        pxsch_bler_test_parse.cpp)
target_link_libraries(ocudu_pxsch_bler_test_helpers ${PXSCH_BLER_TEST_LIBRARIES})

target_link_libraries(pxsch_bler_test ocudu_pxsch_bler_test_helpers)
add_test(pxsch_bler_test pxsch_bler_test -R 10)
# BLER test - single codeblock (1 RB), 32 repetitions.
add_test(pxsch_bler_test_1CBs_32reps pxsch_bler_test -m 0 -R 10 -V [0,2,3,1,0,2,3,1,0,2,3,1,0,2,3,1,0,2,3,1,0,2,3,1,0,2,3,1,0,2,3,1] -B 1)
# BLER test - many codeblocks, one thread.
add_test(pxsch_bler_test_manyCBs_4rep pxsch_bler_test -m 28 -R 10 -V [0,2,3,1] -j 1)
# BLER test - 1024QAM
add_test(pxsch_bler_test_1024QAM pxsch_bler_test -M qam1024 -m 25 -R 10 -V [0,2,3,1] -j 1)
# BLER test - TDLA with Doppler and medium MIMO correlation.
add_test(pxsch_bler_test_tdla_doppler pxsch_bler_test -C TDLA -F rayleigh --doppler-hz 10 --mimo-correlation medium -R 10)

add_executable(pxsch_chain_test pxsch_chain_test.cpp)
target_link_libraries(pxsch_chain_test
        ocudu_ran
        ocudu_channel_processors
        ocudulog
        ocuduvec
        gtest
        gtest_main
)
gtest_discover_tests(pxsch_chain_test)
