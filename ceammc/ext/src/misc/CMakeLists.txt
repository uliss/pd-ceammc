macro(ceammc_misc_external mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen_obj(${mod} ${name})
endmacro()

ceammc_misc_external(live capture)
ceammc_misc_external(aubio pitchshift_tilde)

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
target_compile_definitions(ceammc_misc PRIVATE -DFAUST_MACRO)
target_include_directories(ceammc_misc
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ceammc/extra/aubio
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ceammc/ext/src
)

if(WITH_FLUIDSYNTH)
    target_include_directories(ceammc_misc
        PRIVATE
        ${FLUIDSYNTH_INCLUDES}
        ${FLUIDSYNTH_INCLUDES}/../src)
    target_link_libraries(ceammc_misc PUBLIC ${FLUIDSYNTH_LIBRARIES} ceammc_proto)
    target_sources(ceammc_misc PRIVATE fluid.cpp)
endif()

find_package(PkgConfig QUIET)

if(WITH_SFIZZ)
    cmake_minimum_required(VERSION 3.14)
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    set(SFIZZ_AU OFF CACHE INTERNAL "")
    set(SFIZZ_LV2 OFF CACHE INTERNAL "")
    set(SFIZZ_LV2_UI OFF CACHE INTERNAL "")
    set(SFIZZ_VST OFF CACHE INTERNAL "")
    set(SFIZZ_JACK OFF CACHE INTERNAL "")
    set(SFIZZ_RENDER OFF CACHE INTERNAL "")
    set(SFIZZ_SHARED OFF CACHE INTERNAL "")
    set(SFIZZ_USE_SNDFILE OFF CACHE INTERNAL "")

    FetchContent_Declare(
      sfizz
      URL      https://github.com/sfztools/sfizz/releases/download/1.1.1/sfizz-1.1.1.tar.gz
      URL_HASH MD5=cc4770c287d52da517a7ae25fd826a3e
    )
    FetchContent_MakeAvailable(sfizz)

    target_compile_definitions(ceammc_misc PRIVATE -DWITH_SFIZZ -DSFIZZ_VERSION="1.1.1")
    target_link_libraries(ceammc_misc PUBLIC sfizz)
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
