include_directories(${CMAKE_SOURCE_DIR}/src)

set(ARRAY_SOURCES array_base.cpp)

macro(ceammc_array_extension name)
    list(APPEND ARRAY_SOURCES "array_${name}.cpp")
endmacro()

ceammc_array_extension(p2s)
ceammc_array_extension(s2p)

add_library(ceammc_array STATIC mod_array.h mod_array.cpp ${ARRAY_SOURCES})
if(UNIX)
    set_target_properties(ceammc_array PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
