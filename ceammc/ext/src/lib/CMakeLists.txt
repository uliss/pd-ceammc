include_directories(${PROJECT_SOURCE_DIR}/src)

set(FLAGS "${PD_EXTERNAL_CFLAGS}")
if(${WITH_COVERAGE})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(FLAGS "${FLAGS} --coverage")
    endif()
endif()

# main static library
add_library(ceammc_static STATIC ceammc.c ceammc.cpp ceammc_atom.cpp)
set_target_properties(ceammc_static PROPERTIES
    COMPILE_FLAGS "${FLAGS}" LINK_FLAGS "${FLAGS}")

