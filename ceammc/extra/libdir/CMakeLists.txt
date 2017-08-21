include(PdExtension)
# for m_pd.h and others
add_definitions(-DVERSION="1.10")
include_directories(${PROJECT_SOURCE_DIR}/src)

pd_add_extension(NAME libdir
    FILES libdir/libdir.c
    INTERNAL True
    LIBRARY libdir
    HELP_FILES libdir/libdir-help.pd)
