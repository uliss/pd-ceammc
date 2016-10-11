include(CMakeParseArguments)
cmake_policy(SET CMP0037 OLD)
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

function(pd_add_extension)
    set(_OPTIONS_ARGS INTERNAL)
    set(_ONE_VALUE_ARGS NAME INSTALL_DIR)
    set(_MULTI_VALUE_ARGS FILES HELP_FILES EXTRA_FILES)

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
      PATH_SUFFIXES pd
    )

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

    #    default installation directory
    set(DEFAULT_DEST ${CMAKE_CURRENT_BINARY_DIR})
    if(UNIX AND NOT APPLE)
        set(DEFAULT_DEST "lib/pd/extra")
    elseif(APPLE)
        set(DEFAULT_DEST "$ENV{HOME}/Library/Pd")
    elseif(WIN32)

    endif()

    # explicit INSTALL_DIR used
    if(NOT _PD_EXT_INSTALL_DIR)
        if(_PD_EXT_INTERNAL)
            set(_PD_EXT_INSTALL_DIR "lib/puredata/extra")
        else()
            set(_PD_EXT_INSTALL_DIR ${DEFAULT_DEST})
        endif()
    endif()

    set(INSTALL_DIR "${_PD_EXT_INSTALL_DIR}/${_PD_EXT_NAME}")

    # install extension binary
    install(TARGETS ${_PD_EXT_NAME} LIBRARY DESTINATION ${INSTALL_DIR})

    # install extension README etc. files
    install(DIRECTORY ../${_PD_EXT_NAME}
            DESTINATION ${_PD_EXT_INSTALL_DIR}
            FILES_MATCHING REGEX "(README|LICENSE|NOTES|README.txt|LICENSE.txts|NOTES.txt)")

    # install help files
    if(NOT _PD_EXT_HELP_FILES)
        file(GLOB _PD_EXT_HELP_FILES "*.pd")
    endif()

    foreach(_loop_var ${_PD_EXT_HELP_FILES})
        get_filename_component(_fname ${_loop_var} NAME)
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${_fname}")
        configure_file(${_loop_var} ${fname})
        install(FILES ${fname} DESTINATION ${INSTALL_DIR})
    endforeach()

    foreach(_extra_file ${_PD_EXT_EXTRA_FILES})
        install(FILES ${_extra_file} DESTINATION ${INSTALL_DIR})
    endforeach()
endfunction()

function(pd_add_simple_extension name)
    pd_add_extension(NAME ${name} FILES ${name}.c HELP_FILES ${name}-help.pd)
endfunction()
