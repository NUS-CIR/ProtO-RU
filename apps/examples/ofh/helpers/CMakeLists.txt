# Not final.

add_library(srsran_o_ru_helpers STATIC ru_config_translator.cpp worker_manager.cpp)
target_link_libraries(srsran_o_ru_helpers srsran_ru_generic srsran_lower_phy srsran_cpu_affinities_helper)
target_include_directories(srsran_o_ru_helpers PRIVATE ${CMAKE_SOURCE_DIR})

