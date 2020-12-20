set(XSAMPLE_SRC
    xsample/source/groove.cpp
    xsample/source/inter.cpp
    xsample/source/main.cpp
    xsample/source/play.cpp
    xsample/source/record.cpp
)

pd_add_external(NAME xsample
    FILES ${XSAMPLE_SRC}
    INTERNAL True
    LIBRARY xsample
    LINK flext_static)

if(WIN32)
    # C++11 doesn't defines M_PI etc
    target_compile_options(xsample PUBLIC -U__STRICT_ANSI__ -Wno-deprecated-register)
endif()

target_compile_options(xsample PRIVATE -Wno-deprecated-register)

# help files
file(GLOB _xsample_help_files "xsample/pd/*.pd")
add_custom_target(xsample_help_files SOURCES ${_xsample_help_files})
# pd examples/demo
file(GLOB _xsample_examples "xsample/pd-ex/*.pd")
add_custom_target(xsample_examples SOURCES ${_xsample_examples})
# lib license
file(GLOB _xsample_license "xsample/*.txt")
add_custom_target(xsample_license SOURCES ${_xsample_license})

install(FILES ${_xsample_help_files} ${_xsample_examples} ${_xsample_license}
    DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}/xsample)
