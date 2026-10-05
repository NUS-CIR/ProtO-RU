# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI
# Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.


# There is a compilation defect for CLang version 15 which makes the pseudo-random generator to create invalid
# sequences.
if ((CMAKE_CXX_COMPILER_ID MATCHES "Clang") AND (CMAKE_CXX_COMPILER_VERSION MATCHES "15"))
    message(WARNING "The selected compiler version is not supported with -O2 flag.")
    string(REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
endif ((CMAKE_CXX_COMPILER_ID MATCHES "Clang") AND (CMAKE_CXX_COMPILER_VERSION MATCHES "15"))

set(SOURCES
        low_papr_sequence_collection_impl.cpp
        low_papr_sequence_generator_impl.cpp
        pseudo_random_generator_impl.cpp
        sequence_generator_factories.cpp
        )

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    list(APPEND SOURCES pseudo_random_generator_avx2_impl.cpp)
    set_source_files_properties(pseudo_random_generator_avx2_impl.cpp PROPERTIES
            COMPILE_OPTIONS "-mavx2;")
endif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")

add_library(ocudu_sequence_generators STATIC ${SOURCES})
target_link_libraries(ocudu_sequence_generators ocudu_support ocuduvec)

add_to_exported_libs(ocudu_sequence_generators)
