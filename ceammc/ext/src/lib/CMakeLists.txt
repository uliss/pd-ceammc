include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)
remove_definitions(-DPD_INTERNAL)

set(FLAGS "${PD_EXTERNAL_CFLAGS}")
if(WIN32)
    set(FLAGS "${FLAGS} -lpsapi -static-libstdc++ -static-libgcc")
endif()

if(${WITH_COVERAGE})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(FLAGS "${FLAGS} --coverage")
    endif()
endif()

# main static library
add_library(ceammc_static STATIC
    ceammc.h
    ceammc.hpp
    ceammc.c
    ceammc.cpp
    ceammc_gui.h
    ceammc_atom.cpp
    ceammc_atomlist.cpp
    ceammc_factory.h
    ceammc_faust.h
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

if(WIN32)
    target_link_libraries(ceammc_static psapi)
endif()


# timeline library
add_library(ceammc_timeline STATIC
    ceammc_timeline.cpp)

target_link_libraries(ceammc_timeline ceammc_static)

set_target_properties(ceammc_timeline PROPERTIES
    COMPILE_FLAGS "${FLAGS}" LINK_FLAGS "${FLAGS}")

