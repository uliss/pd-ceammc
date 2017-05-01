//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <stdio.h>

#include "ceammc_gui.h"
#include "ui_link_tcl.h"

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

static const t_rgba LINK_COLOR = hex_to_rgba("#00A0C0");
static const t_rgba LINK_COLOR_HOVER = hex_to_rgba("#FF0080");

static t_symbol* LINK_FONT = gensym("Menlo");

UI_fun(ui_link)::wx_paint(ui_link* zx, t_object*)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        etext_layout_settextcolor(zx->t_e, &zx->link_color);
        etext_layout_set(zx->t_e, zx->title->s_name, zx->t_ef, 3,
            rect.height, rect.width, rect.height,
            ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);

        etext_layout_draw(zx->t_e, g);
        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0., 0.);
}

UI_fun(ui_link)::wx_mousedown_ext(ui_link* zx, t_object*, t_pt, long)
{
    sys_vgui("ceammclink_open {%s} {%s}\n", zx->link->s_name, zx->x_dirsym->s_name);
}

UI_fun(ui_link)::wx_mouseenter_ext(ui_link* zx, t_object*, t_pt, long)
{
    zx->link_color = LINK_COLOR_HOVER;
    ebox_invalidate_layer(asBox(zx), BG_LAYER);
    ws_redraw(zx);
}

UI_fun(ui_link)::wx_mouseleave_ext(ui_link* zx, t_object*, t_pt, long)
{
    zx->link_color = LINK_COLOR;
    ebox_invalidate_layer(asBox(zx), BG_LAYER);
    ws_redraw(zx);
}

static void link_getdrawparams(ui_link* x, t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_bordercolor = x->b_color_background;
    params->d_boxfillcolor = x->b_color_background;
    params->d_cornersize = 0;
}

static size_t text_width(t_symbol* txt, int sz)
{
    const size_t len = strlen(txt->s_name);
    const size_t char_wd = static_cast<size_t>(sys_fontwidth(sz));
    int corr = 0;

#ifdef __APPLE__
    if (len < 6)
        corr = 3;
    if (len > 16)
        corr = -3;
#endif

    return char_wd * len + corr;
}

UI_fun(ui_link)::new_ext(ui_link* zx, t_symbol*, int, t_atom*)
{
    // fixed size
    asBox(zx)->b_flags = EBOX_GROWNO;

    zx->t_e = etext_layout_create();
    zx->t_ef = efont_create(LINK_FONT, FONT_STYLE, FONT_WEIGHT, FONT_SIZE);

    zx->x_canvas = canvas_getcurrent();
    zx->x_dirsym = canvas_getdir(zx->x_canvas);

    if (zx->title) {
        size_t w = text_width(zx->title, FONT_SIZE) * ebox_getzoom(asBox(zx));
        t_atom data[2];
        atom_setfloat(&data[0], w);
        atom_setfloat(&data[1], 0);
        zx->b_box.b_rect.width = w;
        eobj_attr_setvalueof(zx, gensym("size"), 2, data);
    }

    zx->link_color = hex_to_rgba("#00A0C0");
}

UI_fun(ui_link)::free_ext(ui_link* zx)
{
    etext_layout_destroy(zx->t_e);
    efont_destroy(zx->t_ef);
}

UI_fun(ui_link)::wx_oksize(ui_link* zx, t_rect* newrect)
{
    float w = 40;
    if (zx->title)
        w = text_width(zx->title, FONT_SIZE) * ebox_getzoom(asBox(zx));

    newrect->width = pd_clip_min(w, 20);
    newrect->height = ebox_fontheight(asBox(zx));
}

UI_fun(ui_link)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_INVISIBLE            (z, PROP_BACKGROUND_COLOR, 0);
    CLASS_ATTR_INVISIBLE            (z, PROP_BORDER_COLOR, 0);
    CLASS_ATTR_INVISIBLE            (z, "receive", 0);
    CLASS_ATTR_INVISIBLE            (z, "send", 0);
    CLASS_ATTR_INVISIBLE            (z, "size", 0);

    CLASS_ATTR_DEFAULT              (z, "size", 0, "120. 15.");
    CLASS_ATTR_DEFAULT              (z, PROP_BACKGROUND_COLOR, 0, "1.0 1.0 1.0 1.0");

    CLASS_ATTR_SYMBOL               (z, "url", 0, ui_link, link);
    CLASS_ATTR_DEFAULT              (z, "url", 0, "<no link>");
    CLASS_ATTR_LABEL                (z, "url", 0, _("URL"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "url", 0, "<no link>");

    CLASS_ATTR_SYMBOL               (z, "title", 0, ui_link, title);
    CLASS_ATTR_DEFAULT              (z, "title", 0, "<no title>");
    CLASS_ATTR_LABEL                (z, "title", 0, _("Title"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "title", 0, "<no title>");
    // clang-format off

    eclass_addmethod(z, (method)link_getdrawparams, "getdrawparams", A_NULL, 0);
}

UI_fun(ui_link)::wx_attr_changed_ext(ui_link* zx, t_symbol* attr)
{
    if (attr == gensym("title")) {
        size_t w = text_width(zx->title, FONT_SIZE) * ebox_getzoom(asBox(zx));
        t_atom data[2];
        atom_setfloat(&data[0], w);
        atom_setfloat(&data[1], 0);

        zx->b_box.b_rect.width = w;
        eobj_attr_setvalueof(zx, gensym("size"), 2, data);
        ws_redraw(zx);
    }
}

}

extern "C" void setup_ui0x2elink()
{
    uilink_tcl_init();
    ceammc_gui::GuiFactory<ui_link> class1;
    class1.setup_noin("ui.link");
}
