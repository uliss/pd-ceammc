macro(ceammc_misc_external mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen_obj(${mod} ${name})
endmacro()

ceammc_misc_external(live capture)

if(WITH_MODPLUG)
    find_package(ModPlug REQUIRED)
    if(MODPLUG_FOUND)
        include_directories(${MODPLUG_INCLUDES})
        list(APPEND MISC_SOURCES misc_modplug.cpp)
        list(APPEND MISC_LIBRARIES ${MODPLUG_LIBRARIES})
        add_definitions(-DWITH_MODPLUG)
    endif()
endif()

if(WITH_FLUIDSYNTH)
    include_directories(${FLUIDSYNTH_INCLUDES})
    list(APPEND MISC_SOURCES fluid.cpp)
    list(APPEND MISC_LIBRARIES ${FLUIDSYNTH_LIBRARIES})
endif()

# flite TTS engine
include_directories("${CMAKE_SOURCE_DIR}/ceammc/extra/speech/flite/include")
list(APPEND MISC_SOURCES speech_flite.cpp fliterender.cpp)

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
target_link_libraries(ceammc_misc flite ${MISC_LIBRARIES})

set_target_properties(ceammc_misc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")

if(UNIX)
    set_target_properties(ceammc_misc PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
