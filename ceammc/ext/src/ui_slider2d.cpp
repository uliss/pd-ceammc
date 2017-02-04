//
//  ui_slider2d.cpp
//  pd_ext
//
//  Created by Alex on 18/12/16.
//
//

#include <algorithm>
#include <stdio.h>

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "lib/ceammc_gui.h"

template <typename T>
static T clip(T min, T max, T v)
{
    return std::max(min, std::min(max, v));
}

struct ui_slider2d : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;

    t_atom out_list[2];

    float shift_x;
    float range_x;
    float shift_y;
    float range_y;

    float _posx;
    float _posy;

    t_etext* txt_min;
    t_etext* txt_max;
    t_efont* txt_font;

    t_rgba b_color_background;
    t_rgba b_color_border;

public:
    void output()
    {
        atom_setfloat(&out_list[0], _posx * range_x + shift_x);
        atom_setfloat(&out_list[1], _posy * range_y + shift_y);

        outlet_list(out1, &s_list, 2, out_list);
    }

    void setPos(const t_pt& mousePos)
    {
        _posx = clip(0.f, 1.f, mousePos.x / width());
        _posy = clip(0.f, 1.f, mousePos.y / height());
    }
};

namespace ceammc_gui {

UI_fun(ui_slider2d)::wx_paint(t_object* z, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    t_elayer* g = ebox_start_layer(asBox(z), BG_LAYER, rect.width, rect.height);

    if (g) {
        ui_slider2d* zx = asStruct(z);

        float xx = zx->_posx;
        float yy = zx->_posy;
        xx *= rect.width;
        yy *= rect.height;

        float knobsize = clip<float>(5.f, 20.f, std::min(rect.height, rect.width) * 0.1f);

        egraphics_set_line_width(g, 0.5);
        egraphics_set_color_hex(g, gensym("#00C0F0"));

        egraphics_line(g, xx, 0, xx, rect.height);
        egraphics_line(g, 0, yy, rect.width, yy);

        egraphics_stroke(g);

        egraphics_set_line_width(g, 1.);
        egraphics_set_color_hex(g, gensym(zx->mouse_dn ? "#00C0FF" : "#707070")); //(UI_Pf("_mouse_dn")==1)
        egraphics_rectangle(g, xx - 0.5 * knobsize, yy - 0.5 * knobsize, knobsize, knobsize);
        egraphics_stroke(g);

        egraphics_set_color_hex(g, gensym(zx->mouse_dn ? "#003070" : "#C0C0C0"));
        egraphics_rectangle(g, xx - 0.5 * knobsize + 1, yy - 0.5 * knobsize + 1, knobsize - 1, knobsize - 1);
        egraphics_fill(g);

        char c_min[30];
        sprintf(c_min, "[%.2f..%.2f]", zx->shift_x, zx->range_x + zx->shift_x);

        char c_max[30];
        sprintf(c_max, "[%.2f..%.2f]", zx->shift_y, zx->range_y + zx->shift_y);

        etext_layout_set(zx->txt_min, c_min, zx->txt_font, rect.width - 3, rect.height - 12, rect.width, rect.height / 2, ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_min, g);

        etext_layout_set(zx->txt_max, c_max, zx->txt_font, 3, 12, rect.width * 2, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_max, g);

        ebox_end_layer(asBox(z), BG_LAYER);
    }

    ebox_paint_layer(asBox(z), BG_LAYER, 0, 0);
}

UI_fun(ui_slider2d)::wx_mousedrag_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
{
    ui_slider2d* zx = asStruct(z);
    zx->setPos(pt);
    ws_redraw(z);
    zx->output();
}

UI_fun(ui_slider2d)::wx_mousedown_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
{
    ui_slider2d* zx = asStruct(z);
    zx->setPos(pt);
    ws_redraw(z);
    zx->output();
}

UI_fun(ui_slider2d)::wx_mouseup_ext(t_object* z, t_object*, t_pt pt, long)
{
    ui_slider2d* zx = asStruct(z);

    zx->setPos(pt);
    ws_redraw(z);
    zx->output();
}

static void ui_s2_getdrawparams(ui_slider2d* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_slider2d)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "100. 100.");

    CLASS_ATTR_FLOAT                (z, "shift_x", 0, ui_slider2d, shift_x);
    CLASS_ATTR_DEFAULT              (z, "shift_x", 0, "-1");
    CLASS_ATTR_LABEL                (z, "shift_x", 0, "shift_x");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift_x", 0, "-1");

    CLASS_ATTR_FLOAT                (z, "range_x", 0, ui_slider2d, range_x);
    CLASS_ATTR_DEFAULT              (z, "range_x", 0, "2");
    CLASS_ATTR_LABEL                (z, "range_x", 0, "range_x");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range_x", 0, "2");

    CLASS_ATTR_FLOAT                (z, "shift_y", 0, ui_slider2d, shift_y);
    CLASS_ATTR_DEFAULT              (z, "shift_y", 0, "-1");
    CLASS_ATTR_LABEL                (z, "shift_y", 0, "shift_y");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift_y", 0, "-1");

    CLASS_ATTR_FLOAT                (z, "range_y", 0, ui_slider2d, range_y);
    CLASS_ATTR_DEFAULT              (z, "range_y", 0, "2");
    CLASS_ATTR_LABEL                (z, "range_y", 0, "range_y");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range_y", 0, "2");

    CLASS_ATTR_RGBA                 (z, "bgcolor", 0, ui_slider2d, b_color_background);
    CLASS_ATTR_LABEL                (z, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (z, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bgcolor", 0, "0.93 0.93 0.93 1.");
    CLASS_ATTR_STYLE                (z, "bgcolor", 0, "color");

    CLASS_ATTR_RGBA                 (z, "bdcolor", 0, ui_slider2d, b_color_border);
    CLASS_ATTR_LABEL                (z, "bdcolor", 0, "Border Color");
    CLASS_ATTR_ORDER                (z, "bdcolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bdcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (z, "bdcolor", 0, "color");
    // clang-format on

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_s2_getdrawparams), "getdrawparams", A_NULL, 0);
}

UI_fun(ui_slider2d)::new_ext(t_object* x, t_symbol*, int, t_atom*)
{
    ui_slider2d* zx = asStruct(x);
    zx->out1 = outlet_new(x, &s_list);

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);

    // default position in center
    zx->_posx = 0.5;
    zx->_posy = 0.5;
}

UI_fun(ui_slider2d)::free_ext(t_object* x)
{
    ui_slider2d* zx = asStruct(x);
    outlet_free(zx->out1);

    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);
    efont_destroy(zx->txt_font);
}
}

extern "C" void setup_ui0x2eslider2d()
{
    ceammc_gui::GuiFactory<ui_slider2d> class1;
    class1.setup("ui.slider2d");
}
