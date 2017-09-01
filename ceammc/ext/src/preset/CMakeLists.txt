include_directories(${CMAKE_SOURCE_DIR}/src)

set(PRESET_SOURCES )

macro(ceammc_preset_extension name)
    list(APPEND PRESET_SOURCES "param_${name}.cpp")
endmacro()

ceammc_preset_extension(float)
ceammc_preset_extension(storage)

add_library(ceammc_preset STATIC mod_param.h mod_param.cpp ${PRESET_SOURCES})
if(UNIX)
    set_target_properties(ceammc_preset PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
