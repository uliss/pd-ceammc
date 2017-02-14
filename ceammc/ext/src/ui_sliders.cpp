//
//  ui_sliders.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//

#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

struct ui_sliders : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;

    t_atom* val_list;
    int val_list_size;

    t_atom* draw_list;
    int draw_list_size;

    int count;
    float range;
    float shift;

    int sel_idx;
    bool auto_range;
    bool _is_vertical;

    t_etext* txt_min;
    t_etext* txt_max;
    t_efont* txt_font;
};

namespace ceammc_gui {

UI_fun(ui_sliders)::wx_paint(t_object* z, t_object* view)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    t_elayer* g = ebox_start_layer(asBox(z), BG_LAYER, rect.width, rect.height);

    if (g) {
        ui_sliders* zx = asStruct(z);

        if (zx->count < 1)
            zx->count = 1;
        if (zx->count > 1280)
            zx->count = 128;

        for (int i = 0; i < zx->count; i++) {
            float v = (zx->draw_list)[i].a_w.w_float;

            if (v > 1)
                v = 1;
            if (v < 0)
                v = 0;

            float xx, yy, w, h;

            if (zx->_is_vertical) {
                xx = 0;
                yy = i * rect.height / zx->count;
                w = v * rect.width;
                h = rect.height / zx->count - 1;
            } else {
                yy = rect.height - v * rect.height;
                xx = i * rect.width / zx->count;
                h = rect.height;
                w = rect.width / zx->count - 1;
            }

            egraphics_rectangle(g, xx, yy, w, h);
            egraphics_set_color_hex(g, (i == zx->sel_idx) ? gensym("#00C0FF") : gensym("#C0C0C0"));
            egraphics_fill(g);

            if (i == zx->sel_idx) {
                egraphics_rectangle(g, xx, yy, w, h);
                egraphics_set_color_hex(g, gensym("#303030"));
                egraphics_stroke(g);
            }
        }

        char c_min[10];
        sprintf(c_min, "%.2f", zx->shift);

        char c_max[10];
        sprintf(c_max, "%.2f", zx->range + zx->shift);

        etext_layout_set(zx->txt_min, c_min, zx->txt_font,
            3, rect.height - 12, rect.width * 2, rect.height / 2,
            ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
        etext_layout_draw(zx->txt_min, g);

        etext_layout_set(zx->txt_max, c_max, zx->txt_font,
            3, 12, rect.width * 2, rect.height / 2,
            ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_draw(zx->txt_max, g);

        ebox_end_layer(asBox(z), BG_LAYER);
    }

    ebox_paint_layer(asBox(z), BG_LAYER, 0, 0);
}

UI_fun(ui_sliders)::m_set(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_sliders* zx = asStruct(z);

    if (zx->val_list)
        free(zx->val_list);
    if (zx->draw_list)
        free(zx->draw_list);

    zx->val_list = (t_atom*)malloc(sizeof(t_atom) * argc);
    zx->val_list_size = argc;

    zx->draw_list = (t_atom*)malloc(sizeof(t_atom) * argc);
    zx->draw_list_size = argc;

    zx->count = argc;

    if (zx->auto_range) {
        float v_min = 0;
        float v_max = 0;

        for (int i = 0; i < argc; i++) {
            zx->val_list[i] = argv[i];
            v_min = (v_min > argv[i].a_w.w_float) ? argv[i].a_w.w_float : v_min;
            v_max = (v_max < argv[i].a_w.w_float) ? argv[i].a_w.w_float : v_max;
        }

        zx->shift = v_min;
        zx->range = (v_max - v_min);

        for (int i = 0; i < argc; i++) {

            zx->draw_list[i] = argv[i];
            zx->draw_list[i].a_w.w_float = (zx->draw_list[i].a_w.w_float - zx->shift) / zx->range;
        }
    } else {

        for (int i = 0; i < argc; i++) {
            zx->val_list[i] = argv[i];
            zx->draw_list[i] = argv[i];
            zx->draw_list[i].a_w.w_float = (zx->draw_list[i].a_w.w_float - zx->shift) / zx->range;
        }
    }

    ws_redraw(z);
}

UI_fun(ui_sliders)::m_list(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    m_set(z, s, argc, argv);
    ui_sliders* zx = asStruct(z);

    outlet_list(zx->out1, &s_list, zx->val_list_size, zx->val_list);
}

UI_fun(ui_sliders)::m_bang(t_object* z)
{
    ui_sliders* zx = asStruct(z);
    outlet_list(zx->out1, &s_list, zx->val_list_size, zx->val_list);
}

UI_fun(ui_sliders)::wx_oksize(t_object* z, t_rect* newrect)
{
    ui_sliders* zx = asStruct(z);
    zx->_is_vertical = newrect->width < newrect->height;
}

UI_fun(ui_sliders)::wx_mousedrag_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)z, &rect);

    int numslider;
    float val;

    ui_sliders* zx = asStruct(z);

    if (zx->val_list_size != zx->count) {
        if (zx->val_list)
            free(zx->val_list);

        zx->val_list = (t_atom*)malloc(sizeof(t_atom) * zx->count);
        zx->val_list_size = zx->count;

        for (int i = 0; i < zx->count; i++) {

            zx->val_list[i].a_type = A_FLOAT;
            zx->val_list[i].a_w.w_float = 0.;
        }
    }

    if (zx->draw_list_size != zx->count) {
        if (zx->draw_list)
            free(zx->draw_list);

        zx->draw_list = (t_atom*)malloc(sizeof(t_atom) * zx->count);
        zx->draw_list_size = zx->count;

        for (int i = 0; i < zx->count; i++) {
            zx->draw_list[i].a_type = A_FLOAT;
            zx->draw_list[i].a_w.w_float = 0.;
        }
    }

    if (zx->_is_vertical) {
        numslider = floorf(pt.y / rect.height * zx->val_list_size);
        val = pt.x / rect.width;
    } else {

        numslider = floorf(pt.x / rect.width * zx->val_list_size);

        val = 1. - pt.y / rect.height;
    }

    if (numslider > (zx->count - 1))
        numslider = zx->count - 1;
    if (numslider < 0)
        numslider = 0;

    zx->val_list[numslider].a_w.w_float = val * zx->range + zx->shift;
    zx->draw_list[numslider].a_w.w_float = val;

    ws_redraw(z);

    outlet_list(zx->out1, &s_list, zx->val_list_size, zx->val_list);
}

UI_fun(ui_sliders)::wx_mousedown_ext(t_object* z, t_object* view, t_pt pt, long modifiers)
{
    wx_mousedrag_ext(z, view, pt, modifiers);
}

static void sliders_m_range(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_sliders* zx = (ui_sliders*)z;
    zx->range = argv[0].a_w.w_float;

    GuiFactory<ui_sliders>::m_set(z, s, zx->val_list_size, zx->val_list);

    GuiFactory<ui_sliders>::ws_redraw(z);
}

static void sliders_m_shift(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_sliders* zx = (ui_sliders*)z;
    zx->shift = argv[0].a_w.w_float;

    GuiFactory<ui_sliders>::ws_redraw(z);
}

static void sliders_m_select(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_sliders* zx = (ui_sliders*)z;
    zx->sel_idx = (int)argv[0].a_w.w_float;

    GuiFactory<ui_sliders>::ws_redraw(z);
}

static void sliders_m_auto_range(t_object* z, t_symbol* s, int argc, t_atom* argv)
{
    ui_sliders* zx = (ui_sliders*)z;
    zx->auto_range = (argv[0].a_w.w_float > 0);

    GuiFactory<ui_sliders>::m_set(z, s, zx->val_list_size, zx->val_list);
    GuiFactory<ui_sliders>::ws_redraw(z);
}

static void ui_sl_getdrawparams(ui_sliders* x, t_object* patcherview, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_sliders)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "150. 100.");

    CLASS_ATTR_INT                  (z, "count", 0, ui_sliders, count);
    CLASS_ATTR_DEFAULT              (z, "count", 0, "8");
    CLASS_ATTR_LABEL                (z, "count", 0, "Sliders count");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "count", 0, "8");

    CLASS_ATTR_FLOAT                (z, "shift", 0, ui_sliders, shift);
    CLASS_ATTR_DEFAULT              (z, "shift", 0, "0");
    CLASS_ATTR_LABEL                (z, "shift", 0, "shift");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift", 0, "0");

    CLASS_ATTR_FLOAT                (z, "range", 0, ui_sliders, range);
    CLASS_ATTR_DEFAULT              (z, "range", 0, "1");
    CLASS_ATTR_LABEL                (z, "range", 0, "range");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range", 0, "1");

    CLASS_ATTR_INT                  (z, "auto_range", 0, ui_sliders, auto_range);
    CLASS_ATTR_DEFAULT              (z, "auto_range", 0, "0");
    CLASS_ATTR_LABEL                (z, "auto_range", 0, "auto_range");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "auto_range", 0, "0");
    // clang-format on

    eclass_addmethod(z, (method)(sliders_m_range), ("range"), A_GIMME, 0);
    eclass_addmethod(z, (method)(sliders_m_shift), ("shift"), A_GIMME, 0);
    eclass_addmethod(z, (method)(sliders_m_select), ("select"), A_GIMME, 0);
    eclass_addmethod(z, (method)(sliders_m_auto_range), ("auto_range"), A_GIMME, 0);

    eclass_addmethod(z, (method)ui_sl_getdrawparams, "getdrawparams", A_NULL, 0);
}

UI_fun(ui_sliders)::new_ext(t_object* x, t_symbol* s, int argcl, t_atom* argv)
{
    ui_sliders* zx = asStruct(x);

    zx->out1 = outlet_new(x, &s_list);
    zx->val_list_size = 8;
    zx->val_list = (t_atom*)malloc(sizeof(t_atom) * 8);

    ((ui_sliders*)x)->draw_list_size = 8;
    ((ui_sliders*)x)->draw_list = (t_atom*)malloc(sizeof(t_atom) * 8);

    for (int i = 0; i < 8; i++) {
        ((ui_sliders*)x)->val_list[i].a_type = A_FLOAT;
        ((ui_sliders*)x)->val_list[i].a_w.w_float = 0.;

        ((ui_sliders*)x)->draw_list[i].a_type = A_FLOAT;
        ((ui_sliders*)x)->draw_list[i].a_w.w_float = 0.;
    }

    zx->range = 1;
    zx->shift = 0;

    zx->sel_idx = -1;

    zx->auto_range = 0;

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);
}

UI_fun(ui_sliders)::free_ext(t_object* z)
{
    ui_sliders* zx = asStruct(z);
    outlet_free(zx->out1);
    free(zx->val_list);
    free(zx->draw_list);

    efont_destroy(zx->txt_font);
    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);
}
}

extern "C" void setup_ui0x2esliders()
{
    ceammc_gui::GuiFactory<ui_sliders> class1;
    class1.setup("ui.sliders");
}
