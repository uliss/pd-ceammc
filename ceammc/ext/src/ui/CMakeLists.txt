if(WIN32)
    add_definitions("-D_WINDOWS=1")
endif()

macro(ceammc_ui_external name)
    list(APPEND UI_SOURCES "ui_${name}.cpp")
endmacro()

set(NUI_SOURCES nui_bang.cpp nui_toggle.cpp)

add_subdirectory(touchosc)

ceammc_ui_external(arrayview)
ceammc_ui_external(bang)
ceammc_ui_external(button)
ceammc_ui_external(canvas)
ceammc_ui_external(colorpanel)
ceammc_ui_external(display)
ceammc_ui_external(dsp)
ceammc_ui_external(env)
ceammc_ui_external(filter)
ceammc_ui_external(filter_tilde)
ceammc_ui_external(gain)
ceammc_ui_external(gain2)
ceammc_ui_external(icon)
ceammc_ui_external(incdec)
ceammc_ui_external(keyboard)
ceammc_ui_external(knob)
ceammc_ui_external(label)
ceammc_ui_external(lcd)
ceammc_ui_external(link)
ceammc_ui_external(matrix)
ceammc_ui_external(menu)
ceammc_ui_external(meter)
ceammc_ui_external(midi)
ceammc_ui_external(mouse_filter)
ceammc_ui_external(mouse_route)
ceammc_ui_external(number)
ceammc_ui_external(number_tilde)
ceammc_ui_external(plot_tilde)
ceammc_ui_external(polar)
ceammc_ui_external(preset)
ceammc_ui_external(radio)
ceammc_ui_external(rslider)
ceammc_ui_external(scope)
ceammc_ui_external(single_value)
ceammc_ui_external(slider)
ceammc_ui_external(slider2d)
ceammc_ui_external(sliders)
ceammc_ui_external(spectroscope)
ceammc_ui_external(tab)
ceammc_ui_external(toggle)
ceammc_ui_external(touchosc)

add_library(ceammc_ui STATIC mod_ui.cpp ${NUI_SOURCES} ${UI_SOURCES})

target_link_libraries(ceammc_ui PRIVATE ceammc_nui ceammc_env http_lib readerwriterqueue touchosc)

find_package(Cairo)
if(CAIRO_FOUND)
    find_package(Freetype REQUIRED)

    target_sources(ceammc_ui PRIVATE ui_canvas_cairo.cpp)
    target_include_directories(ceammc_ui
        PRIVATE
            ${CAIRO_INCLUDE_DIRS}
            ${FREETYPE_INCLUDE_DIRS}
        )
    target_link_libraries(ceammc_ui
        PUBLIC
            ${CAIRO_LIBRARIES}
            ${FREETYPE_LIBRARIES}
        PRIVATE qrcode
    )
endif()

find_package(Gperf)
if(Gperf_FOUND)
    ecm_gperf_generate(material_font.gperf gperf_material_font.h MySources
        GENERATION_FLAGS "-L C++ --class-name=MaterialFontHash -m 50 --struct-type")

    add_custom_target(gen_material_font DEPENDS gperf_material_font.h)
    add_custom_target(ui_extra SOURCES material_font.gperf)
endif()


target_include_directories(ceammc_ui
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
        ${PROJECT_BINARY_DIR} # for config.h
    )

set(TCL_GEN_SCRIPT ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh)

add_custom_target(ceammc_ui_tcl_gen
    COMMAND ${TCL_GEN_SCRIPT} ui_bang.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_button.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_canvas.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_display.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_dsp.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_filter.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_icon.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_keyboard.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_knob.tcl
    COMMAND ${TCL_GEN_SCRIPT} ui_lcd.tcl
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
        ui_canvas.tcl
        ui_display.tcl
        ui_dsp.tcl
        ui_filter.tcl
        ui_icon.tcl
        ui_keyboard.tcl
        ui_knob.tcl
        ui_lcd.tcl
        ui_link.tcl
        ui_meter.tcl
        ui_number_tilde.tcl
        ui_radio.tcl
        ui_slider2d.tcl
        ui_sliders.tcl
        ui_toggle.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
