# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


set(SOURCES
        beamforming_weights_compressor.cpp
        compression_factory.cpp
        compression_validator.cpp
        iq_compression_bfp_impl.cpp
        iq_compression_none_impl.cpp
        iq_compression_unsupported_impl.cpp
        iq_compressor_selector.cpp
        iq_decompressor_selector.cpp
        packing_utils_generic.cpp)

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    list(APPEND SOURCES
            iq_compression_bfp_avx2.cpp
            iq_compression_bfp_avx512.cpp
            iq_compression_none_avx512.cpp
            iq_compression_none_avx2.cpp)
    set_source_files_properties(iq_compression_bfp_avx2.cpp PROPERTIES COMPILE_OPTIONS "-mavx2;")
    set_source_files_properties(iq_compression_none_avx2.cpp PROPERTIES COMPILE_OPTIONS "-mavx2;")
    # The compression factory checks the same AVX512 instruction sets at run time before selecting these sources.
    set(AVX512_COMPILE_OPTIONS "-mavx512f;-mavx512bw;-mavx512vl;-mavx512cd;-mavx512dq;-mavx512vbmi")
    set_source_files_properties(iq_compression_bfp_avx512.cpp PROPERTIES COMPILE_OPTIONS "${AVX512_COMPILE_OPTIONS}")
    set_source_files_properties(iq_compression_none_avx512.cpp PROPERTIES COMPILE_OPTIONS "${AVX512_COMPILE_OPTIONS}")
endif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    list(APPEND SOURCES iq_compression_bfp_neon.cpp)
    list(APPEND SOURCES iq_compression_none_neon.cpp)
endif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")

add_library(ocudu_ofh_compression STATIC ${SOURCES})
target_link_libraries(ocudu_ofh_compression ocuduvec)
