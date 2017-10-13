set(XSAMPLE_SRC
    xsample/source/groove.cpp
    xsample/source/inter.cpp
    xsample/source/main.cpp
    xsample/source/play.cpp
    xsample/source/record.cpp
)

pd_add_extension(NAME xsample
    FILES ${XSAMPLE_SRC}
    INTERNAL True
    LIBRARY xsample
    LINK flext_static)

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
