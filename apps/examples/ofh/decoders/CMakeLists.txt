# Not final

add_library(srsran_o_ru_decoders STATIC  ofh_uplane_rx_symbol_data_flow_writer.cpp)
target_link_libraries(srsran_o_ru_decoders srslog)