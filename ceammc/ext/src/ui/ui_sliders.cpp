//
//  ui_sliders.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//

#include <algorithm>

#include "ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

using namespace ceammc;

template <typename T>
static T clip(T min, T max, T v)
{
    return std::max(min, std::min(max, v));
}

struct ui_sliders : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;

    AtomList* values_;

    t_etext* txt_min;
    t_etext* txt_max;
    t_efont* txt_font;

    t_rgba slider_color;
    t_rgba slider_color_select;

    int count;
    float range;
    float shift;

    int sel_idx;
    int auto_range;
    int show_range;

    bool _is_vertical;

public:
    void output()
    {
        values_->output(out1);
        send(values_->size(), values_->toPdData());
    }

    void setValueAt(size_t idx, float v)
    {
        if (idx < values_->size()) {
            float min = std::min(shift, shift + range);
            float max = std::max(shift, shift + range);
            values_->at(idx).setFloat(clip(min, max, v));
        }
    }

    void setNormValueAt(size_t idx, float v)
    {
        if (idx < values_->size()) {
            values_->at(idx).setFloat(range * clip(0.f, 1.f, v) + shift);
        }
    }

    size_t sliderCount() const
    {
        return values_->size();
    }

    float normValueAt(size_t idx) const
    {
        // zero division check
        if (range == 0.f)
            return 0;

        return clip(0.f, 1.f, (values_->at(idx).asFloat() - shift) / range);
    }

    void keepInRange()
    {
        if (!auto_range)
            return;

        for (size_t i = 0; i < values_->size(); i++)
            setValueAt(i, values_->at(i).asFloat());
    }
};

#pragma mark -

namespace ceammc_gui {

static const int MAX_SLIDERS_NUM = 1024;

UI_fun(ui_sliders)::wx_paint(ui_sliders* zx, t_object* view)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        // draw bins
        for (size_t i = 0; i < zx->sliderCount(); i++) {
            float v = zx->normValueAt(i);

            float xx, yy, w, h;

            if (zx->_is_vertical) {
                xx = 0;
                yy = i * rect.height / zx->sliderCount();
                w = v * rect.width;
                h = rect.height / zx->sliderCount() - 1;
            } else {
                yy = rect.height - v * rect.height;
                xx = i * rect.width / zx->sliderCount();
                h = rect.height;
                w = rect.width / zx->sliderCount() - 1;
            }

            egraphics_rectangle(g, xx, yy, w, h);
            egraphics_set_color_rgba(g, (i == zx->sel_idx) ? &zx->slider_color_select : &zx->slider_color);
            egraphics_fill(g);

            if (i == zx->sel_idx) {
                egraphics_set_line_width(g, 1);
                egraphics_rectangle(g, xx, yy, w - 1, h);
                egraphics_set_color_rgba(g, &zx->b_color_border);
                egraphics_stroke(g);
            }
        }

        // draw text
        if (zx->show_range) {
            const float xoff = 3 * ebox_getzoom(asBox(zx));
            const float yoff = 12 * ebox_getzoom(asBox(zx));

            char c_min[10];
            sprintf(c_min, "%g", zx->shift);

            char c_max[10];
            sprintf(c_max, "%g", zx->range + zx->shift);

            etext_layout_set(zx->txt_min, c_min, zx->txt_font,
                xoff, rect.height - yoff, rect.width * 2, rect.height / 2,
                ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_min, g);

            etext_layout_set(zx->txt_max, c_max, zx->txt_font,
                xoff, yoff, rect.width * 2, rect.height / 2,
                ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_max, g);
        }

        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0, 0);
}

UI_fun(ui_sliders)::m_set(ui_sliders* zx, t_symbol*, int argc, t_atom* argv)
{
    AtomList args(argc, argv);

    if (zx->auto_range) {
        Atom vmin;
        Atom vmax;
        args.range(vmin, vmax);

        zx->shift = vmin.asFloat();
        zx->range = vmax.asFloat() - zx->shift;

        *zx->values_ = args;
    } else {
        const size_t total = std::min(zx->sliderCount(), static_cast<size_t>(argc));

        for (size_t i = 0; i < total; i++) {
            zx->setValueAt(i, atom_getfloat(argv + i));
        }
    }

    ws_redraw(zx);
}

UI_fun(ui_sliders)::m_list(ui_sliders* z, t_symbol* s, int argc, t_atom* argv)
{
    m_set(z, s, argc, argv);
    z->output();
}

UI_fun(ui_sliders)::m_bang(ui_sliders* z)
{
    z->output();
}

UI_fun(ui_sliders)::wx_oksize(ui_sliders* z, t_rect* newrect)
{
    z->_is_vertical = newrect->width < newrect->height;

    newrect->width = std::max(20.f, newrect->width);
    newrect->height = std::max(20.f, newrect->height);
}

UI_fun(ui_sliders)::wx_mousedrag_ext(ui_sliders* zx, t_object*, t_pt pt, long /*modifiers*/)
{
    t_rect rect;
    zx->getRect(&rect);

    int numslider;
    float val;

    if (zx->_is_vertical) {
        numslider = static_cast<int>(floorf(pt.y / rect.height * zx->sliderCount()));
        val = pt.x / rect.width;
    } else {
        numslider = static_cast<int>(floorf(pt.x / rect.width * zx->sliderCount()));
        val = 1.f - pt.y / rect.height;
    }

    zx->setNormValueAt(numslider, val);
    zx->output();

    ws_redraw(zx);
}

UI_fun(ui_sliders)::wx_mousedown_ext(ui_sliders* z, t_object* view, t_pt pt, long modifiers)
{
    wx_mousedrag_ext(z, view, pt, modifiers);
}

static void sliders_m_select(ui_sliders* zx, t_symbol* s, int argc, t_atom* argv)
{
    zx->sel_idx = atom_getfloat(argv);
    GuiFactory<ui_sliders>::ws_redraw(zx);
}

static void sliders_m_set_at(ui_sliders* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    if (argv[0].a_type != A_FLOAT || argv[1].a_type != A_FLOAT)
        return;

    size_t pos = atom_getfloat(&argv[0]);
    t_float v = atom_getfloat(&argv[1]);

    zx->setValueAt(pos, v);
    GuiFactory<ui_sliders>::ws_redraw(zx);
}

static void sliders_m_auto_range(ui_sliders* z, t_floatarg v)
{
    z->auto_range = (v != 0.f);
    z->keepInRange();
    GuiFactory<ui_sliders>::ws_redraw(z);
}

static void ui_sl_getdrawparams(ui_sliders* x, t_object* patcherview, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

static t_pd_err slider_count_set(ui_sliders* x, t_object* attr, int ac, t_atom* av)
{
    if (!x->values_)
        return 0;

    AtomList args(ac, av);
    if (args.empty()) {
        pd_error(x, "[%s] empty list", eobj_getclassname(x)->s_name);
        return 0;
    }

    if (!args.at(0).isFloat()) {
        pd_error(x, "[%s] invalid count value", eobj_getclassname(x)->s_name);
        return 0;
    }

    int new_sz = clip(1, MAX_SLIDERS_NUM, args.at(0).asInt(1));

    x->values_->resizePad(new_sz, Atom(x->shift));
    x->count = new_sz;
    return 0;
}

static void slider_range_set(ui_sliders* x, t_object* attr, int ac, t_atom* av)
{
    if (ac < 1)
        return;

    float v = Atom(*av).asFloat(0.0001f);
    if (v == 0.f)
        v = 0.0001f;

    x->range = v;
}

UI_fun(ui_sliders)::m_preset(ui_sliders* zx, t_binbuf* b)
{
    if (!zx->values_)
        return;

    binbuf_addv(b, "s", &s_list);
    binbuf_add(b, zx->sliderCount(), zx->values_->toPdData());
}

UI_fun(ui_sliders)::wx_attr_changed_ext(ui_sliders* z, t_symbol*)
{
    ws_redraw(z);
}

#pragma mark -

UI_fun(ui_sliders)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "150. 100.");

    CLASS_ATTR_RGBA                 (z, "fgcolor", 0, ui_sliders, slider_color);
    CLASS_ATTR_LABEL                (z, "fgcolor", 0, _("Slider color"));
    CLASS_ATTR_STYLE                (z, "fgcolor", 0, "color");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "fgcolor", 0, "0.75 0.75 0.75 1.0");

    CLASS_ATTR_INT                  (z, "count", 0, ui_sliders, count);
    CLASS_ATTR_ACCESSORS            (z, "count", NULL, slider_count_set);
    CLASS_ATTR_DEFAULT              (z, "count", 0, "8");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "count", 0, "8");
    CLASS_ATTR_FILTER_MIN           (z, "count", 1);
    CLASS_ATTR_FILTER_MAX           (z, "count", MAX_SLIDERS_NUM);
    CLASS_ATTR_LABEL                (z, "count", 0, _("Sliders count"));
    CLASS_ATTR_STYLE                (z, "count", 0, "number");

    CLASS_ATTR_FLOAT                (z, "shift", 0, ui_sliders, shift);
    CLASS_ATTR_DEFAULT              (z, "shift", 0, "0");
    CLASS_ATTR_LABEL                (z, "shift", 0, _("Value shift"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift", 0, "0");
    CLASS_ATTR_STYLE                (z, "shift", 0, "number");

    CLASS_ATTR_FLOAT                (z, "range", 0, ui_sliders, range);
    CLASS_ATTR_DEFAULT              (z, "range", 0, "1");
    CLASS_ATTR_ACCESSORS            (z, "range", NULL, slider_range_set);
    CLASS_ATTR_LABEL                (z, "range", 0, _("Value range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range", 0, "1");
    CLASS_ATTR_STYLE                (z, "range", 0, "number");

    CLASS_ATTR_INT                  (z, "auto_range", 0, ui_sliders, auto_range);
    CLASS_ATTR_DEFAULT              (z, "auto_range", 0, "0");
    CLASS_ATTR_LABEL                (z, "auto_range", 0, _("Auto range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "auto_range", 0, "0");
    CLASS_ATTR_STYLE                (z, "auto_range", 0, "onoff");

    CLASS_ATTR_INT                  (z, "show_range", 0, ui_sliders, show_range);
    CLASS_ATTR_DEFAULT              (z, "show_range", 0, "1");
    CLASS_ATTR_LABEL                (z, "show_range", 0, _("Show range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "show_range", 0, "1");
    CLASS_ATTR_STYLE                (z, "show_range", 0, "onoff");
    // clang-format on

    eclass_addmethod(z, (method)(sliders_m_select), "select", A_GIMME, 0);
    eclass_addmethod(z, (method)(sliders_m_set_at), "set_at", A_GIMME, 0);
    eclass_addmethod(z, (method)ui_sl_getdrawparams, "getdrawparams", A_NULL, 0);
}

UI_fun(ui_sliders)::new_ext(ui_sliders* zx, t_symbol* s, int argcl, t_atom* argv)
{
    zx->out1 = create_outlet(zx, &s_list);

    zx->count = 8;
    zx->values_ = new AtomList();
    zx->values_->fill(0.f, zx->count);
    zx->slider_color_select = hex_to_rgba("#00CAF0");

    zx->range = 1;
    zx->shift = 0;

    zx->sel_idx = -1;
    zx->auto_range = false;

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);
}

UI_fun(ui_sliders)::free_ext(ui_sliders* zx)
{
    outlet_free(zx->out1);

    efont_destroy(zx->txt_font);
    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);

    delete zx->values_;
}
}

extern "C" void setup_ui0x2esliders()
{
    ceammc_gui::GuiFactory<ui_sliders> class1;
    class1.use_presets(true);
    class1.setup("ui.sliders");
}
