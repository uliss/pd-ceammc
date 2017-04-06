//
//  ui_display.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 21/12/16.
//
//

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "ceammc_gui.h"

using namespace ceammc;

struct ui_display : public ceammc_gui::BaseGuiObject {
    t_etext* txt_type;
    t_etext* txt_val;
    t_efont* txt_font;

    t_clock* t_c;

    bool bang;

    std::string* s_value;
    t_symbol* s_type;

    int show_type;
    int show_bang;

    int auto_size;
};

namespace ceammc_gui {

static t_symbol* COLOR_LIST_TYPE = gensym("#00A0C0");
static t_symbol* COLOR_FLOAT_TYPE = gensym("#C000A0");
static t_symbol* COLOR_SYMBOL_TYPE = gensym("#A0C000");
static t_symbol* COLOR_DEFAULT_TYPE = gensym("#909090");
static const int TYPE_WIDTH = 45;
static const int TEXT_XPAD = 3;
static const int TEXT_YPAD = 2;

#pragma mark setup

static inline t_symbol* msg_color(t_symbol* s_type)
{
    if (s_type == &s_list)
        return COLOR_LIST_TYPE;
    else if (s_type == &s_float)
        return COLOR_FLOAT_TYPE;
    else if (s_type == &s_symbol)
        return COLOR_SYMBOL_TYPE;
    else
        return COLOR_DEFAULT_TYPE;
}

static void draw_message_type(ui_display* zx, t_elayer* g, float x, float y, float width, float height)
{
    egraphics_set_color_hex(g, msg_color(zx->s_type));
    egraphics_rectangle(g, x, y, width, height);
    egraphics_fill(g);

    etext_layout_set(zx->txt_type, zx->s_type->s_name, zx->txt_font,
        x + TEXT_XPAD, y + TEXT_YPAD, width - TEXT_XPAD, height,
        ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
    etext_layout_draw(zx->txt_type, g);
}

#pragma mark ui

static void draw_msg_value(ui_display* zx, t_elayer* g, float x, float y, float width, float height)
{
    if (zx->bang)
        egraphics_set_color_hex(g, GuiFactory<BaseGuiObject>::COLOR_ACTIVE);
    else
        egraphics_set_color_rgba(g, &zx->b_color_background);

    egraphics_rectangle(g, x, y, width, height);
    egraphics_fill(g);

    etext_layout_set(zx->txt_val, zx->s_value->c_str(), zx->txt_font,
        x + TEXT_XPAD, y + TEXT_YPAD, width - TEXT_XPAD, height,
        ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
    etext_layout_draw(zx->txt_val, g);
}

UI_fun(ui_display)::wx_paint(ui_display* zx, t_object* /*view*/)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        if (zx->show_type) {
            int zoom = ebox_getzoom(asBox(zx));
            draw_message_type(zx, g, 0, 0, TYPE_WIDTH * zoom, rect.height);
            draw_msg_value(zx, g, TYPE_WIDTH * zoom, 0, rect.width, rect.height);
        } else {
            draw_msg_value(zx, g, 0, 0, rect.width, rect.height);
        }

        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0., 0.);
}

UI_fun(ui_display)::m_anything(ui_display* zx, t_symbol* s, int argc, t_atom* argv)
{
    zx->s_type = s;
    (*zx->s_value) = to_string(AtomList(argc, argv));

    if (zx->show_bang) {
        zx->bang = true;
        clock_delay(zx->t_c, 100);
    }

    if (zx->auto_size) {
        float w = zx->s_value->size() * 8 + (zx->show_type * 50) + 7;
        float h = int(w / 250) * 15 + 15;
        w = std::min(std::max(w, 20.f), 250.f); // 20 <= w <= 250

        h *= ebox_getzoom(asBox(zx));
        w *= ebox_getzoom(asBox(zx));

        zx->b_box.b_rect.width = w;
        zx->b_box.b_rect.height = h;

        ebox_invalidate_layer(asBox(zx), BG_LAYER);
        ebox_redraw(asBox(zx));
        ebox_notify(asBox(zx), s_size, 0, 0, 0);
    }

    ws_redraw(zx);
}

UI_fun(ui_display)::m_list(ui_display* z, t_symbol*, int argc, t_atom* argv)
{
    m_anything(z, &s_list, argc, argv);
}

UI_fun(ui_display)::m_symbol(ui_display* z, t_symbol* s)
{
    t_atom a;
    atom_setsym(&a, s);
    m_anything(z, &s_symbol, 1, &a);
}

UI_fun(ui_display)::m_float(ui_display* z, t_float f)
{
    AtomList list1 = AtomList(Atom(f));
    m_anything(z, &s_float, 1, list1.toPdData());
}

UI_fun(ui_display)::m_bang(ui_display* z)
{
    m_anything(z, &s_bang, 0, 0);
}

void display_clock(t_object* z)
{
    ui_display* zx = reinterpret_cast<ui_display*>(z);
    zx->bang = false;
    GuiFactory<BaseGuiObject>::ws_redraw(zx);
}

UI_fun(ui_display)::wx_oksize(ui_display* zx, t_rect* newrect)
{
    float min_width = 40;
    if (zx->show_type != 0)
        min_width += TYPE_WIDTH;

    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 18);
}

#pragma mark setup

UI_fun(ui_display)::new_ext(ui_display* zx, t_symbol* /*s*/, int /*argcl*/, t_atom* /*argv*/)
{
    zx->s_value = new std::string;
    zx->s_type = gensym("...");

    zx->txt_val = etext_layout_create();
    zx->txt_type = etext_layout_create();

    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE);

    zx->t_c = clock_new(zx, reinterpret_cast<t_method>(display_clock));

    zx->show_bang = 1;
    zx->show_type = 0;
}

UI_fun(ui_display)::free_ext(ui_display* zx)
{
    delete zx->s_value;

    efont_destroy(zx->txt_font);

    clock_unset(zx->t_c);
    clock_free(zx->t_c);

    etext_layout_destroy(zx->txt_type);
    etext_layout_destroy(zx->txt_val);
}

static void ui_disp_getdrawparams(ui_display* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_display)::wx_attr_changed_ext(ui_display* z, t_symbol*)
{
    ws_redraw(z);
}

UI_fun(ui_display)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "150. 18.");
    CLASS_ATTR_INVISIBLE            (z, "send", 0);

    CLASS_ATTR_INT                  (z, "display_events", 0, ui_display, show_bang);
    CLASS_ATTR_DEFAULT              (z, "display_events", 0, "1");
    CLASS_ATTR_LABEL                (z, "display_events", 0, _("Display events"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "display_events", 0, "1");
    CLASS_ATTR_STYLE                (z, "display_events", 0, "onoff");

    CLASS_ATTR_INT                  (z, "display_type", 0, ui_display, show_type);
    CLASS_ATTR_DEFAULT              (z, "display_type", 0, "0");
    CLASS_ATTR_LABEL                (z, "display_type", 0, _("Display type"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "display_type", 0, "0");
    CLASS_ATTR_STYLE                (z, "display_type", 0, "onoff");

    CLASS_ATTR_INT                  (z, "auto_size", 0, ui_display, auto_size);
    CLASS_ATTR_DEFAULT              (z, "auto_size", 0, "0");
    CLASS_ATTR_LABEL                (z, "auto_size", 0, _("Auto size"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "auto_size", 0, "1");
    CLASS_ATTR_STYLE                (z, "auto_size", 0, "onoff");

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_disp_getdrawparams), "getdrawparams", A_NULL, 0);
    // clang-format on
}
}

extern "C" void setup_ui0x2edisplay()
{
    ceammc_gui::GuiFactory<ui_display> class1;
    class1.setup("ui.display");
}
