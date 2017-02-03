//
//  ui_spectroscope~.cpp
//  pd_ext
//
//  Created by Alex on 19/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

struct ui_spectroscope : public ceammc_gui::BaseGuiObject {
    t_edspobj d_dsp;

    t_inlet* in1;
    t_sample input;
    t_sample buf[8192];

    int counter;

    t_rgba b_color_background;
    t_rgba b_color_border;

} _ui_spectroscope;

namespace ceammc_gui {

#pragma mark dsp

static void ui_spectroscope_perform(ui_spectroscope* x, t_object* dsp64, t_sample** ins, long numins, t_sample** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    t_sample* in1 = ins[0];
    int n = (int)sampleframes;

    t_sample* out = &(x)->buf[x->counter * sampleframes];

    while (n--) {
        *out++ = *in1++;
    } //

    x->counter++;
    if (x->counter == 32) {
        x->counter = 0;
        ceammc_gui::GuiFactory<ui_spectroscope>::ws_redraw(((t_object*)x));
    }
}

void ui_spectroscope_dsp(ui_spectroscope* x, t_object* dsp, short* count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)ui_spectroscope_perform, 0, NULL);
}

#pragma mark ui

UI_fun(ui_spectroscope)::wx_paint(t_object* z, t_object* view)
{
    t_symbol* bgl = gensym("background_layer");
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)z, &rect);

    t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);

    if (g) {
        egraphics_set_line_width(g, 1);

        egraphics_set_color_hex(g, gensym("#A0A0A0"));
        egraphics_line(g, 0, rect.height / 2, rect.width, rect.height / 2);
        egraphics_line(g, 0, rect.height * .25, rect.width, rect.height * .25);
        egraphics_line(g, 0, rect.height * .75, rect.width, rect.height * .75);
        egraphics_stroke(g);

        egraphics_set_line_width(g, 1);

        egraphics_set_color_hex(g, gensym("#00C0FF"));

        t_sample* in1 = ((ui_spectroscope*)z)->buf;

        t_sample zero[8192];

        t_sample* zero1 = zero;

        int fft_size = 1024;

        for (int i = 0; i < fft_size; i++) {
            zero[i] = 0;
        }
        //int half_fft = fft_size/2;

        mayer_fft(fft_size, in1, zero1);
        int i2 = 0;
        int n = fft_size;

        t_sample out_buf[8192];
        t_sample* out1 = out_buf;
        while (n--) {

            t_sample f_r = 1. / fft_size * in1[(int)floor(i2 / 2)];
            t_sample f_i = 1. / fft_size * in1[(int)floor(i2 / 2) + fft_size];

            out1[i2] = sqrtf(f_r * f_r + f_i * f_i);

            i2++;

        } //

        ((ui_spectroscope*)z)->buf[0] = 0;
        ; //    instead of windowind lol
        egraphics_move_to(g, 0, (1 - ((ui_spectroscope*)z)->buf[0]) * rect.height);

        for (int i = 0; i < fft_size; i += 4) {

            float xx = float(i + 1) / fft_size * rect.width;
            float val = (out1[i] == out1[i]) ? out1[i] : 0.;

            float yy = (1. - val) * rect.height;

            egraphics_line_to(g, xx, yy);
        }

        egraphics_stroke(g);
    }

    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);
}
// these are added as a quick fix
UI_fun(ui_spectroscope)::wx_mousedown(t_object* z, t_object* view, t_pt pt, long modifiers)
{
}

UI_fun(ui_spectroscope)::wx_mouseup(t_object* z, t_object* view, t_pt pt, long modifiers)
{
}

UI_fun(ui_spectroscope)::wx_mousemove(t_object* z, t_object* view, t_pt pt, long modifiers)
{
}

UI_fun(ui_spectroscope)::wx_mousedrag(t_object* z, t_object* view, t_pt pt, long modifiers)
{
}
}

#pragma mark setup

UI_fun(ui_spectroscope)::new_ext(t_object* z, t_symbol* s, int argcl, t_atom* argv)
{
    eobj_dspsetup(z, 1, 0);
    printf("new ext \n");
}

static void ui_sps_getdrawparams(ui_spectroscope* x, t_object* patcherview, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_spectroscope)::init_ext(t_eclass* z)
{
    eclass_addmethod(z, (method)ui_spectroscope_dsp, "dsp", A_CANT, 0);
    CLASS_ATTR_DEFAULT(z, "size", 0, "150. 100.");

    CLASS_ATTR_RGBA(z, "bgcolor", 0, ui_spectroscope, b_color_background);
    CLASS_ATTR_LABEL(z, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER(z, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "bgcolor", 0, "0.93 0.93 0.93 1.");
    CLASS_ATTR_STYLE(z, "bgcolor", 0, "color");

    CLASS_ATTR_RGBA(z, "bdcolor", 0, ui_spectroscope, b_color_border);
    CLASS_ATTR_LABEL(z, "bdcolor", 0, "Border Color");
    CLASS_ATTR_ORDER(z, "bdcolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "bdcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE(z, "bdcolor", 0, "color");

    eclass_addmethod(z, (method)ui_sps_getdrawparams, "getdrawparams", A_NULL, 0);
}

extern "C" void setup_ui0x2espectroscope()
{
    ceammc_gui::GuiFactory<ui_spectroscope> class1;
    class1.setup_dsp("ui.spectroscope");
}