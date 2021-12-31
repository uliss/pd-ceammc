if(WIN32)
    add_definitions("-D_WINDOWS=1")
endif()

macro(ceammc_ui_extension name)
    list(APPEND UI_SOURCES "ui_${name}.cpp")
endmacro()

ceammc_ui_extension(arrayview)
ceammc_ui_extension(bang)
ceammc_ui_extension(button)
ceammc_ui_extension(colorpanel)
ceammc_ui_extension(display)
ceammc_ui_extension(dsp)
ceammc_ui_extension(env)
ceammc_ui_extension(filter)
ceammc_ui_extension(filter_tilde)
ceammc_ui_extension(gain)
ceammc_ui_extension(gain2)
ceammc_ui_extension(icon)
ceammc_ui_extension(incdec)
ceammc_ui_extension(keyboard)
ceammc_ui_extension(knob)
ceammc_ui_extension(label)
ceammc_ui_extension(link)
ceammc_ui_extension(matrix)
ceammc_ui_extension(menu)
ceammc_ui_extension(meter)
ceammc_ui_extension(midi)
ceammc_ui_extension(mouse_filter)
ceammc_ui_extension(mouse_route)
ceammc_ui_extension(number)
ceammc_ui_extension(number_tilde)
ceammc_ui_extension(plot_tilde)
ceammc_ui_extension(polar)
ceammc_ui_extension(preset)
ceammc_ui_extension(radio)
ceammc_ui_extension(rslider)
ceammc_ui_extension(scope)
ceammc_ui_extension(single_value)
ceammc_ui_extension(slider)
ceammc_ui_extension(slider2d)
ceammc_ui_extension(sliders)
ceammc_ui_extension(spectroscope)
ceammc_ui_extension(tab)
ceammc_ui_extension(toggle)

add_library(ceammc_ui STATIC mod_ui.cpp ${UI_SOURCES})

target_link_libraries(ceammc_ui ceammc_env)

set(TCL_GEN_SCRIPT ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh)

add_custom_target(ceammc_ui_tcl_gen
    COMMAND ${TCL_GEN_SCRIPT} ui_bang.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_button.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_display.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_dsp.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_filter.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_icon.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_keyboard.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_knob.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_link.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_meter.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_number_tilde.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_radio.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_slider2d.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_sliders.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_toggle.tcl
    SOURCES
        ui_bang.tcl
        ui_button.tcl
        ui_display.tcl
        ui_dsp.tcl
        ui_filter.tcl
        ui_icon.tcl
        ui_keyboard.tcl
        ui_knob.tcl
        ui_link.tcl
        ui_meter.tcl
        ui_number_tilde.tcl
        ui_radio.tcl
        ui_slider2d.tcl
        ui_sliders.tcl
        ui_toggle.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
