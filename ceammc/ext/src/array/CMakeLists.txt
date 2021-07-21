set(ARRAY_SOURCES
    array_base.cpp
    byte_code.cpp
    grain.cpp
    grain.h
    grain_cloud.cpp
    grain_cloud.h
    grain_random.cpp
    grain_random.h
    plot_linspace_tilde.cpp
    plot_logspace_tilde.cpp
    plot_geomspace_tilde.cpp
    plot_hist_tilde.cpp
    plot_response_tilde.cpp
)

macro(ceammc_array_external name)
    list(APPEND ARRAY_SOURCES "array_${name}.cpp")
endmacro()

ceammc_array_external(bpm)
ceammc_array_external(circular_tilde)
ceammc_array_external(copy)
ceammc_array_external(do)
ceammc_array_external(each)
ceammc_array_external(fill)
ceammc_array_external(grainer)
ceammc_array_external(hist)
ceammc_array_external(mean)
ceammc_array_external(minmax)
ceammc_array_external(p2s)
ceammc_array_external(play_tilde)
ceammc_array_external(plot)
ceammc_array_external(plot_tilde)
ceammc_array_external(resample)
ceammc_array_external(rms)
ceammc_array_external(s2p)
ceammc_array_external(set)
ceammc_array_external(stddev)
ceammc_array_external(stretch)
ceammc_array_external(sum)
ceammc_array_external(sum2)
ceammc_array_external(variance)
ceammc_array_external(vline_play)
ceammc_array_external(window)

add_library(ceammc_array STATIC mod_array.h mod_array.cpp ${ARRAY_SOURCES})

if(UNIX)
    target_link_libraries(ceammc_array PRIVATE soxr)
endif()

find_package(RAGEL 6.6)

if(RAGEL_FOUND)
    RAGEL_TARGET(ragel_grain_expr
        grain_expr.rl ${CMAKE_CURRENT_SOURCE_DIR}/grain_expr_parser.cpp
        COMPILE_FLAGS "-G2 -s")
    add_custom_target(lemon_grain_parser
        COMMAND lemon -s ${CMAKE_CURRENT_SOURCE_DIR}/grain_expr.y
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/grain_expr.c ${CMAKE_CURRENT_SOURCE_DIR}/grain_expr_lemon.h
        COMMAND rm ${CMAKE_CURRENT_SOURCE_DIR}/grain_expr.out)
endif()

# ragel generated sources
target_sources(ceammc_array PRIVATE grain_expr_parser.cpp)

target_include_directories(ceammc_array PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "$<TARGET_PROPERTY:soundtouch,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:soxr,INCLUDE_DIRECTORIES>"
    "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

target_link_libraries(ceammc_array PRIVATE
    soundtouch
)
