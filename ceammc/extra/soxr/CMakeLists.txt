if(APPLE)
    set(WITH_OPENMP OFF CACHE BOOL "turn off OPENMP" FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/code/cmake/Modules")

add_subdirectory(code)
