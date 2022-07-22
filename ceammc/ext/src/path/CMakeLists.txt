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

ceammc_path_external(basename)
ceammc_path_external(dirname)
ceammc_path_external(exists)
ceammc_path_external(is_dir)
ceammc_path_external(listdir)
ceammc_path_external(monitor)
ceammc_path_external(normalize)
ceammc_path_external(search)
ceammc_path_external(split)


