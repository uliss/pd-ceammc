cmake_minimum_required(VERSION 3.13)
project(puredata)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation for pure-data is not allowed within the source directory!"
        "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
        "\n "
        "\n rm CMakeCache.txt"
        "\n cd .."
        "\n mkdir bin"
        "\n cd bin"
        "\n cmake .."
        "\n")
endif()

if(APPLE)
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    else()
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()

    if(MACOSX_SDK)
        set(CMAKE_OSX_SYSROOT ${MACOSX_SDK})
    endif()
elseif(WIN32)
    if(DEFINED ENV{MINGW_PREFIX})
        if(DEFINED ENV{PKG_CONFIG_PATH})
            set(pkg_path)
            foreach(_path $ENV{PKG_CONFIG_PATH})
                string(REGEX REPLACE "^[A-Z]:/" "\\\\" _apath ${_path})
                set(pkg_path "${pkg_path}:${_apath}")
            endforeach()
            string(REGEX REPLACE "^:" "" _xpath ${pkg_path})
            set(ENV{PKG_CONFIG_PATH} ${_xpath})
        else()
            set(_xpath "$ENV{MINGW_PREFIX}/lib/pkgconfig")
            string(REGEX REPLACE "^[A-Z]:/" "\\\\" _ypath ${_xpath})
            set(ENV{PKG_CONFIG_PATH} ${_ypath})
        endif()
        message(STATUS "pkg-config path: $ENV{PKG_CONFIG_PATH}")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(POLICY CMP0074)
    #policy for <PackageName>_ROOT variables
    cmake_policy(SET CMP0074 NEW)
endif()

if(UNIX)
    if(APPLE)
        set(CMAKE_MACOSX_RPATH TRUE)
        # @rpath on macosx
        # set(CMAKE_SKIP_RPATH True)
        if(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
            set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")
        endif()
        # set(CMAKE_SKIP_BUILD_RPATH FALSE)
        # set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        # set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR TRUE)
        set(CMAKE_INSTALL_RPATH "@loader_path")
        # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
     else()
        set(CMAKE_SKIP_BUILD_RPATH FALSE)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
        set(CMAKE_INSTALL_RPATH "\$ORIGIN")
     endif()

     set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
enable_language(CXX C)
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(CheckLSBTypes)
include(build_options)
include(ceammc/external_version.cmake)
include(version)
include(install_paths)
find_package(LeapMotion)
include(PdPlatform)
include(PdExternal)
find_package(Gettext)
#find_package(LibUSB)
include(config)
include(sound)
include(cmake_uninstall)

if(WITH_PD_INSTANCE)
    add_definitions(-DPD_INSTANCE)
endif()

if(WITH_CLANG_TIDY)
    find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
        if(NOT CLANG_TIDY_COMMAND)
            message(WARNING "WITH_CLANG_TIDY is ON but clang-tidy is not found!")
            set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
        else()
            set(CLANG_TIDY_CHECKS "-*,modernize-*")
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
      endif()
endif()

# libsndfile backend
if(WITH_LIBSNDFILE)
    find_package(LibSndFile REQUIRED)
    set(CEAMMC_HAVE_LIBSNDFILE TRUE)
    if(PKGCONFIG_LIBSNDFILE_VERSION)
        set(LIBSNDFILE_VERSION ${PKGCONFIG_LIBSNDFILE_VERSION})
    endif()
endif()

# FFTW3
if(WITH_FFTW)
    find_package(FFTW REQUIRED)
    if(NOT FFTW_FOUND)
        set(WITH_FFTW OFF CACHE BOOL "" FORCE)
    endif()
endif()

add_subdirectory(portmidi)
add_subdirectory(src)
add_subdirectory(tcl)
add_subdirectory(extra)
add_subdirectory(doc)
add_subdirectory(mac)


if(GETTEXT_FOUND)
    add_subdirectory(po)
endif()

if(ENABLE_TESTS)
    enable_testing(true)
endif()

add_subdirectory(ceammc)

include(package)
include(summary)

add_custom_target(puredata_changelog SOURCES CHANGELOG.md)

file(GLOB _DEB_FILES "debian/*")
add_custom_target(deb_files SOURCES ${_DEB_FILES})

add_custom_target(ci_files SOURCES appveyor.yml)
