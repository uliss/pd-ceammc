include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/CicmWrapper/Sources)

set(FLAGS "${PD_EXTERNAL_CFLAGS}")
if(WIN32)
    set(FLAGS "${FLAGS} -lpsapi")
endif()

if(${WITH_COVERAGE})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(FLAGS "${FLAGS} --coverage")
    endif()
endif()

# main static library
add_library(ceammc_core SHARED
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
    ceammc_property.cpp
    ceammc_timeline.cpp)

set_target_properties(ceammc_core PROPERTIES
    COMPILE_FLAGS "${FLAGS} ${PD_EXTERNAL_CFLAGS}"
    LINK_FLAGS    "${FLAGS} ${PD_EXTERNAL_LDFLAGS}")

if(WIN32)
    target_link_libraries(ceammc_core psapi puredata-core)
endif()

#
# sound library
#
set(CEAMMC_LOAD_SRC ceammc_sound.h ceammc_sound.cpp)
set(CEAMMC_LOAD_LIBS "")
set(CEAMMC_LOAD_DEFS "")

# libsndfile backend
include(FindLibSndFile)
if(LIBSNDFILE_FOUND)
    list(APPEND CEAMMC_LOAD_DEFS "-DWITH_LIBSOUNDFILE")
    list(APPEND CEAMMC_LOAD_LIBS ${LIBSNDFILE_LIBRARIES})
    include_directories(${LIBSNDFILE_INCLUDE_DIRS})
endif()

if(LIBSNDFILE_FOUND)
    list(APPEND CEAMMC_LOAD_SRC ceammc_loader_sndfile.cpp)
endif()

add_library(ceammc_sound SHARED ${CEAMMC_LOAD_SRC})
set_target_properties(ceammc_sound PROPERTIES
    COMPILE_FLAGS "${FLAGS} ${CEAMMC_LOAD_DEFS}"
    LINK_FLAGS "${FLAGS} ${PD_EXTERNAL_LDFLAGS}")
target_link_libraries(ceammc_sound ${CEAMMC_LOAD_LIBS})

# install
if(WIN32)
    install(TARGETS ceammc_core RUNTIME DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
    install(TARGETS ceammc_sound RUNTIME DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
endif()



