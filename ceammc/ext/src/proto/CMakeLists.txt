add_subdirectory(firmata)

add_library(ceammc_proto STATIC mod_proto.cpp)

function(ceammc_proto_extension name)
    target_sources(ceammc_proto PRIVATE "proto_${name}.cpp")
endfunction()

ceammc_proto_extension(firmata)
ceammc_proto_extension(hui)
ceammc_proto_extension(midi)
ceammc_proto_extension(midi_sysex)
ceammc_proto_extension(sp_alpaca)
ceammc_proto_extension(xtouch_ext)

target_sources(ceammc_proto PRIVATE proto_midi_parser.cpp)

target_link_libraries(ceammc_proto proto_firmata)
