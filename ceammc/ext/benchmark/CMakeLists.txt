include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../extra)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_library(bm_main STATIC bm_main.cpp)

set(BM_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}")

function(add_benchmark name)
    add_executable(bm_${name} bm_${name}.cpp)
    target_link_libraries(bm_${name} bm_main ceammc_core ceammc_base puredata-core
        ceammc_list ceammc_array ceammc_data ceammc_core)

    message(STATUS "Bechmark target added: benchmark_${name}")

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
add_benchmark(data_tree)
add_benchmark(dataptr)


# lowlevel tests
add_executable(bm_lowlevel bm_lowlevel.cpp)
target_link_libraries(bm_lowlevel bm_main fmt)
target_link_libraries(bm_core re2)
get_target_property(_re2_include_dirs re2 INCLUDE_DIRECTORIES)
target_include_directories(bm_core PRIVATE ${_re2_include_dirs})

add_custom_target(benchmark_lowlevel
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bm_lowlevel -r html -o "${BM_HTML_DIR}/benchmark_lowlevel.html"
    COMMAND open "${BM_HTML_DIR}/benchmark_lowlevel.html"
    DEPENDS bm_lowlevel)
