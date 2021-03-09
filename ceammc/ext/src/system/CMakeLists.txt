include_directories(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra) # for readerwriterqueue
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/subprocess/tiny-process-library)

set(SYSTEM_SOURCES
    sys_pipe.h
    sys_process.h
)

set(SYSTEM_CURSOR_TCL system_cursor.tcl)
set(SYSTEM_CURSOR_TCL_H system_cursor.tcl.h)
set(SYSTEM_COLORPANEL_TCL system_colorpanel.tcl)
set(SYSTEM_COLORPANEL_TCL_H system_colorpanel.tcl.h)

add_custom_target(system_cursor_tcl
    DEPENDS ${SYSTEM_CURSOR_TCL}
    SOURCES ${SYSTEM_CURSOR_TCL} ${SYSTEM_CURSOR_TCL_H}
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh ${SYSTEM_CURSOR_TCL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(system_colorpanel_tcl
    DEPENDS ${SYSTEM_COLORPANEL_TCL}
    SOURCES ${SYSTEM_COLORPANEL_TCL} ${SYSTEM_COLORPANEL_TCL_H}
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh ${SYSTEM_COLORPANEL_TCL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

macro(ceammc_system_extension name)
    list(APPEND SYSTEM_SOURCES "system_${name}.cpp")
endmacro()

ceammc_system_extension(colorpanel)
ceammc_system_extension(cursor)
ceammc_system_extension(exec)
ceammc_system_extension(exit)
ceammc_system_extension(getenv)
ceammc_system_extension(hostname)
ceammc_system_extension(memsize)
ceammc_system_extension(memused)
ceammc_system_extension(screen_size)

add_library(ceammc_system STATIC mod_system.cpp ${SYSTEM_SOURCES})
target_include_directories(ceammc_system PRIVATE $<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>)

if(UNIX)
    target_sources(ceammc_system PRIVATE
                    sys_pipe_unix.cpp
                    sys_process_unix.cpp
                    sys_fileactions.cpp
                    sys_fileactions.h)
elseif(WIN32)
    target_sources(ceammc_system PRIVATE sys_process_win.cpp)
endif()

target_link_libraries(ceammc_system tiny-process-library)
