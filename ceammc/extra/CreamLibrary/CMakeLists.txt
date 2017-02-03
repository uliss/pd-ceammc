include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)
# for CicmWrapper
include_directories(../CicmWrapper/Sources)

set(CREAM_SRC

    Deprecated/c.loadmess.cpp
    Deprecated/c.prepend.cpp
    Deprecated/c.pak.cpp
    Deprecated/c.patcherargs.cpp
    Deprecated/c.patcherinfos.cpp
    Deprecated/c.patchermess.cpp
#    Deprecated/c.prepend.cpp
#    Sources/c.bang.cpp
    Sources/c.blackboard.cpp
    Sources/c.breakpoints.cpp
#    Sources/c.camomile.cpp
    Sources/c.colorpanel.cpp
    Sources/c.dsp_tilde.cpp
#    Sources/c.gain_tilde.cpp
    Sources/c.incdec.cpp
#    Sources/c.keyboard.cpp
    Sources/c.knob.cpp
    Sources/c.matrix.cpp
    Sources/c.menu.cpp
    Sources/c.meter_tilde.cpp
#    Sources/c.number.cpp
#    Sources/c.number_tilde.cpp
#    Sources/c.plane.cpp
#    Sources/c.preset.cpp
    Sources/c.radio.cpp
    Sources/c.rslider.cpp
    Sources/c.scope_tilde.cpp
    Sources/c.slider.cpp
    Sources/c.tab.cpp
    Sources/c.toggle.cpp
#    Sources/c.wavesel.cpp

    Sources/с.bang.cpp
    Sources/с.number_tilde.cpp
    Sources/с.number.cpp
    Sources/с.preset.cpp
    Sources/с.radio.cpp
    Sources/с.rslider.cpp
    Sources/с.slider.cpp
    Sources/с.tab.cpp
    Sources/с.toggle.cpp
)

add_library(cream_core STATIC ${CREAM_SRC})
set(CREAM_COMPILE_FLAGS "")
if(UNIX)
    list(APPEND CREAM_COMPILE_FLAGS "-fPIC")
else()
endif()
set_target_properties(cream_core PROPERTIES COMPILE_FLAGS "${CREAM_COMPILE_FLAGS}")

#target_link_libraries(cream_core cicm_static)

pd_add_extension(NAME creammc
    FILES c.library.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK cicm_static cream_core)
