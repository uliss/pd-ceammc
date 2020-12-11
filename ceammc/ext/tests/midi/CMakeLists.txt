add_executable(test_ext_midi)
target_sources(test_ext_midi
    PRIVATE test_midi_base.h)

target_compile_definitions(test_ext_midi
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_MIDI_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(test_ext_midi
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/midi"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_midi PUBLIC
    tests_main_lib
    ceammc_midi
    ceammc_core
    puredata-core)

add_test(NAME test_ext_midi
    COMMAND test_ext_midi
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_midi_test src)
    target_sources(test_ext_midi PRIVATE "test_midi_${src}.cpp")
endfunction()

add_midi_test(ctl_to_str)
add_midi_test(file)
add_midi_test(key_to_str)
add_midi_test(note)
add_midi_test(prg_to_str)
add_midi_test(track)
add_midi_test(vramp)
