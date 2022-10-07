include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}) # for config.h
include(tests.cmake)
add_definitions("-DTEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
add_definitions("-DTEST_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"")

if(WIN32)
    remove_definitions(-DPD_INTERNAL)
endif()

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Catch2)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_target_properties(Catch2::Catch PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_library(tests_main_lib STATIC
    tests_main.cpp
    test_approx.cpp
    test_datatypes.cpp
    test_external.cpp
    test_external_log_output.cpp
    test_wrappers.cpp
)

target_include_directories(tests_main_lib PUBLIC "${CATCH_INCLUDE_DIR}")

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../src/lib")
endif()

ceammc_add_core_test("ceammc::arrayname_parser" test_arrayname_parser)
ceammc_add_core_test("ceammc::BaseObject" test_baseobject)
ceammc_add_core_test("ceammc::Canvas" test_canvas)
ceammc_add_core_test("ceammc::External" test_pd_external)
ceammc_add_core_test("ceammc::Factory" test_factory)
ceammc_add_core_test("ceammc::ObjectInfoStorage" test_object_info)
ceammc_add_core_test("ceammc::SharedData" test_shareddata)
ceammc_add_core_test("ceammc::SoundExternal" test_sound_external)
ceammc_add_core_test("ceammc::faust" test_faust)
ceammc_add_core_test("ceammc::log" test_log)
ceammc_add_core_test("ceammc::proxy" test_proxy)
ceammc_add_core_test("ceammc::signal" test_signal)
ceammc_add_core_test("ceammc::window" test_window)

ceammc_add_core_test("pd::ceammc" test_pd_core)

if(WITH_LIBSNDFILE)
    include(FindLibSndFile)
    if(LIBSNDFILE_FOUND)
        include_directories(${LIBSNDFILE_INCLUDE_DIRS})
        ceammc_add_core_test("ceammc::libsndfile" test_libsndfile)
    endif()
endif()

ceammc_add_test_linked(NAME test_sound TITLE "ceammc::sound" SRC test_sound_minimp3.cpp)
if(WITH_LIBSAMPLERATE)
    target_compile_definitions(test_sound PRIVATE WITH_LIBSAMPLERATE)
endif()

ceammc_add_test_linked(TITLE "ceammc::pd" NAME test_pd SRC "../src/base/debug_gensym.cpp" LINK ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeMidiStream"
    NAME test_datatype_midistream
    SRC test_datatype_midistream.cpp
    LINK ceammc_core puredata-core ceammc_midi ceammc_core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeMidiTrack"
    NAME test_datatype_miditrack
    SRC test_datatype_miditrack.cpp
    LINK ceammc_core puredata-core ceammc_midi ceammc_core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeEnv"
    NAME test_datatype_env
    SRC test_datatype_env.cpp
    LINK ceammc_env ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeProperty"
    NAME test_datatype_property
    SRC test_datatype_property.cpp
    LINK ceammc_base ceammc_core puredata-core)

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    ceammc_add_test_linked(
        TITLE "ceammc::coreaudio_loader"
        NAME test_coreaudio
        LINK ceammc_core)
endif()

ceammc_add_test_linked(
    TITLE "Serial"
    NAME test_serialport
    LINK serial)

ceammc_add_test_linked(TITLE "ArduinoThread"
    NAME test_arduino_thread
    LINK tests_main_lib serial hw_arduino ceammc_core puredata-core)
ceammc_add_test_linked(TITLE "Arduino" NAME test_arduino LINK tests_main_lib hw_arduino ceammc_core puredata-core)

add_custom_target(test_src SOURCES gen_test_wav.py)

# put binaries from subdir targets in current directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(cells)

add_subdirectory(an)
add_subdirectory(array)
add_subdirectory(base)
add_subdirectory(conv)
add_subdirectory(data)
add_subdirectory(env)
add_subdirectory(flow)
add_subdirectory(flt)
add_subdirectory(fx)
add_subdirectory(lang)
add_subdirectory(list)
add_subdirectory(math)
add_subdirectory(midi)
add_subdirectory(misc)
add_subdirectory(music)
add_subdirectory(net)
add_subdirectory(nui)
add_subdirectory(path)
add_subdirectory(preset)
add_subdirectory(proto)
add_subdirectory(spat)
add_subdirectory(random)
add_subdirectory(string)
add_subdirectory(system)
add_subdirectory(tl)
add_subdirectory(ui)
add_subdirectory(wrapper)
