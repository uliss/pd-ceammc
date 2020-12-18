add_executable(test_ext_misc test_misc_base.cpp)

target_compile_definitions(test_ext_misc
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_MIDI_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(test_ext_misc
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/misc"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_misc PUBLIC
    tests_main_lib
    ceammc_misc
    ceammc_core
    puredata-core)

add_test(NAME test_ext_misc
    COMMAND test_ext_misc
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_misc_test src)
    target_sources(test_ext_misc PRIVATE "test_misc_${src}.cpp")
endfunction()


if(WITH_TTS_FLITE)
    add_misc_test(speech_flite)
endif()

if(WITH_FLUIDSYNTH)
    add_misc_test(fluid)
endif()
