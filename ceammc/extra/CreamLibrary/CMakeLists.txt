include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)
# for CicmWrapper
include_directories(../CicmWrapper/Sources)

set(CREAM_SRC
    Deprecated/c.patcherargs.cpp
    Deprecated/c.patcherinfos.cpp
    Deprecated/c.patchermess.cpp
#    Sources/c.camomile.cpp
    Sources/c.colorpanel.cpp
    Sources/c.dsp_tilde.cpp
#    Sources/c.gain_tilde.cpp
    Sources/c.incdec.cpp
    Sources/c.matrix.cpp
    Sources/c.menu.cpp
    Sources/c.meter_tilde.cpp


    Sources/c.bang.cpp
    Sources/c.number_tilde.cpp
    Sources/c.number.cpp
    Sources/c.radio.cpp
    Sources/c.rslider.cpp
    Sources/c.slider.cpp
    Sources/c.tab.cpp
    Sources/c.toggle.cpp
    Sources/c.preset.cpp
)

set(CREAM_COMPLETION "cream_objects.txt")
add_custom_target(cream_completion SOURCES ${CREAM_COMPLETION})
install(FILES ${CREAM_COMPLETION} DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc)

add_library(cream_core STATIC ${CREAM_SRC})
set(CREAM_COMPILE_FLAGS "")
if(UNIX)
    list(APPEND CREAM_COMPILE_FLAGS "-fPIC")
else()
endif()
set_target_properties(cream_core PROPERTIES COMPILE_FLAGS "${CREAM_COMPILE_FLAGS}")

target_link_libraries(cream_core cicm_static)

pd_add_extension(NAME creammc
    FILES c.library.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK cicm_static cream_core)


