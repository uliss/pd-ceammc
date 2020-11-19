add_subdirectory(firmata)

set(PROTO_SOURCES)

macro(ceammc_proto_extension name)
    list(APPEND PROTO_SOURCES "proto_${name}.cpp")
endmacro()

ceammc_proto_extension(firmata)
ceammc_proto_extension(hui)
ceammc_proto_extension(sp_alpaca)

add_library(ceammc_proto STATIC mod_proto.cpp ${PROTO_SOURCES})
target_link_libraries(ceammc_proto proto_firmata)
