set(UI_SOURCES "")

set(UILINK_TCL ui_link.tcl)

add_custom_target(uilink_tcl
    DEPENDS ${UILINK_TCL}
    SOURCES ${UILINK_TCL} ui_link_tcl.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tcl_to_c.sh ${UILINK_TCL} uilink_tcl_init > ui_link_tcl.h
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

macro(ceammc_ui_extension name)
    list(APPEND UI_SOURCES "ui_${name}.cpp")
endmacro()

ceammc_ui_extension(keyboard)
ceammc_ui_extension(slider2d)
ceammc_ui_extension(sliders)
ceammc_ui_extension(knob)
ceammc_ui_extension(scope~)
ceammc_ui_extension(spectroscope~)
ceammc_ui_extension(bpfunc)
ceammc_ui_extension(display)
ceammc_ui_extension(link)

add_library(ceammc_ui STATIC mod_ui.cpp ${UI_SOURCES})
if(UNIX)
    set_target_properties(ceammc_ui PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
