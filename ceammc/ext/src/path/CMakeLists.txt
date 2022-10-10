add_library(ceammc_path STATIC
    mod_path.cpp
    datatype_path.cpp
    file_size.cpp
)
target_include_directories(ceammc_path
    PRIVATE
        ${CMAKE_BINARY_DIR}
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

function(ceammc_path_external name)
    target_sources(ceammc_path PRIVATE "path_${name}.cpp")
endfunction()

find_package(RAGEL 6.6)

if(RAGEL_FOUND)
    RAGEL_TARGET(new_file_pattern.rl
        new_file_pattern.rl new_file_pattern.cpp
        COMPILE_FLAGS "-G2 -s")

    add_custom_target(ceammc_path_rl SOURCES new_file_pattern.rl)
endif()

target_sources(ceammc_path PRIVATE new_file_pattern.cpp)

ceammc_path_external(basename)
ceammc_path_external(dirname)
ceammc_path_external(exists)
ceammc_path_external(file)
ceammc_path_external(is_dir)
ceammc_path_external(listdir)
ceammc_path_external(monitor)
ceammc_path_external(normalize)
ceammc_path_external(pattern)
ceammc_path_external(search)
ceammc_path_external(split)


