add_subdirectory(firmata)

add_library(ceammc_proto STATIC mod_proto.cpp)

function(ceammc_proto_external name)
    target_sources(ceammc_proto PRIVATE "proto_${name}.cpp")
endfunction()

ceammc_proto_external(firmata)
ceammc_proto_external(hui)
ceammc_proto_external(midi)
ceammc_proto_external(midi_sysex)
ceammc_proto_external(mpv)
ceammc_proto_external(sp_alpaca)
ceammc_proto_external(xtouch_ext)

target_sources(ceammc_proto PRIVATE proto_midi_parser.cpp)
target_include_directories(ceammc_proto PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra/readerwriterqueue)
target_link_libraries(ceammc_proto PRIVATE proto_firmata)
