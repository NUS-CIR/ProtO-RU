#
# Copyright 2021-2025 Software Radio Systems Limited
#
# This file is part of srsRAN
#
# srsRAN is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# srsRAN is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# A copy of the GNU Affero General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

set_directory_properties(PROPERTIES LABELS "ofh")

add_subdirectory(decoders)
add_subdirectory(helpers)

set (SOURCES
        ofh_uplane_fragment_size_calculator.cpp
        data_flow_uplane_uplink.cpp
        upper_phy_fake.cpp
        ru_ofh_transmitter.cpp
        ru_emulator_cli11_schema.cpp
        ru_emulator.cpp
        ru_emulator_rx_window_checker.cpp
        ru_emulator_socket_transceiver.cpp
        ru_emulator_timing_notifier.cpp)
if (DPDK_FOUND)
    list(APPEND SOURCES ru_emulator_dpdk_transceiver.cpp)
endif (DPDK_FOUND)

add_executable(ru_emulator ${SOURCES})
target_include_directories(ru_emulator PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external)
target_link_libraries(ru_emulator 
                srslog 
                srsran_ofh 
                srsran_phy_support 
                srsran_support
                srsran_upper_phy_support
                srsran_lower_phy
                srsran_signal_processors
                srsran_channel_processors
                srsran_channel_precoder 
                srsran_ofh_compression
                srsran_o_ru_decoders 
                srsran_o_ru_helpers)

if (DPDK_FOUND)
    target_compile_options(ru_emulator PRIVATE ${DPDK_CFLAGS})
    target_link_libraries(ru_emulator hal_dpdk ${DPDK_LIBRARIES})
    add_definitions(-DDPDK_FOUND)
endif (DPDK_FOUND)

install(TARGETS ru_emulator
        RUNTIME)
