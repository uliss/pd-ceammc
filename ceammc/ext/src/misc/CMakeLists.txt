set(MISC_SOURCES "")
macro(ceammc_misc_extension mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen(${mod} ${name})
endmacro()

ceammc_misc_extension(live capture)

find_package(ModPlug)
if(MODPLUG_FOUND)
    include_directories(${MODPLUG_FOUND_INCLUDES})
    list(APPEND MISC_SOURCES misc_modplug.cpp)
endif()

# flite TTS engine
include_directories("${CMAKE_SOURCE_DIR}/ceammc/extra/speech/flite/include")
list(APPEND MISC_SOURCES speech_flite.cpp fliterender.cpp)

# FluidSynth
list(APPEND MISC_SOURCES fluid.cpp)
include_directories("${CMAKE_SOURCE_DIR}/ceammc/extra/fluidsynth/fluidsynth/include")
include_directories("${CMAKE_BINARY_DIR}/ceammc/extra/fluidsynth")

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
target_link_libraries(ceammc_misc flite)
set_target_properties(ceammc_misc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")

if(UNIX)
    set_target_properties(ceammc_misc PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(MODPLUG_FOUND)
    target_link_libraries(ceammc_misc ${MODPLUG_LIBRARIES})
endif()

target_link_libraries(ceammc_misc fluidsynth)
