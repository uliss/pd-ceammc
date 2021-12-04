/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ebox.h"
#include "eclass.h"
#include "egraphics.h"
#include "eobj.h"
#include "g_style.h"

#include "ceammc.h"

#include <algorithm>
#include <array>
#include <boost/algorithm/string.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <string>
#include <tuple>

t_symbol* ceammc_realizeraute(t_canvas* cnv, t_symbol* s);
std::string ceammc_raute2dollar(const char* s);
t_symbol* ceammc_dollar2raute(const char* s);
std::string ceammc_quote_str(const std::string& str, char q = '\'');

static std::array<const char*, ECURSOR_HDOUBLE_ARROW + 1> my_cursorlist = {
    "left_ptr",
#ifdef __WIN32
    "arrow",
#else
    "center_ptr",
#endif
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "left_side",
    "exchange",
    "xterm",
    "fleur",
    "sb_v_double_arrow",
    "sb_h_double_arrow",
};

static std::array<const char*, ECAPSTYLE_SQUARE + 1> my_capstylelist = {
    "butt",
    "round",
    "projecting"
};

static std::array<const char*, EDASHSTYLE_64 + 1> my_dashstylelist = {
    "",
    "-dash .",
    "-dash -"
};

static const char* SYM_CENTER = "center";
static const char* SYM_RIGHT = "right";
static const char* SYM_LEFT = "left";
static const char* SYM_N = "n";
static const char* SYM_NE = "ne";
static const char* SYM_E = "e";
static const char* SYM_SE = "se";
static const char* SYM_S = "s";
static const char* SYM_SW = "sw";
static const char* SYM_W = "w";
static const char* SYM_NW = "nw";

static const char* SYM_DEFAULT_FONT_FAMILY = "Helvetica";
static const char* SYM_BOLD = "bold";
static const char* SYM_NORMAL = "normal";
static const char* SYM_ITALIC = "italic";
static const char* SYM_ROMAN = "roman";
static const char* SYM_ALL_PROPS = "@*";

static const char* SYM_CHECKBUTTON = "checkbutton";
static const char* SYM_MENU = "menu";
static const char* SYM_COLOR = "color";

static t_pt ebox_calc_pos(t_ebox* x, t_glist* glist);
static void ebox_create_window(t_ebox* x, t_glist* glist);
static void ebox_invalidate_all(t_ebox* x);
static void ebox_draw_border(t_ebox* x);
static void ebox_draw_iolets(t_ebox* x);
static void layers_erase(t_ebox* x);
static void ebox_erase(t_ebox* x);
static void ebox_select(t_ebox* x);
static void ebox_move(t_ebox* x);
static void ebox_attrprocess_default(t_ebox* x);
static void ebox_newzoom(t_ebox* x);
static void elayer_free_content(t_elayer& l);

static const char* anchor_to_symbol(etextanchor_flags anchor)
{
    switch (anchor) {
    case ETEXT_UP:
        return SYM_N;
    case ETEXT_UP_RIGHT:
        return SYM_NE;
    case ETEXT_RIGHT:
        return SYM_E;
    case ETEXT_DOWN_RIGHT:
        return SYM_SE;
    case ETEXT_DOWN:
        return SYM_S;
    case ETEXT_DOWN_LEFT:
        return SYM_SW;
    case ETEXT_LEFT:
        return SYM_W;
    case ETEXT_UP_LEFT:
        return SYM_NW;
    default:
        return SYM_CENTER;
    }
}

static const char* justify_to_symbol(etextjustify_flags justify)
{
    switch (justify) {
    case ETEXT_JCENTER:
        return SYM_CENTER;
    case ETEXT_JRIGHT:
        return SYM_RIGHT;
    default:
        return SYM_LEFT;
    }
}

enum LabelPosition {
    LABEL_POSITION_INNER = 0,
    LABEL_POSITION_OUTER
};

enum LabelAlign {
    LABEL_ALIGN_LEFT = 0,
    LABEL_ALIGN_CENTER,
    LABEL_ALIGN_RIGHT
};

enum LabelVAlign {
    LABEL_VALIGN_TOP = 0,
    LABEL_VALIGN_CENTER,
    LABEL_VALIGN_BOTTOM
};

enum LabelSide {
    LABEL_SIDE_LEFT = 0,
    LABEL_SIDE_TOP,
    LABEL_SIDE_RIGHT,
    LABEL_SIDE_BOTTOM
};

static void ebox_erase_label(t_ebox* x)
{
    if (ebox_isvisible(x)
        && x->b_label
        && x->b_label != s_null) {
        sys_vgui("::ceammc::ui::label_delete %s %lx %d\n",
            x->b_canvas_id->s_name, x, (int)x->label_inner);
    }
}

static LabelAlign label_align_idx(t_symbol* s)
{
    if (s == s_value_label_align_left)
        return LABEL_ALIGN_LEFT;
    else if (s == s_value_label_align_center)
        return LABEL_ALIGN_CENTER;
    else if (s == s_value_label_align_right)
        return LABEL_ALIGN_RIGHT;
    else
        return static_cast<LabelAlign>(-1);
}

static LabelVAlign label_valign_idx(t_symbol* s)
{
    if (s == s_value_label_valign_top)
        return LABEL_VALIGN_TOP;
    else if (s == s_value_label_valign_center)
        return LABEL_VALIGN_CENTER;
    else if (s == s_value_label_valign_bottom)
        return LABEL_VALIGN_BOTTOM;
    else
        return static_cast<LabelVAlign>(-1);
}

static LabelSide label_side_idx(t_symbol* s)
{
    if (s == s_value_label_side_left)
        return LABEL_SIDE_LEFT;
    else if (s == s_value_label_side_top)
        return LABEL_SIDE_TOP;
    else if (s == s_value_label_side_right)
        return LABEL_SIDE_RIGHT;
    else if (s == s_value_label_side_bottom)
        return LABEL_SIDE_BOTTOM;
    else
        return static_cast<LabelSide>(-1);
}

static const char* ebox_label_anchor(LabelPosition pos, LabelSide side, LabelAlign align, LabelVAlign valign)
{
    static const char* anchor_inner[3][3] = {
        { // valign: top
            "nw", "n", "ne" },
        { // valign: middle
            "w", "center", "e" },
        { // valign: bottom
            "sw", "s", "se" }
    };

    static const char* anchor_outer[4][3][3] = {
        { // side: left
            { // valign: top
                "nw", "n", "ne" },
            { // valign: middle
                "w", "center", "e" },
            { // valign: bottom
                "sw", "s", "se" } },
        { // side: top
            { "sw", "s", "se" }, { "sw", "s", "se" }, { "sw", "s", "se" } },
        { // side: right
            { "nw", "n", "ne" }, { "w", "center", "e" }, { "sw", "s", "se" } },
        { // side: bottom
            { "nw", "n", "ne" }, { "nw", "n", "nw" }, { "nw", "n", "ne" } }
    };

    if (pos == LABEL_POSITION_INNER)
        return anchor_inner[valign][align];
    else
        return anchor_outer[side][valign][align];
}

static std::pair<int, int> ebox_label_coord(t_ebox* x,
    LabelPosition pos, LabelSide side, LabelAlign align, LabelVAlign valign)
{
    const int MIN_MARGIN = 2;

    switch (pos) {
    case LABEL_POSITION_INNER: {
        const auto w = int(x->b_rect.width * x->b_zoom);
        const auto h = int(x->b_rect.height * x->b_zoom);
        const auto xc = int(w * 0.5);
        const auto yc = int(h * 0.5);

        const int margin_left = int((x->label_margins[0] + MIN_MARGIN) * x->b_zoom);
        const int margin_right = int(w - (x->label_margins[0] + MIN_MARGIN) * x->b_zoom);
        const int margin_top = int((x->label_margins[1] + MIN_MARGIN) * x->b_zoom);
        const int margin_bottom = int(h - (x->label_margins[1] + MIN_MARGIN) * x->b_zoom);

        switch (valign) {
        case LABEL_VALIGN_TOP: {
            switch (align) {
            case LABEL_ALIGN_LEFT:
                return std::make_pair(margin_left, margin_top);
            case LABEL_ALIGN_CENTER:
                return std::make_pair(xc, margin_top);
            case LABEL_ALIGN_RIGHT:
                return std::make_pair(margin_right, margin_top);
            }
            break;
        }
        case LABEL_VALIGN_CENTER: {
            switch (align) {
            case LABEL_ALIGN_LEFT:
                return std::make_pair(margin_left, yc);
            case LABEL_ALIGN_CENTER:
                return std::make_pair(xc, yc);
            case LABEL_ALIGN_RIGHT:
                return std::make_pair(margin_right, yc);
            }
            break;
        }
        case LABEL_VALIGN_BOTTOM: {
            switch (align) {
            case LABEL_ALIGN_LEFT:
                return std::make_pair(margin_left, margin_bottom);
            case LABEL_ALIGN_CENTER:
                return std::make_pair(xc, margin_bottom);
            case LABEL_ALIGN_RIGHT:
                return std::make_pair(margin_right, margin_bottom);
            }
        } break;
        }
    } break;
    case LABEL_POSITION_OUTER: {
        int x0 = int(x->b_rect.x);
        int y0 = int(x->b_rect.y);
        int x1 = int(x0 + x->b_rect.width * x->b_zoom);
        int y1 = int(y0 + x->b_rect.height * x->b_zoom);
        int xc = int(x0 + x->b_rect.width * x->b_zoom * 0.5);
        int yc = int(y0 + x->b_rect.height * x->b_zoom * 0.5);

        switch (side) {
        case LABEL_SIDE_LEFT: {
            const int margin_right = int(x0 - (x->label_margins[0] + MIN_MARGIN) * x->b_zoom);
            const int margin_top = int(y0 + x->label_margins[1] * x->b_zoom);
            const int margin_bottom = int(y1 - x->label_margins[1] * x->b_zoom);

            switch (valign) {
            case LABEL_VALIGN_TOP:
                return std::make_pair(margin_right, margin_top);
            case LABEL_VALIGN_CENTER:
                return std::make_pair(margin_right, yc);
            case LABEL_VALIGN_BOTTOM:
                return std::make_pair(margin_right, margin_bottom);
            }
        } break;
        case LABEL_SIDE_TOP: {
            const int margin_left = int(x0 + x->label_margins[0] * x->b_zoom);
            const int margin_right = int(x1 - x->label_margins[0] * x->b_zoom);
            const int margin_bottom = int(y0 - (x->label_margins[1] + MIN_MARGIN) * x->b_zoom);

            // ignore valign
            switch (align) {
            case LABEL_ALIGN_LEFT:
                return std::make_pair(margin_left, margin_bottom);
            case LABEL_ALIGN_CENTER:
                return std::make_pair(xc, margin_bottom);
            case LABEL_ALIGN_RIGHT:
                return std::make_pair(margin_right, margin_bottom);
            }
        } break;
        case LABEL_SIDE_BOTTOM: {
            const int margin_left = int(x0 + x->label_margins[0] * x->b_zoom);
            const int margin_right = int(x1 - x->label_margins[0] * x->b_zoom);
            const int margin_top = int(y1 + (x->label_margins[1] + MIN_MARGIN) * x->b_zoom);

            switch (align) {
            case LABEL_ALIGN_LEFT:
                return std::make_pair(margin_left, margin_top);
            case LABEL_ALIGN_CENTER:
                return std::make_pair(xc, margin_top);
            case LABEL_ALIGN_RIGHT:
                return std::make_pair(margin_right, margin_top);
            }
        } break;
        case LABEL_SIDE_RIGHT: {
            const int margin_left = int(x1 + (x->label_margins[0] + MIN_MARGIN) * x->b_zoom);
            const int margin_top = int(y0 + x->label_margins[1] * x->b_zoom);
            const int margin_bottom = int(y1 - x->label_margins[1] * x->b_zoom);

            switch (valign) {
            case LABEL_VALIGN_TOP:
                return std::make_pair(margin_left, margin_top);
            case LABEL_VALIGN_CENTER:
                return std::make_pair(margin_left, yc);
            case LABEL_VALIGN_BOTTOM:
                return std::make_pair(margin_left, margin_bottom);
            }
        }
        }
    } break;
    }

    return std::make_pair(x->b_rect.x, x->b_rect.y);
}

using LabelProps = std::tuple<LabelPosition, LabelSide, LabelAlign, LabelVAlign>;

static LabelProps label_enums(t_ebox* x)
{
    LabelPosition pos = (x->label_inner == 1) ? LABEL_POSITION_INNER : LABEL_POSITION_OUTER;

    LabelSide side = label_side_idx(x->label_side);
    if (side < 0)
        side = LABEL_SIDE_LEFT;

    LabelAlign align = label_align_idx(x->label_align);
    if (align < 0)
        align = LABEL_ALIGN_LEFT;

    LabelVAlign valign = label_valign_idx(x->label_valign);
    if (valign < 0)
        valign = LABEL_VALIGN_TOP;

    return std::make_tuple(pos, side, align, valign);
}

static void ebox_create_label(t_ebox* x)
{
    auto enums = label_enums(x);
    auto pt = ebox_label_coord(x, std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums));

    sys_vgui("::ceammc::ui::label_create %s %lx "
             "%d %d %d "
             "%s %s \"%s\" %d "
             "#%6.6x {%s}\n",
        x->b_canvas_id->s_name, x,
        pt.first, pt.second, (int)x->label_inner,
        ebox_label_anchor(std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums)),
        x->label_align->s_name,
        x->b_font.c_family->s_name,
        int(x->b_font.c_sizereal * x->b_zoom),
        rgba_to_hex_int(x->b_boxparameters.d_labelcolor),
        x->b_label->s_name);
}

static void ebox_update_label_pos(t_ebox* x)
{
    if (ebox_isvisible(x) && x->b_label && x->b_label != s_null) {
        auto enums = label_enums(x);
        auto pt = ebox_label_coord(x, std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums));

        sys_vgui("::ceammc::ui::label_pos %s %lx %d %d %d %s %s\n",
            x->b_canvas_id->s_name,
            x, pt.first, pt.second, (int)x->label_inner,
            ebox_label_anchor(std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums)),
            x->label_align->s_name);
    }
}

static void ebox_update_label_font(t_ebox* x)
{
    if (ebox_isvisible(x) && x->b_label != s_null) {
        sys_vgui("::ceammc::ui::label_font %s %lx %d \"%s\" %d\n",
            x->b_canvas_id->s_name, x,
            (int)x->label_inner, x->b_font.c_family->s_name,
            int(x->b_font.c_sizereal * x->b_zoom));
    }
}

void ebox_new(t_ebox* x, long flags)
{
    static t_symbol* SYM_LEFT = gensym("left");

    x->b_flags = flags;
    x->b_ready_to_draw = false;
    x->b_have_window = false;
    x->b_layers = nullptr;
    x->b_receive_id = s_null;
    x->b_send_id = s_null;
    x->b_objpreset_id = s_null;
    x->b_visible = true;
    x->b_zoom = 1;

    x->b_label = s_null;
    x->label_align = s_value_label_align_left;
    x->label_valign = s_value_label_valign_center;
    x->label_inner = 0;
    x->label_side = SYM_LEFT;
    x->label_margins[0] = 0;
    x->label_margins[1] = 0;
    x->cursor = ECURSOR_LEFT_PTR;

    x->wis_canvas = nullptr;

    eobj_getclass(&x->b_obj)->c_widget.w_dosave = reinterpret_cast<t_typ_method>(ebox_dosave);
    ebox_attrprocess_default(x);
}

void ebox_ready(t_ebox* x)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    x->b_selected_item = EITEM_NONE;
    x->b_selected_box = 0;
    x->b_selected_inlet = -1;
    x->b_selected_outlet = -1;
    x->b_mouse_down = 0;
    x->b_resize = false;
    x->b_resize_redraw_all = true;
    x->b_zoom = 1;

    x->b_boxparameters.d_bordercolor = rgba_black;
    x->b_boxparameters.d_boxfillcolor = rgba_white;
    x->b_boxparameters.d_labelcolor = rgba_black;
    x->b_boxparameters.d_hideiolets = false;
    x->b_boxparameters.d_hideborder = false;

    if (c->c_widget.w_getdrawparameters)
        c->c_widget.w_getdrawparameters(x, &x->b_boxparameters);

    x->b_ready_to_draw = true;

    ebox_newzoom(x);
}

void ebox_free(t_ebox* x)
{
    eobj_free(&x->b_obj);
    if (x->b_receive_id && x->b_receive_id != s_null) {
        // replace #n => $d
        t_symbol* sname = ceammc_realizeraute(eobj_getcanvas(&x->b_obj), x->b_receive_id);
        pd_unbind(&x->b_obj.o_obj.te_g.g_pd, sname);
    }
    gfxstub_deleteforkey(x);

    if (eobj_isdsp(&x->b_obj))
        eobj_dspfree(&x->b_obj);
}

t_symbol* ebox_getfontname(t_ebox* x)
{
    return x->b_font.c_family;
}

t_symbol* ebox_getfontslant(t_ebox* x)
{
    return x->b_font.c_slant;
}

t_symbol* ebox_getfontweight(t_ebox* x)
{
    return x->b_font.c_weight;
}

float ebox_getfontsize(t_ebox* x)
{
    return x->b_font.c_size;
}

t_pd* ebox_getsender(t_ebox* x)
{
    if (x->b_send_id && x->b_send_id != s_null) {
        if (x->b_send_id == x->b_receive_id) {
            pd_error(x, "[%s] send/receive loop: @send == @receive == '%s'", eobj_getclassname(&x->b_obj)->s_name, x->b_send_id->s_name);
            return nullptr;
        }

        auto sname = ceammc_realizeraute(eobj_getcanvas(&x->b_obj), x->b_send_id);

        if (sname && sname->s_thing)
            return sname->s_thing;
    }

    return nullptr;
}

bool ebox_isdrawable(t_ebox* x)
{
    if (eobj_isbox(&x->b_obj) && x->b_obj.o_canvas) {
        if (x->b_ready_to_draw && glist_isvisible(x->b_obj.o_canvas)) {
            return true;
        }
    }
    return false;
}

void ebox_set_cursor(t_ebox* x, t_cursor cursor)
{
    if (x->cursor != cursor) {
        sys_vgui("::ceammc::ui::mouse_cursor %s %lx %s\n",
            x->b_canvas_id->s_name, x, my_cursorlist[cursor]);

        x->cursor = cursor;
    }
}

void ebox_attrprocess_viatoms(t_ebox* x, int argc, t_atom* argv)
{
    char buffer[MAXPDSTRING] = "@";
    int defc = 0;
    t_atom* defv = nullptr;
    t_eclass* c = eobj_getclass(&x->b_obj);

    for (size_t i = 0; i < c->c_nattr; i++) {
        strcpy(&buffer[1], c->c_attr[i]->name->s_name); // append to buffer preserving initial '@'
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(&x->b_obj, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = nullptr;
        }
    }
}

void ebox_attrprocess_viabinbuf(t_ebox* x, t_binbuf* d)
{
    char attr_name[MAXPDSTRING] = "@";

    int defc = 0;
    t_atom* defv = nullptr;
    t_eclass* c = eobj_getclass(&x->b_obj);
    for (size_t i = 0; i < c->c_nattr; i++) {
        strcpy(&attr_name[1], c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(&x->b_obj, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = nullptr;
        }
    }
}

static void ebox_attrprocess_default(t_ebox* x)
{
    t_eclass* c = eobj_getclass(&x->b_obj);

    for (size_t i = 0; i < c->c_nattr; i++) {
        auto defvals = c->c_attr[i]->defvals;

        // skip if default is not set
        if (!defvals)
            continue;

        if (c->c_attr[i]->size < 1) {
            pd_error(x, "[%s] invalid attribute size: %ld", c->c_class.c_name->s_name, c->c_attr[i]->size);
            return;
        }

        const size_t N = c->c_attr[i]->size;
        t_atom defv[N];
        memset(defv, 0, sizeof(defv));

        // list of numbers
        auto type = c->c_attr[i]->type;
        if (type == s_int || type == s_long || type == &s_float || type == s_double) {
            std::vector<std::string> result;
            boost::split(result, defvals->s_name, boost::is_any_of(" "), boost::token_compress_on);

            if (N != result.size()) {
                pd_error(x, "[%s] mismatched size of default values: %d != %d",
                    c->c_class.c_name->s_name, int(N), int(result.size()));
                return;
            }

            for (size_t j = 0; j < N; j++) {
                const char* s = result[j].c_str();
                char* e = nullptr;
#if PD_FLOATSIZE == 32
                float f = std::strtof(s, &e);
                if (f == HUGE_VALF) {
                    pd_error(x, "[%s] value is too big: %s", c->c_class.c_name->s_name, s);
                    return;
                } else if (f == 0 && e == s) {
                    pd_error(x, "[%s] can't parse value: %s", c->c_class.c_name->s_name, s);
                    return;
                } else {
                    atom_setfloat(&defv[j], f);
                }
#elif PD_FLOATSIZE == 64
                double f = std::strtod(s, &e);
                if (f == HUGE_VAL) {
                    pd_error(x, "[%s] value is too big: %s", c->c_class.c_name->s_name, s);
                    return;
                } else if (f == 0 && e == s) {
                    pd_error(x, "[%s] can't parse value: %s", c->c_class.c_name->s_name, s);
                    return;
                } else {
                    atom_setfloat(&defv[j], f);
                }
#endif
            }
        } else {
            // single symbol
            if (N == 1) {
                atom_setsym(&defv[0], defvals);
            } else {
                // symbol list
                std::vector<std::string> result;
                boost::split(result, defvals->s_name, boost::is_any_of(" "), boost::token_compress_on);

                if (N != result.size()) {
                    pd_error(x, "[%s] mismatched size of default values: %d != %d",
                        c->c_class.c_name->s_name, int(N), int(result.size()));
                    return;
                }

                for (size_t j = 0; j < N; j++)
                    atom_setsym(&defv[j], gensym(result[j].c_str()));
            }
        }

        bool readonly = (c->c_attr[i]->getter) && (!c->c_attr[i]->setter);
        if (!readonly)
            eobj_attr_setvalueof(&x->b_obj, c->c_attr[i]->name, int(N), defv);
    }
}

//! Widget
void ebox_wgetrect(t_gobj* z, t_glist* glist, int* xp1, int* yp1, int* xp2, int* yp2)
{
    t_ebox* x = reinterpret_cast<t_ebox*>(z);
    *xp1 = text_xpix(&x->b_obj.o_obj, glist);
    *yp1 = text_ypix(&x->b_obj.o_obj, glist);
    *xp2 = int(*xp1 + x->b_rect.width * x->b_zoom);
    *yp2 = int(*yp1 + x->b_rect.height * x->b_zoom);
}

static void ebox_paint(t_ebox* x)
{
    layers_erase(x);

    // prevent crash if called by some reason before calling
    // create_widget
    if (!x->b_drawing_id) {
        bug("ebox_paint");
        return;
    }

    if (x->b_pinned)
        sys_vgui("::ceammc::ui::widget_lower %s %lx\n", x->b_canvas_id->s_name, x);

    sys_vgui("::ceammc::ui::widget_bg %s %lx #%6.6x\n",
        x->b_canvas_id->s_name, x, rgba_to_hex_int(x->b_boxparameters.d_boxfillcolor));

    t_eclass* c = eobj_getclass(&x->b_obj);

    if (c->c_widget.w_paint)
        c->c_widget.w_paint(x);

    if (x->b_label != s_null) {
        // update label color
        sys_vgui("::ceammc::ui::label_color %s %lx %d #%6.6x\n",
            x->b_canvas_id->s_name, x, (int)x->label_inner,
            rgba_to_hex_int(x->b_boxparameters.d_labelcolor));

        sys_vgui("::ceammc::ui::label_inner_sync %s %lx %d\n",
            x->b_canvas_id->s_name, x, (int)x->label_inner);
    }

    if (!x->b_boxparameters.d_hideborder) {
        ebox_draw_border(x);
        ebox_draw_iolets(x);
    }
}

//! Widget
void ebox_wvis(t_gobj* z, t_glist* glist, int vis)
{
    t_ebox* x = reinterpret_cast<t_ebox*>(z);
    x->wis_canvas = glist;

    if (vis) {
        if (eobj_isbox(&x->b_obj) && x->b_ready_to_draw && x->b_visible) {

            ebox_invalidate_all(x);
            ebox_create_window(x, glist);
            ebox_paint(x);
        }
    } else {
        ebox_erase(x);
        canvas_fixlinesfor(glist_getcanvas(glist), reinterpret_cast<t_text*>(x));
    }
}

//! Widget
void ebox_wdisplace(t_gobj* z, t_glist* /*glist*/, int dx, int dy)
{
    t_ebox* x = reinterpret_cast<t_ebox*>(z);

#ifdef _WINDOWS
    if (x->b_selected_box) {
#endif

        // prevents crash on early call
        if (!x->b_canvas_id)
            return;

        x->b_rect.x += dx;
        x->b_rect.y += dy;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix += dy;

        sys_vgui("::ceammc::ui::widget_move %s %lx %d %d\n",
            x->b_canvas_id->s_name, x, dx, dy);
        ebox_move(x);

#ifdef _WINDOWS
    }
#endif
}

//! Widget
void ebox_wselect(t_gobj* z, t_glist* /*glist*/, int selected)
{
    t_ebox* x = reinterpret_cast<t_ebox*>(z);
    if (selected)
        x->b_selected_box = 1;
    else
        x->b_selected_box = 0;
    ebox_select(x);
}

//! Widget
void ebox_wdelete(t_gobj* z, t_glist* glist)
{
    t_ebox* x = reinterpret_cast<t_ebox*>(z);
    ebox_erase(x);
    canvas_deletelinesfor(glist, reinterpret_cast<t_text*>(z));
}

//! Initialize all the ebox ids (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param canvas    The ebox's canvas
 \ @return          Nothing
 */
static void ebox_tk_ids(t_ebox* x, t_canvas* canvas)
{
    char buffer[MAXPDSTRING];
    x->b_obj.o_canvas = canvas;
    sprintf(buffer, ".x%lx.c", canvas);
    x->b_canvas_id = gensym(buffer);
    sprintf(buffer, "%s.ecanvas%lx", x->b_canvas_id->s_name, x);
    x->b_drawing_id = gensym(buffer);
    sprintf(buffer, "all%lx", x);
    x->b_all_id = gensym(buffer);
}

static void ebox_bind_events(t_ebox* x)
{
    sys_vgui("::ceammc::ui::mouse_events_bind %s %lx %s "
             "down up move enter leave right_click\n",
        x->b_canvas_id->s_name, x, x->b_obj.o_id->s_name);

    t_eclass* c = eobj_getclass(&x->b_obj);

    if (c->c_widget.w_dblclick)
        sys_vgui("::ceammc::ui::mouse_events_bind %s %lx %s "
                 "double_click\n",
            x->b_canvas_id->s_name, x, x->b_obj.o_id->s_name);

    if (c->c_widget.w_mousewheel)
        sys_vgui("::ceammc::ui::mouse_events_bind %s %lx %s "
                 "wheel\n",
            x->b_canvas_id->s_name, x, x->b_obj.o_id->s_name);

    if (c->c_widget.w_key || c->c_widget.w_keyfilter) {
        sys_vgui("bind %s <Key>  {+pdsend {%s key  %%k %%N}} \n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
}

static void ebox_create_widget(t_ebox* x)
{
    sys_vgui("::ceammc::ui::create_widget %lx %s %d %d %d\n",
        x, x->b_drawing_id->s_name,
        int(x->b_rect.width), int(x->b_rect.height), int(x->b_zoom));
}

static t_pt ebox_calc_pos(t_ebox* x, t_glist* glist)
{
    t_pt pos;

    if (!glist->gl_havewindow) {
        pos.x = x->b_obj.o_obj.te_xpix;
        pos.y = x->b_obj.o_obj.te_ypix;

        while (glist && !glist->gl_havewindow) {
            pos.x -= glist->gl_xmargin;
            pos.y -= glist->gl_ymargin;
            pos.x += glist->gl_obj.te_xpix;
            pos.y += glist->gl_obj.te_ypix;
            glist = glist->gl_owner;
        }
    } else {
        pos.x = x->b_obj.o_obj.te_xpix;
        pos.y = x->b_obj.o_obj.te_ypix;
    }

    pos.x *= x->b_zoom;
    pos.y *= x->b_zoom;

    return pos;
}

static void ebox_create_window(t_ebox* x, t_glist* glist)
{
    x->b_have_window = false;
    x->b_isinsubcanvas = !glist->gl_havewindow;
    auto pos = ebox_calc_pos(x, glist);
    x->b_rect.x = pos.x;
    x->b_rect.y = pos.y;

    ebox_tk_ids(x, glist_getcanvas(glist));
    ebox_create_widget(x);
    ebox_bind_events(x);

    sys_vgui("::ceammc::ui::create_window %s %lx %d %d %d %d %d\n",
        x->b_canvas_id->s_name, x,
        (int)x->b_rect.x, (int)x->b_rect.y, (int)x->b_rect.width, (int)x->b_rect.height,
        (int)x->b_zoom);

    if (x->b_label != s_null)
        ebox_create_label(x);

    // create callback
    t_eclass* c = eobj_getclass(&x->b_obj);
    if (c->c_widget.w_create)
        c->c_widget.w_create(x);

    x->b_have_window = true;
}

static char is_platform_control(long mod)
{
#ifdef __WIN32
    return mod & EMOD_CTRL;
#else
    return mod == EMOD_CMD;
#endif
}

static char is_for_box(t_ebox* x, long mod)
{
    return (!x->b_obj.o_canvas->gl_edit || (x->b_obj.o_canvas->gl_edit && is_platform_control(mod)));
}

static long modifier_wrapper(long mod)
{
#ifdef __APPLE__
#elif __WIN32
#else
    if (mod == 24) //right click
        mod = EMOD_CMD;
    else if (mod & EMOD_CMD) {
        mod ^= EMOD_CMD;
        mod |= EMOD_ALT;
    }
#endif
    // post("MOD : %ld", mod);
    return mod;
}

void ebox_mouse_enter(t_ebox* x)
{
    t_eclass* c = eobj_getclass(&x->b_obj);

    if (!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        sys_vgui("::ceammc::ui::widget_focus %s %lx\n", x->b_canvas_id->s_name, x);

        if (c->c_widget.w_mouseenter)
            c->c_widget.w_mouseenter(x);
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass* c = eobj_getclass(&x->b_obj);

    if (!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        if (c->c_widget.w_mouseleave) {
            c->c_widget.w_mouseleave(x);
        }
        ebox_set_cursor(x, ECURSOR_LEFT_PTR);
    } else if (x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        ebox_set_cursor(x, ECURSOR_HAND);
    }
}

void ebox_mouse_move(t_ebox* x, t_floatarg xpos, t_floatarg ypos, t_floatarg mod)
{
    const t_pt mouse { xpos, ypos };
    t_atom av[2];
    long modif = modifier_wrapper(static_cast<long>(mod));
    t_eclass* c = eobj_getclass(&x->b_obj);

    // mouse move
    if (!x->b_mouse_down) {
        if (is_for_box(x, modif)) {
            if (!(x->b_flags & EBOX_IGNORELOCKCLICK)) {
                ebox_set_cursor(x, ECURSOR_CENTER_PTR);
                if (c->c_widget.w_mousemove) {
                    c->c_widget.w_mousemove(x, x->b_obj.o_canvas, mouse, modif);
                }
            } else {
                ebox_set_cursor(x, ECURSOR_LEFT_PTR);
            }
        } else if (!x->b_isinsubcanvas) {
            x->b_selected_outlet = -1;
            x->b_selected_inlet = -1;
            x->b_selected_item = EITEM_NONE;
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);

            const int right = int(x->b_rect.width * x->b_zoom);
            const int bottom = int(x->b_rect.height * x->b_zoom);
            const int CURSOR_AREA = 3 * x->b_zoom;

            // BOTTOM & RIGHT //
            if (mouse.y >= bottom - CURSOR_AREA
                && mouse.y <= bottom
                && mouse.x > right - CURSOR_AREA
                && mouse.x <= right) {

                x->b_selected_item = EITEM_CORNER;
                ebox_set_cursor(x, ECURSOR_RIGHT_CORNER);
                return;
            }
            // BOTTOM //
            else if (mouse.y >= bottom - CURSOR_AREA && mouse.y <= bottom) {
                const int N = obj_noutlets(&x->b_obj.o_obj);
                const int XLET_W = 7;
                const int XLET_ZW = int(XLET_W * x->b_zoom);
                for (int i = 0; i < N; i++) {
                    int pos_x_outlet = 0;
                    if (N > 1)
                        pos_x_outlet = int(i / float(N - 1) * (x->b_rect.width * x->b_zoom - (XLET_ZW + 1)));

                    if (mouse.x >= pos_x_outlet && mouse.x <= pos_x_outlet + XLET_ZW) {
                        x->b_selected_outlet = i;
                        ebox_set_cursor(x, ECURSOR_CIRCLE);
                        break;
                    }
                }
                if (x->b_selected_outlet == -1) {
                    x->b_selected_item = EITEM_BOTTOM;
                    ebox_set_cursor(x, ECURSOR_BOTTOM);
                }
                return;
            }
            // RIGHT //
            else if (mouse.x > right - 3 && mouse.x <= right) {
                x->b_selected_item = EITEM_RIGHT;
                ebox_set_cursor(x, ECURSOR_RIGHT_SIDE);
                return;
            }

            // BOX //
            ebox_set_cursor(x, ECURSOR_HAND);
        } else {
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
        }
    } else { // mouse drag
        if (is_for_box(x, modif)) {
            if (c->c_widget.w_mousedrag && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
                c->c_widget.w_mousedrag(x, x->b_obj.o_canvas, mouse, modif);
            }
        } else if (!x->b_isinsubcanvas) {
            if (x->b_selected_item == EITEM_NONE) {
                sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
            } else if (!(x->b_flags & EBOX_GROWNO)) {
                if (x->b_flags & EBOX_GROWLINK) {
                    if (x->b_selected_item == EITEM_BOTTOM) {
                        atom_setfloat(av, x->b_rect_last.width + (mouse.y - x->b_rect_last.height));
                        atom_setfloat(av + 1, mouse.y);
                    } else if (x->b_selected_item == EITEM_RIGHT) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, x->b_rect_last.height + (mouse.x - x->b_rect_last.width));
                    } else if (x->b_selected_item == EITEM_CORNER) {
                        if (mouse.y > mouse.x) {
                            atom_setfloat(av, mouse.y);
                            atom_setfloat(av + 1, mouse.y);
                        } else {
                            atom_setfloat(av, mouse.x);
                            atom_setfloat(av + 1, mouse.x);
                        }
                    }
                } else if (x->b_flags & EBOX_GROWINDI) {
                    if (x->b_selected_item == EITEM_BOTTOM) {
                        atom_setfloat(av, x->b_rect_last.width);
                        atom_setfloat(av + 1, mouse.y);
                    } else if (x->b_selected_item == EITEM_RIGHT) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, x->b_rect_last.height);
                    } else if (x->b_selected_item == EITEM_CORNER) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, mouse.y);
                    }
                }

                x->b_resize_redraw_all = !(modif & EMOD_SHIFT);
                x->b_resize = true;
                mess3((t_pd*)x, s_attr_size, s_attr_size, (void*)2, (void*)av);
                x->b_resize = false;
            }
        } else {
            sys_vgui("eobj_canvas_motion %s 1\n", x->b_canvas_id->s_name);
        }
    }
}

void ebox_mouse_down(t_ebox* x, t_floatarg xpos, t_floatarg ypos, t_floatarg absx, t_floatarg absy, t_floatarg mod)
{
    const auto modif = modifier_wrapper(static_cast<long>(mod));

    t_eclass* c = eobj_getclass(&x->b_obj);
    if (is_for_box(x, modif)) {
        if (c->c_widget.w_mousedown && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
            t_pt mouse { xpos, ypos };
            t_pt mouse_abs { absx, absy };

            c->c_widget.w_mousedown(x, x->b_obj.o_canvas, mouse, mouse_abs, modif);
        }
    } else {
        if (x->b_selected_item == EITEM_NONE) {
            if (modif == EMOD_SHIFT) {
                sys_vgui("eobj_canvas_down %s 1\n", x->b_canvas_id->s_name);
            } else if (modif == EMOD_RIGHT) {
                sys_vgui("eobj_canvas_right %s\n", x->b_canvas_id->s_name);
            } else {
                sys_vgui("eobj_canvas_down %s 0\n", x->b_canvas_id->s_name);
            }
        } else {
            const float z = x->b_zoom;
            t_rect br = x->b_rect;
            br.height *= z;
            br.width *= z;
            x->b_rect_last = br;
        }
    }

    x->b_mouse_down = true;
}

void ebox_mouse_up(t_ebox* x, t_floatarg xpos, t_floatarg ypos, t_floatarg mod)
{
    const auto modif = modifier_wrapper(static_cast<long>(mod));

    t_eclass* c = eobj_getclass(&x->b_obj);
    if (is_for_box(x, modif)) {
        if (c->c_widget.w_mouseup && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
            t_pt mouse { xpos, ypos };
            c->c_widget.w_mouseup(x, x->b_obj.o_canvas, mouse, modif);
        }
    } else {
        if (!x->b_isinsubcanvas && !x->b_resize_redraw_all) {
            ebox_invalidate_all(x);
            ebox_redraw(x);
            x->b_resize_redraw_all = true;
        }

        sys_vgui("eobj_canvas_up %s\n", x->b_canvas_id->s_name);
    }

    x->b_mouse_down = false;
}

void ebox_mouse_dblclick(t_ebox* x, t_symbol* /*s*/, int argc, t_atom* argv)
{
    t_pt mouse;
    t_eclass* c = eobj_getclass(&x->b_obj);
    long modif = modifier_wrapper((long)atom_getfloat(argv + 2));
    if (is_for_box(x, modif) && c->c_widget.w_dblclick && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
        mouse.x = atom_getfloat(argv);
        mouse.y = atom_getfloat(argv + 1);
        c->c_widget.w_dblclick(x, x->b_obj.o_canvas, mouse, modif);
    }
}

static void ebox_open_help(t_ebox* x)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    open_via_helppath(class_gethelpname(&c->c_class), class_gethelpdir(&c->c_class));
}

void ebox_mouse_rightclick(t_ebox* x, t_floatarg xpos, t_floatarg ypos, t_floatarg absx, t_floatarg absy, t_floatarg mod)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    const auto modif = modifier_wrapper(static_cast<long>(mod));

    // show help menu in all modes (edit/performance) with pressed Shift
    if (modif & EMOD_SHIFT)
        return ebox_open_help(x);

    // show object properties in all modes (edit/performance) with pressed Alt
    if (modif & EMOD_ALT)
        return ebox_properties(x, nullptr);

    if (x->b_obj.o_canvas->gl_edit) {
        // in edit mode show standart Pd popup
        sys_vgui("eobj_canvas_right %s\n", x->b_canvas_id->s_name);
    } else {
        // in performance mode if widget defines their own popup dialogs: show them
        // or show standart Pd popup otherwise
        if (c->c_widget.w_rightclick && !(x->b_flags & EBOX_IGNORELOCKCLICK))
            c->c_widget.w_rightclick(x, { xpos, ypos }, { absx, absy });
        else
            sys_vgui("eobj_canvas_right %s\n", x->b_canvas_id->s_name);
    }
}

void ebox_mouse_wheel(t_ebox* x, t_floatarg xpos, t_floatarg ypos, t_floatarg delta, t_floatarg mod)
{
    const auto modif = modifier_wrapper(static_cast<long>(mod));

    t_eclass* c = eobj_getclass(&x->b_obj);
    if (is_for_box(x, modif) && c->c_widget.w_mousewheel && !(x->b_flags & EBOX_IGNORELOCKCLICK))
        c->c_widget.w_mousewheel(x, { xpos, ypos }, modif, delta);
}

void ebox_key(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_eclass* c = eobj_getclass(&x->b_obj);

    constexpr int KEY_TAB = 0xFF09;
    constexpr int KEY_ENTER = 0xFF0D;
    constexpr int KEY_ESCAPE = 0xFF1B;
    constexpr int KEY_BACKSPACE = 65288;

    if (argc >= 2 && argv && atom_gettype(argv + 1) == A_FLOAT) {
        if (!x->b_obj.o_canvas->gl_edit) {
            const int key = atom_getfloat(argv + 1);
            if (key == KEY_BACKSPACE) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_DEL, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_DEL, 0);
                }
            } else if (key == KEY_TAB) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_TAB, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_TAB, 0);
                }
            } else if (key == KEY_ENTER) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ENTER, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_ENTER, 0);
                }
            } else if (key == KEY_ESCAPE) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ESC, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_ESC, 0);
                }
            } else {
                if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, key, 0);
                }
            }
        }
    }
}

void ebox_dosave(t_ebox* x, t_binbuf* b)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    if (c && b) {
        auto state = canvas_suspend_dsp();
        binbuf_addv(b, "ssiis", &s__X, s_obj, (int)x->b_obj.o_obj.te_xpix, (int)x->b_obj.o_obj.te_ypix, eobj_getclassname(&x->b_obj));
        for (size_t i = 0; i < c->c_nattr; i++) {
            if (c->c_attr[i] && c->c_attr[i]->save && c->c_attr[i]->name) {
                int argc = 0;
                t_atom* argv = nullptr;
                eobj_attr_getvalueof(&x->b_obj, c->c_attr[i]->name, &argc, &argv);
                if (argc && argv) {
                    char attr_name[MAXPDSTRING];
                    snprintf(attr_name, MAXPDSTRING, "@%s", c->c_attr[i]->name->s_name);
                    binbuf_append_attribute(b, gensym(attr_name), argc, argv);
                    free(argv);
                }
            }
        }

        if (c->c_widget.w_save != nullptr)
            c->c_widget.w_save(x, b);

        binbuf_addv(b, ";");
        canvas_resume_dsp(state);
    }
}

void ebox_pos(t_ebox* x, t_float newx, t_float newy)
{
    x->b_rect.x = newx;
    x->b_rect.y = newy;
    x->b_obj.o_obj.te_xpix = short(newx);
    x->b_obj.o_obj.te_ypix = short(newy);

    ebox_move(x);
}

void ebox_vis(t_ebox* x, t_float v)
{
    const bool vis = (v != 0);
    if (x->b_visible != vis) {
        x->b_visible = vis;

        if (ebox_isdrawable(x)) {
            if (vis) {

                if (x->wis_canvas) {
                    auto pos = ebox_calc_pos(x, x->wis_canvas);
                    x->b_rect.x = pos.x;
                    x->b_rect.y = pos.y;
                }

                ebox_invalidate_all(x);
                ebox_create_window(x, x->wis_canvas);

                ebox_paint(x);
            } else {
                ebox_erase(x);
            }
        }
    }
}

t_pd_err ebox_set_receiveid(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        t_symbol* new_sym = atom_getsymbol(argv);

        if (new_sym == x->b_receive_id)
            return 0; // no change

        // unbind previous
        if (x->b_receive_id != s_null) {
            // replace #0 => ID
            t_symbol* sname = ceammc_realizeraute(eobj_getcanvas(&x->b_obj), x->b_receive_id);
            if (sname)
                pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }

        // bind new
        x->b_receive_id = new_sym;
        t_symbol* sname = ceammc_realizeraute(eobj_getcanvas(&x->b_obj), x->b_receive_id);
        pd_bind(&x->b_obj.o_obj.ob_pd, sname);
    } else {
        // unbind
        if (x->b_receive_id != s_null) {
            t_symbol* sname = ceammc_realizeraute(eobj_getcanvas(&x->b_obj), x->b_receive_id);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }

        x->b_receive_id = s_null;
    }

    return 0;
}

t_pd_err ebox_set_sendid(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        x->b_send_id = atom_getsymbol(argv);
    } else {
        x->b_send_id = s_null;
    }

    return 0;
}

t_pd_err ebox_set_label(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {

        if (x->b_label == s_null) {
            // create new label
            x->b_label = atom_getsymbol(argv);
            if (ebox_isvisible(x))
                ebox_create_label(x);
        } else {
            // change label text
            if (argc == 1)
                x->b_label = atom_getsymbol(argv);
            else {
                auto to_str = [](const t_atom& a) -> std::string {
                    auto t = atom_gettype(&a);
                    if (t == A_SYMBOL)
                        return atom_getsymbol(&a)->s_name;
                    else if (t == A_FLOAT) {
                        return atom_gensym(&a)->s_name;
                    } else
                        return {};
                };

                std::string res;
                for (int j = 0; j < argc; j++) {
                    if (j != 0)
                        res.push_back(' ');

                    res += to_str(argv[j]);
                }

                x->b_label = gensym(res.c_str());
            }

            if (ebox_isvisible(x)) {
                sys_vgui("::ceammc::ui::label_text %s %lx %d {%s}\n",
                    x->b_canvas_id->s_name, x, (int)x->label_inner, x->b_label->s_name);
            }
        }
    } else {
        // erase label
        if (ebox_isvisible(x) && x->b_label != s_null)
            ebox_erase_label(x);

        x->b_label = s_null;
    }

    return 0;
}

t_pd_err ebox_set_label_align(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    static t_symbol* items[] = {
        s_value_label_align_center,
        s_value_label_align_left,
        s_value_label_align_right
    };

    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        t_symbol* s = atom_getsymbol(argv);

        auto it = std::find(std::begin(items), std::end(items), s);
        if (it == std::end(items)) {
            pd_error(x, "[%s] invalid @label_align property value: %s", eobj_getclassname(&x->b_obj)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(&x->b_obj)->s_name, values.c_str());
            return 1;
        }

        x->label_align = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_valign(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    static t_symbol* items[] = {
        s_value_label_valign_top,
        s_value_label_valign_center,
        s_value_label_valign_bottom
    };

    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        t_symbol* s = atom_getsymbol(argv);

        auto it = std::find(std::begin(items), std::end(items), s);
        if (it == std::end(items)) {
            pd_error(x, "[%s] invalid @label_valign property value: %s", eobj_getclassname(&x->b_obj)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(&x->b_obj)->s_name, values.c_str());
            return 1;
        }

        x->label_valign = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_side(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    static t_symbol* items[] = {
        s_value_label_side_bottom,
        s_value_label_side_left,
        s_value_label_side_right,
        s_value_label_side_top
    };

    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        t_symbol* s = atom_getsymbol(argv);
        auto it = std::find(std::begin(items), std::end(items), s);
        if (it == std::end(items)) {
            pd_error(x, "[%s] invalid @label_side property value: %s", eobj_getclassname(&x->b_obj)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(&x->b_obj)->s_name, values.c_str());
            return 1;
        }

        x->label_side = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_position(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_FLOAT) {
        int pos = (atom_getfloat(argv) != 0) ? 1 : 0;

        if (x->label_inner != pos && x->b_label != s_null) {
            const bool is_vis = ebox_isvisible(x);

            if (is_vis)
                ebox_erase_label(x);

            x->label_inner = pos;

            if (is_vis)
                ebox_create_label(x);
        }
    }

    return 0;
}

t_pd_err ebox_set_label_margins(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc == 2 && argv && atom_gettype(argv) == A_FLOAT) {
        x->label_margins[0] = int(atom_getfloat(argv));
        x->label_margins[1] = int(atom_getfloat(argv + 1));
        ebox_update_label_pos(x);
    } else
        pd_error(x, "[%s] X Y margin pair expected", eobj_getclassname(&x->b_obj)->s_name);

    return 0;
}

t_symbol* ebox_get_presetid(t_ebox* x)
{
    if (x->b_objpreset_id != s_null) {
        return x->b_objpreset_id;
    } else {
        return s_null;
    }
}

t_pd_err ebox_set_presetid(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        x->b_objpreset_id = atom_getsymbol(argv);
    } else {
        x->b_objpreset_id = s_null;
    }
    return 0;
}

t_pd_err ebox_set_font(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        if (atom_getsymbol(argv) == s_null || atom_getsymbol(argv) == &s_)
            x->b_font.c_family = gensym(SYM_DEFAULT_FONT_FAMILY);
        else
            x->b_font.c_family = atom_getsymbol(argv);
    } else
        x->b_font.c_family = gensym(SYM_DEFAULT_FONT_FAMILY);

    auto ftname = strdup(x->b_font.c_family->s_name);
    if (!ftname)
        return -1;

    auto ftname_uc = strtok(ftname, " ',.-");
    if (!ftname_uc)
        return -1;

    ftname_uc[0] = (char)toupper(ftname_uc[0]);
    x->b_font.c_family = gensym(ftname_uc);
    free(ftname);

    ebox_update_label_font(x);

    return 0;
}

t_pd_err ebox_set_fontweight(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        if (atom_getsymbol(argv) == gensym(SYM_BOLD))
            x->b_font.c_weight = gensym(SYM_BOLD);
        else
            x->b_font.c_weight = gensym(SYM_NORMAL);
    } else
        x->b_font.c_weight = gensym(SYM_NORMAL);

    return 0;
}

t_pd_err ebox_set_fontslant(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        if (atom_getsymbol(argv) == gensym(SYM_ITALIC))
            x->b_font.c_slant = gensym(SYM_ITALIC);
        else
            x->b_font.c_slant = gensym(SYM_ROMAN);
    } else
        x->b_font.c_slant = gensym(SYM_ROMAN);

    return 0;
}

#ifdef __APPLE__
static const int FONT_SIZE = 11;
#elif __WIN32
static const int FONT_SIZE = 9;
#else
static const int FONT_SIZE = 10;
#endif

t_pd_err ebox_set_fontsize(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_FLOAT)
        x->b_font.c_sizereal = static_cast<int>(pd_clip_min(atom_getfloat(argv), 4));
    else
        x->b_font.c_sizereal = FONT_SIZE;

#ifdef __APPLE__
    x->b_font.c_size = x->b_font.c_sizereal;
#elif _WINDOWS
    x->b_font.c_size = x->b_font.c_sizereal - 2;
#else
    x->b_font.c_size = x->b_font.c_sizereal - 3;
#endif

    ebox_update_label_font(x);
    return 0;
}

t_pd_err ebox_size_set(t_ebox* x, t_object* /*attr*/, int argc, t_atom* argv)
{
    float width, height;
    if (argc && argv) {
        if (x->b_flags & EBOX_GROWNO)
            return 0;
        else if (x->b_flags & EBOX_GROWLINK) {
            if (atom_gettype(argv) == A_FLOAT) {
                width = (float)pd_clip_min(atom_getfloat(argv), 4) / x->b_zoom;
                height = x->b_rect.height;
                x->b_rect.height += width - x->b_rect.width;
                if (x->b_rect.height < 4) {
                    x->b_rect.width += 4 - height;
                    x->b_rect.height = 4;
                } else {
                    x->b_rect.width = width;
                }
            }

        } else if (x->b_flags & EBOX_GROWINDI) {
            if (atom_gettype(argv) == A_FLOAT)
                x->b_rect.width = (float)pd_clip_min(atom_getfloat(argv), 4) / x->b_zoom;
            if (atom_gettype(argv + 1) == A_FLOAT)
                x->b_rect.height = (float)pd_clip_min(atom_getfloat(argv + 1), 4) / x->b_zoom;
        }
    }

    return 0;
}

bool ebox_notify(t_ebox* x, t_symbol* s)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    if (s == s_size) {
        if (c->c_widget.w_oksize != nullptr)
            c->c_widget.w_oksize(x, &x->b_rect);

        if (x->b_resize_redraw_all) {
            ebox_invalidate_all(x);
        }

        if (ebox_isvisible(x)) {
            sys_vgui("::ceammc::ui::widget_resize %s %lx %d %d %d\n",
                x->b_canvas_id->s_name, x,
                int(x->b_rect.width), int(x->b_rect.height), int(x->b_zoom));

            canvas_fixlinesfor(x->b_obj.o_canvas, (t_text*)x);
            ebox_update_label_pos(x);
        }

        ebox_redraw(x);

    } else if (s == s_pinned && ebox_isvisible(x)) {
        if (x->b_pinned) {
            sys_vgui("::ceammc::ui::widget_lower %s %lx\n", x->b_canvas_id->s_name, x);
        } else {
            sys_vgui("::ceammc::ui::widget_raise %s %lx\n", x->b_canvas_id->s_name, x);
        }
    }

    return true;
}

void ebox_attr_dump(t_ebox* x)
{
    t_object* xobj = &x->b_obj.o_obj;
    t_class* xc = xobj->te_pd;
    const char* name = class_getname(xc);

    // print methods
    for (int i = 0; i < xc->c_nmethod; i++) {
        auto& m = eclass_methods(xc)[i];
        // ignore property methods
        if (m.me_name->s_name[0] == '@')
            continue;

        post("[%s] method: %s", name, m.me_name->s_name);
    }

    // print xlets
    post("[%s] inlets: %i", name, obj_ninlets(xobj));
    post("[%s] outlets: %i", name, obj_noutlets(xobj));

    // print properties
    t_eclass* c = eobj_getclass(&x->b_obj);
    for (size_t i = 0; i < c->c_nattr; i++) {
        int argc = 0;
        t_atom* argv = nullptr;
        eobj_attr_getvalueof(&x->b_obj, c->c_attr[i]->name, &argc, &argv);

        if (argc && argv) {
            char buf[MAXPDSTRING];
            buf[0] = '\0';

            for (int j = 0; j < argc; j++) {
                char prop_buf[256];
                atom_string(&argv[j], prop_buf, 256);
                strcat(buf, prop_buf);
                strcat(buf, " ");
            }

            free(argv);
            post("[%s] property: @%s = %s", name, c->c_attr[i]->name->s_name, buf);
        }
    }
}

void ebox_output_all_attrs(t_ebox* x)
{
    if (!x->b_obj.o_obj.te_outlet) {
        pd_error(x, "[%s] object has no outlets.", class_getname(((t_object*)x)->te_pd));
        return;
    }

    t_eclass* c = eobj_getclass(&x->b_obj);
    size_t argc = c->c_nattr;
    if (argc < 1) {
        post("[%s] no properties", class_getname(((t_object*)x)->te_pd));
        return;
    }

    char buf[MAXPDSTRING];
    t_atom* argv = (t_atom*)malloc(argc * sizeof(t_atom));
    for (size_t i = 0; i < c->c_nattr; i++) {
        sprintf(buf, "@%s", c->c_attr[i]->name->s_name);
        atom_setsym(&argv[i], gensym(buf));
    }

    outlet_anything(x->b_obj.o_obj.te_outlet, gensym(SYM_ALL_PROPS), argc, argv);
    free(argv);
}

void ebox_properties(t_ebox* x, t_glist* glist)
{
    t_eclass* c = eobj_getclass(&x->b_obj);
    char temp[MAXPDSTRING];

    sprintf(temp, "pdtk_%s_dialog %%s", c->c_class.c_name->s_name);
    std::string buffer(temp);

    for (size_t i = 0; i < c->c_nattr; i++) {
        if (!c->c_attr[i]->invisible) {
            t_atom* argv = 0;
            int argc = 0;
            eobj_attr_getvalueof(&x->b_obj, c->c_attr[i]->name, &argc, &argv);
            // tcl: using curly brackets instead of double quotes
            // to prevent variable (with $) substitution
            buffer += " {";
            if (argc && argv) {
                for (int j = 0; j < argc; j++) {
                    if (j > 0)
                        buffer.push_back(' ');

                    atom_string(argv + j, temp, MAXPDSTRING);
                    // quote string if it contains spaces
                    if (c->c_attr[i]->type == &s_symbol && strchr(temp, ' ')) {
                        // right trim spaces
                        auto str = ceammc_raute2dollar(temp);
                        str.erase(str.find_last_not_of(' ') + 1);

                        buffer += ceammc_quote_str(str);
                    } else {
                        buffer += ceammc_raute2dollar(temp);
                    }
                }
            }

            free(argv);
            buffer.push_back('}');
        }
    }
    buffer.push_back('\n');

    gfxstub_new((t_pd*)x, x, buffer.c_str());
}

t_symbol* ceammc_dollar2raute(const char* s)
{
    if (strlen(s) >= MAXPDSTRING)
        return gensym(s);

    char buf[MAXPDSTRING + 1];
    char* s2 = buf;
    for (const char* s1 = s; /**/; s1++, s2++) {
        if (*s1 == '$')
            *s2 = '#';
        else if (!(*s2 = *s1))
            break;
    }

    return gensym(buf);
}

t_symbol* ceammc_realizeraute(t_canvas* cnv, t_symbol* s)
{
    if (strchr(s->s_name, '#')) {
        constexpr int MAX_N = 128;
        char buf[MAX_N + 1];

        const char* a = s->s_name;
        char* b = buf;
        int i = 0;
        for (i = 0; i < MAX_N; i++, a++, b++) {
            const auto ch = *a;
            *b = (ch == '#') ? '$' : ch;
            if (ch == '\0')
                break;
        }

        buf[i] = '\0';

        return canvas_realizedollar(cnv, gensym(buf));
    } else
        return s;
}

std::string ceammc_raute2dollar(const char* s)
{
    if (strlen(s) >= MAXPDSTRING)
        return s;

    char buf[MAXPDSTRING + 1];
    char* s2 = buf;
    for (const char* s1 = s; /**/; s1++, s2++) {
        if (*s1 == '#')
            *s2 = '$';
        else if (!(*s2 = *s1))
            break;
    }

    return buf;
}

std::string ceammc_quote_str(const std::string& str, char q)
{
    std::string res;
    res.push_back(q);
    res += str;
    res.push_back(q);
    return res;
}

void ebox_dialog(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    static t_symbol* S_COLOR = gensym(SYM_COLOR);
    static t_symbol* S_CHECKBUTTON = gensym(SYM_CHECKBUTTON);
    static t_symbol* S_MENU = gensym(SYM_MENU);

    static const bool is_ceammc = (getenv("is_ceammc") != nullptr);
    const bool use_sframe = !is_ceammc;

    t_eclass* c = eobj_getclass(&x->b_obj);
    t_atom* av = NULL;
    int ac;
    char temp[MAXPDSTRING];
    t_rgb color;

    if (argc > 2 && argv) {
        ebox_attrprocess_viatoms(x, argc, argv);

        if (atom_gettype(argv) == A_SYMBOL && atom_gettype(argv + 1) == A_FLOAT) {
            int attrindex = (int)atom_getfloat(argv + 1) - 1;
            if (attrindex >= 0 && attrindex < c->c_nattr) {
                av = 0;
                ac = 0;

                char WIDGET_ID[64];
                if (use_sframe) {
                    snprintf(WIDGET_ID, sizeof(WIDGET_ID), "%s.canvas.container.content.w%i",
                        atom_getsymbol(argv)->s_name,
                        attrindex + 1);
                } else {
                    snprintf(WIDGET_ID, sizeof(WIDGET_ID), "%s.f.w%i",
                        atom_getsymbol(argv)->s_name,
                        attrindex + 1);
                }

                eobj_attr_getvalueof(&x->b_obj, c->c_attr[attrindex]->name, &ac, &av);

                if (ac && av) {
                    const auto style = c->c_attr[attrindex]->style;

                    if (style == S_CHECKBUTTON) {
                        if (atom_getfloat(av) == 0)
                            sys_vgui("%s state !selected\n", WIDGET_ID);
                        else
                            sys_vgui("%s state selected\n", WIDGET_ID);
                    } else if (style == S_COLOR) {
                        color.red = atom_getfloat(av);
                        color.green = atom_getfloat(av + 1);
                        color.blue = atom_getfloat(av + 2);
                        sys_vgui("%s configure -readonlybackground #%6.6x\n", WIDGET_ID, rgb_to_hex_int(color));
                    } else if (style == S_MENU) {
                        atom_string(av, temp, MAXPDSTRING);
                        std::string buffer(temp);
                        for (int i = 1; i < ac; i++) {
                            atom_string(av + i, temp, MAXPDSTRING);
                            buffer.push_back(' ');
                            buffer += temp;
                        }
                        sys_vgui("%s delete 0 end\n", WIDGET_ID);
                        sys_vgui("%s insert 0 {%s}\n", WIDGET_ID, buffer.c_str());
                    } else {
                        atom_string(av, temp, MAXPDSTRING);
                        std::string buffer(temp);

                        for (int i = 1; i < ac; i++) {
                            atom_string(av + i, temp, MAXPDSTRING);
                            buffer.push_back(' ');
                            buffer += temp;
                        }

                        // if have spaces and text entry
                        if (c->c_attr[attrindex]->type == &s_symbol
                            && buffer.find(' ') != std::string::npos) {
                            // trim spaces
                            buffer.erase(0, buffer.find_first_not_of(' '));
                            buffer.erase(buffer.find_last_not_of(' ') + 1);

                            buffer.insert(buffer.begin(), '\'');
                            buffer.push_back('\'');
                        }

                        sys_vgui("%s delete 0 end \n", WIDGET_ID);
                        // replace #\d+ -> $\d+
                        // tcl: regsub -all {#(\d+)} $s {$\1}
                        sys_vgui("%s insert 0 [regsub -all {#(\\d+)} {%s} {$\\1}]\n",
                            WIDGET_ID, buffer.c_str());
                    }

                    free(av);
                }
            }
        }
    }
}

void ebox_redraw(t_ebox* x)
{
    if (ebox_isvisible(x))
        ebox_paint(x);
}

void ebox_get_rect_for_view(t_ebox* x, t_rect* rect)
{
    rect->x = x->b_rect.x;
    rect->y = x->b_rect.y;
    rect->width = x->b_rect.width;
    rect->height = x->b_rect.height;
}

t_elayer* ebox_start_layer(t_ebox* x, t_symbol* name, float width, float height)
{
    char text[MAXPDSTRING];

    if (x->b_layers == nullptr)
        x->b_layers = new std::vector<t_elayer>();

    for (auto& l : *x->b_layers) {
        if (l.e_name == name) {
            t_elayer* graphic = &l;
            if (graphic->e_state == EGRAPHICS_INVALID) {
                graphic->e_owner = (t_object*)x;

                egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
                graphic->e_line_width = 1.f;
                graphic->e_line_capstyle = ECAPSTYLE_BUTT;
                graphic->e_line_dashstyle = EDASHSTYLE_NONE;
                graphic->e_line_smooth = ESMOOTH_NONE;
                graphic->e_color = 0;
                graphic->e_rect.x = 0.f;
                graphic->e_rect.y = 0.f;
                graphic->e_rect.height = (float)pd_clip_min(height, 0.);
                graphic->e_rect.width = (float)pd_clip_min(width, 0.);
                graphic->e_optimize = false;

                graphic->e_objects.clear();
                graphic->e_new_objects.e_points.clear();

                sprintf(text, "%s%" PRIxPTR, name->s_name, (intptr_t)x);
                graphic->e_id = gensym(text);

                graphic->e_new_objects.e_image = NULL;

                graphic->e_state = EGRAPHICS_OPEN;
                return graphic;
            } else {
                return nullptr;
            }
        }
    }

    // if layer not found - create one
    x->b_layers->emplace_back();
    t_elayer* graphic = &x->b_layers->back();

    graphic->e_owner = (t_object*)x;

    egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
    graphic->e_line_width = 1.f;
    graphic->e_line_capstyle = ECAPSTYLE_BUTT;
    graphic->e_line_dashstyle = EDASHSTYLE_NONE;
    graphic->e_line_smooth = ESMOOTH_NONE;
    graphic->e_color = 0;
    graphic->e_rect.x = 0.f;
    graphic->e_rect.y = 0.f;
    graphic->e_rect.height = (float)pd_clip_min(height, 0.);
    graphic->e_rect.width = (float)pd_clip_min(width, 0.);

    graphic->e_name = name;
    sprintf(text, "%s%" PRIdPTR, name->s_name, (intptr_t)x);
    graphic->e_state = EGRAPHICS_OPEN;
    graphic->e_id = gensym(text);
    return graphic;
}

t_pd_err ebox_end_layer(t_ebox* x, t_symbol* name)
{
    if (!x->b_layers)
        return -1;

    for (auto& l : *x->b_layers) {
        if (l.e_name == name) {
            l.e_state = EGRAPHICS_TODRAW;
            return 0;
        }
    }

    return -1;
}

static inline t_elayer* ebox_get_layer(t_ebox* x, t_symbol const* name)
{
    if (!x->b_layers)
        return nullptr;

    for (auto& l : *x->b_layers) {
        if (l.e_name == name)
            return &l;
    }

    return nullptr;
}

t_pd_err ebox_invalidate_layer(t_ebox* x, t_symbol* name)
{
    t_elayer* g = ebox_get_layer(x, name);
    if (!g)
        return -1;

    g->e_state = EGRAPHICS_INVALID;
    return 0;
}

static void ebox_do_paint_rect(t_elayer* g, t_ebox* x, t_egobj const* gobj, float x_p, float y_p)
{
    t_pt const* pt = &gobj->e_points[0];

    sys_vgui("%s create rectangle %d %d %d %d ", x->b_drawing_id->s_name,
        (int)(pt[1].x + x_p), (int)(pt[1].y + y_p),
        (int)(pt[2].x + x_p), (int)(pt[2].y + y_p));

    if (gobj->e_filled) {
        sys_vgui("-fill #%6.6x -outline #%6.6x -width 1 -tags { %s %s }\n",
            gobj->e_color, gobj->e_color, g->e_id->s_name, x->b_all_id->s_name);
    } else {
        sys_vgui("-outline #%6.6x -width %0.1f -tags { %s %s }\n",
            gobj->e_color, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
    }
}

static void ebox_do_paint_oval(t_elayer* g, t_ebox* x, t_egobj const* gobj, float x_p, float y_p)
{
    t_pt const* pt = &gobj->e_points[0];

    sys_vgui("%s create oval %d %d %d %d ", x->b_drawing_id->s_name,
        (int)(pt[1].x + x_p), (int)(pt[1].y + y_p),
        (int)(pt[2].x + x_p), (int)(pt[2].y + y_p));

    if (gobj->e_filled) {
        sys_vgui("-fill #%6.6x -outline #%6.6x -width 1 -tags { %s %s }\n",
            gobj->e_color, gobj->e_color, g->e_id->s_name, x->b_all_id->s_name);
    } else {
        sys_vgui("-outline #%6.6x -width %.1f -tags { %s %s }\n",
            gobj->e_color, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
    }
}

static void ebox_do_paint_arc(t_elayer* g, t_ebox* x, t_egobj const* gobj, float x_p, float y_p)
{
    t_pt const* pt = &gobj->e_points[0];

    sys_vgui("%s create arc %d %d %d %d -extent %.2f -start %.2f ", x->b_drawing_id->s_name,
        (int)(pt[1].x + x_p), (int)(pt[1].y + y_p),
        (int)(pt[2].x + x_p), (int)(pt[2].y + y_p), pt[3].y, pt[3].x);

    if (gobj->e_filled) {
        sys_vgui("-style pieslice -fill #%6.6x -outline #%6.6x -width 1 -tags { %s %s }\n",
            gobj->e_color, gobj->e_color, g->e_id->s_name, x->b_all_id->s_name);
    } else {
        sys_vgui("-style arc -outline #%6.6x -width %.1f -tags { %s %s }\n",
            gobj->e_color, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
    }
}

static void ebox_do_paint_image(t_elayer* g, t_ebox* x, t_egobj const* gobj, float x_p, float y_p)
{
    t_pt const* pt = &gobj->e_points[0];

    sys_vgui("image create photo %s -width %u -height %u -data %s\n",
        gobj->e_image->name->s_name,
        (unsigned int)gobj->e_image->width,
        (unsigned int)gobj->e_image->height,
        gobj->e_image->data_base64);

    sys_vgui("%s create image %d %d -anchor %s -image %s -tags {%s %s}\n", x->b_drawing_id->s_name,
        (int)(pt[1].x + x_p), (int)(pt[1].y + y_p),
        anchor_to_symbol(gobj->e_image->anchor),
        gobj->e_image->name->s_name,
        g->e_id->s_name, x->b_all_id->s_name);
}

t_pd_err ebox_paint_layer(t_ebox* x, t_symbol* name, float x_p, float y_p)
{
    char header[256];
    char bottom[256];
    t_elayer* g = ebox_get_layer(x, name);
    if (g && g->e_state == EGRAPHICS_TODRAW) {
        for (const auto& gobj : g->e_objects) {
            if (gobj.e_type == E_GOBJ_PATH) {
                if (gobj.e_filled) {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-fill #%6.6x -width 0 -tags { %s %s }\n",
                        gobj.e_color, g->e_id->s_name, x->b_all_id->s_name);
                } else {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-fill #%6.6x -width %.1f -capstyle %s %s %s -tags { %s %s }\n",
                        gobj.e_color, gobj.e_width,
                        my_capstylelist[gobj.e_capstyle],
                        my_dashstylelist[gobj.e_dashstyle],
                        gobj.e_smooth == ESMOOTH_NONE ? "" : "-smooth true",
                        g->e_id->s_name, x->b_all_id->s_name);
                }

                int mode = E_PATH_MOVE;
                for (int j = 0; j < gobj.e_points.size();) {
                    switch ((int)gobj.e_points[j].x) {
                    case E_PATH_MOVE:
                        if (mode != E_PATH_MOVE) // output previous
                            sys_vgui("%s", bottom);

                        sys_vgui("%s", header);
                        sys_vgui("%d %d ",
                            (int)(gobj.e_points[j + 1].x + x_p),
                            (int)(gobj.e_points[j + 1].y + y_p));
                        j += 2;
                        mode = E_PATH_MOVE;
                        break;
                    case E_PATH_LINE:
                        sys_vgui("%d %d ",
                            (int)(gobj.e_points[j + 1].x + x_p),
                            (int)(gobj.e_points[j + 1].y + y_p));
                        j += 2;
                        mode = E_PATH_LINE;
                        break;
                    case E_PATH_CURVE:
                        sys_vgui("%d %d %d %d %d %d ",
                            (int)(gobj.e_points[j + 1].x + x_p), (int)(gobj.e_points[j + 1].y + y_p),
                            (int)(gobj.e_points[j + 2].x + x_p), (int)(gobj.e_points[j + 2].y + y_p),
                            (int)(gobj.e_points[j + 3].x + x_p), (int)(gobj.e_points[j + 3].y + y_p));
                        j += 4;
                        mode = E_PATH_CURVE;
                        break;
                    case E_PATH_CLOSE:
                        j += 1;
                        mode = E_PATH_CLOSE;
                        break;
                    }
                }
                sys_vgui("%s", bottom);
            } else if (gobj.e_type == E_GOBJ_TEXT) {
                int zoom = ebox_getzoom(x);
                sys_vgui("%s create text %d %d -text {%s} "
                         "-anchor %s -justify %s -font {{%s} %d %s %s} "
                         "-fill #%6.6x -width %d -tags { %s %s }\n",
                    x->b_drawing_id->s_name,
                    (int)(gobj.e_points[0].x + x_p),
                    (int)(gobj.e_points[0].y + y_p),
                    gobj.e_text,
                    anchor_to_symbol(gobj.e_anchor),
                    justify_to_symbol(gobj.e_justify),
                    gobj.e_font.c_family->s_name,
                    (int)gobj.e_font.c_size * zoom,
                    gobj.e_font.c_weight->s_name,
                    gobj.e_font.c_slant->s_name,
                    gobj.e_color,
                    (int)(gobj.e_points[1].x - gobj.e_points[0].x),
                    g->e_id->s_name,
                    x->b_all_id->s_name);

            } else if (gobj.e_type == E_GOBJ_SHAPE) {
                int type = gobj.e_points[0].x;
                switch (type) {
                case E_SHAPE_RECT:
                    ebox_do_paint_rect(g, x, &gobj, x_p, y_p);
                    break;
                case E_SHAPE_OVAL:
                    ebox_do_paint_oval(g, x, &gobj, x_p, y_p);
                    break;
                case E_SHAPE_ARC:
                    ebox_do_paint_arc(g, x, &gobj, x_p, y_p);
                    break;
                case E_SHAPE_IMAGE:
                    ebox_do_paint_image(g, x, &gobj, x_p, y_p);
                    break;
                default:
                    pd_error(x, "unknown shape: %d", type);
                    break;
                }
            } else {
                return -1;
            }
        }
        g->e_state = EGRAPHICS_CLOSE;
    } else {
        return -1;
    }

    return 0;
}

static void ebox_draw_border(t_ebox* x)
{
    const bool selected = (x->b_selected_box == EITEM_OBJ);
    const int color = selected ? rgba_to_hex_int(rgba_blue)
                               : rgba_to_hex_int(x->b_boxparameters.d_bordercolor);

    sys_vgui("::ceammc::ui::border_draw %s %lx %d %d %d #%6.6x\n",
        x->b_canvas_id->s_name, x,
        int(x->b_rect.width), int(x->b_rect.height), int(x->b_zoom),
        color);
}

static void do_draw_inlets(const char* cnv, t_object* x, int w, int h, int zoom, ceammc::XletGetAnnotationFn fn)
{
    const int N_IN = obj_ninlets(x);
    char buf[N_IN + 1];

    for (int i = 0; i < N_IN; i++)
        buf[i] = obj_issignalinlet(x, i) ? '~' : '_';

    buf[N_IN] = '\0';

    sys_vgui("::ceammc::ui::inlets_draw %s %lx %d %d %d \"%s\"\n",
        cnv, x, w, h, zoom, buf);

    if (fn) {
        for (int i = 0; i < N_IN; i++) {
            const char* str = fn(x, ceammc::XLET_IN, i);
            if (str != nullptr && str[0] != '\0')
                sys_vgui("::ceammc::ui::inlet_tooltip %s %lx %d {%s}\n", cnv, x, i, str);
        }
    }
}

static void do_draw_outlets(const char* cnv, t_object* x, int w, int h, int zoom, ceammc::XletGetAnnotationFn fn)
{
    const int N_OUT = obj_noutlets(x);
    char buf[N_OUT + 1];

    for (int i = 0; i < N_OUT; i++)
        buf[i] = obj_issignaloutlet(x, i) ? '~' : '_';

    buf[N_OUT] = '\0';

    sys_vgui("::ceammc::ui::outlets_draw %s %lx %d %d %d \"%s\"\n",
        cnv, x, w, h, zoom, buf);

    if (fn) {
        for (int i = 0; i < N_OUT; i++) {
            const char* str = fn(x, ceammc::XLET_OUT, i);
            if (str != nullptr && str[0] != '\0') {
                sys_vgui("::ceammc::ui::outlet_tooltip %s %lx %d {%s}\n", cnv, x, i, str);
            }
        }
    }
}

static void ebox_draw_iolets(t_ebox* x)
{
    if (!x->b_boxparameters.d_hideiolets) {
        if (!x->b_isinsubcanvas) {
            t_object* obj = reinterpret_cast<t_object*>(x);

            auto ann_fn = ceammc::ceammc_get_annotation_fn(&obj->te_g.g_pd);

            do_draw_inlets(x->b_canvas_id->s_name, obj,
                x->b_rect.width, x->b_rect.height, x->b_zoom, ann_fn);

            do_draw_outlets(x->b_canvas_id->s_name, obj,
                x->b_rect.width, x->b_rect.height, x->b_zoom, ann_fn);
        }
    }
}

static void ebox_invalidate_all(t_ebox* x)
{
    if (!x->b_layers)
        return;

    for (auto& l : *x->b_layers)
        l.e_state = EGRAPHICS_INVALID;
}

static void layers_erase(t_ebox* x)
{
    if (!x->b_layers)
        return;

    for (auto& l : *x->b_layers) {
        if (l.e_state == EGRAPHICS_INVALID)
            sys_vgui("%s delete %s\n", x->b_drawing_id->s_name, l.e_id->s_name);
    }
}

static void ebox_erase(t_ebox* x)
{
    if (x->b_obj.o_canvas && glist_isvisible(x->b_obj.o_canvas) && x->b_have_window) {
        if (x->b_obj.o_canvas->gl_havewindow) {
            // erase callback
            t_eclass* c = eobj_getclass(&x->b_obj);
            if (c->c_widget.w_erase)
                c->c_widget.w_erase(x);

            ebox_erase_label(x);
        }

        sys_vgui("::ceammc::ui::widget_delete %s %lx\n", x->b_canvas_id->s_name, x);
        x->b_have_window = false;
    }

    if (x->b_layers) {
        for (auto& l : *x->b_layers)
            elayer_free_content(l);

        delete x->b_layers;
        x->b_layers = nullptr;
    }
}

static void ebox_select(t_ebox* x)
{
    if (ebox_isvisible(x)) {
        const int color = (x->b_selected_box == EITEM_OBJ)
            ? rgba_to_hex_int(rgba_blue)
            : rgba_to_hex_int(x->b_boxparameters.d_bordercolor);

        sys_vgui("::ceammc::ui::widget_select %s %lx #%6.6x\n",
            x->b_canvas_id->s_name, x, color);
    }
}

static void ebox_move(t_ebox* x)
{
    if (ebox_isvisible(x)) {
        if (x->wis_canvas) {
            auto pos = ebox_calc_pos(x, x->wis_canvas);
            x->b_rect.x = pos.x;
            x->b_rect.y = pos.y;
        }

        sys_vgui("::ceammc::ui::widget_pos %s %lx %d %d\n",
            x->b_canvas_id->s_name, x,
            (int)x->b_rect.x, (int)x->b_rect.y);

        if (x->b_label != s_null)
            ebox_update_label_pos(x);
    }

    canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
}

void ebox_setzoom(t_ebox* x, t_float f)
{
    x->b_zoom = f;

    t_eclass* c = eobj_getclass(&x->b_obj);
    if (c->c_widget.w_onzoom)
        c->c_widget.w_onzoom(x, f);
}

static void ebox_newzoom(t_ebox* x)
{
    t_canvas* c = eobj_getcanvas(&x->b_obj);
    if (!c)
        return;

    int z = glist_getzoom(c);
    if (z < 1)
        z = 1;

    if (z != 1) {
        x->b_zoom = z;
    }
}

float ebox_getzoom(t_ebox* x)
{
    return x->b_zoom < 1 ? 1 : x->b_zoom;
}

float ebox_getzoomfontsize(t_ebox* x)
{
    return x->b_font.c_size * ebox_getzoom(x);
}

float ebox_fontwidth(t_ebox* x)
{
    return sys_zoomfontwidth(ebox_getfontsize(x), ebox_getzoom(x), 0);
}

float ebox_fontheight(t_ebox* x)
{
    return sys_zoomfontheight(ebox_getfontsize(x), ebox_getzoom(x), 1);
}

void elayer_free_content(t_elayer& l)
{
    l.e_objects.clear();
    l.e_new_objects.e_points.clear();
}

void ebox_free_layer(t_elayer* l)
{
    if (!l)
        return;

    elayer_free_content(*l);
    free(l);
}

bool ebox_isvisible(t_ebox* x)
{
    return x && ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_have_window && x->b_visible;
}
