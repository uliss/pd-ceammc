include_directories(${CMAKE_BINARY_DIR}) # for config.h

set(BASE_SOURCES
    base_log.cpp
    base_sync.cpp
    canvas_current.cpp
    canvas_dir.cpp
    canvas_name.cpp
    canvas_path.cpp
    canvas_top.cpp
    ceammc_search.cpp
    expand_env.cpp
    function.cpp
    function_call.cpp
    load_expr.cpp
    load_msg.cpp
    metro_pattern.cpp
    metro_random.cpp
    metro_seq.cpp
    msg.cpp
    msg_after.cpp
    obj_info.cpp
    obj_props.cpp
    patch_args.cpp
    patch_deps.cpp
    patch_props.cpp
    patch_tree.cpp
    prop.cpp
    prop_declare.cpp
    prop_get.cpp
    prop_get_tilde.cpp
    prop_join.cpp
    prop_set.cpp
    prop_split.cpp
    radio.cpp
    replace.cpp)

# predicates
list(APPEND BASE_SOURCES
    is_any.cpp
    is_bang.cpp
    is_data.cpp
    is_even.cpp
    is_file.cpp
    is_float.cpp
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
    base_dac.cpp
    gain.cpp
    matrix.cpp
    mix.cpp
    nsig_tilde.cpp
    xfade_tilde.cpp
    xfade2_tilde.cpp)

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
target_compile_definitions(ceammc_base PUBLIC -DWITH_SND_FILE)
target_sources(ceammc_base PRIVATE snd_file.cpp)
target_include_directories(ceammc_base PRIVATE "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>")

# separate extension:
# because it use CEAMMC Pd API
ceammc_cxx_extension(debug gensym)
