# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

add_executable(pseudo_random_generator_benchmark pseudo_random_generator_benchmark.cpp)
target_link_libraries(pseudo_random_generator_benchmark ocudu_sequence_generators ocuduvec ocudulog)
add_test(pseudo_random_generator_benchmark pseudo_random_generator_benchmark -s -R 1 -T generic)
