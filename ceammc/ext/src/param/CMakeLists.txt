include_directories(${CMAKE_SOURCE_DIR}/src)

set(PARAM_SOURCES )

macro(ceammc_param_extension name)
    list(APPEND PARAM_SOURCES "param_${name}.cpp")
endmacro()

ceammc_param_extension(float)

add_library(ceammc_param STATIC mod_param.h mod_param.cpp ${PARAM_SOURCES})
if(UNIX)
    set_target_properties(ceammc_param PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
