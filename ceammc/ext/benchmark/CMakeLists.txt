add_library(bm_main STATIC bm_main.cpp)
target_include_directories(bm_main PUBLIC ${PROJECT_SOURCE_DIR}/ceammc/extra)

set(BM_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}")

function(add_benchmark name)
    add_executable(bm_${name} bm_${name}.cpp)
    if(UNIX)
        target_compile_options(bm_${name} PRIVATE "-fPIC")
    endif()

    target_include_directories(bm_${name}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/ceammc/ext/src
        ${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib
    )

    target_link_libraries(bm_${name}
    PRIVATE
        bm_main
        ceammc_base
        ceammc_list
        ceammc_array
        ceammc_net
        ceammc_data
        ceammc_core
        puredata-core
        fmt
        re2
    )

    set_property(TARGET bm_${name} APPEND_STRING PROPERTY LINK_FLAGS "${PD_EXTERNAL_LDFLAGS}")

    message(STATUS "Benchmark target added: benchmark_${name}")

    set(_REPORT "${BM_HTML_DIR}/benchmark_${name}.html")
    add_custom_target(benchmark_${name}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bm_${name} -r html -o ${_REPORT}
        COMMAND open ${_REPORT}
        DEPENDS bm_${name})
endfunction()

add_benchmark(array)
add_benchmark(atomlist)
add_benchmark(control_externals)
add_benchmark(core)
add_benchmark(dataptr)
add_benchmark(grain_expr)
add_benchmark(lowlevel)
add_benchmark(parse)

# extra options
target_include_directories(bm_core
    PRIVATE
        $<TARGET_PROPERTY:re2,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>
)
target_include_directories(bm_grain_expr PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/ext/src/array)
