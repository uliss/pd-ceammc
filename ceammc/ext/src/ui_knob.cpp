//
//  ui_knob.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//

#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

struct ui_knob : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;

    float _value;

    float range;
    float shift;

    t_etext* txt_min;
    t_etext* txt_max;
    t_efont* txt_font;

public:
    t_float realValue() const
    {
        return _value * range + shift;
    }
};

namespace ceammc_gui {

void e_polyarc(t_elayer* g, float xc, float yc, float radius, float angle1, float angle2, float lod)
{
    float d_angle = angle2 - angle1;
    for (int i = 0; i < lod; i++) {
        float x1, x2, y1, y2;

        x1 = xc + cosf(angle1 + d_angle * float(i / lod)) * radius;
        x2 = xc + cosf(angle1 + d_angle * float((i + 1) / lod)) * radius;

        y1 = yc - sinf(angle1 + d_angle * float(i / lod)) * radius;
        y2 = yc - sinf(angle1 + d_angle * float((i + 1) / lod)) * radius;

        egraphics_line(g, x1, y1, x2, y2);
    }
}

void e_marc(t_elayer* g, float xc, float yc, float radius, float angle1, float angle2, float anglem, float lod)
{
    float x1, x2, y1, y2;
    float d_angle = angle2 - angle1;

    x1 = xc + cosf(angle1) * radius; // + d_angle* float(i/lod)) *radius;
    x2 = xc + cosf(angle1 + d_angle) * radius;

    y1 = yc - sinf(angle1) * radius;
    y2 = yc - sinf(angle1 + d_angle) * radius;

    float angle_l = 1.f - 0.99f * ((angle1 + d_angle) / d_angle) * anglem;

    egraphics_move_to(g, x1, y1);
    egraphics_arc_to(g, xc, yc, angle_l);
}

UI_fun(ui_knob)::wx_paint(t_object* z, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    t_elayer* g = ebox_start_layer(asBox(z), BG_LAYER, rect.width, rect.height);

    ui_knob* zx = asStruct(z);

    if (g) {

        float rmin = 0.5f * rect.width * 0.7f - 2;
        float rmid = 0.5f * rect.width - 2;
        float rmax = 0.5f * rect.width - 2;
        float amin = 5 / 8.f * M_PI * 2.f;
        float amax = -1 / 8.f * M_PI * 2.f;
        float aval = M_PI * 2.f * 6 / 8.f * zx->_value; //UI_Pf("_value");

        float cx = rect.width * 0.5f;
        float cy = rect.height * 0.5f;

        egraphics_set_line_width(g, 1);
        egraphics_set_color_hex(g, gensym("#C0C0C0"));
        //e_marc(g, cx, cy, rmax, amin, amin+ 2*M_PI, EPD_2PI*6/8., 20);
        e_polyarc(g, cx, cy, rmin, amax + aval, amax, 30);
        e_polyarc(g, cx, cy, rmax, amin, amax, 30);
        //egraphics_circle(g, cx, cy, rmax);

        egraphics_line(g, cx + cosf(amin) * 0, cy - sinf(amin) * 0, cx + cosf(amin) * rmax, cy - sinf(amin) * rmax);
        egraphics_line(g, cx + cosf(amax) * 0, cy - sinf(amax) * 0, cx + cosf(amax) * rmax, cy - sinf(amax) * rmax);

        egraphics_stroke(g);

        egraphics_set_line_width(g, 2);
        egraphics_set_color_hex(g, gensym("#00C0FF"));
        //e_marc(g, cx, cy, rmid, amax + aval, amin, EPD_2PI*6/8., 20);
        e_polyarc(g, cx, cy, rmid, amax + aval, amin, 30 - floor(29 * zx->_value)); //UI_Pf("_value")
        //egraphics_arc(g, cx, cy, rmid, amax+aval, amin);
        egraphics_line(g, cx, cy, cx + cosf(amax + aval) * rmid, cy - sinf(amax + aval) * rmid);
        egraphics_line(g, cx, cy, cx + cosf(amin) * rmid, cy - sinf(amin) * rmid);
        egraphics_stroke(g);

        char c_min[10];
        sprintf(c_min, "%.2f", zx->shift);

        char c_max[10];
        sprintf(c_max, "%.2f", zx->range + zx->shift);

        etext_layout_set(zx->txt_min, c_min, zx->txt_font, 3, rect.height - 12, rect.width * 2, rect.height / 2, ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_min, g);

        etext_layout_set(zx->txt_max, c_max, zx->txt_font, rect.width - 3, rect.height - 12, rect.width, rect.height / 2, ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_max, g);

        ebox_end_layer(asBox(z), BG_LAYER);
    }

    ebox_paint_layer(asBox(z), BG_LAYER, 0., 0.);
}

UI_fun(ui_knob)::wx_oksize(t_object*, t_rect* newrect)
{
    newrect->width = floorf(newrect->width / 5.f) * 5;
    newrect->height = newrect->width;
}

UI_fun(ui_knob)::wx_mousedrag_ext(t_object* z, t_object*, t_pt pt, long)
{
    ui_knob* zx = asStruct(z);

    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    float val;
    val = 1 - pt.y / rect.height;

    if (val > 1)
        val = 1;
    if (val < 0)
        val = 0;

    zx->_value = val;

    ws_redraw(z);

    outlet_float(zx->out1, (1 - val) * zx->range + zx->shift);
}

UI_fun(ui_knob)::wx_mousedown_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
{
    wx_mousedrag_ext(z, view, pt, modifiers);
}

UI_fun(ui_knob)::m_float(t_object* z, t_float f)
{
    ui_knob* zx = asStruct(z);
    zx->_value = 1.f - (f / 127.f);

    ws_redraw(z);
    outlet_float(zx->out1, zx->realValue());
}

UI_fun(ui_knob)::m_bang(t_object* z)
{
    ui_knob* zx = asStruct(z);
    outlet_float(zx->out1, zx->realValue());
}

static void ui_kn_getdrawparams(ui_knob* x, t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_knob)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT (z, "size", 0, "60. 60.");

    CLASS_ATTR_FLOAT                (z, "shift", 0, ui_knob, shift);
    CLASS_ATTR_DEFAULT              (z, "shift", 0, "0");
    CLASS_ATTR_LABEL                (z, "shift", 0, "shift");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift", 0, "0");

    CLASS_ATTR_FLOAT                (z, "range", 0, ui_knob, range);
    CLASS_ATTR_DEFAULT              (z, "range", 0, "127");
    CLASS_ATTR_LABEL                (z, "range", 0, "range");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range", 0, "127");
    // clang-format on

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_kn_getdrawparams), "getdrawparams", A_NULL, 0);
}

UI_fun(ui_knob)::new_ext(t_object* x, t_symbol*, int, t_atom*)
{
    ui_knob* zx = asStruct(x);
    zx->out1 = outlet_new(x, &s_float);
    zx->_value = 1.f;

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, 8);
}

UI_fun(ui_knob)::free_ext(t_object* z)
{
    ui_knob* zx = asStruct(z);
    outlet_free(zx->out1);

    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);
    efont_destroy(zx->txt_font);
}
}

extern "C" void setup_ui0x2eknob()
{
    ceammc_gui::GuiFactory<ui_knob> class1;
    class1.setup("ui.knob");
}
