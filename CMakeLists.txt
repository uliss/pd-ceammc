cmake_minimum_required(VERSION 2.8)
project(puredata)

add_definitions(-std=c99)
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CheckIncludeFiles)
include(FindALSA)
include(FindJack)
include(FindPortAudio)

option(SOUND_DUMMY "Build dummy sound module" OFF)

check_include_files(unistd.h HAVE_UNISTD_H)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

add_subdirectory(src)
