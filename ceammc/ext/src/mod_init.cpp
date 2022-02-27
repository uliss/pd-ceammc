/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "mod_init.h"
#include "ceammc_object.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "lib/ceammc.h"
#include "tcl/ceammc_tcl.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
#include "s_stuff.h"
}

#include "an/mod_analyze.h"
#include "array/mod_array.h"
#include "base/mod_base.h"
#include "conv/mod_conv.h"
#include "data/mod_data.h"
#include "dyn/mod_dyn.h"
#include "env/mod_env.h"
#include "flow/mod_flow.h"
#include "flt/mod_flt.h"
#include "fx/mod_fx.h"
#include "hw/mod_hw.h"
#include "lang/mod_lang.h"
#include "lfo/mod_lfo.h"
#include "list/mod_list.h"
#include "math/mod_math.h"
#include "midi/mod_midi.h"
#include "misc/mod_misc.h"
#include "music/mod_music.h"
#include "net/mod_net.h"
#include "noise/mod_noise.h"
#include "osc/mod_osc.h"
#include "path/mod_path.h"
#include "preset/mod_preset.h"
#include "proto/mod_proto.h"
#include "random/mod_random.h"
#include "samp/mod_samp.h"
#include "spat/mod_spat.h"
#include "string/mod_string.h"
#include "synth/mod_synth.h"
#include "system/mod_system.h"
#include "tl/mod_tl.h"
#include "ui/mod_ui.h"
#include "window/mod_window.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

extern t_class* ceammc_class;

std::set<std::string>& ceammc_ext_list()
{
    static std::set<std::string> instance_;
    return instance_;
}

static void setup_env_doc_path()
{
    if (!sys_libdir)
        return;

    std::string path(sys_libdir->s_name);
    path += "/doc";
    ceammc::platform::set_env("DOC", path.c_str());
}

static void setup_env_ceammc_doc_path()
{
    if (!ceammc_class || !ceammc_class->c_externdir) {
        pd_error(nullptr, "[ceammc] library is not initialized");
        return;
    }

    std::string path(ceammc_class->c_externdir->s_name);
    ceammc::platform::set_env("CEAMMC", path.c_str());
}

namespace {

t_visfn ceammc_pd_vanilla_visfn = nullptr;

void t_object_get_xlet_id(t_glist* glist, void* x, char* buf, size_t bufsize, ceammc::XletType type, int xlet_idx)
{
    if (type == ceammc::XLET_IN)
        snprintf(buf, bufsize, "%si%d", rtext_gettag(glist_findrtext(glist, (t_object*)x)), xlet_idx);
    else
        snprintf(buf, bufsize, "%so%d", rtext_gettag(glist_findrtext(glist, (t_object*)x)), xlet_idx);
}

void ceammc_vis_fn(t_gobj* z, t_glist* glist, int vis)
{
    ceammc_pd_vanilla_visfn(z, glist, vis);

    if (vis) {
        auto ann_fn = ceammc::ceammc_get_annotation_fn(&z->g_pd);
        if (ann_fn) {
            t_object* o = (t_object*)z;
            const int NINS = obj_ninlets(o);
            const int NOUTS = obj_noutlets(o);

            // annotate inlets
            for (int i = 0; i < NINS; i++)
                ceammc::ceammc_xlet_bind_tooltip(o, glist, t_object_get_xlet_id, ann_fn, ceammc::XLET_IN, i);

            // annotate outlets
            for (int i = 0; i < NOUTS; i++)
                ceammc::ceammc_xlet_bind_tooltip(o, glist, t_object_get_xlet_id, ann_fn, ceammc::XLET_OUT, i);
        }
    }
}
}

void ceammc_init()
{
    using namespace std;

    const auto is_ceammc = getenv("is_ceammc");
    if (is_ceammc && strcmp(is_ceammc, "true") == 0) {
        if (text_widgetbehavior.w_visfn && text_widgetbehavior.w_visfn != ceammc_vis_fn) {
            ceammc_pd_vanilla_visfn = text_widgetbehavior.w_visfn;
            auto wb = &text_widgetbehavior;
            wb->w_visfn = ceammc_vis_fn;
        }

        ceammc::ceammc_tcl_init_tooltips();
        post("[ceammc] distribution: internal ceammc");
    } else {
        post("[ceammc] distribution: external deken");
    }

    ceammc::ceammc_tcl_init();

    if (!ceammc::pd::addPdPrintDataSupport())
        pd_error(nullptr, "can't add datatype printing support to vanilla [print] object");

    // setup env variables
    setup_env_doc_path();
    setup_env_ceammc_doc_path();

#ifndef __WIN32
    // save vanilla external list
    vector<string> l = ceammc::pd::currentListOfExternals();
    set<string> vanilla_set(l.begin(), l.end());
#endif

    ceammc::BaseObject::initInletDispatchNames();

    ceammc_analyze_setup();
    ceammc_array_setup();
    ceammc_base_setup();
    ceammc_conv_setup();
    ceammc_data_setup();
    ceammc_dyn_setup();
    ceammc_env_setup();
    ceammc_flow_setup();
    ceammc_flt_setup();
    ceammc_fx_setup();
    ceammc_hw_setup();
    ceammc_lang_setup();
    ceammc_lfo_setup();
    ceammc_list_setup();
    ceammc_math_setup();
    ceammc_midi_setup();
    ceammc_misc_setup();
    ceammc_music_setup();
    ceammc_net_setup();
    ceammc_noise_setup();
    ceammc_preset_setup();
    ceammc_path_setup();
    ceammc_proto_setup();
    ceammc_osc_setup();
    ceammc_random_setup();
    ceammc_samp_setup();
    ceammc_spat_setup();
    ceammc_string_setup();
    ceammc_synth_setup();
    ceammc_system_setup();
    ceammc_tl_setup();
    ceammc_ui_setup();
    ceammc_window_setup();

#ifndef __WIN32
    // get ceammc extension list
    l = ceammc::pd::currentListOfExternals();
    set<string> current_set(l.begin(), l.end());

    set_difference(current_set.begin(), current_set.end(),
        vanilla_set.begin(), vanilla_set.end(),
        inserter(ceammc_ext_list(), ceammc_ext_list().end()));
#endif
}
