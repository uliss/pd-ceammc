set(FLEXT_SRC
    flext/source/flatom.cpp
    flext/source/flatom_part.cpp
    flext/source/flatom_pr.cpp
    flext/source/flattr.cpp
    flext/source/flattr_ed.cpp
    flext/source/flbase.cpp
    flext/source/flbind.cpp
    flext/source/flbuf.cpp
    flext/source/fldsp.cpp
    flext/source/flext.cpp
    flext/source/flitem.cpp
    flext/source/fllib.cpp
    flext/source/flmap.cpp
    flext/source/flmeth.cpp
    flext/source/flmsg.cpp
    flext/source/flout.cpp
    flext/source/flproxy.cpp
    flext/source/flqueue.cpp
    flext/source/flsimd.cpp
    #flext/source/flsndobj.cpp
    #flext/source/flstk.cpp
    flext/source/flsupport.cpp
    flext/source/flthr.cpp
    flext/source/fltimer.cpp
    flext/source/flutil.cpp
    flext/source/flxlet.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_definitions(-DFLEXT_SYS_PD -DFLEXT_ATTRIBUTES)
if(APPLE)
    add_definitions(-DFLEXT_THREADS=3)
endif()
add_library(flext_static STATIC ${FLEXT_SRC})

if(UNIX)
    set_target_properties(flext_static PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

set(ZCONF_SRC
    zconf/zconf.cpp
    zconf/zconf_browse.cpp
    zconf/zconf_domains.cpp
    zconf/zconf_meta.cpp
    zconf/zconf_resolve.cpp
    zconf/zconf_service.cpp
)

include(PdExtension)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/flext/source)
pd_add_extension(NAME "zconf"
    FILES ${ZCONF_SRC}
    INTERNAL True
    LIBRARY ceammc
    LINK flext_static)


