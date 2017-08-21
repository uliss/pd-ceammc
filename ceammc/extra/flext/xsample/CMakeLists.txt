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
    LIBRARY xample
    LINK flext_static)
