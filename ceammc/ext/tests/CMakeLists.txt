include_directories(Catch/single_include)
include_directories(../src/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}) # for config.h
include(tests.cmake)
add_definitions("-DTEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
#remove_definitions(-DPD_INTERNAL)

add_library(tests_main_lib STATIC tests_main.cpp test_external test_datatypes.cpp
    test_approx.cpp
    test_wrappers.cpp)

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../src/lib")
endif()

ceammc_add_test("ceammc::Array" test_array)
ceammc_add_test("ceammc::Atom" test_atom)
ceammc_add_test("ceammc::Atom2" test_atom2)
ceammc_add_test("ceammc::AtomList" test_atomlist)
ceammc_add_test("ceammc::AtomList2" test_atomlist2)
ceammc_add_test("ceammc::Canvas" test_canvas)
ceammc_add_test("ceammc::ControlValue" test_message)
ceammc_add_test("ceammc::DataAtom" test_dataatom)
ceammc_add_test("ceammc::DataAtomList" test_dataatomlist)
ceammc_add_test("ceammc::Data" test_data)
ceammc_add_test("ceammc::DataStorage" test_datastorage)
ceammc_add_test("ceammc::SharedData" test_shareddata)
ceammc_add_test("ceammc::pd::External" test_pd_external)
ceammc_add_test("ceammc::formatting" test_format)
ceammc_add_test("ceammc::properties" test_properties)
ceammc_add_test("ceammc::BaseObject" test_baseobject)
ceammc_add_test("ceammc::SoundExternal" test_sound_external)
ceammc_add_test("ceammc::Factory" test_factory)
ceammc_add_test("ceammc::list" test_fn_list)
ceammc_add_test("ceammc::list2" test_fn_list2)
ceammc_add_test("ceammc::vector" test_fn_vector)
ceammc_add_test("ceammc::faust" test_faust)
ceammc_add_test("ceammc::music_theory" test_music_theory)
ceammc_add_test("ceammc::music_theory_alteration" test_music_theory_alteration)
ceammc_add_test("ceammc::music_theory_pitch_name" test_music_theory_pitch_name)
ceammc_add_test("ceammc::music_theory_pitch_class" test_music_theory_pitch_class)
ceammc_add_test("ceammc::music_theory_keyboard" test_music_theory_keyboard)
ceammc_add_test("ceammc::timeline" test_timeline)
ceammc_add_test("ceammc::platform" test_platform)
ceammc_add_test("ceammc::convert" test_convert)
ceammc_add_test("ceammc::signal" test_signal)
ceammc_add_test("ceammc::string" test_string)
ceammc_add_test("ceammc::window" test_window)
ceammc_add_test("ceammc::proxy" test_proxy)

ceammc_external_test(array bpm)
ceammc_external_test(array copy)
ceammc_external_test(array do)
ceammc_external_test(array each)
ceammc_external_test(array fill)
ceammc_external_test(array minmax)
ceammc_external_test(array p2s)
ceammc_external_test(array s2p)
ceammc_external_test(array stretch)
ceammc_external_test(array vplay)

ceammc_external_test(base function)
ceammc_external_test(base gain)
ceammc_external_test(base matrix)
ceammc_external_test(base mix)
ceammc_external_test(base prop_get)
ceammc_external_test(base radio)
ceammc_external_test(base xfade)
ceammc_external_test(base xfade2)

ceammc_add_test("[canvas.args]" test_ext_canvas_args)
ceammc_add_test("[canvas.current]" test_ext_canvas_current)
ceammc_add_test("[canvas.top]" test_ext_canvas_top)
ceammc_add_test("[metro.pattern]" test_ext_metro_pattern)
ceammc_add_test("[metro.seq]" test_ext_metro_seq)

ceammc_external_test(env env)
ceammc_external_test(env to_vline)
ceammc_external_test(env tscale)
ceammc_external_test(env tshift)

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
ceammc_external_test(flow pass)
ceammc_external_test(flow pass_if)
ceammc_external_test(flow reject)
ceammc_external_test(flow route)
ceammc_external_test(flow split)
ceammc_external_test(flow speedlim)

ceammc_external_test(fx looper)

ceammc_external_test(conv amp2dbfs)
ceammc_external_test(conv cc2amp)
ceammc_external_test(conv dbfs2amp)
ceammc_external_test(conv lin2exp)
ceammc_external_test(conv lin2lin)

ceammc_external_test(data dict)
ceammc_external_test(data dict_contains)
ceammc_external_test(data dict_get)
ceammc_external_test(data global_list)
ceammc_external_test(data global_mlist)
ceammc_external_test(data global_set)
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

ceammc_external_test(list all_of)
ceammc_external_test(list any_of)
ceammc_external_test(list append)
ceammc_external_test(list apply_to)
ceammc_external_test(list at)
ceammc_external_test(list choice)
ceammc_external_test(list contains)
ceammc_external_test(list count)
ceammc_external_test(list count_if)
ceammc_external_test(list do)
ceammc_external_test(list each)
ceammc_external_test(list enumerate)
ceammc_external_test(list equal)
ceammc_external_test(list gen)
ceammc_external_test(list insert)
ceammc_external_test(list last)
ceammc_external_test(list length)
ceammc_external_test(list max)
ceammc_external_test(list mean)
ceammc_external_test(list min)
ceammc_external_test(list none_of)
ceammc_external_test(list normalize)
ceammc_external_test(list pass_if)
ceammc_external_test(list prepend)
ceammc_external_test(list product)
ceammc_external_test(list range)
ceammc_external_test(list reduce)
ceammc_external_test(list remove)
ceammc_external_test(list remove_if)
ceammc_external_test(list repeat)
ceammc_external_test(list reverse)
ceammc_external_test(list rldecode)
ceammc_external_test(list rlencode)
ceammc_external_test(list rotate)
ceammc_external_test(list route)
ceammc_external_test(list shift)
ceammc_external_test(list search)
ceammc_external_test(list set)
ceammc_external_test(list slice)
ceammc_external_test(list stretch)
ceammc_external_test(list sort)
ceammc_external_test(list sort_with)
ceammc_external_test(list split)
ceammc_external_test(list sum)
ceammc_external_test(list unique)
ceammc_external_test(list unpack)
ceammc_external_test(list xat)
ceammc_external_test(list xcontains)
ceammc_external_test(list xsearch)
ceammc_external_test(list zip)

ceammc_external_test(math and)
ceammc_external_test(math approx)
ceammc_external_test(math div)
ceammc_external_test(math expr)
ceammc_external_test(math gcd)
ceammc_external_test(math lcm)
ceammc_external_test(math mul)
ceammc_external_test(math or)
ceammc_external_test(math polyeval)
ceammc_external_test(math round_tilde)

ceammc_external_test(midi file)
ceammc_external_test(midi note)
ceammc_external_test(midi track)
ceammc_external_test(midi ctl_to_str)
ceammc_external_test(midi key_to_str)
ceammc_external_test(midi prg_to_str)

ceammc_external_test(random discrete)
ceammc_external_test(random linear)
ceammc_external_test(random pw_lin)
ceammc_external_test(random pw_const)

ceammc_external_test(preset float)
ceammc_external_test(preset symbol)
ceammc_external_test(preset list)

ceammc_external_test(string format)
ceammc_external_test(string str)
ceammc_external_test(string split)
ceammc_external_test(string to_symbol)
ceammc_external_test(string ends_with)
ceammc_external_test(string starts_with)
ceammc_external_test(string length)

ceammc_external_test(system getenv)
ceammc_external_test(system shell)

ceammc_external_test(path lsdir)

ceammc_external_test(spat pan_lin)
ceammc_external_test(spat pan_sqrt)
ceammc_external_test(spat pan_cos)
ceammc_external_test(spat pan_spread)

ceammc_add_test_linked(TITLE "tl.transport"
    NAME test_ext_tl_transport
    LINK ceammc_tl ceammc_ui ceammc_core puredata-core ceammc_core)
ceammc_external_test(tl timeline)

ceammc_add_extension_test("snd_file" "../src/base/snd_file.cpp")
ceammc_add_extension_test("ext_replace" "../src/base/replace.cpp")
ceammc_add_extension_test("ext_expand_env" "../src/base/expand_env.cpp")
ceammc_add_extension_test("ext_msg" "../src/base/msg.cpp")

ceammc_add_extension_test("list_walk" "../src/list/list_walk.cpp")
ceammc_add_extension_test("list_walk2" "../src/list/list_walk.cpp")
ceammc_add_extension_test("ext_list_seq" "../src/list/list_seq.cpp")
ceammc_add_extension_test("ext_symbol_equal" "../src/symbol/symbol_equal.cpp")
ceammc_add_extension_test("ext_symbol_num_compare" "../src/symbol/symbol_num_compare.cpp")

ceammc_add_test_linked(TITLE "tl::parser" NAME test_tl_parser LINK ceammc_tl)
ceammc_add_test_linked(TITLE "ceammc::preset" NAME test_preset LINK ceammc_preset ceammc_core puredata-core ceammc_core)
ceammc_add_test_linked(TITLE "ceammc::pd" NAME test_pd SRC "../src/base/debug_gensym.cpp" LINK ceammc_core puredata-core)
ceammc_add_test_linked(TITLE "ceammc::DataString"
    NAME test_data_string
    SRC "test_data_string.cpp"
    LINK ceammc_core ceammc_string puredata-core ceammc_core)

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
    LINK tests_main_lib hw_arduino ceammc_core puredata-core)
ceammc_add_test_linked(TITLE "Arduino" NAME test_arduino LINK tests_main_lib hw_arduino ceammc_core puredata-core)

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
ceammc_ui_test("ui.label")
ceammc_ui_test("ui.link")
ceammc_ui_test("ui.matrix")
ceammc_ui_test("ui.menu")
ceammc_ui_test("ui.meter")
ceammc_ui_test("ui.number")
ceammc_ui_test("ui.number_tilde")
ceammc_ui_test("ui.radio")
ceammc_ui_test("ui.rslider")
ceammc_ui_test("ui.preset")
ceammc_ui_test("ui.slider2d")
ceammc_ui_test("ui.sliders")
ceammc_ui_test("ui.tab")
ceammc_ui_test("ui.toggle")


ceammc_add_test_linked(TITLE "[env->to_array]"
    NAME test_ext_env_to_array
    LINK tests_main_lib ceammc_array ceammc_env ceammc_array ceammc_core puredata-core)

ceammc_add_test_linked(TITLE "[speech.flite]"
    NAME test_ext_misc_speech_flite
    LINK ceammc_core ceammc_misc ceammc_string puredata-core ${FLUIDSYNTH_LIBRARIES})


add_custom_target(test_src SOURCES gen_test_wav.py)
