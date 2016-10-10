include(CMakeParseArguments)
cmake_policy(SET CMP0037 OLD)
set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

set(PLATFORM_LINK_LIBRARIES)

macro(pd_add_extension)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME)
    set(_MULTI_VALUE_ARGS FILES HELP_FILES)

    cmake_parse_arguments(_PD_EXT "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    find_path(_PD_INCLUDE_DIR
      NAMES
        m_pd.h
      PATHS
        ${CMAKE_SOURCE_DIR}/src
        ${PD_INCLUDE_DIR}
        /usr/include
        /usr/local/include
        /opt/local/include
        /Applications
    )

    # Mandatory
    if(_PD_EXT_NAME)
        message(STATUS "adding PureData extension: ${_PD_EXT_NAME}")
        add_library(${_PD_EXT_NAME} SHARED ${_PD_EXT_FILES})

        foreach(_src_file ${_PD_EXT_FILES})
            set_source_files_properties(${_src_file} COMPILE_FLAGS "")
        endforeach()

        include_directories(${_PD_INCLUDE_DIR})
        set_target_properties(${_PD_EXT_NAME} PROPERTIES
            PREFIX ""
            SUFFIX ${PD_EXTERNAL_EXTENSION}
            COMPILE_FLAGS ${PD_EXTERNAL_CFLAGS}
            LINK_FLAGS ${PD_EXTERNAL_LDFLAGS}
            )
    else()
        message(FATAL_ERROR "pd_add_extension: 'NAME' argument required.")
    endif()

    foreach(_loop_var ${_PD_EXT_HELP_FILES})
        configure_file(${_loop_var} ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()
endmacro()

macro(pd_add_simple_extension name)
    pd_add_extension(NAME ${name} FILES ${name}.c HELP_FILES ${name}-help.pd)
endmacro()

if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -funroll-loops -fomit-frame-pointer")
    set(PD_EXTERNAL_EXTENSION ".d_fat")
    set(PD_EXTERNAL_CFLAGS "-fPIC") 
    set(PD_EXTERNAL_LDFLAGS "-flat_namespace -undefined dynamic_lookup")

    if(BUNDLE_VERSION)
        set(BUNDLE "Pd-${BUNDLE_VERSION}.app")
    else()
        set(BUNDLE "Pd.app")
    endif()

    set(MAKE_BUNDLE_SCRIPT ${PROJECT_BINARY_DIR}/dist/build_mac.sh)
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")

    add_custom_command(
        OUTPUT ${MAKE_BUNDLE_SCRIPT}
        COMMAND ${CMAKE_COMMAND}
            -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
            -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -P ${PROJECT_SOURCE_DIR}/cmake/cmake-build-mac.cmake)

    add_custom_command(
        OUTPUT ${BUNDLE_FULL_PATH}
        COMMAND sh ${PROJECT_BINARY_DIR}/dist/build_mac.sh
        COMMAND ${CMAKE_COMMAND}
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -P ${PROJECT_SOURCE_DIR}/cmake/bundle.cmake
        DEPENDS pd)

    add_custom_target(app DEPENDS ${MAKE_BUNDLE_SCRIPT} ${BUNDLE_FULL_PATH})

    add_custom_target(codesign
        COMMAND sh ${PROJECT_SOURCE_DIR}/mac/codesign.sh ${BUNDLE_FULL_PATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mac)

    find_library(CoreServices_LIBRARY CoreServices)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CoreServices_LIBRARY})
    find_library(AudioUnit_LIBRARY AudioUnit)
    list(APPEND PLATFORM_LINK_LIBRARIES ${AudioUnit_LIBRARY})
    find_library(AudioToolbox_LIBRARY AudioToolbox)
    list(APPEND PLATFORM_LINK_LIBRARIES ${AudioToolbox_LIBRARY})
    find_library(CoreMidi_LIBRARY CoreMidi)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CoreMidi_LIBRARY})
    find_library(CoreAudio_LIBRARY CoreAudio)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CoreAudio_LIBRARY})
    find_library(CoreFoundation_LIBRARY CoreFoundation)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CoreFoundation_LIBRARY})
endif()
