macro(ceammc_misc_external mod name)
    list(APPEND MISC_SOURCES "${mod}_${name}.cpp")
    ceammc_faust_gen_obj(${mod} ${name})
endmacro()

ceammc_misc_external(aubio pitchshift_tilde)
ceammc_misc_external(live capture)
ceammc_misc_external(risset glissando)
ceammc_misc_external(misc qrcode)

add_library(ceammc_misc STATIC mod_misc.cpp ${MISC_SOURCES})
target_compile_definitions(ceammc_misc PRIVATE -DFAUST_MACRO)
target_include_directories(ceammc_misc
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ceammc/extra/aubio
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
        ${PROJECT_BINARY_DIR}/ceammc # ceammc_config.h
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ceammc/ext/src
)

target_link_libraries(ceammc_misc PRIVATE qrcode)

if(WITH_FLUIDSYNTH)
    target_link_libraries(ceammc_misc PUBLIC fluidsynth ceammc_proto)
    target_sources(ceammc_misc PRIVATE fluid.cpp)
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_FLUIDSYNTH=1)
endif()

find_package(PkgConfig QUIET)

if(WITH_SFIZZ)
    target_include_directories(ceammc_misc PRIVATE ${SFIZZ_INCLUDE_DIRS})
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_SFIZZ)
    target_link_libraries(ceammc_misc PUBLIC ${SFIZZ_LIBRARIES})
    target_sources(ceammc_misc PRIVATE sfizz_tilde.cpp)
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_SFIZZ -DSFIZZ_VERSION="1.2.0")

    target_include_directories(ceammc_misc PUBLIC ${sfizz_SOURCE_DIR}/src)
    if(WIN32)
        target_link_libraries(ceammc_misc PUBLIC sfizz::shared)
    else()
        target_link_libraries(ceammc_misc PRIVATE sfizz_static)
    endif()
endif()

if(WITH_MODPLUG)
    find_package(ModPlug REQUIRED)
    target_include_directories(ceammc_misc PRIVATE ${MODPLUG_INCLUDES})
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_MODPLUG)
    target_link_libraries(ceammc_misc PUBLIC ${MODPLUG_LIBRARIES})
    target_sources(ceammc_misc PRIVATE misc_modplug.cpp)
endif()

if(WITH_TTS_FLITE)
    target_link_libraries(ceammc_misc PUBLIC flite readerwriterqueue)
    target_sources(ceammc_misc PRIVATE speech_flite.cpp fliterender.cpp speech_flite_tilde.cpp)
    target_compile_definitions(ceammc_misc PRIVATE -DWITH_TTS_FLITE)
endif()

if(WITH_TTS_RHVOICE)
    find_package(RHVoice REQUIRED)

    if(RHVOICE_FOUND)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
        target_include_directories(ceammc_misc PRIVATE ${RHVOICE_INCLUDE_DIRS})
        target_include_directories(ceammc_misc PRIVATE "${PROJECT_SOURCE_DIR}/ceammc/extra/soxr/code/src")
        target_compile_definitions(ceammc_misc PRIVATE -DWITH_RHVOICE)
        target_link_libraries(ceammc_misc PRIVATE ${RHVOICE_LIBRARIES} soxr)
        target_link_libraries(ceammc_misc PUBLIC readerwriterqueue)
        target_sources(ceammc_misc PRIVATE speech_rhvoice_tilde.cpp)
    endif()
else()
    target_sources(ceammc_misc PRIVATE speech_rhvoice_tilde_stub.cpp)
endif()

if(WIN32)
    set(DOC_INSTALL_PATH "extra/ceammc")
else()
    set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")
endif()
add_custom_target(misc_tts_files SOURCES "RHVoice.conf")
# install extra files
install(FILES
    "RHVoice.conf"
    DESTINATION ${DOC_INSTALL_PATH}/
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
