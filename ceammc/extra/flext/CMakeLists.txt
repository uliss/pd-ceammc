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
add_definitions(-DFLEXT_SYS_PD)
add_library(flext_static STATIC ${FLEXT_SRC})

if(UNIX)
    set_target_properties(flext_static PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
