//
//  tl_transport.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 10/12/16.
//
//

#include "ceammc_log.h"
#include "ceammc_timeline.h"
#include "m_pd.h"

static t_class* tl_transport_class;

struct tl_transport {
    t_object x_obj;
    t_canvas* cnv;
};

static void tl_transport_cue(tl_transport* x, t_floatarg cue)
{
    using namespace ceammc;

    if (cue < 0) {
        LIB_ERR << "invalid cue index given: " << cue;
        return;
    }

    ceammc::tl::trigger_actions(x->cnv, static_cast<size_t>(cue));
}

static void tl_transport_info(tl_transport* x, t_symbol* s, int argc, t_atom* argv)
{
    //    //printf("info\n");

    //    //    tll_cue_dump();
    //    tll_ui_dump();
}

static void* tl_transport_new()
{
    tl_transport* x = reinterpret_cast<tl_transport*>(pd_new(tl_transport_class));
    x->cnv = canvas_getcurrent();
    return static_cast<void*>(x);
}

extern "C" void setup_tl0x2etransport()
{
    tl_transport_class = class_new(gensym("tl.transport"),
        reinterpret_cast<t_newmethod>(tl_transport_new),
        reinterpret_cast<t_method>(0),
        sizeof(tl_transport), 0, A_NULL);

    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_info), gensym("info"), A_NULL);
    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_cue), gensym("cue"), A_DEFFLOAT, 0);
}
