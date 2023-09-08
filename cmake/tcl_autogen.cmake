function(tcl_autogen_header tcl_file)
    set(_ONE_VALUE_ARGS TARGET SOURCES)

    cmake_parse_arguments(_TCL_AUTOGEN "" "${_ONE_VALUE_ARGS}" "" ${ARGN})

    set(TCL_HEADER ${tcl_file}.h)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TCL_HEADER}
        COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/${tcl_file} > ${TCL_HEADER}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${tcl_file}
        VERBATIM)

    if (${_TCL_AUTOGEN_TARGET})
        target_sources(${_TCL_AUTOGEN_TARGET} PRIVATE ${TCL_HEADER})
    endif()
    if (${_TCL_AUTOGEN_SOURCES})
        target_sources(${_TCL_AUTOGEN_SOURCES} PRIVATE ${tcl_file}})
    endif()
endfunction()
