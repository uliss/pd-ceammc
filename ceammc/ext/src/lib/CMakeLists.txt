# main shared library
add_library(ceammc_core SHARED ceammc.cpp)
target_include_directories(ceammc_core PRIVATE ${CMAKE_BINARY_DIR}) # for config.h
target_include_directories(ceammc_core PRIVATE
    ${PROJECT_SOURCE_DIR}/src # m_pd.h
    ${PROJECT_BINARY_DIR}/ceammc # ceammc_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8rewind
)

add_executable(lemon lex/lemon.c)

set(UTF8REWIND_SOURCES
    utf8rewind/utf8rewind.c
    utf8rewind/unicodedatabase.c
    utf8rewind/internal/casemapping.c
    utf8rewind/internal/codepoint.c
    utf8rewind/internal/composition.c
    utf8rewind/internal/database.c
    utf8rewind/internal/decomposition.c
    utf8rewind/internal/seeking.c
    utf8rewind/internal/streaming.c
)
target_sources(ceammc_core PRIVATE ${UTF8REWIND_SOURCES})

# tcl stuff
include(tcl_autogen)
add_custom_target(ceammc_lib_tcl SOURCES)
tcl_autogen_header(tcl/ceammc_tooltips.tcl TARGET ceammc_core SOURCES ceammc_lib_tcl)
tcl_autogen_header(tcl/ceammc_texteditor.tcl TARGET ceammc_core SOURCES ceammc_lib_tcl)
tcl_autogen_header(tcl/ceammc_tableeditor.tcl TARGET ceammc_core SOURCES ceammc_lib_tcl)
tcl_autogen_header(tcl/ceammc_patheditor.tcl TARGET ceammc_core SOURCES ceammc_lib_tcl)
target_include_directories(ceammc_core PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/tcl)

set(CEAMMC_LIB_SOURCES
    ceammc.h
    ceammc_abstractdata.cpp
    args/argcheck.cpp
    tcl/ceammc_tcl.cpp
    ceammc_array.cpp
    ceammc_atom.cpp
    ceammc_atomlist.cpp
    ceammc_atomlist_view.cpp
    ceammc_base64.cpp
    ceammc_canvas.cpp
    ceammc_control.cpp
    ceammc_convert.cpp
    ceammc_convert_pitch.cpp
    ceammc_data.cpp
    ceammc_datastorage.cpp
    ceammc_deprecated.cpp
    ceammc_dsp.cpp
    ceammc_editor_object.cpp
    ceammc_either.cpp
    ceammc_externals.cpp
    ceammc_factory.cpp
    ceammc_faust.cpp
    ceammc_faust_ui.cpp
    ceammc_filesystem.cpp
    ceammc_fn_list.cpp
    ceammc_format.cpp
    ceammc_function.cpp
    ceammc_impl.cpp
    ceammc_inlet.cpp
    ceammc_json.cpp
    ceammc_json.h
    ceammc_log.cpp
    ceammc_message.cpp
    ceammc_mime_type.cpp
    ceammc_music_chord.cpp
    ceammc_music_scale.cpp
    ceammc_music_spn.cpp
    ceammc_music_temperament.cpp
    ceammc_music_theory.cpp
    ceammc_music_theory_duration.cpp
    ceammc_music_theory_names.cpp
    ceammc_music_theory_pitch_class.cpp
    ceammc_music_theory_tempo.cpp
    ceammc_music_theory_timesig.cpp
    ceammc_node_external.cpp
    ceammc_notify.cpp
    ceammc_numeric.h
    ceammc_object.cpp
    ceammc_object_info.cpp
    ceammc_outlet.cpp
    ceammc_output.cpp
    ceammc_pd.cpp
    ceammc_platform.cpp
    ceammc_pipe_dispatcher.cpp
    ceammc_poll_dispatcher.cpp
    ceammc_socket_dispatcher.cpp
    ceammc_preset.cpp
    ceammc_property.cpp
    ceammc_property_bpm.cpp
    ceammc_property_callback.cpp
    ceammc_property_duration.cpp
    ceammc_property_enum.cpp
    ceammc_property_info.cpp
    ceammc_property_message.cpp
    ceammc_property_timesig.cpp
    ceammc_random.cpp
    ceammc_regexp.cpp
    ceammc_rtree.cpp
    ceammc_score.cpp
    ceammc_sound_external.cpp
    ceammc_soxr_resampler.cpp
    ceammc_string.cpp
    ceammc_string_split.cpp
    ceammc_table_editor.cpp
    ceammc_thread.cpp
    ceammc_timeline.cpp
    ceammc_transport.cpp
    ceammc_units.cpp
    ceammc_window.cpp
    datatype_color.cpp
    datatype_dict.cpp
    datatype_env.cpp
    datatype_json.cpp
    datatype_mlist.cpp
    datatype_property.cpp
    datatype_set.cpp
    datatype_string.cpp
)

if(UNIX)
    list(APPEND CEAMMC_LIB_SOURCES ceammc_platform_unix.cpp)
elseif(WIN32)
    list(APPEND CEAMMC_LIB_SOURCES ceammc_platform_win.cpp)
endif()

target_sources(ceammc_core PRIVATE ${CEAMMC_LIB_SOURCES})

#
# UI library
#
target_include_directories(ceammc_core PRIVATE cicm/Sources)

set(CEAMMC_UI_SRC
    cicm/Sources/ebox.cpp
    cicm/Sources/eclass.cpp
    cicm/Sources/ecommon.cpp
    cicm/Sources/egraphics.cpp
    cicm/Sources/eobj.cpp
    cicm/Sources/epopup.cpp
    ceammc_cicm.h
    ceammc_cicm.cpp
    ceammc_ui.h
    ceammc_ui_object.h
    ceammc_ui_object.cpp
    image_detect.cpp)

target_sources(ceammc_core PRIVATE ${CEAMMC_UI_SRC})

find_package(RAGEL 6.6)

if(RAGEL_FOUND)
    RAGEL_TARGET(parser_numeric
        lex/parser_numeric.rl lex/parser_numeric.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_color
        lex/parser_color.rl lex/parser_color.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_hexbeat
        lex/parser_hexbeat.rl lex/parser_hexbeat.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_music
        lex/parser_music.rl lex/parser_music.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_midi
        lex/parser_midi.rl lex/parser_midi.cpp)
    RAGEL_TARGET(parser_units
        lex/parser_units.rl lex/parser_units.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_props
        lex/parser_props.rl lex/parser_props.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_clone
        lex/parser_clone.rl lex/parser_clone.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_strings
        lex/parser_strings.rl lex/parser_strings.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(lemon_data_string_parser
        lex/lemon_data_string_parser.rl lex/lemon_data_string_parser.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_argcheck
        args/argcheck.rl args/argcheck.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_array_saver
        lex/parser_array_saver.rl lex/parser_array_saver.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_faust
        lex/parser_faust.rl lex/parser_faust.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(ragel_music
        lex/ragel_music.rl lex/ragel_music.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_guido
        lex/parser_guido.rl lex/parser_guido.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_timeline
        lex/parser_timeline.rl lex/parser_timeline.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_ui_bind
        lex/parser_ui_bind.rl lex/parser_ui_bind.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(ceammc_editor_unescape
        ceammc_editor_unescape.rl ceammc_editor_unescape.cpp
        COMPILE_FLAGS "-G2 -s")
    RAGEL_TARGET(parser_dict_expr
        lex/parser_dict_expr.rl lex/parser_dict_expr.cpp
        COMPILE_FLAGS "-G2 -s")
endif()

add_custom_target(lemon_data_parser
    COMMAND $<TARGET_FILE:lemon> -s -q lemon_data_string.y
    COMMAND mv lemon_data_string.c lemon_data_parser_impl.cpp
    SOURCES lex/lemon_data_string.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex
)

# ragel generated sources
target_sources(ceammc_core
    PRIVATE
        ceammc_editor_unescape.cpp
        lex/lemon_data_parser_impl.cpp
        lex/lemon_data_parser_impl.h
        lex/lemon_data_string_parser.cpp
        lex/lemon_data_string_parser.h
        lex/parser_array_saver.cpp
        lex/parser_clone.cpp
        lex/parser_color.cpp
        lex/parser_dict_expr.cpp
        lex/parser_faust.cpp
        lex/parser_guido.cpp
        lex/parser_hexbeat.cpp
        lex/parser_midi.cpp
        lex/parser_music.cpp
        lex/parser_numeric.cpp
        lex/parser_props.cpp
        lex/parser_props_details.cpp
        lex/parser_strings.cpp
        lex/parser_timeline.cpp
        lex/parser_ui_bind.cpp
        lex/parser_units.cpp
        lex/ragel_music.cpp
)

# ragel files
file(GLOB RAGEL_SOURCES lex/*.rl)
add_custom_target(lex_ragel_sources SOURCES ${RAGEL_SOURCES} args/argcheck.rl)

# liblo
find_package(liblo)
if(LIBLO_FOUND)
    target_compile_definitions(ceammc_core PRIVATE WITH_LIBLO)
    target_include_directories(ceammc_core PRIVATE ${LIBLO_INCLUDE_DIRS})
    target_link_libraries(ceammc_core PUBLIC ${LIBLO_LIBRARIES})
endif()
target_sources(ceammc_core PRIVATE ceammc_osc.cpp)

if(BISON_FOUND AND FLEX_FOUND)
    add_custom_target(ceammc_arrayloader_gen
        COMMAND $<TARGET_FILE:reflex-bin> array_loader.l
        COMMAND ${BISON_EXECUTABLE} array_loader.y
        SOURCES lex/array_loader.l
                lex/array_loader.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex)

    add_custom_target(ceammc_select_gen
        COMMAND $<TARGET_FILE:reflex-bin> select.l
        COMMAND ${BISON_EXECUTABLE} select.y
        SOURCES lex/select.l lex/select.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex)

    add_custom_target(ceammc_score_gen
        COMMAND $<TARGET_FILE:reflex-bin> score.l
        COMMAND ${BISON_EXECUTABLE} score.y
        SOURCES lex/score.l lex/score.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex)
endif()

if(WIN32)
    # this is rather important thing, we link ceammc_core to fmt whole static archive and then
    # we DON'T LINK(!) to fmt no more. If we need to use fmt we should link to ceammc_core
    target_link_libraries(ceammc_core PRIVATE -Wl,--whole-archive fmt::fmt soxr -Wl,--no-whole-archive)
else()
    target_link_libraries(ceammc_core PRIVATE fmt soxr)
endif()

if(WITH_LIBSAMPLERATE)
    find_package(LibSamplerate)
    if(NOT LIBSAMPLERATE_FOUND)
        message(FATAL "libsamplerate not found")
    endif()
    target_include_directories(ceammc_core PRIVATE ${LIBSAMPLERATE_INCLUDE_DIR}>)
    target_link_libraries(ceammc_core PRIVATE ${LIBSAMPLERATE_LIBRARIES})
    target_compile_definitions(ceammc_core PRIVATE WITH_LIBSAMPLERATE)
endif()

find_package(LibMagic)
if(LibMagic_FOUND AND NOT WIN32) # strange crash in windows
    target_include_directories(ceammc_core PRIVATE ${LIBMAGIC_INCLUDE_DIRS})
    target_link_libraries(ceammc_core PRIVATE ${LIBMAGIC_LIBRARIES})
    target_compile_definitions(ceammc_core PRIVATE WITH_LIBMAGIC)
endif()

target_link_libraries(ceammc_core PRIVATE re2)
target_link_libraries(ceammc_core PRIVATE reflex)
target_link_libraries(ceammc_core PRIVATE muparser)
target_link_libraries(ceammc_core PRIVATE readerwriterqueue)
target_link_libraries(ceammc_core PRIVATE colorm)
#target_link_libraries(ceammc_core PRIVATE ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
#target_link_libraries(ceammc_core PRIVATE puredata-core)

target_include_directories(ceammc_core PRIVATE $<TARGET_PROPERTY:re2,INCLUDE_DIRECTORIES>)
target_include_directories(ceammc_core PRIVATE $<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>)
target_include_directories(ceammc_core PRIVATE $<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>)
target_include_directories(ceammc_core PRIVATE $<TARGET_PROPERTY:soxr,INCLUDE_DIRECTORIES>)

target_compile_options(ceammc_core PRIVATE "${PD_EXTERNAL_CFLAGS}")

# we don't have target_link_options on travis now
set_property(TARGET ceammc_core APPEND_STRING PROPERTY LINK_FLAGS "${PD_EXTERNAL_LDFLAGS}")

if(WIN32)
    target_link_libraries(ceammc_core PUBLIC psapi shlwapi userenv puredata-core)
    target_link_options(ceammc_core PRIVATE "-Wl,--enable-auto-import")

    # copy library to tests folder
    add_custom_command(TARGET ceammc_core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:ceammc_core>"
            "${CMAKE_CURRENT_BINARY_DIR}/../tests")
endif()


# sound library
#
set(CEAMMC_LOAD_SRC
    ceammc_sound.h
    ceammc_sound.cpp
    lex/array_loader.lexer.cpp
    lex/array_loader.parser.cpp
    lex/array_loader.cpp
    lex/array_saver.cpp
    lex/select.lexer.cpp
    lex/select.parser.cpp
    lex/score.lexer.cpp
    lex/score.parser.cpp
    array_load_pattern.h
    array_load_pattern.cpp)

target_sources(ceammc_core PRIVATE ${CEAMMC_LOAD_SRC})

# libsndfile backend
if(LIBSNDFILE_FOUND)
    target_sources(ceammc_core PRIVATE ceammc_loader_sndfile.cpp)
    target_include_directories(ceammc_core PRIVATE ${LIBSNDFILE_INCLUDE_DIRS})
    target_link_libraries(ceammc_core PUBLIC ${LIBSNDFILE_LIBRARIES})
endif()

# minimp3
target_sources(ceammc_core PRIVATE ceammc_loader_minimp3.cpp)
target_include_directories(ceammc_core PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/extra/minimp3)

# coreaudio backend
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_sources(ceammc_core PRIVATE
        ceammc_loader_coreaudio.cpp
        ceammc_loader_coreaudio_impl.m)

    # find_library(QUICKTIME_LIBRARY QuickTime)
    # find_library(APP_SERVICES_LIBRARY ApplicationServices)

    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    target_link_libraries(ceammc_core PRIVATE ${FOUNDATION_LIBRARY})

    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
    target_link_libraries(ceammc_core PRIVATE ${AUDIOTOOLBOX_LIBRARY})
endif()

# install
if(WIN32)
    install(TARGETS ceammc_core  RUNTIME DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
else()
    install(TARGETS ceammc_core  LIBRARY DESTINATION ${PDCORE_LIB_INSTALL_PATH})
endif()

# utilities used while developing
if(ENABLE_TESTS)
    add_library(ext_load STATIC ext_load_lib.cpp)
    target_include_directories(ext_load PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_compile_definitions(ext_load
        PRIVATE
            "CEAMMC_DLL=\"$<TARGET_FILE:ceammc>\""
            "STK_RAWWAVES=\"${PROJECT_SOURCE_DIR}/ceammc/extra/stk/stk/rawwaves\"")

    add_executable(ext_aliases ext_aliases.cpp)
    target_include_directories(ext_aliases PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_aliases PRIVATE ext_load puredata-core)

    add_executable(ext_list ext_list.cpp)
    target_include_directories(ext_list PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_list PRIVATE ext_load puredata-core)

    add_executable(ext_methods ext_methods.cpp)
    target_include_directories(ext_methods PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_methods PRIVATE ext_load puredata-core)

    add_executable(ext_props ext_props.cpp)
    target_include_directories(ext_props PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_props PRIVATE ext_load puredata-core)

    add_executable(ext_info ext_info.cpp)
    target_include_directories(ext_info PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_info PRIVATE ext_load puredata-core)

    if(WIN32)
        target_link_libraries(ext_info PRIVATE soxr)
        target_link_libraries(ext_list PRIVATE soxr)
        target_link_libraries(ext_methods PRIVATE soxr)
        target_link_libraries(ext_props PRIVATE soxr)
    endif()
endif()

add_custom_target(ceammc_cicm_tcl_gen
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh cicm/Sources/cicm_common.tcl
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh cicm/Sources/cicm_common_bind.tcl
    SOURCES cicm/Sources/cicm_common.tcl cicm/Sources/cicm_common_bind.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# new UI library
add_subdirectory(nui)
