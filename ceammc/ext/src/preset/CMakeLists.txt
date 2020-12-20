include_directories(${CMAKE_SOURCE_DIR}/src)

set(PRESET_SOURCES )

macro(ceammc_preset_extension name)
    list(APPEND PRESET_SOURCES "preset_${name}.cpp")
endmacro()

ceammc_preset_extension(base)
ceammc_preset_extension(float)
ceammc_preset_extension(symbol)
ceammc_preset_extension(list)
ceammc_preset_extension(storage)

add_library(ceammc_preset STATIC mod_preset.h mod_preset.cpp ${PRESET_SOURCES})
