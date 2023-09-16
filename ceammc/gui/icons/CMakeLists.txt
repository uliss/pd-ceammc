set(SVG_ICON "pd_ceammc5.svg")
set(SVG_ICON_WIN "pd_ceammc_win.svg")

set(CMAKE_FIND_FRAMEWORK "NEVER")
set(CMAKE_FIND_APPBUNDLE "NEVER")

find_program(INKSCAPE inkscape
    PATHS
        /usr/bin
        /usr/local/bin
        /Applications/Inkscape.app/Contents/MacOS
)

if(INKSCAPE)
    message(STATUS "Inkscape found: ${INKSCAPE}")
    find_program(ICONUTIL iconutil)
    find_program(MAGICK_CONVERT convert)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_icons.sh ${CMAKE_CURRENT_BINARY_DIR} @ONLY)
    add_custom_target(icons
        COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/make_icons.sh ${SVG_ICON} ${SVG_ICON_WIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        USES_TERMINAL)
    message(STATUS "*icons* target added to generate pixmap icons from SVG files")
endif()

add_custom_target(scripts SOURCES make_icons.sh)
add_custom_target(win_app_icon SOURCES pd_ceammc.ico)
install(FILES pd_ceammc.ico DESTINATION ${PD_TCL_INSTALL_PATH}/tcl)
