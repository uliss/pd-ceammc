macro(ceammc_misc_external mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen_obj(${mod} ${name})
endmacro()

ceammc_misc_external(live capture)

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
target_compile_definitions(ceammc_misc PRIVATE -DFAUST_MACRO)

if(WITH_FLUIDSYNTH)
    target_include_directories(ceammc_misc PRIVATE ${FLUIDSYNTH_INCLUDES})
    target_include_directories(ceammc_misc PRIVATE ${FLUIDSYNTH_INCLUDES}/../src)
    target_link_libraries(ceammc_misc PUBLIC ${FLUIDSYNTH_LIBRARIES})
    target_sources(ceammc_misc PRIVATE fluid.cpp)
endif()

find_package(PkgConfig QUIET)

if(WITH_SFIZZ)
    pkg_check_modules(SFIZZ "sfizz" REQUIRED)
else()
    pkg_check_modules(SFIZZ "sfizz")
endif()

if(SFIZZ_FOUND)
    target_include_directories(ceammc_misc PRIVATE ${SFIZZ_INCLUDE_DIRS})
    target_link_libraries(ceammc_misc PUBLIC ${SFIZZ_LIBRARIES})
    target_sources(ceammc_misc PRIVATE sfizz_tilde.cpp)
endif()

if(WITH_MODPLUG)
    find_package(ModPlug REQUIRED)
    target_include_directories(ceammc_misc PRIVATE ${MODPLUG_INCLUDES})
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_MODPLUG)
    target_link_libraries(ceammc_misc PUBLIC ${MODPLUG_LIBRARIES})
    target_sources(ceammc_misc PRIVATE misc_modplug.cpp)
endif()

if(WITH_TTS_FLITE)
    target_include_directories(ceammc_misc PRIVATE
        ${FLITE_INCLUDES}
        ${PROJECT_SOURCE_DIR}/ceammc/extra
    )

    target_link_libraries(ceammc_misc PUBLIC ${FLITE_LIBRARIES})
    target_sources(ceammc_misc PRIVATE speech_flite.cpp fliterender.cpp speech_flite_tilde.cpp)
endif()
