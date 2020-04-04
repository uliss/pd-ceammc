include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}) # for config.h
include(tests.cmake)
add_definitions("-DTEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
add_definitions("-DTEST_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"")

if(WIN32)
    remove_definitions(-DPD_INTERNAL)
endif()

add_library(tests_main_lib1 STATIC
    tests_main.cpp
    test_approx.cpp
    test_datatypes.cpp
    test_external.cpp
    test_external_log_output.cpp
    test_wrappers.cpp
)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Catch2)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_target_properties(Catch2::Catch PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_library(tests_main_lib2 STATIC
    tests_main.cpp
    test_approx.cpp
    test_datatypes.cpp
    test_external.cpp
    test_external_log_output.cpp
    test_wrappers.cpp
)
target_link_libraries(tests_main_lib2 PUBLIC Catch2::Catch)

target_include_directories(tests_main_lib1 PUBLIC Catch/single_include)

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../src/lib")
endif()

ceammc_add_core_test("ceammc::ArgumentCheck" test_args)
ceammc_add_core_test("ceammc::Array" test_array)
ceammc_add_core_test("ceammc::Atom" test_atom)
ceammc_add_core_test("ceammc::Atom2" test_atom2)
ceammc_add_core_test("ceammc::AtomList" test_atomlist)
ceammc_add_core_test("ceammc::AtomList2" test_atomlist2)
ceammc_add_core_test("ceammc::AtomListView" test_atomlist_view)
ceammc_add_core_test("ceammc::Canvas" test_canvas)
ceammc_add_core_test("ceammc::ControlValue" test_message)
ceammc_add_core_test("ceammc::DataAtom" test_data)
ceammc_add_core_test("ceammc::DataTypeString" test_datatype_string)
ceammc_add_core_test("ceammc::SharedData" test_shareddata)
ceammc_add_core_test("ceammc::pd::External" test_pd_external)
ceammc_add_core_test("ceammc::formatting" test_format)
ceammc_add_core_test("ceammc::ObjectInfoStorage" test_object_info)
ceammc_add_core_test("ceammc::FloatProperty" test_property_float)
ceammc_add_core_test("ceammc::SymbolProperty" test_property_symbol)
ceammc_add_core_test("ceammc::properties" test_properties)
ceammc_add_core_test("ceammc::properties2" test_properties2)
ceammc_add_core_test("ceammc::PropertyInfo" test_propertyinfo)
ceammc_add_core_test("ceammc::BaseObject" test_baseobject)
ceammc_add_core_test("ceammc::SoundExternal" test_sound_external)
ceammc_add_core_test("ceammc::Factory" test_factory)
ceammc_add_core_test("ceammc::list" test_fn_list)
ceammc_add_core_test("ceammc::list2" test_fn_list2)
ceammc_add_core_test("ceammc::log" test_log)
ceammc_add_core_test("ceammc::faust" test_faust)
ceammc_add_core_test("ceammc::music_theory" test_music_theory)
ceammc_add_core_test("ceammc::music_theory_alteration" test_music_theory_alteration)
ceammc_add_core_test("ceammc::music_theory_pitch_name" test_music_theory_pitch_name)
ceammc_add_core_test("ceammc::music_theory_pitch_class" test_music_theory_pitch_class)
ceammc_add_core_test("ceammc::music_theory_keyboard" test_music_theory_keyboard)
ceammc_add_core_test("ceammc::timeline" test_timeline)
ceammc_add_core_test("ceammc::platform" test_platform)
ceammc_add_core_test("ceammc::convert" test_convert)
ceammc_add_core_test("ceammc::signal" test_signal)
ceammc_add_core_test("ceammc::string" test_string)
ceammc_add_core_test("ceammc::regexp" test_regexp)
ceammc_add_core_test("ceammc::window" test_window)
ceammc_add_core_test("ceammc::proxy" test_proxy)
ceammc_add_core_test("ceammc::units" test_units)
ceammc_add_core_test("ceammc::DataStreamParser" test_parser_data_string)
ceammc_add_core_test("class_wrapper::tuple" test_wrapper_tuple)
ceammc_add_core_test("class_wrapper::fn_traits" test_wrapper_fn_traits)

ceammc_external_test(analyze onset)

ceammc_external_test(base function)
ceammc_external_test(base gain)
ceammc_external_test(base matrix)
ceammc_external_test(base mix)
ceammc_external_test(base obj_props)
ceammc_external_test(base prop)
ceammc_external_test(base prop_declare)
ceammc_external_test(base prop_get)
ceammc_external_test(base radio)
ceammc_external_test(base xfade)
ceammc_external_test(base xfade2)

ceammc_add_test("[patch.args]" test_ext_patch_args)
ceammc_add_test("[canvas.current]" test_ext_canvas_current)
ceammc_add_test("[canvas.top]" test_ext_canvas_top)
ceammc_add_test("[metro.pattern]" test_ext_metro_pattern)
ceammc_add_test("[metro.seq]" test_ext_metro_seq)

ceammc_external_test(env env)
ceammc_external_test(env to_vline)
ceammc_external_test(env tscale)
ceammc_external_test(env tshift)
ceammc_external_test(env ar)
ceammc_external_test(env adsr)
ceammc_external_test(env asr)

ceammc_external_test(flow count)
ceammc_external_test(flow gate)
ceammc_external_test(flow group)
ceammc_external_test(flow less)
ceammc_external_test(flow less_eq)
ceammc_external_test(flow demultiplex)
ceammc_external_test(flow demux_tilde)
ceammc_external_test(flow demux2_tilde)
ceammc_external_test(flow multiplex)
ceammc_external_test(flow once)
ceammc_external_test(flow pack)
ceammc_external_test(flow pass)
ceammc_external_test(flow pass_if)
ceammc_external_test(flow reject)
ceammc_external_test(flow route)
ceammc_external_test(flow split)
ceammc_external_test(flow speedlim)

ceammc_external_test(flt median)

ceammc_external_test(fx looper)

ceammc_external_test(conv amp2dbfs)
ceammc_external_test(conv cc2amp)
ceammc_external_test(conv dbfs2amp)
ceammc_external_test(conv lin2exp)
ceammc_external_test(conv lin2lin)
ceammc_external_test(conv lin2lin_tilde)
ceammc_external_test(conv list2props)

ceammc_external_test(data dict)
ceammc_external_test(data dict_contains)
ceammc_external_test(data dict_each)
ceammc_external_test(data dict_from_list)
ceammc_external_test(data dict_get)
ceammc_external_test(data dict_pass)
ceammc_external_test(data dict_reject)
ceammc_external_test(data dict_to_list)
ceammc_external_test(data global_list)
ceammc_external_test(data global_mlist)
ceammc_external_test(data global_set)
ceammc_external_test(data fifo)
ceammc_external_test(data float)
ceammc_external_test(data int)
ceammc_external_test(data list)
ceammc_external_test(data local_float)
ceammc_external_test(data local_list)
ceammc_external_test(data local_mlist)
ceammc_external_test(data mlist)
ceammc_external_test(data set)
ceammc_external_test(data set_difference)
ceammc_external_test(data set_intersection)
ceammc_external_test(data set_size)
ceammc_external_test(data set_symdiff)
ceammc_external_test(data set_union)

ceammc_external_test(proto firmata)

ceammc_external_test(midi file)
ceammc_external_test(midi note)
ceammc_external_test(midi track)
ceammc_external_test(midi ctl_to_str)
ceammc_external_test(midi key_to_str)
ceammc_external_test(midi prg_to_str)

ceammc_external_test(net host)
target_include_directories(test_ext_net_host PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra)

ceammc_external_test(preset float)
ceammc_external_test(preset symbol)
ceammc_external_test(preset list)

ceammc_external_test(system getenv)
ceammc_external_test(system shell)
target_include_directories(test_ext_system_shell PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra)

ceammc_external_test(path exists)
ceammc_external_test(path lsdir)

ceammc_external_test(spat pan_lin)
ceammc_external_test(spat pan_sqrt)
ceammc_external_test(spat pan_cos)
ceammc_external_test(spat pan_spread)
ceammc_add_test_linked(
    TITLE "hoa_process"
    NAME test_ext_spat_hoa_process
    LINK ceammc_spat ceammc_core puredata-core ceammc_core
    INCLUDE_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources
        ${EIGEN3_DIR}
)
ceammc_add_test_linked(
    TITLE "hoa_in"
    NAME test_ext_spat_hoa_in
    LINK ceammc_spat ceammc_core puredata-core ceammc_core
    INCLUDE_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources
        ${EIGEN3_DIR}
)

ceammc_add_test_linked(TITLE "tl.transport"
    NAME test_ext_tl_transport
    LINK ceammc_tl ceammc_ui ceammc_core puredata-core ceammc_core)
ceammc_external_test(tl timeline)

ceammc_add_extension_test("snd_file" "../src/base/snd_file.cpp")
ceammc_add_extension_test("ext_replace" "../src/base/replace.cpp")
ceammc_add_extension_test("ext_expand_env" "../src/base/expand_env.cpp")
ceammc_add_extension_test("ext_msg" "../src/base/msg.cpp")

ceammc_add_extension_test("ext_symbol_equal" "../src/symbol/symbol_equal.cpp")
ceammc_add_extension_test("ext_symbol_num_compare" "../src/symbol/symbol_num_compare.cpp")

ceammc_add_test_linked(TITLE "tl::parser" NAME test_tl_parser LINK ceammc_tl)
ceammc_add_test_linked(TITLE "ceammc::preset" NAME test_preset LINK ceammc_preset ceammc_core puredata-core ceammc_core)
ceammc_add_test_linked(TITLE "ceammc::pd" NAME test_pd SRC "../src/base/debug_gensym.cpp" LINK ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "[flow.change]"
    NAME test_ext_flow_change
    SRC test_ext_flow_change.cpp
    LINK ceammc_core puredata-core ceammc_base ceammc_flow ceammc_core)

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

ceammc_add_test_linked(TITLE "ceammc::DataTypeMList"
    NAME test_datatype_mlist
    SRC test_datatype_mlist.cpp
    LINK ceammc_data ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeDict"
    NAME test_datatype_dict
    SRC test_datatype_dict.cpp
    LINK ceammc_data ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "ceammc::DataTypeProperty"
    NAME test_datatype_property
    SRC test_datatype_property.cpp
    LINK ceammc_base ceammc_core puredata-core)

if(WITH_LIBSNDFILE)
    include(FindLibSndFile)
endif()

if(LIBSNDFILE_FOUND)
    include_directories(${LIBSNDFILE_INCLUDE_DIRS})
    ceammc_add_test_linked(
        TITLE "ceammc::libsndfile"
        NAME test_libsndfile
        LINK ceammc_sound)

    ceammc_add_test_linked(
        TITLE "ceammc::sound"
        NAME test_sound
        LINK ceammc_sound)
endif()

ceammc_add_test_linked(
    TITLE "ceammc::arrayname_parser"
    NAME test_arrayname_parser
    LINK ceammc_sound)

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    ceammc_add_test_linked(
        TITLE "ceammc::coreaudio_loader"
        NAME test_coreaudio
        LINK ceammc_sound)
endif()

ceammc_add_test_linked(
    TITLE "Serial"
    NAME test_serialport
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/ceammc/extra/serial/serial/include
    LINK serial)

ceammc_add_test_linked(TITLE "FirmataParser[bison]" NAME test_firmata_parser LINK proto_firmata)
ceammc_add_test_linked(TITLE "firmata::Pin" NAME test_firmata_pin LINK proto_firmata ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "ArduinoThread"
    NAME test_arduino_thread
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/ceammc/extra/serial/serial/include
    LINK tests_main_lib lib_catch1_main hw_arduino ceammc_core puredata-core)
ceammc_add_test_linked(TITLE "Arduino" NAME test_arduino LINK tests_main_lib lib_catch1_main hw_arduino ceammc_core puredata-core)

# UI
ceammc_add_test_linked(TITLE "ceammc::UIObjectFactory"
    NAME test_ui_factory
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../src/ui
    LINK ceammc_ui ceammc_core puredata-core)

ceammc_ui_test("ui.aview")
ceammc_ui_test("ui.bang")
ceammc_ui_test("ui.colorpanel")
ceammc_ui_test("ui.display")
ceammc_ui_test("ui.dsp~")
ceammc_ui_test("ui.gain~")
ceammc_ui_test("ui.icon")
ceammc_ui_test("ui.incdec")
ceammc_ui_test("ui.knob")
ceammc_ui_test("ui.keyboard")
ceammc_ui_test("ui.label")
ceammc_ui_test("ui.link")
ceammc_ui_test("ui.matrix")
ceammc_ui_test("ui.menu")
ceammc_ui_test("ui.meter")
ceammc_ui_test("ui.number")
ceammc_ui_test("ui.number_tilde")
ceammc_ui_test("ui.radio")
ceammc_ui_test("ui.rslider")
ceammc_ui_test("ui.polar")
ceammc_ui_test("ui.preset")
ceammc_ui_test("ui.slider2d")
ceammc_ui_test("ui.sliders")
ceammc_ui_test("ui.tab")
ceammc_ui_test("ui.toggle")


ceammc_add_test_linked(TITLE "[env->to_array]"
    NAME test_ext_env_to_array
    LINK tests_main_lib lib_catch1_main ceammc_array ceammc_env ceammc_array ceammc_core puredata-core)

if(WITH_TTS_FLITE)
    ceammc_external_test(misc speech_flite)
endif()

if(WITH_FLUIDSYNTH)
    ceammc_external_test(misc fluid)
endif()

add_executable(test_pd_core test_pd_core.cpp)
target_link_libraries(test_pd_core puredata-core)

ceammc_add_test_linked(TITLE "class_wrapper::class" NAME test_wrapper_class LINK wrapper_lib)
ceammc_add_test_linked(TITLE "class_wrapper::method" NAME test_wrapper_method LINK wrapper_lib)
ceammc_add_test_linked(TITLE "class_wrapper::static_method" NAME test_wrapper_static_method LINK wrapper_lib)

add_executable(test_exec test_exec.cpp)

add_custom_target(test_src SOURCES gen_test_wav.py)

add_subdirectory(array)
add_subdirectory(list)
add_subdirectory(math)
add_subdirectory(random)
add_subdirectory(string)
