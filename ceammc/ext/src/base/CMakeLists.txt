include_directories(${CMAKE_BINARY_DIR}) # for config.h

set(BASE_SOURCES
    expand_env.cpp
    function.cpp
    function_call.cpp
    load_msg.cpp
    msg.cpp
    msg_after.cpp
    obj_info.cpp
    obj_props.cpp
    prop.get.cpp
    prop.set.cpp
    prop.get_tilde.cpp
    patch_args.cpp
    canvas_current.cpp
    canvas_top.cpp
    metro_pattern.cpp
    metro_seq.cpp
    radio.cpp
    replace.cpp)

# predicates
list(APPEND BASE_SOURCES
    is_any.cpp
    is_bang.cpp
    is_data.cpp
    is_even.cpp
    is_float.cpp
    is_file.cpp
    is_list.cpp
    is_odd.cpp
    is_pointer.cpp
    is_symbol.cpp)

# test
list(APPEND BASE_SOURCES
    test_data.cpp
    test_expect.cpp)

# sound
list(APPEND BASE_SOURCES
    base_click.cpp
    gain.cpp
    matrix.cpp
    mix.cpp
    xfade_tilde.cpp
    xfade2_tilde.cpp)

if(TARGET ceammc_sound)
    add_definitions(-DWITH_SND_FILE)
    list(APPEND BASE_SOURCES snd_file.cpp)
endif()

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
if(UNIX)
    set_target_properties(ceammc_base PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

# separate extension:
# becaust it use CEAMMC Pd API
ceammc_cxx_extension(debug gensym)
