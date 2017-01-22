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

using namespace ceammc;

static t_class* tl_transport_class;

struct tl_transport {
    t_object x_obj;
    t_canvas* cnv;
};

static void tl_transport_cue(tl_transport* x, t_floatarg cue)
{
    if (cue < 0) {
        LIB_ERR << "invalid cue index given: " << cue;
        return;
    }

    tl::trigger_actions(x->cnv, static_cast<size_t>(cue));
}

static void dump_cues(tl_transport* x)
{
    const size_t cue_count = tl::CueStorage::cueCount(x->cnv);
    for (size_t i = 0; i < cue_count; i++) {
        tl::CueData* c = tl::CueStorage::at(x->cnv, i);
        if (!c)
            continue;

        post("[tl.transport] %s: pos = %d", c->name().c_str(), c->xPos());
    }
}

static void dump_ui(tl_transport* x)
{
    const size_t ui_count = tl::UIStorage::size();
    for (size_t i = 0; i < ui_count; i++) {
        tl::TimelineData* tl = tl::UIStorage::at(i);
        if (tl == 0)
            continue;

        if (tl->canvas() != x->cnv)
            continue;

        post("[tl.transport] ui: pos = %d", tl->xPos());
    }
}

static void tl_transport_info(tl_transport* x, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    dump_cues(x);
    dump_ui(x);
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

    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_info), gensym("dump"), A_NULL);
    class_addmethod(tl_transport_class, reinterpret_cast<t_method>(tl_transport_cue), gensym("cue"), A_DEFFLOAT, 0);
}
