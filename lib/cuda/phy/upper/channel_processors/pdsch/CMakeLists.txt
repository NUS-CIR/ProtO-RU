# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_library(ocudu_cuda_pdsch pdsch_fused_ratematcher_modulator.cu)

target_link_libraries(ocudu_cuda_pdsch ocudu_cuda_adt ocudu_support CUDA::cudart)

add_to_exported_libs(ocudu_cuda_pdsch)
