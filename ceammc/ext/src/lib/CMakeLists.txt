# main shared library
add_library(ceammc_core SHARED ceammc.cpp)
target_include_directories(ceammc_core PRIVATE ${CMAKE_BINARY_DIR}) # for config.h
target_include_directories(ceammc_core PRIVATE
    ${PROJECT_SOURCE_DIR}/src # m_pd.h
    ${PROJECT_SOURCE_DIR}/ceammc/extra
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8rewind
)

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

set(CEAMMC_LIB_SOURCES
    ceammc.h
    ceammc_abstractdata.cpp
    ceammc_args.cpp
    args/argcheck.lexer.cpp
    args/argcheck.parser.cpp
    args/arg_checker_node.cpp
    args/arg_checker_single.cpp
    args/arg_group.cpp
    args/arg_group_and.cpp
    args/arg_group_or.cpp
    args/arg_is_atom.cpp
    args/arg_is_bool.cpp
    args/arg_is_data.cpp
    args/arg_is_float.cpp
    args/arg_is_int.cpp
    args/arg_is_symbol.cpp
    lex/data_string.lexer.cpp
    lex/data_string.parser.cpp
    lex/quoted_string.parser.cpp
    ceammc_array.cpp
    ceammc_atom.cpp
    ceammc_atomlist.cpp
    ceammc_atomlist_view.cpp
    ceammc_canvas.cpp
    ceammc_control.cpp
    ceammc_convert.cpp
    ceammc_convert_pitch.cpp
    ceammc_data.cpp
    ceammc_datastorage.cpp
    ceammc_externals.cpp
    ceammc_factory.h
    ceammc_faust.cpp
    ceammc_fn_list.cpp
    ceammc_format.cpp
    ceammc_impl.cpp
    ceammc_json.h
    ceammc_json.cpp
    ceammc_log.cpp
    ceammc_message.cpp
    ceammc_music_scale.cpp
    ceammc_music_temperament.cpp
    ceammc_music_theory.cpp
    ceammc_music_theory_names.cpp
    ceammc_music_theory_pitch_class.cpp
    ceammc_node_external.cpp
    ceammc_numeric.h
    ceammc_object.cpp
    ceammc_object_info.cpp
    ceammc_output.cpp
    ceammc_pd.cpp
    ceammc_platform.cpp
    ceammc_preset.cpp
    ceammc_property.cpp
    ceammc_property_callback.cpp
    ceammc_property_enum.cpp
    ceammc_property_info.cpp
    ceammc_regexp.cpp
    ceammc_sound_external.cpp
    ceammc_string.cpp
    ceammc_thread.cpp
    ceammc_timeline.cpp
    ceammc_units.cpp
    ceammc_window.cpp
    datatype_dict.cpp
    datatype_mlist.cpp
    datatype_property.cpp
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

if(BISON_FOUND AND FLEX_FOUND)
    add_custom_target(ceammc_argcheck_gen
        COMMAND $<TARGET_FILE:reflex-bin> argcheck.l
        COMMAND ${BISON_EXECUTABLE} argcheck.yy
        SOURCES args/argcheck.l args/argcheck.yy
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/args)

    add_custom_target(ceammc_parser_gen
        COMMAND $<TARGET_FILE:reflex-bin> data_string.l
        COMMAND ${BISON_EXECUTABLE} data_string.y
        COMMAND ${BISON_EXECUTABLE} quoted_string.y
        SOURCES lex/data_string.l
                lex/data_string.y
                lex/quoted_string.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex)

    add_custom_target(ceammc_arrayloader_gen
        COMMAND $<TARGET_FILE:reflex-bin> array_loader.l
        COMMAND ${BISON_EXECUTABLE} array_loader.y
        SOURCES lex/array_loader.l
                lex/array_loader.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lex)
endif()

if(WIN32)
    # this is rather important thing, we link ceammc_core to fmt whole static archive and then
    # we DON'T LINK(!) to fmt no more. If we need to use fmt we should link to ceammc_core
    target_link_libraries(ceammc_core PRIVATE -Wl,--whole-archive fmt::fmt soxr -Wl,--no-whole-archive)
else()
    target_link_libraries(ceammc_core PRIVATE fmt soxr)
endif()

target_link_libraries(ceammc_core PRIVATE re2)
target_link_libraries(ceammc_core PRIVATE reflex)
target_link_libraries(ceammc_core PRIVATE muparser)
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

#
# sound library
#
set(CEAMMC_LOAD_SRC
    ceammc_sound.h
    ceammc_sound.cpp
    lex/array_loader.lexer.cpp
    lex/array_loader.parser.cpp
    lex/array_loader.cpp
    array_load_pattern.h
    array_load_pattern.cpp)

target_sources(ceammc_core PRIVATE ${CEAMMC_LOAD_SRC})

# libsndfile backend
if(LIBSNDFILE_FOUND)
    target_sources(ceammc_core PRIVATE ceammc_loader_sndfile.cpp)
    target_include_directories(ceammc_core PRIVATE ${LIBSNDFILE_INCLUDE_DIRS})
    target_link_libraries(ceammc_core PUBLIC ${LIBSNDFILE_LIBRARIES})
endif()

# coreaudio backend
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_sources(ceammc_core PRIVATE
        ceammc_loader_coreaudio.cpp
        ceammc_loader_coreaudio_impl.m)

    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolBox)
    find_library(QUICKTIME_LIBRARY QuickTime)
    find_library(APP_SERVICES_LIBRARY ApplicationServices)

    target_link_libraries(ceammc_core PRIVATE ${FOUNDATION_LIBRARY} ${AUDIOTOOLBOX_LIBRARY})
endif()

# install
if(WIN32)
    install(TARGETS ceammc_core  RUNTIME DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
else()
    install(TARGETS ceammc_core  LIBRARY DESTINATION ${PDCORE_LIB_INSTALL_PATH})
endif()

# utilities used while developing
if(ENABLE_TESTS)
    add_executable(ext_list ext_list.cpp ../mod_init.cpp)
    target_include_directories(ext_list
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${PROJECT_SOURCE_DIR}/src")
    target_link_libraries(ext_list PRIVATE ${CEAMMC_EXT_LINK_LIBRARIES} puredata-core)

    add_executable(ext_methods ext_methods.cpp ../mod_init.cpp)
    target_include_directories(ext_methods PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_methods PRIVATE ${CEAMMC_EXT_LINK_LIBRARIES} puredata-core)

    add_executable(ext_props ext_props.cpp ../mod_init.cpp)
    target_include_directories(ext_props PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ext_props PRIVATE ${CEAMMC_EXT_LINK_LIBRARIES} puredata-core)

    add_executable(ext_info ext_info.cpp ../mod_init.cpp)
    target_include_directories(ext_info
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")
    target_link_libraries(ext_info PRIVATE ${CEAMMC_EXT_LINK_LIBRARIES} puredata-core)

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
