macro(ceammc_ui_external name)
    list(APPEND UI_SOURCES "ui_${name}.cpp")
endmacro()

add_custom_target(ceammc_ui_tcl SOURCES)
include(tcl_autogen)
function(tcl_ui_deps tcl_file)
    tcl_autogen_header(${tcl_file} TARGET ceammc_ui SOURCES ceammc_ui_tcl)
endfunction()

set(NUI_SOURCES nui_bang.cpp nui_toggle.cpp)

add_subdirectory(touchosc)

ceammc_ui_external(arrayview)
ceammc_ui_external(bang)
ceammc_ui_external(button)
ceammc_ui_external(canvas)
ceammc_ui_external(circular_meter)
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
target_include_directories(ceammc_ui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

find_package(Rust)
if(Rust_FOUND)
    target_include_directories(ceammc_ui
        PRIVATE
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/resvg/crates
            ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/rimg
        )
    target_link_libraries(ceammc_ui PRIVATE resvg rimg)
    target_compile_definitions(ceammc_ui PRIVATE WITH_RIMG WITH_RESVG)
    if(WITH_VEROVIO)
        if(WIN32)
            # verovio expected to be installed with ./script/build_verovio.sh
            # into this directory
            set(VEROVIO_ROOT $ENV{USERPROFILE}/.local/verovio)
        else()
            # faust expected to be installed with ./script/build_verovio.sh
            # into this directory
            set(VEROVIO_ROOT $ENV{HOME}/.local/verovio)
        endif()
        message(STATUS "Verovio root is: ${VEROVIO_ROOT}")
        find_package(Verovio REQUIRED)
        target_compile_definitions(ceammc_ui PRIVATE WITH_VEROVIO)
        target_link_libraries(ceammc_ui PRIVATE ${VEROVIO_LIBRARIES})
        target_include_directories(ceammc_ui PRIVATE ${VEROVIO_INCLUDE_DIRS})
        if (UNIX AND NOT APPLE)
            set(VEROVIO_INSTALL_DIR ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
            install(FILES "${VEROVIO_LIBRARY}" DESTINATION ${VEROVIO_INSTALL_DIR})
        elseif(WIN32) # install verovio dll
            set(VEROVIO_INSTALL_DIR ${PDCORE_LIB_INSTALL_PATH})
            install(FILES "${VEROVIO_ROOT}/bin/libverovio.dll" DESTINATION ${VEROVIO_INSTALL_DIR})
        endif()

        install(DIRECTORY ${VEROVIO_DATA}
            DESTINATION ${PD_DOC_INSTALL_PATH}/extra/ceammc/music
            FILES_MATCHING PATTERN "*.xml" PATTERN "*.svg" PATTERN "*.css")
    endif()
else()
    target_compile_definitions(ceammc_ui PRIVATE WITH_STB)
endif()

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
        GENERATION_FLAGS "-L C++ --class-name=MaterialFontHash -m 25 --struct-type --size-multiple=2")

    add_custom_target(gen_material_font DEPENDS gperf_material_font.h)
    add_custom_target(ui_extra SOURCES material_font.gperf)
endif()

target_include_directories(ceammc_ui
    PRIVATE
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
        ${PROJECT_BINARY_DIR} # for config.h
    )

tcl_ui_deps(ui_circular_meter.tcl)
tcl_ui_deps(ui_bang.tcl)
tcl_ui_deps(ui_button.tcl)
tcl_ui_deps(ui_canvas.tcl)
tcl_ui_deps(ui_display.tcl)
tcl_ui_deps(ui_dsp.tcl)
tcl_ui_deps(ui_filter.tcl)
tcl_ui_deps(ui_icon.tcl)
tcl_ui_deps(ui_keyboard.tcl)
tcl_ui_deps(ui_knob.tcl)
tcl_ui_deps(ui_lcd.tcl)
tcl_ui_deps(ui_link.tcl)
tcl_ui_deps(ui_meter.tcl)
tcl_ui_deps(ui_number_tilde.tcl)
tcl_ui_deps(ui_radio.tcl)
tcl_ui_deps(ui_slider2d.tcl)
tcl_ui_deps(ui_sliders.tcl)
tcl_ui_deps(ui_toggle.tcl)
