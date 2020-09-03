add_executable(test_ext_music)
#target_sources(test_ext_music PRIVATE test_misc_base.cpp)

target_compile_definitions(test_ext_music
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_MIDI_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(test_ext_music
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_music PUBLIC
    tests_main_lib
    ceammc_core
    puredata-core)

add_test(NAME test_ext_music
    COMMAND test_ext_music
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_music_test src)
    target_sources(test_ext_music PRIVATE "test_music_${src}.cpp")
endfunction()

add_music_test(theory)
add_music_test(theory_alteration)
add_music_test(theory_keyboard)
add_music_test(theory_pitch_class)
add_music_test(theory_pitch_name)
