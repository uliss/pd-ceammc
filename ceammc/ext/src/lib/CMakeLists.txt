include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

set(FLAGS "${PD_EXTERNAL_CFLAGS}")
if(${WITH_COVERAGE})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(FLAGS "${FLAGS} --coverage")
    endif()
endif()

# main static library
add_library(ceammc_static STATIC ceammc.c
    ceammc.cpp
    ceammc_atom.cpp
    ceammc_atomlist.cpp
    ceammc_fn_list.cpp
    ceammc_fn_vector.cpp
    ceammc_log.cpp
    ceammc_message.cpp
    ceammc_format.cpp
    ceammc_object.cpp
    ceammc_impl.cpp
    ceammc_property.cpp)

set_target_properties(ceammc_static PROPERTIES
    COMPILE_FLAGS "${FLAGS}" LINK_FLAGS "${FLAGS}")


# timeline library
add_library(ceammc_timeline STATIC
    ceammc_timeline.cpp)

set_target_properties(ceammc_timeline PROPERTIES
    COMPILE_FLAGS "${FLAGS}" LINK_FLAGS "${FLAGS}")

