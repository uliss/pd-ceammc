# pm_common

# set the build directory for libportmidi.a to be in portmidi, not in 
#    portmidi/pm_common
if(APPLE OR WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
    # set the build directory for .dylib libraries
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

    # the first time CMake configures, save off CMake's built-in flags
    if(NOT DEFAULT_DEBUG_FLAGS)
        set(DEFAULT_DEBUG_FLAGS ${CMAKE_C_FLAGS_DEBUG} CACHE
            STRING "CMake's default debug flags" FORCE)
        set(DEFAULT_RELEASE_FLAGS ${CMAKE_C_FLAGS_RELEASE} CACHE
            STRING "CMake's default release flags" FORCE)
    else(NOT DEFAULT_DEBUG_FLAGS)
        message(STATUS "DEFAULT_DEBUG_FLAGS not nil: " ${DEFAULT_DEBUG_FLAGS})
    endif(NOT DEFAULT_DEBUG_FLAGS)
else()
    if(WITH_ALSA)
        set(LINUX_FLAGS "-DPMALSA")
    else()
        set(LINUX_FLAGS "-DPMNULL")
    endif()
endif()

macro(prepend_path RESULT PATH)
    set(${RESULT})
    foreach(FILE ${ARGN})
        list(APPEND ${RESULT} "${PATH}${FILE}")
    endforeach()
endmacro()

set(CMAKE_C_FLAGS_DEBUG 
    "${DEFAULT_DEBUG_FLAGS} -DPM_CHECK_ERRORS=1 -DDEBUG ${LINUX_FLAGS}"
    CACHE STRING "enable extra checks for debugging" FORCE)

set(CMAKE_C_FLAGS_RELEASE "${DEFAULT_RELEASE_FLAGS} ${LINUX_FLAGS}"
    CACHE STRING "flags for release version" FORCE)

# first include the appropriate system-dependent file:
if(UNIX)
    # add the -g switch for Linux and Mac OS X (not used in Win32)
    set (CMAKE_C_FLAGS_DEBUG "-g ${CMAKE_C_FLAGS_DEBUG}"
        CACHE STRING "enable extra checks for debugging" FORCE)
    if(APPLE)
        set (CMAKE_C_FLAGS_DEBUG "-msse -msse2 ${CMAKE_C_FLAGS_DEBUG}")
        set (CMAKE_C_FLAGS_RELEASE "-msse -msse2 ${CMAKE_C_FLAGS_RELEASE}")

        set(MACSRC pmmacosxcm pmmac readbinaryplist finddefault)
        prepend_path(LIBSRC ../pm_mac/ ${MACSRC})
        list(APPEND LIBSRC ../porttime/ptmacosx_mach)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../porttime)

        include_directories(${CMAKE_OSX_SYSROOT}/Developer/Headers/FlatCarbon)
        set(FRAMEWORK_PATH ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks)
        set(COREAUDIO_LIB "${FRAMEWORK_PATH}/CoreAudio.framework")
        set(COREFOUNDATION_LIB "${FRAMEWORK_PATH}/CoreFoundation.framework")
        set(COREMIDI_LIB "${FRAMEWORK_PATH}/CoreMIDI.framework")
        set(CORESERVICES_LIB "${FRAMEWORK_PATH}/CoreServices.framework")
        set(PM_NEEDED_LIBS ${COREAUDIO_LIB} ${COREFOUNDATION_LIB}
            ${COREMIDI_LIB} ${CORESERVICES_LIB}
            CACHE INTERNAL "")

        message(STATUS "SYSROOT: " ${CMAKE_OSX_SYSROOT})
    else()
        # LINUX settings...
        set(LINUXSRC pmlinux finddefault)
        if(WITH_ALSA)
            list(APPEND LINUXSRC pmlinuxalsa)
        endif()
        prepend_path(LIBSRC ../pm_linux/ ${LINUXSRC})
        list(APPEND LIBSRC ../porttime/ptlinux)

        set(PM_NEEDED_LIBS pthread asound)
        include_directories(../porttime)
    endif()
else(UNIX)
    if(WIN32)
        # /MDd is multithread debug DLL, /MTd is multithread debug
        # /MD is multithread DLL, /MT is multithread. Change to static:
        include(../pm_win/static.cmake)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../porttime)

        set(WINSRC pmwin pmwinmm)
        prepend_path(LIBSRC ../pm_win/ ${WINSRC})
        list(APPEND LIBSRC ../porttime/ptwinmm)
    endif()
endif()

# this completes the list of library sources by adding shared code
list(APPEND LIBSRC pmutil portmidi)

# now add the shared files to make the complete list of library sources
add_library(portmidi-static ${LIBSRC})
set_target_properties(portmidi-static PROPERTIES OUTPUT_NAME "portmidi_s")
target_link_libraries(portmidi-static ${PM_NEEDED_LIBS})
