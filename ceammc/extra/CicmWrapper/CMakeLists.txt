set(CICM_SRC
    Sources/ebox.c
    Sources/eclass.c
    Sources/ecommon.c
    Sources/egraphics.c
    Sources/eobj.c
    Sources/epopup.c
)

# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(cicm_static STATIC ${CICM_SRC})
