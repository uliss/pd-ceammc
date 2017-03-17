include(CMakeParseArguments)
if(${CMAKE_VERSION} VERSION_GREATER "3.0")
    cmake_policy(SET CMP0037 OLD)
endif()
set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

if(APPLE)
    set(PD_EXTERNAL_EXTENSION ".d_fat")
    set(PD_EXTERNAL_CFLAGS "-fPIC")
    set(PD_EXTERNAL_LDFLAGS "-flat_namespace -undefined dynamic_lookup")
endif()

if(LINUX)
    set(PD_EXTERNAL_EXTENSION ".pd_linux")
    set(PD_EXTERNAL_CFLAGS "-fPIC")
    set(PD_EXTERNAL_LDFLAGS "-Wl,--export-dynamic -shared -fPIC")
endif()

if(WIN32)
    set(PD_EXTERNAL_EXTENSION ".dll")
    set(PD_EXTERNAL_CFLAGS "-mms-bitfields")
    set(PD_EXTERNAL_LDFLAGS "-shared -Wl,--enable-auto-import")
endif()

function(pd_add_extension)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME INSTALL_DIR INTERNAL LIBRARY)
    set(_MULTI_VALUE_ARGS FILES HELP_FILES EXTRA_FILES LINK)

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
        /Applications/Pd.app/Contents/Resources/include
        /Applications/Pd.app/Contents/Resources/src
        $ENV{PROGRAMFILES}/Pd/include
      PATH_SUFFIXES pd
    )

    if(_PD_EXT_NAME)
        message(STATUS "adding PureData extension: ${_PD_EXT_NAME}")
        add_library(${_PD_EXT_NAME} SHARED ${_PD_EXT_FILES})

        foreach(_src_file ${_PD_EXT_FILES})
            set_source_files_properties(${_src_file} COMPILE_FLAGS "")
        endforeach()

        include_directories(${_PD_INCLUDE_DIR})
        include_directories(${CMAKE_SOURCE_DIR}/src)
        set_target_properties(${_PD_EXT_NAME} PROPERTIES
            PREFIX ""
            SUFFIX "${PD_EXTERNAL_EXTENSION}"
            COMPILE_FLAGS "${PD_EXTERNAL_CFLAGS}"
            LINK_FLAGS "${PD_EXTERNAL_LDFLAGS}"
            )

        if(WIN32)
            list(APPEND _PD_EXT_LINK puredata-core)
        endif()

        target_link_libraries(${_PD_EXT_NAME} ${_PD_EXT_LINK})
    else()
        message(FATAL_ERROR "pd_add_extension: 'NAME' argument required.")
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
    #install(DIRECTORY "../${_PD_EXT_NAME}"
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

    # install extension binary
    if(WIN32)
        install(TARGETS ${_PD_EXT_NAME} DESTINATION "${INSTALL_DIR}")
    else()
        install(TARGETS ${_PD_EXT_NAME} LIBRARY DESTINATION "${INSTALL_DIR}")
    endif()
endfunction()

function(pd_add_simple_extension)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME INTERNAL)
    set(_MULTI_VALUE_ARGS)

    cmake_parse_arguments(_PD_EXT "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})

    pd_add_extension(NAME ${_PD_EXT_NAME}
        FILES ${_PD_EXT_NAME}.c
        HELP_FILES ${_PD_EXT_NAME}-help.pd
        INTERNAL ${_PD_EXT_INTERNAL})
endfunction()

function(pd_add_internal_extension name)
    pd_add_simple_extension(NAME ${name}
        FILES ${name}.c
        HELP_FILES ${name}-help.pd
        INTERNAL TRUE)
endfunction()
