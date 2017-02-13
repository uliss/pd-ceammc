//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

#include "m_imp.h" /* FIXME need access to c_externdir... */

struct ui_link : public ceammc_gui::BaseGuiObject {
    t_etext* t_e;
    t_efont* t_ef;

    t_symbol* link;
    t_symbol* title;

    t_canvas* x_canvas;
    t_symbol* x_dirsym;

    t_rgba link_color;
};

namespace ceammc_gui {

UI_fun(ui_link)::wx_paint(t_object* z, t_object*)
{
    t_rect rect;
    ebox_get_rect_for_view(asBox(z), &rect);

    t_elayer* g = ebox_start_layer(asBox(z), BG_LAYER, rect.width, rect.height);

    ui_link* zx = asStruct(z);

    if (g) {
        etext_layout_settextcolor(zx->t_e, &zx->link_color);
        etext_layout_set(zx->t_e, zx->title->s_name, zx->t_ef, 3, rect.height, rect.width, rect.height, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);

        etext_layout_draw(zx->t_e, g);
        ebox_end_layer(asBox(z), BG_LAYER);
    }

    ebox_paint_layer(asBox(z), BG_LAYER, 0., 0.);
}

UI_fun(ui_link)::wx_mousedown_ext(t_object* z, t_object*, t_pt, long)
{
    ui_link* zx = asStruct(z);
    sys_vgui("pddplink_open {%s} {%s}\n", zx->link->s_name, zx->x_dirsym);
}

UI_fun(ui_link)::wx_mouseenter(t_object* z, t_object*, t_pt, long)
{
    ui_link* zx = asStruct(z);
    zx->t_ef->c_weight = gensym("bold");
    ws_redraw(z);
}

UI_fun(ui_link)::wx_mouseleave(t_object* z, t_object*, t_pt, long)
{
    ui_link* zx = asStruct(z);
    zx->t_ef->c_weight = FONT_WEIGHT;
    ws_redraw(z);
}

static void link_getdrawparams(t_object*, t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_bordercolor.red = 1;
    params->d_bordercolor.green = 1;
    params->d_bordercolor.blue = 1;
    params->d_cornersize = 0;
}

UI_fun(ui_link)::new_ext(t_object* z, t_symbol*, int, t_atom*)
{
    ui_link* zx = asStruct(z);

    zx->t_e = etext_layout_create();
    zx->t_ef = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE);

    zx->x_canvas = canvas_getcurrent();
    zx->x_dirsym = canvas_getdir(zx->x_canvas);

    zx->link_color = hex_to_rgba("#00A0C0");
}

UI_fun(ui_link)::free_ext(t_object* z)
{
    ui_link* zx = asStruct(z);
    etext_layout_destroy(zx->t_e);
    efont_destroy(zx->t_ef);
}

UI_fun(ui_link)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_INVISIBLE            (z, "bgcolor", 0);
    CLASS_ATTR_INVISIBLE            (z, "bdcolor", 0);
    CLASS_ATTR_INVISIBLE            (z, "receive", 0);
    CLASS_ATTR_INVISIBLE            (z, "send", 0);

    CLASS_ATTR_DEFAULT              (z, "size", 0, "120. 15.");

    CLASS_ATTR_SYMBOL               (z, "link", 0, ui_link, link);
    CLASS_ATTR_DEFAULT              (z, "link", 0, "<no link>");
    CLASS_ATTR_LABEL                (z, "link", 0, "Link");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "link", 0, "<no link>");

    CLASS_ATTR_SYMBOL               (z, "title", 0, ui_link, title);
    CLASS_ATTR_DEFAULT              (z, "title", 0, "<no title>");
    CLASS_ATTR_LABEL                (z, "title", 0, "Title");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "title", 0, "<no title>");
    // clang-format off

    eclass_addmethod(z, (method)link_getdrawparams, "getdrawparams", A_NULL, 0);
}

UI_fun(ui_link)::wx_attr_changed_ext(t_object* z, t_symbol* attr)
{
    if (attr == gensym("title")) {

        ws_redraw(z);
    }
}

}

extern "C" void setup_ui0x2elink()
{
    ceammc_gui::GuiFactory<ui_link> class1;
    class1.setup_noin("ui.link");
}
