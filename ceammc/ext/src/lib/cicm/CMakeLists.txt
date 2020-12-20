if(WIN32)
    add_definitions("-D_WINDOWS=1")
endif()

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

set(CICM_COMPILE_FLAGS "")

endif()


set_target_properties(cicm_static PROPERTIES
    COMPILE_FLAGS "${CICM_COMPILE_FLAGS}")
