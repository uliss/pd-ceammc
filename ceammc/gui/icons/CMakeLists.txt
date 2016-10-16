set(SVG_ICON "pd_ceammc4.svg")
add_custom_target(icons
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/make_icons.sh ${SVG_ICON}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
