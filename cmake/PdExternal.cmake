include(CMakeParseArguments)
include(TargetArch)

set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

if(APPLE)
    if("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()

    list(LENGTH CMAKE_OSX_ARCHITECTURES _NARCH)
    # serveral arch's
    if(${_NARCH} GREATER "1")
        set(PD_EXTERNAL_EXTENSION ".d_fat")
    else()
        if("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "x86_64")
            set(PD_EXTERNAL_EXTENSION ".d_amd64")
        elseif("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "i386")
            set(PD_EXTERNAL_EXTENSION ".d_i386")
        else()
            set(PD_EXTERNAL_EXTENSION ".pd_darwin")
        endif()
    endif()
    set(PD_EXTERNAL_CFLAGS "-fPIC")
    set(PD_EXTERNAL_LDFLAGS "-flat_namespace -undefined dynamic_lookup")
endif()

if(LINUX)
    set(PD_EXTERNAL_EXTENSION ".pd_linux")
    set(PD_EXTERNAL_CFLAGS "-fPIC")
    set(PD_EXTERNAL_LDFLAGS "-Wl,--export-dynamic -shared -fPIC")
endif()

if(WIN32)
    target_architecture(_TARGET_ARCH)
    if("${_TARGET_ARCH}" STREQUAL "x86_64")
        set(PD_EXTERNAL_EXTENSION ".m_amd64")
    elseif("${_TARGET_ARCH}" STREQUAL "i386")
        set(PD_EXTERNAL_EXTENSION ".m_i386")
    else()
        set(PD_EXTERNAL_EXTENSION ".dll")
    endif()

    set(PD_EXTERNAL_CFLAGS "-mms-bitfields")
    set(PD_EXTERNAL_LDFLAGS "-shared -Wl,--enable-auto-import")
endif()

# adds puredata external
#
# arguments:
#   NAME        - external name
#   LIBRARY     - external library name. Install to this subdirectory if specified.
#   INSTALL_DIR - explicit directory for installation
#   INTERNAL    - flag for internal install, if INSTALL_DIR not specified and this flag is true,
#                 install to ${PD_INTERNAL_EXT_INSTALL_PATH},
#                 otherwise install system Pd directory
#   FILES       - list of external source files (*.c, *.cpp, *.h etc)
#   HELP_FILES  - list of external help files (*-help.pd)
#   EXTRA_FILES - list of extra files to install
#   LINK        - list of libraries to link with
#
function(pd_add_external)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME INSTALL_DIR INTERNAL LIBRARY)
    set(_MULTI_VALUE_ARGS FILES HELP_FILES EXTRA_FILES LINK)

    cmake_parse_arguments(_PD_EXT "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    if(_PD_EXT_NAME)
        set(TARGET_NAME ${_PD_EXT_NAME})

        # check for ending "~". replace it to _tilde
        # because targets with ~ in names are not allowed, see CMP0037
        if("${_PD_EXT_NAME}" MATCHES ".+~$")
            string(LENGTH ${_PD_EXT_NAME} _len)
            math(EXPR _len "${_len} - 1")
            string(SUBSTRING ${_PD_EXT_NAME} 0 ${_len} _name)
            set(TARGET_NAME "${_name}_tilde")
        endif()

        message(STATUS "adding Pd external: [${_PD_EXT_NAME}]")
        add_library(${TARGET_NAME} SHARED ${_PD_EXT_FILES})
        set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${_PD_EXT_NAME})

        foreach(_src_file ${_PD_EXT_FILES})
            set_source_files_properties(${_src_file} COMPILE_FLAGS "")
        endforeach()

        target_include_directories(${TARGET_NAME}
            PRIVATE ${PROJECT_SOURCE_DIR}/src)

        set_target_properties(${TARGET_NAME} PROPERTIES
            PREFIX        ""
            SUFFIX        "${PD_EXTERNAL_EXTENSION}"
            COMPILE_FLAGS "${PD_EXTERNAL_CFLAGS}"
            OUTPUT        "${_PD_EXT_NAME}"
            )

        if(WIN32)
            list(APPEND _PD_EXT_LINK puredata-core)
        endif()

        target_link_libraries(${TARGET_NAME} PRIVATE ${_PD_EXT_LINK})
        # we don't have target_link_options on travis now
        set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS "${PD_EXTERNAL_LDFLAGS}")
    else()
        message(FATAL_ERROR "pd_add_external: 'NAME' argument required.")
    endif()

    #    default installation directory
    set(DEFAULT_DEST ${CMAKE_CURRENT_BINARY_DIR})
    if(UNIX AND NOT APPLE)
        set(DEFAULT_DEST "lib/pd/extra")
    elseif(APPLE)
        set(DEFAULT_DEST "$ENV{HOME}/Library/Pd")
    elseif(WIN32)
        set(DEFAULT_DEST "extra")
    endif()

    # explicit INSTALL_DIR used
    if(NOT _PD_EXT_INSTALL_DIR)
        # internal extensions use this
        if(_PD_EXT_INTERNAL)
            set(_PD_EXT_INSTALL_DIR ${PD_INTERNAL_EXT_INSTALL_PATH})
        else()
            set(_PD_EXT_INSTALL_DIR ${DEFAULT_DEST})
        endif()
    endif()

    # if extension part of library
    # install it to library directory
    if(_PD_EXT_LIBRARY)
        set(INSTALL_DIR "${_PD_EXT_INSTALL_DIR}/${_PD_EXT_LIBRARY}")
    else()
        set(INSTALL_DIR "${_PD_EXT_INSTALL_DIR}/${_PD_EXT_NAME}")
    endif()

    #message(STATUS ${INSTALL_DIR})

    # install extension README etc. files
    #install(DIRECTORY "../${TARGET_NAME}"
    #        DESTINATION "${_PD_EXT_INSTALL_DIR}"
    #        FILES_MATCHING REGEX "(README|LICENSE|NOTES|README.txt|LICENSE.txts|NOTES.txt)")

    # install help files
    if(NOT _PD_EXT_HELP_FILES)
        file(GLOB _PD_EXT_HELP_FILES "*.pd")
    endif()

    foreach(_loop_var ${_PD_EXT_HELP_FILES})
        get_filename_component(_fname ${_loop_var} NAME)
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${_fname}")
        configure_file(${_loop_var} ${fname})
        install(FILES ${fname} DESTINATION "${INSTALL_DIR}")
    endforeach()

    # installing extra files
    foreach(_extra_file ${_PD_EXT_EXTRA_FILES})
        install(FILES ${_extra_file} DESTINATION "${INSTALL_DIR}")
    endforeach()

    # install external binary
    if(WIN32)
        install(TARGETS ${TARGET_NAME} DESTINATION "${INSTALL_DIR}")
    else()
        install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION "${INSTALL_DIR}")
    endif()
endfunction()

# adds simple pd external (C-language)
# these files are added: ${name}.c and ${name}-help.pd
#
# arguments:
#   NAME        - external name
#
function(pd_add_simple_c_external)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME INTERNAL)
    set(_MULTI_VALUE_ARGS)

    cmake_parse_arguments(_PD_EXT "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    pd_add_external(NAME ${_PD_EXT_NAME}
        FILES ${_PD_EXT_NAME}.c
        HELP_FILES ${_PD_EXT_NAME}-help.pd
        INTERNAL ${_PD_EXT_INTERNAL})
endfunction()

function(pd_add_internal_external name)
    pd_add_simple_c_external(NAME ${name}
        FILES ${name}.c
        HELP_FILES ${name}-help.pd
        INTERNAL TRUE)
endfunction()
