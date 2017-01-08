include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

pd_add_extension(NAME system.cursor
    FILES system.cursor.c
    INTERNAL True
    LIBRARY ceammc
    )

pd_add_extension(NAME system.colorpanel
    FILES system.colorpanel.c
    INTERNAL True
    LIBRARY ceammc
    )
