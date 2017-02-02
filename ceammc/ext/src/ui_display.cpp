//
//  ui_display.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 21/12/16.
//
//

#include <cmath>
#include <cstdio>

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "lib/ceammc_gui.h"

using namespace ceammc;

struct ui_display : public ceammc_gui::BaseGuiObject {
    t_ebox x_gui;

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

    t_rgba b_color_background;
    t_rgba b_color_border;
};

namespace ceammc_gui {

static t_symbol* COLOR_LIST_TYPE = gensym("#00A0C0");
static t_symbol* COLOR_FLOAT_TYPE = gensym("#C000A0");
static t_symbol* COLOR_DEFAULT_TYPE = gensym("#909090");

static inline t_symbol* msg_color(t_symbol* s_type)
{
    if (s_type == &s_list)
        return COLOR_LIST_TYPE;

    if (s_type == &s_float)
        return COLOR_FLOAT_TYPE;

    return COLOR_DEFAULT_TYPE;
}

UI_fun(ui_display)::wx_paint(t_object* z, t_object* /*view*/)
{
    t_symbol* bgl = BG_LAYER;

    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    t_elayer* g = ebox_start_layer(asBox(z), bgl, rect.width, rect.height);

    if (g) {
        ui_display* zx = asStruct(z);

        if (zx->show_type) {
            egraphics_set_color_hex(g, msg_color(zx->s_type));

            egraphics_rectangle(g, 0, 0, 45, rect.height);
            egraphics_fill(g);

            egraphics_set_color_rgba(g, &zx->b_color_background);
            if (zx->bang)
                egraphics_set_color_hex(g, COLOR_ACTIVE);
            egraphics_rectangle(g, 45, 0, rect.width, rect.height);
            egraphics_fill(g);

            etext_layout_set(zx->txt_type, zx->s_type->s_name, zx->txt_font, 3, rect.height / 1, 45, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_set(zx->txt_val, zx->s_value->c_str(), zx->txt_font, 48, rect.height / 1, rect.width - 50, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_type, g);
            etext_layout_draw(zx->txt_val, g);
        } else {
            egraphics_set_color_rgba(g, &zx->b_color_background);
            if (zx->bang)
                egraphics_set_color_hex(g, COLOR_ACTIVE);
            egraphics_rectangle(g, 0, 0, rect.width, rect.height);
            egraphics_fill(g);

            etext_layout_set(zx->txt_val, zx->s_value->c_str(), zx->txt_font, 3, rect.height / 1, rect.width - 5, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);

            etext_layout_draw(zx->txt_val, g);
        }

        ebox_end_layer(asBox(z), bgl);
    }

    ebox_paint_layer(asBox(z), bgl, 0., 0.);
}

UI_fun(ui_display)::m_anything(t_object* z, t_symbol* s, int argc, t_atom* argv)

{
    ui_display* zx = asStruct(z);
    zx->s_type = s;
    (*zx->s_value) = to_string(AtomList(argc, argv));

    if (zx->show_bang) {
        zx->bang = true;
        clock_delay(zx->t_c, 100);
    }

    if (zx->auto_size) {
        float w = (*zx->s_value).size() * 8 + (zx->show_type * 50) + 7;
        float h = int(w / 250) * 15 + 15;
        w = (w > 250) ? 250 : w;

        zx->x_gui.b_rect.width = w;
        zx->x_gui.b_rect.height = h;

        AtomList argv;
        argv.append(Atom(w));
        argv.append(Atom(h));
        eobj_attr_setvalueof(zx, gensym("size"), 2, argv.toPdData());
    }

    ceammc_gui::GuiFactory<ceammc_gui::BaseGuiObject>::ws_redraw(z);
}

UI_fun(ui_display)::m_list(t_object* z, t_symbol* /*s*/, int argc, t_atom* argv)
{
    ceammc_gui::GuiFactory<ui_display>::m_anything(z, &s_list, argc, argv);
}

UI_fun(ui_display)::m_float(t_object* z, t_float f)
{
    AtomList list1 = AtomList(Atom(f));
    ceammc_gui::GuiFactory<ui_display>::m_anything(z, &s_float, 1, list1.toPdData());
}

UI_fun(ui_display)::m_bang(t_object* z, t_symbol* /*s*/, int /*argc*/, t_atom* /*argv*/)
{
    ceammc_gui::GuiFactory<ui_display>::m_anything(z, &s_bang, 0, 0);
}

void display_clock(t_object* z)
{
    ui_display* zx = reinterpret_cast<ui_display*>(z);
    zx->bang = false;
    ceammc_gui::GuiFactory<ceammc_gui::BaseGuiObject>::ws_redraw(z);
}

UI_fun(ui_display)::wx_oksize(t_object* /*z*/, t_rect* newrect)
{
    newrect->height = floorf(newrect->height / 15.f) * 15.f;
    newrect->height = (newrect->height > 15) ? newrect->height : 15;
}

UI_fun(ui_display)::new_ext(t_object* z, t_symbol* /*s*/, int /*argcl*/, t_atom* /*argv*/)
{
    ui_display* zx = asStruct(z);

    zx->s_value = new std::string;
    zx->s_type = &s_anything;

    zx->txt_val = etext_layout_create();
    zx->txt_type = etext_layout_create();

    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE);

    zx->t_c = clock_new(zx, reinterpret_cast<t_method>(display_clock));

    zx->show_bang = 1;
    zx->show_type = 0;
}

UI_fun(ui_display)::free_ext(t_object* z)
{
    ui_display* zx = asStruct(z);
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

UI_fun(ui_display)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT              (z, "size", 0, "150. 15.");

    CLASS_ATTR_INT                  (z, "display_events", 0, ui_display, show_bang);
    CLASS_ATTR_DEFAULT              (z, "display_events", 0, "1");
    CLASS_ATTR_LABEL                (z, "display_events", 0, "Display events");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "display_events", 0, "1");
    CLASS_ATTR_STYLE                (z, "display_events", 0, "onoff");

    CLASS_ATTR_INT                  (z, "display_type", 0, ui_display, show_type);
    CLASS_ATTR_DEFAULT              (z, "display_type", 0, "0");
    CLASS_ATTR_LABEL                (z, "display_type", 0, "Display type");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "display_type", 0, "0");
    CLASS_ATTR_STYLE                (z, "display_type", 0, "onoff");

    CLASS_ATTR_INT                  (z, "auto_size", 0, ui_display, auto_size);
    CLASS_ATTR_DEFAULT              (z, "auto_size", 0, "0");
    CLASS_ATTR_LABEL                (z, "auto_size", 0, "Auto size");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "auto_size", 0, "1");
    CLASS_ATTR_STYLE                (z, "auto_size", 0, "onoff");

    CLASS_ATTR_RGBA                 (z, "bgcolor", 0, ui_display, b_color_background);
    CLASS_ATTR_LABEL                (z, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (z, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bgcolor", 0, "0.93 0.93 0.93 1.");
    CLASS_ATTR_STYLE                (z, "bgcolor", 0, "color");

    CLASS_ATTR_RGBA                 (z, "bdcolor", 0, ui_display, b_color_border);
    CLASS_ATTR_LABEL                (z, "bdcolor", 0, "Border Color");
    CLASS_ATTR_ORDER                (z, "bdcolor", 0, "2");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "bdcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (z, "bdcolor", 0, "color");

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_disp_getdrawparams), "getdrawparams", A_NULL, 0);
    // clang-format on
}
}

extern "C" void setup_ui0x2edisplay()
{
    ceammc_gui::GuiFactory<ui_display> class1;
    class1.setup("ui.display");
}
