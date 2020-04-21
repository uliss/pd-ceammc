cmake_policy(SET CMP0077 NEW) # to override WITH_OPENMP

set(WITH_OPENMP OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/code/cmake/Modules")

add_subdirectory(code)
