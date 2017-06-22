include_directories(Catch/single_include)
include_directories(../lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}) # for config.h
include(tests.cmake)
add_definitions("-DTEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
#remove_definitions(-DPD_INTERNAL)

add_library(tests_main_lib STATIC tests_main.cpp test_external)

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../lib")
endif()

ceammc_add_test("ceammc::Array" test_array)
ceammc_add_test("ceammc::Atom" test_atom)
ceammc_add_test("ceammc::Atom2" test_atom2)
ceammc_add_test("ceammc::AtomList" test_atomlist)
ceammc_add_test("ceammc::AtomList2" test_atomlist2)
ceammc_add_test("ceammc::ControlValue" test_message)
ceammc_add_test("ceammc::DataAtom" test_dataatom)
ceammc_add_test("ceammc::DataAtomList" test_dataatomlist)
ceammc_add_test("ceammc::Data" test_data)
ceammc_add_test("ceammc::DataStorage" test_datastorage)
ceammc_add_test("ceammc::SharedData" test_shareddata)
ceammc_add_test("ceammc::formatting" test_format)
ceammc_add_test("ceammc::properties" test_properties)
ceammc_add_test("ceammc::BaseObject" test_baseobject)
ceammc_add_test("ceammc::Factory" test_factory)
ceammc_add_test("ceammc::list" test_fn_list)
ceammc_add_test("ceammc::vector" test_fn_vector)
ceammc_add_test("ceammc::faust" test_faust)
ceammc_add_test("ceammc::timeline" test_timeline)
ceammc_add_test("ceammc::platform" test_platform)
ceammc_add_test("ceammc::convert" test_convert)
ceammc_add_test("ceammc::string" test_string)
ceammc_add_test("ceammc::window" test_window)

ceammc_external_test(array copy)
ceammc_external_test(array do)
ceammc_external_test(array each)
ceammc_external_test(array fill)
ceammc_external_test(array minmax)
ceammc_external_test(array p2s)
ceammc_external_test(array s2p)

ceammc_external_test(base function)

ceammc_external_test(flow change)
ceammc_external_test(flow group)
ceammc_external_test(flow pass)
ceammc_external_test(flow pass_if)
ceammc_external_test(flow reject)
ceammc_external_test(flow split)

ceammc_external_test(conv cc2amp)
ceammc_external_test(conv lin2exp)
ceammc_external_test(conv lin2lin)

ceammc_external_test(data set)
ceammc_external_test(data set_size)
ceammc_external_test(data set_intersection)
ceammc_external_test(data set_difference)
ceammc_external_test(data set_symdiff)
ceammc_external_test(data set_union)

ceammc_external_test(list all_of)
ceammc_external_test(list any_of)
ceammc_external_test(list none_of)
ceammc_external_test(list rotate)
ceammc_external_test(list slice)

ceammc_external_test(math approx)
ceammc_external_test(math and)
ceammc_external_test(math gcd)
ceammc_external_test(math lcm)
ceammc_external_test(math or)

ceammc_external_test(string format)
ceammc_external_test(string str)
ceammc_external_test(string split)
ceammc_external_test(string to_symbol)
ceammc_external_test(string ends_with)
ceammc_external_test(string starts_with)
ceammc_external_test(string length)

ceammc_add_extension_test("list_walk" "../list/list_walk.cpp")
ceammc_add_extension_test("list_walk2" "../list/list_walk.cpp")
ceammc_add_extension_test("snd_file" "../base/snd_file.cpp")
ceammc_add_extension_test("ext_data_list" "../data/data_list.cpp")
ceammc_add_extension_test("ext_replace" "../base/replace.cpp")
ceammc_add_extension_test("ext_expand_env" "../base/expand_env.cpp")
ceammc_add_extension_test("ext_msg" "../base/msg.cpp")
ceammc_add_extension_test("ext_list_at" "../list/list_at.cpp")
ceammc_add_extension_test("ext_list_each" "../list/list_each.cpp")
ceammc_add_extension_test("ext_list_gen" "../list/list_gen.cpp")
ceammc_add_extension_test("ext_list_seq" "../list/list_seq.cpp")
ceammc_add_extension_test("ext_path_lsdir" "../path/path_listdir.cpp")
ceammc_add_extension_test("ext_symbol_equal" "../symbol/symbol_equal.cpp")
ceammc_add_extension_test("ext_symbol_num_compare" "../symbol/symbol_num_compare.cpp")
ceammc_add_extension_test("ext_system_getenv" "../system/system_getenv.cpp")

ceammc_add_test_linked(TITLE "ceammc::pd" NAME test_pd SRC "../base/debug_gensym.cpp" LINK ceammc_core puredata-core)
ceammc_add_test_linked(TITLE "ceammc::DataString"
    NAME test_data_string
    SRC "test_data_string.cpp"
    LINK ceammc_core ceammc_string puredata-core ceammc_core)


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

add_custom_target(test_src SOURCES gen_test_wav.py)
