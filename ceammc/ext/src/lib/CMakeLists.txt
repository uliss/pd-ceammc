include_directories(${PROJECT_SOURCE_DIR}/src)

# main static library
add_library(ceammc_static STATIC ceammc.c ceammc.cpp)
set_target_properties(ceammc_static PROPERTIES
    COMPILE_FLAGS ${PD_EXTERNAL_CFLAGS})


