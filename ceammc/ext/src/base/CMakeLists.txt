set(BASE_SOURCES
    base_log.cpp
    base_sync.cpp
    base_spring.cpp
    base_clone.cpp
    canvas_active.cpp
    canvas_current.cpp
    canvas_dir.cpp
    canvas_name.cpp
    canvas_path.cpp
    canvas_top.cpp
    ceammc_search.cpp
    chrono_datetime.cpp
    expand_env.cpp
    function.cpp
    function_call.cpp
    load_expr.cpp
    metro_choice.cpp
    metro_pattern.cpp
    metro_random.cpp
    metro_seq.cpp
    metro_wchoice.cpp
    msg.cpp
    msg_after.cpp
    msg_onclose.cpp
    msg_onload.cpp
    msg_pack.cpp
    msg_sched.cpp
    msg_unpack.cpp
    obj_props.cpp
    patch_args.cpp
    patch_deps.cpp
    patch_props.cpp
    patch_tree.cpp
    prop.cpp
    prop_declare.cpp
    prop_get.cpp
    prop_join.cpp
    prop_random.cpp
    prop_route.cpp
    prop_route_tilde.cpp
    prop_set.cpp
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
    is_prop.cpp
    is_symbol.cpp)

# test
list(APPEND BASE_SOURCES
    test_data.cpp
    test_expect.cpp)

# sound
list(APPEND BASE_SOURCES
    base_click.cpp
    base_convolve_tilde.cpp
    base_dac.cpp
    base_gate_tilde.cpp
    base_merge_tilde.cpp
    base_split_tilde.cpp
    base_z_tilde.cpp
    gain.cpp
    matrix.cpp
    mix.cpp
    nsig_tilde.cpp
    snd_play_tilde.cpp
    xfade_tilde.cpp
    xfade2_tilde.cpp)

find_package(RAGEL 6.6)
if(RAGEL_FOUND)
    RAGEL_TARGET(parser_canvas_id
        parser_canvas_id.rl parser_canvas_id.cpp
        COMPILE_FLAGS "-G2 -s")

    add_custom_target(ragel_base SOURCES parser_canvas_id.rl)
endif()

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
target_compile_definitions(ceammc_base PUBLIC -DWITH_SND_FILE)
target_sources(ceammc_base PRIVATE snd_file.cpp parser_canvas_id.cpp)
target_include_directories(ceammc_base
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
        "$<TARGET_PROPERTY:aubio,INCLUDE_DIRECTORIES>"
        ${PROJECT_SOURCE_DIR}/ceammc/extra/readerwriterqueue
        ${CMAKE_BINARY_DIR} # for config.h

)

target_link_libraries(ceammc_base PRIVATE fftconv)

find_package(RubberBand REQUIRED)
if(RUBBERBAND_FOUND)
    target_include_directories(ceammc_base PRIVATE ${RUBBERBAND_INCLUDE_DIRS})
    target_link_libraries(ceammc_base PRIVATE ${RUBBERBAND_LIBRARIES})
endif()


# separate external:
# because it use CEAMMC Pd API
ceammc_cxx_extension(debug gensym)
