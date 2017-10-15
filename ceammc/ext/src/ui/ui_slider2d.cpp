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

#include "ceammc_gui.h"

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

    int show_range;

public:
    float left() const { return shift_x; }
    float right() const { return shift_x + range_x; }
    float top() const { return shift_y; }
    float bottom() const { return shift_y + range_y; }

    bool inXRange(t_float v) const
    {
        return std::min(left(), right()) <= v && v <= std::max(left(), right());
    }

    bool inYRange(t_float v) const
    {
        return std::min(top(), bottom()) <= v && v <= std::max(top(), bottom());
    }

    float xValue() const { return _posx * range_x + shift_x; }
    float yValue() const { return _posy * range_y + shift_y; }

    void output()
    {
        atom_setfloat(&out_list[0], xValue());
        atom_setfloat(&out_list[1], yValue());

        outlet_list(out1, &s_list, 2, out_list);
        send(2, out_list);
    }

    void setPos(const t_pt& mousePos)
    {
        _posx = clip(0.f, 1.f, mousePos.x / width());
        _posy = clip(0.f, 1.f, mousePos.y / height());
    }

    void setX(t_float xVal)
    {
        if (range_x != 0.f && inXRange(xVal))
            _posx = (xVal - shift_x) / range_x;
    }

    void setY(t_float yVal)
    {
        if (range_y != 0.f && inYRange(yVal))
            _posy = (yVal - shift_y) / range_y;
    }

    void set(t_float xVal, t_float yVal)
    {
        setX(xVal);
        setY(yVal);
    }
};

namespace ceammc_gui {

static const float KNOB_MIN_SIZE = 5.f;
static const float KNOB_MAX_SIZE = 20.f;
static const float KNOB_RATIO = 0.1f;
static const float KNOB_BORDER_WIDTH = 1.f;
static t_symbol* KNOB_FILL = gensym("#C0C0C0");
static t_symbol* KNOB_BORDER = gensym("#707070");
static t_symbol* KNOB_FILL_ACTIVE = gensym("#003070");
static t_symbol* KNOB_BORDER_ACTIVE = gensym("#00C0FF");
static const float GUIDE_LINE_WIDTH = 0.5f;
static t_symbol* GUIDE_LINE_COLOR = gensym("#00C0F0");

static t_symbol* SEL_POS = gensym("pos");
static t_symbol* SEL_POS_X = gensym("x");
static t_symbol* SEL_POS_Y = gensym("y");

static void ui_slider2d_value(ui_slider2d* z, t_symbol* s, int argc, t_atom* argv);

static t_symbol* knobBorderColor(int mouseState)
{
    return mouseState == 0 ? KNOB_BORDER : KNOB_BORDER_ACTIVE;
}

static t_symbol* knobFillColor(int mouseState)
{
    return mouseState == 0 ? KNOB_FILL : KNOB_FILL_ACTIVE;
}

UI_fun(ui_slider2d)::wx_paint(ui_slider2d* zx, t_object* view)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        const float xx = zx->_posx * rect.width;
        const float yy = zx->_posy * rect.height;

        // draw lines guides
        egraphics_set_line_width(g, GUIDE_LINE_WIDTH);
        egraphics_set_color_hex(g, GUIDE_LINE_COLOR);

        egraphics_line(g, xx, 0, xx, rect.height);
        egraphics_line(g, 0, yy, rect.width, yy);

        egraphics_stroke(g);

        // draw knob
        const float knobsize = clip<float>(KNOB_MIN_SIZE, KNOB_MAX_SIZE,
            std::min(rect.height, rect.width) * KNOB_RATIO);

        // knob border
        egraphics_set_line_width(g, KNOB_BORDER_WIDTH);

        egraphics_rectangle(g, xx - 0.5f * knobsize, yy - 0.5f * knobsize, knobsize, knobsize);

        egraphics_set_color_hex(g, knobFillColor(zx->mouse_dn));
        egraphics_fill_preserve(g); // use path twice
        egraphics_set_color_hex(g, knobBorderColor(zx->mouse_dn));
        egraphics_stroke(g);

        if (zx->show_range != 0) {
            char buf[30];

            const float xoff = 3 * ebox_getzoom(asBox(zx));
            const float yoff = 12 * ebox_getzoom(asBox(zx));

            snprintf(buf, 30, "X: [%g..%g]", zx->left(), zx->right());
            etext_layout_set(zx->txt_max, buf, zx->txt_font,
                xoff, yoff, rect.width * 2, rect.height / 2,
                ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_max, g);

            snprintf(buf, 30, "Y: [%g..%g]", zx->top(), zx->bottom());
            etext_layout_set(zx->txt_min, buf, zx->txt_font,
                rect.width - xoff, rect.height - yoff, rect.width, rect.height / 2,
                ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_min, g);
        }

        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0, 0);
}

UI_fun(ui_slider2d)::wx_mousedrag_ext(ui_slider2d* zx, t_object* view, t_pt pt, long modifiers)
{
    zx->setPos(pt);
    ws_redraw(zx);
    zx->output();
}

UI_fun(ui_slider2d)::wx_mousedown_ext(ui_slider2d* zx, t_object* view, t_pt pt, long modifiers)
{
    zx->setPos(pt);
    ws_redraw(zx);
    zx->output();
}

UI_fun(ui_slider2d)::wx_mouseup_ext(ui_slider2d* zx, t_object*, t_pt pt, long)
{
    zx->setPos(pt);
    ws_redraw(zx);
    zx->output();
}

static void ui_s2_getdrawparams(ui_slider2d* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

static void ui_slider2d_xpos(ui_slider2d* z, t_floatarg x)
{
    z->setX(x);
    GuiFactory<ui_slider2d>::ws_redraw(z);
}

static void ui_slider2d_ypos(ui_slider2d* z, t_floatarg y)
{
    z->setY(y);
    GuiFactory<ui_slider2d>::ws_redraw(z);
}

UI_fun(ui_slider2d)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "100. 100.");

    CLASS_ATTR_INT                  (z, "show_range", 0, ui_slider2d, show_range);
    CLASS_ATTR_DEFAULT              (z, "show_range", 0, "1");
    CLASS_ATTR_LABEL                (z, "show_range", 0, _("Show range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "show_range", 0, "1");
    CLASS_ATTR_STYLE                (z, "show_range", 0, "onoff");

    CLASS_ATTR_FLOAT                (z, "shift_x", 0, ui_slider2d, shift_x);
    CLASS_ATTR_DEFAULT              (z, "shift_x", 0, "-1");
    CLASS_ATTR_LABEL                (z, "shift_x", 0, _("Leftmost value"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift_x", 0, "-1");
    CLASS_ATTR_STYLE                (z, "shift_x", 0, "number");

    CLASS_ATTR_FLOAT                (z, "range_x", 0, ui_slider2d, range_x);
    CLASS_ATTR_DEFAULT              (z, "range_x", 0, "2");
    CLASS_ATTR_LABEL                (z, "range_x", 0, _("Horizontal range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range_x", 0, "2");
    CLASS_ATTR_STYLE                (z, "range_x", 0, "number");
    CLASS_ATTR_FILTER_MIN           (z, "range_x", 0.0001f);

    CLASS_ATTR_FLOAT                (z, "shift_y", 0, ui_slider2d, shift_y);
    CLASS_ATTR_DEFAULT              (z, "shift_y", 0, "-1");
    CLASS_ATTR_LABEL                (z, "shift_y", 0, _("Topmost value"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "shift_y", 0, "-1");
    CLASS_ATTR_STYLE                (z, "shift_y", 0, "number");

    CLASS_ATTR_FLOAT                (z, "range_y", 0, ui_slider2d, range_y);
    CLASS_ATTR_DEFAULT              (z, "range_y", 0, "2");
    CLASS_ATTR_LABEL                (z, "range_y", 0, _("Vertical range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "range_y", 0, "2");
    CLASS_ATTR_STYLE                (z, "range_y", 0, "number");
    CLASS_ATTR_FILTER_MIN           (z, "range_y", 0.0001f);
    // clang-format on

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_s2_getdrawparams), "getdrawparams", A_NULL, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_slider2d_value), "set", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_slider2d_xpos), "set_x", A_DEFFLOAT, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_slider2d_ypos), "set_y", A_DEFFLOAT, 0);
}

UI_fun(ui_slider2d)::new_ext(ui_slider2d* zx, t_symbol*, int, t_atom*)
{
    zx->out1 = create_outlet(zx, &s_list);

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);

    // default position in center
    zx->_posx = 0.5;
    zx->_posy = 0.5;
}

UI_fun(ui_slider2d)::free_ext(ui_slider2d* zx)
{
    outlet_free(zx->out1);

    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);
    efont_destroy(zx->txt_font);
}

UI_fun(ui_slider2d)::wx_attr_changed_ext(ui_slider2d* z, t_symbol*)
{
    ws_redraw(z);
}

UI_fun(ui_slider2d)::m_bang(ui_slider2d* zx)
{
    zx->output();
}

UI_fun(ui_slider2d)::m_preset(ui_slider2d* zx, t_binbuf* b)
{
    binbuf_addv(b, "sff", &s_list, zx->xValue(), zx->yValue());
}

void ui_slider2d_value(ui_slider2d* zx, t_symbol* s, int argc, t_atom* argv)
{
    if (argc != 2) {
        pd_error(zx, "[ui.slider2d] invalid list given: X, Y values are expected");
        return;
    }

    if (zx->range_x == 0.f || zx->range_y == 0.f) {
        pd_error(zx, "[ui.slider2d] invalid range values: %g - %g",
            static_cast<double>(zx->range_x),
            static_cast<double>(zx->range_y));

        return;
    }

    t_float x = atom_getfloat(&argv[0]);
    t_float y = atom_getfloat(&argv[1]);
    zx->set(x, y);

    GuiFactory<ui_slider2d>::ws_redraw(zx);
}

UI_fun(ui_slider2d)::m_list(ui_slider2d* zx, t_symbol* s, int argc, t_atom* argv)
{
    ui_slider2d_value(zx, s, argc, argv);
    zx->output();
}

UI_fun(ui_slider2d)::wx_oksize(ui_slider2d* z, t_rect* newrect)
{
    newrect->height = std::max(30.f, newrect->height);
    newrect->width = std::max(30.f, newrect->width);
}
}

extern "C" void setup_ui0x2eslider2d()
{
    ceammc_gui::GuiFactory<ui_slider2d> c;
    c.use_presets(true);
    c.setup("ui.slider2d");
}
