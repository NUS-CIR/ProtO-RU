# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_executable(pdsch_fused_kernel_test pdsch_fused_kernel_test.cpp)
target_link_libraries(pdsch_fused_kernel_test
        ocudu_cuda_pdsch
        ocudu_cuda_adt
        ocudu_channel_coding
        ocudu_channel_modulation
        ocudu_sequence_generators
        ocudulog
        GTest::gtest
        GTest::gtest_main)
add_cuda_test(pdsch_fused_kernel_test pdsch_fused_kernel_test)
