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
    LIBRARY zconf
    LINK flext_static)

set(VASP_SRC
    vasp/source/arg.cpp
    vasp/source/arg.h
    vasp/source/buflib.cpp
    vasp/source/buflib.h
    vasp/source/classes.cpp
    vasp/source/classes.h
    vasp/source/env.cpp
    vasp/source/env.h
    vasp/source/main.cpp
    vasp/source/main.h
    vasp/source/mixfft.hpp
    vasp/source/obj_chns.cpp
    vasp/source/obj_frames.cpp
    vasp/source/obj_imm.cpp
    vasp/source/obj_offs.cpp
    vasp/source/obj_part.cpp
    vasp/source/obj_peaks.cpp
    vasp/source/obj_q.cpp
    vasp/source/obj_radio.cpp
    vasp/source/obj_size.cpp
    vasp/source/obj_split.cpp
    vasp/source/obj_sync.cpp
    vasp/source/obj_vasp.cpp
    vasp/source/obj_vecs.cpp
    vasp/source/opbase.cpp
    vasp/source/opbase.h
    vasp/source/opdefs.h
    vasp/source/opfuns.h
    vasp/source/oploop.h
    vasp/source/opparam.cpp
    vasp/source/opparam.h
    vasp/source/oppermute.h
    vasp/source/ops.h
    vasp/source/ops_arith.cpp
    vasp/source/ops_arith.h
    vasp/source/ops_assign.cpp
    vasp/source/ops_assign.h
    vasp/source/ops_carith.cpp
    vasp/source/ops_carith.h
    vasp/source/ops_cmp.cpp
    vasp/source/ops_cmp.h
    vasp/source/ops_cplx.cpp
    vasp/source/ops_cplx.h
    vasp/source/ops_dft.cpp
    vasp/source/ops_dft.h
    vasp/source/ops_feature.cpp
    vasp/source/ops_feature.h
    vasp/source/ops_flt.cpp
    vasp/source/ops_flt.h
    vasp/source/ops_gate.cpp
    vasp/source/ops_gen.cpp
    vasp/source/ops_gen.h
    vasp/source/ops_qminmax.cpp
    vasp/source/ops_rearr.cpp
    vasp/source/ops_rearr.h
    vasp/source/ops_resmp.cpp
    vasp/source/ops_resmp.h
    vasp/source/ops_search.cpp
    vasp/source/ops_search.h
    vasp/source/ops_trnsc.cpp
    vasp/source/ops_trnsc.h
    vasp/source/ops_wnd.cpp
    vasp/source/ops_wnd.h
    vasp/source/opvecs.cpp
    vasp/source/rdx2fft.hpp
    vasp/source/rvfft.hpp
    vasp/source/util.cpp
    vasp/source/util.h
    vasp/source/vasp-Info.plist
    vasp/source/vasp.cpp
    vasp/source/vasp.h
    vasp/source/vasp.rsrc
    vasp/source/vbuffer.cpp
    vasp/source/vbuffer.h
    vasp/source/vecblk.cpp
    vasp/source/vecblk.h
)

pd_add_extension(NAME vasp
    FILES ${VASP_SRC}
    INTERNAL True
    LIBRARY vasp
    LINK flext_static)


