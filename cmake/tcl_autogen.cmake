function(tcl_autogen_header)
    set(_OPTS VERBOSE)
    set(_ONE_VALUE_ARGS TARGET SOURCES)
    list(POP_FRONT ARGN tcl_file)
    cmake_parse_arguments(_TCL_AUTOGEN "${_OPTS}" "${_ONE_VALUE_ARGS}" "" ${ARGN})

    if (${_TCL_AUTOGEN_VERBOSE})
        message(STATUS "tcl source added: '${tcl_file}'")
    endif()

    set(TCL_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${tcl_file}.h)
    add_custom_command(
        OUTPUT ${TCL_HEADER}
        COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/${tcl_file} > ${TCL_HEADER}
        DEPENDS ${tcl_file}
        VERBATIM)

    if (DEFINED _TCL_AUTOGEN_TARGET)
        target_sources(${_TCL_AUTOGEN_TARGET} PRIVATE ${TCL_HEADER})

        if (${_TCL_AUTOGEN_VERBOSE})
            message(STATUS "tcl header '${TCL_HEADER}' added to target: '${_TCL_AUTOGEN_TARGET}'")
        endif()
    endif()
    if (DEFINED _TCL_AUTOGEN_SOURCES)
        target_sources(${_TCL_AUTOGEN_SOURCES} PRIVATE ${tcl_file})

        if (${_TCL_AUTOGEN_VERBOSE})
            message(STATUS "tcl file '${tcl_file}' added to source target: '${_TCL_AUTOGEN_SOURCES}'")
        endif()
    endif()
endfunction()
