set(SVG_ICON "pd_ceammc4.svg")

find_program(INKSCAPE inkscape
    PATHS /usr/bin /usr/local/bin)

if(INKSCAPE)
    message(STATUS "Inkscape found: ${INKSCAPE}")
    find_program(ICONUTIL iconutil)
    find_program(MAGICK_CONVERT convert)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_icons.sh ${CMAKE_CURRENT_BINARY_DIR} @ONLY)
    add_custom_target(icons
        COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/make_icons.sh ${SVG_ICON}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS "*icons* target added to generate pixmap icons from SVG files")
endif()


install(FILES pd_ceammc.ico DESTINATION ${PD_TCL_INSTALL_PATH}/tcl)
