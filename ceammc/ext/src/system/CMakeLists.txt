include_directories(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/subprocess/tiny-process-library)

set(SYSTEM_SOURCES "")
set(SYSTEM_CURSOR_TCL system_cursor.tcl)
set(SYSTEM_CURSOR_TCL_H system_cursor_tcl.h)
set(SYSTEM_CURSOR_TCL_INIT system_cursor_tcl_init)

add_compile_definitions(SYSTEM_CURSOR_TCL_INIT=${SYSTEM_CURSOR_TCL_INIT})

add_custom_target(system_cursor_tcl
    DEPENDS ${SYSTEM_CURSOR_TCL}
    SOURCES ${SYSTEM_CURSOR_TCL} ${SYSTEM_CURSOR_TCL_H}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../ui/tcl_to_c.sh ${SYSTEM_CURSOR_TCL} ${SYSTEM_CURSOR_TCL_INIT} > ${SYSTEM_CURSOR_TCL_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

macro(ceammc_system_extension name)
    list(APPEND SYSTEM_SOURCES "system_${name}.cpp")
endmacro()

ceammc_system_extension(cursor)
ceammc_system_extension(getenv)
ceammc_system_extension(hostname)
ceammc_system_extension(memsize)
ceammc_system_extension(memused)
ceammc_system_extension(screen_size)
ceammc_system_extension(shell)

add_library(ceammc_system STATIC mod_system.cpp ${SYSTEM_SOURCES})
if(UNIX)
    set_target_properties(ceammc_system PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_link_libraries(ceammc_system tiny-process-library)

