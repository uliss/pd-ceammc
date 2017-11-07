add_subdirectory(firmata)

set(PROTO_SOURCES)

macro(ceammc_proto_extension name)
    list(APPEND PROTO_SOURCES "proto_${name}.cpp")
endmacro()

ceammc_proto_extension(firmata)

add_library(ceammc_proto STATIC mod_proto.cpp ${PROTO_SOURCES})
target_link_libraries(ceammc_proto proto_firmata)

if(UNIX)
    set_target_properties(ceammc_proto PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
