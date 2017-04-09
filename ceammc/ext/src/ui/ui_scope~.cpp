//
//  ui_scope~.cpp
//  pd_ext
//
//  Created by Alex on 19/12/16.
//
//

#include <algorithm>
#include <cassert>
#include <cstdio>

#include "ceammc_gui.h"

static const size_t BUFSIZE = 8912;

struct ui_scope : public ceammc_gui::BaseSoundGuiStruct {
    t_sample buf[BUFSIZE];
    size_t counter;
    int b_freeze;
    float xscale;
    float yscale;

    inline t_sample valueAt(size_t i) const
    {
        assert(i < BUFSIZE);
        return -buf[i] * 0.5f * yscale + 0.5f;
    }
};

namespace ceammc_gui {

static t_symbol* HRULE_COLOR = gensym("#A0A0A0");
static const float HRULE_LINE_WIDTH = 1.0f;
static const float GRAPH_LINE_WIDTH = 1.5f;

static void ui_scope_freeze(ui_scope* x, t_floatarg f)
{
    x->b_freeze = static_cast<int>(f) == 0 ? 0 : 1;
}

#pragma mark dsp

static void ui_scope_perform(ui_scope* x, t_object*,
    t_sample** ins, long,
    t_sample**, long,
    long sampleframes, long, void*)
{
    if (x->b_freeze)
        return;

    const size_t idx = x->counter * sampleframes;
    if (idx >= BUFSIZE)
        return;

    t_sample* in = ins[0];
    t_sample* out = &x->buf[idx];
    std::copy(in, in + sampleframes, out);

    x->counter++;
    if (x->counter == 32) {
        x->counter = 0;
        ceammc_gui::GuiFactory<ui_scope>::ws_redraw(x);
    }
}

static void ui_scope_dsp(ui_scope* x, t_object* dsp, short* /*count*/, double /*samplerate*/, long /*maxvectorsize*/, long /*flags*/)
{
    object_method(dsp, gensym("dsp_add"), x, reinterpret_cast<method>(ui_scope_perform), 0, NULL);
}

#pragma mark ui

UI_fun(ui_scope)::wx_paint(ui_scope* zx, t_object* /*view*/)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        // draw hrules
        egraphics_set_line_width(g, HRULE_LINE_WIDTH);
        egraphics_set_color_hex(g, HRULE_COLOR);
        egraphics_line(g, 0, rect.height / 2, rect.width, rect.height / 2);
        egraphics_line(g, 0, rect.height * 0.25f, rect.width, rect.height * 0.25f);
        egraphics_line(g, 0, rect.height * 0.75f, rect.width, rect.height * 0.75f);
        egraphics_stroke(g);

        egraphics_set_line_width(g, GRAPH_LINE_WIDTH);
        egraphics_set_color_hex(g, COLOR_ACTIVE);
        egraphics_move_to(g, 0, zx->valueAt(0) * rect.height);

        for (size_t i = 0; i < 128; i++) {
            float xx = (i * rect.width) / 128.0f + 1.f;
            float yy = zx->valueAt(i) * rect.height;
            egraphics_line_to(g, xx, yy);
        }

        egraphics_stroke(g);
        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0., 0.);
}

UI_fun(ui_scope)::wx_oksize(ui_scope*, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30);
    newrect->height = pd_clip_min(newrect->height, 30);
}

UI_fun(ui_scope)::wx_mousewheel_ext(ui_scope* zx, t_object*, t_pt, long modifiers, double delta)
{
    if (modifiers == EMOD_SHIFT) {
        float change = ((delta < 0) ? -1 : 1) * 0.1f;
        float new_yscale = zx->yscale + change;
        if (0.1f <= new_yscale && new_yscale <= 10.f)
            zx->yscale = new_yscale;
    }
}

#pragma mark setup

static void ui_s_getdrawparams(ui_scope* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_scope)::new_ext(ui_scope* zx, t_symbol*, int, t_atom*)
{
    eobj_dspsetup(zx, 1, 0);
    zx->b_freeze = 0;
    zx->counter = 0;
}

UI_fun(ui_scope)::init_ext(t_eclass* z)
{
    eclass_addmethod(z, reinterpret_cast<method>(ui_scope_dsp), "dsp", A_CANT, 0);

    // clang-format off
    CLASS_ATTR_INVISIBLE            (z, "send", 1);

    CLASS_ATTR_DEFAULT(z, "size", 0, "150. 100.");

    CLASS_ATTR_FLOAT                (z, "yscale", 0, ui_scope, yscale);
    CLASS_ATTR_DEFAULT              (z, "yscale", 0, "1");
    CLASS_ATTR_LABEL                (z, "yscale", 0, "Y-scale");
    CLASS_ATTR_STYLE                (z, "yscale", 0, "number");
    CLASS_ATTR_FILTER_CLIP          (z, "yscale", 0.1f, 10.0f);
    CLASS_ATTR_STEP                 (z, "yscale", 0.1);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "yscale", 0, "1");
    // clang-format off

    eclass_addmethod(z, reinterpret_cast<method>(ui_s_getdrawparams), "getdrawparams", A_NULL, 0);
    eclass_addmethod(z, reinterpret_cast<method>(ui_scope_freeze), "freeze", A_DEFFLOAT, 0);
}
}

extern "C" void setup_ui0x2escope_tilde()
{
    ceammc_gui::GuiFactory<ui_scope> class1;
    class1.setup_dsp("ui.scope~");
}
