include(CMakeParseArguments)
cmake_policy(SET CMP0037 OLD)
set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

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

    message(STATUS ${CMAKE_SHARED_LINKER_FLAGS})

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

    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/gen_build_mac.sh
        COMMAND cmake ${CMAKE_COMMAND}
            -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
            -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
            -P ${PROJECT_SOURCE_DIR}/mac/cmake-build-mac.cmake)
    add_custom_target(app DEPENDS gen_build_mac.sh)
endif()
