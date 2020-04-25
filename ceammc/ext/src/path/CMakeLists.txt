include_directories(${CMAKE_BINARY_DIR})
set(PATH_SOURCES "")

macro(ceammc_path_extension name)
    list(APPEND PATH_SOURCES "path_${name}.cpp")
endmacro()

ceammc_path_extension(exists)
ceammc_path_extension(dirname)
ceammc_path_extension(basename)
ceammc_path_extension(listdir)
ceammc_path_extension(is_dir)

add_library(ceammc_path STATIC mod_path.cpp ${PATH_SOURCES})
