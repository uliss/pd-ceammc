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
#include "egraphics.h"
#include "eobj.h"
#include "g_style.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include <iostream>
#include <string>
#include <tuple>

int egraphics_smooth();

std::string ceammc_raute2dollar(const char* s);
t_symbol* ceammc_dollar2raute(const char* s);
std::string ceammc_quote_str(const std::string& str, char q = '\'');

static std::array<const char*, ECURSOR_XTERM + 1> my_cursorlist = {
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
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

static const char* SYM_SIZE = "size";
static const char* SYM_DEFAULT_FONT_FAMILY = "Helvetica";
static const char* SYM_BOLD = "bold";
static const char* SYM_NORMAL = "normal";
static const char* SYM_ITALIC = "italic";
static const char* SYM_ROMAN = "roman";
static const char* SYM_ALL_PROPS = "@*";

static const char* SYM_CHECKBUTTON = "checkbutton";
static const char* SYM_MENU = "menu";
static const char* SYM_COLOR = "color";
static const char* SYM_ENTRY = "entry";

#define LABEL_TAG "label_%s"

static void ebox_create_window(t_ebox* x, t_glist* glist);
static void ebox_invalidate_all(t_ebox* x);
static void ebox_draw_border(t_ebox* x);
static void ebox_draw_iolets(t_ebox* x);
static void layers_erase(t_ebox* x);
static void ebox_erase(t_ebox* x);
static void ebox_select(t_ebox* x);
static void ebox_move(t_ebox* x);
static void ebox_attrprocess_default(void* x);
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

static t_symbol* label_draw_id(t_ebox* x)
{
    if (x->label_inner == 0)
        return x->b_canvas_id;
    else
        return x->b_drawing_id;
}

static void ebox_erase_label(t_ebox* x)
{
    if (x->b_label != s_null)
        sys_vgui("%s delete " LABEL_TAG "\n", label_draw_id(x)->s_name, x->b_canvas_id->s_name);
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

static const char* ebox_label_anchor(t_ebox* x,
    LabelPosition pos, LabelSide side, LabelAlign align, LabelVAlign valign)
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
        const int w = x->b_rect.width * x->b_zoom;
        const int h = x->b_rect.height * x->b_zoom;
        const int xc = w * 0.5;
        const int yc = h * 0.5;

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
        int x0 = x->b_rect.x;
        int y0 = x->b_rect.y;
        int x1 = x0 + x->b_rect.width * x->b_zoom;
        int y1 = y0 + x->b_rect.height * x->b_zoom;
        int xc = x0 + x->b_rect.width * x->b_zoom * 0.5;
        int yc = y0 + x->b_rect.height * x->b_zoom * 0.5;

        switch (side) {
        case LABEL_SIDE_LEFT: {
            const int margin_right = x0 - (x->label_margins[0] + MIN_MARGIN) * x->b_zoom;
            const int margin_top = y0 + x->label_margins[1] * x->b_zoom;
            const int margin_bottom = y1 - x->label_margins[1] * x->b_zoom;

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
            const int margin_left = x0 + x->label_margins[0] * x->b_zoom;
            const int margin_right = x1 - x->label_margins[0] * x->b_zoom;
            const int margin_bottom = y0 - (x->label_margins[1] + MIN_MARGIN) * x->b_zoom;

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
            const int margin_left = x0 + x->label_margins[0] * x->b_zoom;
            const int margin_right = x1 - x->label_margins[0] * x->b_zoom;
            const int margin_top = y1 + (x->label_margins[1] + MIN_MARGIN) * x->b_zoom;

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
            const int margin_left = x1 + (x->label_margins[0] + MIN_MARGIN) * x->b_zoom;
            const int margin_top = y0 + x->label_margins[1] * x->b_zoom;
            const int margin_bottom = y1 - x->label_margins[1] * x->b_zoom;

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

static std::tuple<LabelPosition, LabelSide, LabelAlign, LabelVAlign> label_enums(t_ebox* x)
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

    sys_vgui("%s create text %d %d -anchor %s "
             "-justify %s "
             "-text {%s} -fill #%6.6x "
             "-font {{%s} %d roman normal} "
             "-tags { " LABEL_TAG " }\n",
        label_draw_id(x)->s_name,
        pt.first, pt.second,
        ebox_label_anchor(x, std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums)),
        x->label_align->s_name,
        x->b_label->s_name,
        rgba_to_hex_int(x->b_boxparameters.d_labelcolor),
        x->b_font.c_family->s_name,
        (int)(x->b_font.c_sizereal * x->b_zoom),
        x->b_canvas_id->s_name);
}

static void ebox_update_label_pos(t_ebox* x)
{
    if (ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_visible && x->b_label != s_null) {
        auto enums = label_enums(x);

        t_symbol* cnv = label_draw_id(x);

        sys_vgui("%s itemconfigure " LABEL_TAG " -anchor %s -justify %s\n",
            cnv->s_name,
            x->b_canvas_id->s_name,
            ebox_label_anchor(x, std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums)),
            x->label_align->s_name);

        auto pt = ebox_label_coord(x, std::get<0>(enums), std::get<1>(enums), std::get<2>(enums), std::get<3>(enums));
        sys_vgui("%s coords " LABEL_TAG " %d %d\n",
            cnv->s_name,
            x->b_canvas_id->s_name,
            pt.first,
            pt.second);
    }
}

static void ebox_update_label_font(t_ebox* x)
{
    if (ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_visible && x->b_label != s_null) {
        t_symbol* cnv = label_draw_id(x);

        sys_vgui("%s itemconfigure " LABEL_TAG " -font {{%s} %d roman normal}\n",
            cnv->s_name,
            x->b_canvas_id->s_name,
            x->b_font.c_family->s_name,
            (int)(x->b_font.c_sizereal * x->b_zoom));
    }
}

void ebox_new(t_ebox* x, long flags)
{
    x->b_flags = flags;
    x->b_ready_to_draw = 0;
    x->b_number_of_layers = 0;
    x->b_layers = NULL;
    x->b_window_id = NULL;
    x->b_receive_id = s_null;
    x->b_send_id = s_null;
    x->b_objpreset_id = s_null;
    x->b_visible = 1;
    x->b_zoom = 1;
    x->b_smooth_method = egraphics_smooth();
    x->b_force_redraw = 0;

    x->b_label = s_null;
    x->label_align = s_value_label_align_left;
    x->label_valign = s_value_label_valign_center;
    x->label_inner = 0;
    x->label_side = gensym("left");
    x->label_margins[0] = 0;
    x->label_margins[1] = 0;

    eobj_getclass(x)->c_widget.w_dosave = (t_typ_method)ebox_dosave;
    ebox_attrprocess_default(x);
}

void ebox_ready(t_ebox* x)
{
    t_eclass* c = eobj_getclass(x);
    x->b_selected_item = EITEM_NONE;
    x->b_selected_box = 0;
    x->b_selected_inlet = -1;
    x->b_selected_outlet = -1;
    x->b_mouse_down = 0;
    x->b_resize = 0;
    x->b_zoom = 1;

    x->b_boxparameters.d_borderthickness = 1;
    x->b_boxparameters.d_bordercolor = rgba_black;
    x->b_boxparameters.d_boxfillcolor = rgba_white;
    x->b_boxparameters.d_labelcolor = rgba_black;

    if (c->c_widget.w_getdrawparameters)
        c->c_widget.w_getdrawparameters(x, &x->b_boxparameters);

    x->b_ready_to_draw = 1;

    ebox_newzoom(x);
}

void ebox_free(t_ebox* x)
{
    eobj_free(x);
    if (x->b_receive_id && x->b_receive_id != s_null) {
        // replace #n => $d
        t_symbol* sname_dollar = gensym(ceammc_raute2dollar(x->b_receive_id->s_name).c_str());
        t_symbol* sname = canvas_realizedollar(eobj_getcanvas(x), sname_dollar);
        pd_unbind((t_pd*)x, sname);
    }
    gfxstub_deleteforkey(x);
    if (eobj_isdsp(x)) {
        eobj_dspfree(x);
    }
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
        t_symbol* dollar_sname = gensym(ceammc_raute2dollar(x->b_send_id->s_name).c_str());
        t_symbol* sname = canvas_realizedollar(eobj_getcanvas(x), dollar_sname);

        if (sname && sname->s_thing)
            return sname->s_thing;
    }

    return nullptr;
}

bool ebox_isdrawable(t_ebox* x)
{
    if (eobj_isbox(x) && x->b_obj.o_canvas) {
        if (x->b_ready_to_draw && glist_isvisible(x->b_obj.o_canvas)) {
            return true;
        }
    }
    return false;
}

void ebox_set_cursor(t_ebox* x, t_cursor cursor)
{
    sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[cursor]);
}

void ebox_attrprocess_viatoms(void* x, int argc, t_atom* argv)
{
    char buffer[MAXPDSTRING];
    int defc = 0;
    t_atom* defv = NULL;
    t_eclass* c = eobj_getclass(x);

    for (int i = 0; i < c->c_nattr; i++) {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void ebox_attrprocess_viabinbuf(void* x, t_binbuf* d)
{
    char attr_name[MAXPDSTRING];

    int defc = 0;
    t_atom* defv = NULL;
    t_eclass* c = eobj_getclass(x);
    for (int i = 0; i < c->c_nattr; i++) {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

static void ebox_attrprocess_default(void* x)
{
    int i, j, k;
    long defc = 0;
    t_atom* defv = NULL;
    t_eclass* c = eobj_getclass(x);

    for (i = 0; i < c->c_nattr; i++) {
        if (c->c_attr[i]->defvals) {
            defc = c->c_attr[i]->size;
            defv = (t_atom*)calloc((unsigned long)defc, sizeof(t_atom));
            if (defc && defv) {
                char check = 0;
                const char* str_start = c->c_attr[i]->defvals->s_name;
                char* str_end = nullptr;
                for (j = 0; j < defc; j++) {
                    for (k = 0; k < (int)(strlen(str_start)); k++) {
                        if (isalpha(str_start[k]))
                            check = 1;
                    }
                    if (check || strpbrk(str_start, "<>()'\"")) {
                        atom_setsym(defv + j, gensym(str_start));
                    } else {
                        float val = std::strtof(str_start, &str_end);
                        atom_setfloat(defv + j, val);
                        str_start = str_end;
                    }
                }
                eobj_attr_setvalueof(x, c->c_attr[i]->name, (int)defc, defv);
            }
            if (defv) {
                free(defv);
                defv = NULL;
            }
        }
    }
}

//! Widget
void ebox_wgetrect(t_gobj* z, t_glist* glist, int* xp1, int* yp1, int* xp2, int* yp2)
{
    t_ebox* x = (t_ebox*)z;
    *xp1 = text_xpix(&x->b_obj.o_obj, glist);
    *yp1 = text_ypix(&x->b_obj.o_obj, glist) - (int)(x->b_boxparameters.d_borderthickness);
    *xp2 = *xp1 + (int)x->b_rect.width * x->b_zoom + (int)(x->b_boxparameters.d_borderthickness);
    *yp2 = *yp1 + (int)x->b_rect.height * x->b_zoom + (int)(x->b_boxparameters.d_borderthickness);
}

static void ebox_paint(t_ebox* x)
{
    layers_erase(x);

    // prevent crash if called by some reason before calling
    // create_widget
    if (!x->b_drawing_id)
        return;

    sys_vgui("%s configure -bg #%6.6x\n",
        x->b_drawing_id->s_name, rgba_to_hex_int(x->b_boxparameters.d_boxfillcolor));

    if (x->b_pinned)
        sys_vgui("lower %s\n", x->b_drawing_id->s_name);

    t_eclass* c = eobj_getclass(x);
    if (c->c_widget.w_paint)
        c->c_widget.w_paint(x);

    if (x->b_label != s_null) {
        if (x->label_inner) {
            // raise up
            sys_vgui("%s raise " LABEL_TAG " %s\n",
                label_draw_id(x)->s_name, x->b_canvas_id->s_name, x->b_all_id->s_name);
        }

        // update label color
        sys_vgui("%s itemconfigure " LABEL_TAG " -fill #%6.6x\n",
            label_draw_id(x)->s_name, x->b_canvas_id->s_name, rgba_to_hex_int(x->b_boxparameters.d_labelcolor));
    }

    ebox_draw_border(x);
    ebox_draw_iolets(x);
}

//! Widget
void ebox_wvis(t_gobj* z, t_glist* glist, int vis)
{
    t_ebox* x = (t_ebox*)z;
    if (vis) {
        if (eobj_isbox(x) && x->b_ready_to_draw && x->b_visible) {
            ebox_invalidate_all(x);
            ebox_create_window(x, glist);
            ebox_paint(x);
        }
    } else {
        ebox_erase(x);
        canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
    }
}

//! Widget
void ebox_wdisplace(t_gobj* z, t_glist* glist, int dx, int dy)
{
#ifdef _WINDOWS
    t_ebox* x = (t_ebox*)z;
    if (x->b_selected_box) {
        x->b_rect.x += dx;
        x->b_rect.y += dy;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix += dy;
        ebox_move(x);
    }
#else
    t_ebox* x = (t_ebox*)z;

    x->b_rect.x += dx;
    x->b_rect.y += dy;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix += dy;
    ebox_move(x);
#endif
}

//! Widget
void ebox_wselect(t_gobj* z, t_glist* glist, int selected)
{
    t_ebox* x = (t_ebox*)z;
    if (selected)
        x->b_selected_box = 1;
    else
        x->b_selected_box = 0;
    ebox_select(x);
}

//! Widget
void ebox_wdelete(t_gobj* z, t_glist* glist)
{
    t_ebox* x = (t_ebox*)z;
    ebox_erase(x);
    canvas_deletelinesfor(glist, (t_text*)z);
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
    sprintf(buffer, ".x%" PRIxPTR ".c", (uintptr_t)canvas);
    x->b_canvas_id = gensym(buffer);
    sprintf(buffer, "%s.ecanvas%" PRIxPTR, x->b_canvas_id->s_name, (uintptr_t)x);
    x->b_drawing_id = gensym(buffer);
    sprintf(buffer, "%s.ewindow%" PRIxPTR, x->b_canvas_id->s_name, (uintptr_t)x);
    x->b_window_id = gensym(buffer);
    sprintf(buffer, "all%" PRIxPTR, (uintptr_t)x);
    x->b_all_id = gensym(buffer);
}

static void ebox_bind_events(t_ebox* x)
{
    t_eclass* c = (t_eclass*)eobj_getclass(x);
    sys_vgui("bind %s <Button-3> {+pdsend {%s mousedown %%x %%y %%X %%Y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
    sys_vgui("bind %s <Button-2> {+pdsend {%s mousedown %%x %%y %%X %%Y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
    sys_vgui("bind %s <Button-1> {+pdsend {%s mousedown %%x %%y %%X %%Y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <ButtonRelease> {+pdsend {%s mouseup %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Motion> {+pdsend {%s mousemove %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    sys_vgui("bind %s <Enter> {+pdsend {%s mouseenter}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Leave> {+pdsend {%s mouseleave}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    if (c->c_widget.w_dblclick) {
        sys_vgui("bind %s <Double-Button-1> {+pdsend {%s dblclick %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
    if (c->c_widget.w_mousewheel) {
        sys_vgui("bind %s <MouseWheel> {+pdsend {%s mousewheel  %%x %%y %%D %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
    if (c->c_widget.w_key || c->c_widget.w_keyfilter) {
        sys_vgui("bind %s <Key>  {+pdsend {%s key  %%k %%N}} \n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
}

static void ebox_create_widget(t_ebox* x)
{
    sys_vgui("namespace eval ebox%lx {} \n", x);
    sys_vgui("destroy %s \n", x->b_drawing_id->s_name);

    sys_vgui("canvas %s -width %d -height %d -bd 0 -highlightthickness 0 -insertborderwidth 0 -state normal -takefocus 1 -insertwidth 0 -confine 0\n",
        x->b_drawing_id->s_name,
        (int)(x->b_rect.width * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.),
        (int)(x->b_rect.height * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.));
}

static void ebox_create_window(t_ebox* x, t_glist* glist)
{
    x->b_force_redraw = 0;
    if (!glist->gl_havewindow) {
        x->b_isinsubcanvas = 1;
        x->b_rect.x = x->b_obj.o_obj.te_xpix;
        x->b_rect.y = x->b_obj.o_obj.te_ypix;

        while (!glist->gl_havewindow) {
            x->b_rect.x -= glist->gl_xmargin;
            x->b_rect.y -= glist->gl_ymargin;
            x->b_rect.x += glist->gl_obj.te_xpix;
            x->b_rect.y += glist->gl_obj.te_ypix;
            glist = glist->gl_owner;
        }
    } else {
        x->b_isinsubcanvas = 0;
        x->b_rect.x = x->b_obj.o_obj.te_xpix;
        x->b_rect.y = x->b_obj.o_obj.te_ypix;
    }

    ebox_tk_ids(x, glist_getcanvas(glist));
    ebox_create_widget(x);
    ebox_bind_events(x);

    sys_vgui("%s create window %d %d -anchor nw -window %s -tags %s -width %d -height %d\n",
        x->b_canvas_id->s_name,
        (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness),
        (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness),
        x->b_drawing_id->s_name,
        x->b_window_id->s_name,
        (int)(x->b_rect.width * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.),
        (int)(x->b_rect.height * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.));

    if (x->b_label != s_null)
        ebox_create_label(x);
}

static char is_platform_control(long mod)
{
#ifdef _WINDOWS
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
    if (mod >= 256) {
        mod -= 256;
    }
#elif _WINDOWS

    if (mod >= 131072) {
        mod -= 131072;
        mod += EMOD_ALT;
    }
/*else
        mod -= 8;*/
#else
    if (mod == 24) //right click
        mod = EMOD_CMD;
    else if (mod & EMOD_CMD) {
        mod ^= EMOD_CMD;
        mod |= EMOD_ALT;
    }
#endif
    //post("MOD : %ld", mod);
    return mod;
}

void ebox_mouse_enter(t_ebox* x)
{
    t_eclass* c = eobj_getclass(x);

    if (!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        sys_vgui("focus %s\n", x->b_drawing_id->s_name);
        if (c->c_widget.w_mouseenter) {
            c->c_widget.w_mouseenter(x);
        }
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass* c = eobj_getclass(x);

    if (!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        if (c->c_widget.w_mouseleave) {
            c->c_widget.w_mouseleave(x);
        }
        ebox_set_cursor(x, ECURSOR_LEFT_PTR);
    } else if (x->b_obj.o_canvas->gl_edit && !x->b_mouse_down) {
        ebox_set_cursor(x, ECURSOR_HAND);
    }
}

void ebox_mouse_move(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_pt mouse;
    t_atom av[2];
    long modif = modifier_wrapper((long)atom_getfloat(argv + 2));
    t_eclass* c = eobj_getclass(x);

    // mouse move
    if (!x->b_mouse_down) {
        if (is_for_box(x, modif)) {
            if (!(x->b_flags & EBOX_IGNORELOCKCLICK)) {
                ebox_set_cursor(x, ECURSOR_CENTER_PTR);
                if (c->c_widget.w_mousemove) {
                    mouse.x = atom_getfloat(argv);
                    mouse.y = atom_getfloat(argv + 1);
                    c->c_widget.w_mousemove(x, x->b_obj.o_canvas, mouse, modif);
                }
            } else {
                ebox_set_cursor(x, ECURSOR_LEFT_PTR);
            }
        } else if (!x->b_isinsubcanvas) {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);
            x->b_selected_outlet = -1;
            x->b_selected_inlet = -1;
            x->b_selected_item = EITEM_NONE;
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);

            const int right = (int)(x->b_rect.width * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.);
            const int bottom = (int)(x->b_rect.height * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.);
            const int CURSOR_AREA = 3;

            // BOTTOM & RIGHT //
            if (mouse.y > bottom - CURSOR_AREA
                && mouse.y <= bottom
                && mouse.x > right - CURSOR_AREA
                && mouse.x <= right) {

                x->b_selected_item = EITEM_CORNER;
                ebox_set_cursor(x, ECURSOR_RIGHT_CORNER);
                return;
            }
            // BOTTOM //
            else if (mouse.y > bottom - CURSOR_AREA && mouse.y <= bottom) {
                const int N = obj_noutlets((t_object*)x);
                const int XLET_W = 7;
                const int XLET_ZW = XLET_W * x->b_zoom;
                for (int i = 0; i < N; i++) {
                    int pos_x_outlet = 0;
                    if (N > 1)
                        pos_x_outlet = (int)(i / (float)(N - 1) * (x->b_rect.width * x->b_zoom - (XLET_ZW + 1)));

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
                mouse.x = atom_getfloat(argv);
                mouse.y = atom_getfloat(argv + 1);
                c->c_widget.w_mousedrag(x, x->b_obj.o_canvas, mouse, modif);
            }
        } else if (!x->b_isinsubcanvas) {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);
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
                x->b_resize = 1;
                mess3((t_pd*)x, s_attr_size, s_attr_size, (void*)2, (void*)av);
                x->b_resize = 0;
            }
        } else {
            sys_vgui("eobj_canvas_motion %s 1\n", x->b_canvas_id->s_name);
        }
    }
}

void ebox_mouse_down(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc != 5) {
        fprintf(stderr, "[ebox_mouse_down] warning: not enough arguments: %d", argc);
        return;
    }

    long modif = modifier_wrapper((long)atom_getfloat(argv + 4));
    t_eclass* c = eobj_getclass(x);
    if (is_for_box(x, modif)) {
        if (c->c_widget.w_mousedown && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
            t_pt mouse;
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);

            t_pt mouse_abs;
            mouse_abs.x = atom_getfloat(argv + 2);
            mouse_abs.y = atom_getfloat(argv + 3);

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
    x->b_mouse_down = 1;
}

void ebox_mouse_up(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_pt mouse;
    long modif = modifier_wrapper((long)atom_getfloat(argv + 2));
    t_eclass* c = eobj_getclass(x);
    if (is_for_box(x, modif)) {
        if (c->c_widget.w_mouseup && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);
            c->c_widget.w_mouseup(x, x->b_obj.o_canvas, mouse, modif);
        }
    } else {
        sys_vgui("eobj_canvas_up %s\n", x->b_canvas_id->s_name);
    }
    x->b_mouse_down = 0;
}

void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_pt mouse;
    t_eclass* c = eobj_getclass(x);
    long modif = modifier_wrapper((long)atom_getfloat(argv + 2));
    if (is_for_box(x, modif) && c->c_widget.w_dblclick && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
        mouse.x = atom_getfloat(argv);
        mouse.y = atom_getfloat(argv + 1);
        c->c_widget.w_dblclick(x, x->b_obj.o_canvas, mouse, modif);
    }
}

void ebox_mouse_wheel(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_pt mouse;
    long modif = modifier_wrapper((long)atom_getfloat(argv + 3));
    t_eclass* c = eobj_getclass(x);
    if (is_for_box(x, modif) && c->c_widget.w_mousewheel && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
        // map mouse window coords to widget coords
        mouse.x = atom_getfloat(argv) - x->b_rect.x;
        mouse.y = atom_getfloat(argv + 1) - x->b_rect.y;
        double delta = atom_getfloat(argv + 2);
        c->c_widget.w_mousewheel(x, x->b_obj.o_canvas, mouse, modif, delta);
    }
}

void ebox_key(t_ebox* x, t_symbol* s, int argc, t_atom* argv)
{
    t_eclass* c = eobj_getclass(x);

    if (argc >= 2 && argv && atom_gettype(argv + 1) == A_FLOAT) {
        if (!x->b_obj.o_canvas->gl_edit) {
            if (atom_getfloat(argv + 1) == 65288) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_DEL, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_DEL, 0);
                }
            } else if (atom_getfloat(argv + 1) == 65289) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_TAB, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_TAB, 0);
                }
            } else if (atom_getfloat(argv + 1) == 65293) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ENTER, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_ENTER, 0);
                }
            } else if (atom_getfloat(argv + 1) == 65307) {
                if (c->c_widget.w_keyfilter) {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ESC, 0);
                } else if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, EKEY_ESC, 0);
                }
            } else {
                if (c->c_widget.w_key) {
                    c->c_widget.w_key(x, NULL, (char)atom_getfloat(argv + 1), 0);
                }
            }
        }
    }
}

void ebox_dosave(t_ebox* x, t_binbuf* b)
{
    int i = 0, state = 0, argc = 0;
    char attr_name[MAXPDSTRING];
    t_atom* argv = NULL;
    t_eclass* c = eobj_getclass(x);
    if (c && b) {
        state = canvas_suspend_dsp();
        binbuf_addv(b, (char*)"ssiis", &s__X, s_obj, (int)x->b_obj.o_obj.te_xpix, (int)x->b_obj.o_obj.te_ypix, eobj_getclassname(x));
        for (i = 0; i < c->c_nattr; i++) {
            if (c->c_attr[i] && c->c_attr[i]->save && c->c_attr[i]->name) {
                argc = 0;
                argv = NULL;
                eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
                if (argc && argv) {
                    snprintf(attr_name, MAXPDSTRING, "@%s", c->c_attr[i]->name->s_name);
                    binbuf_append_attribute(b, gensym(attr_name), argc, argv);
                    free(argv);
                }
            }
        }

        if (c->c_widget.w_save != NULL)
            c->c_widget.w_save(x, b);

        binbuf_addv(b, (char*)";");
        canvas_resume_dsp(state);
    }
}

void ebox_pos(t_ebox* x, float newx, float newy)
{
    x->b_rect.x = newx;
    x->b_rect.y = newy;
    x->b_obj.o_obj.te_xpix = (short)newx;
    x->b_obj.o_obj.te_ypix = (short)newy;

    ebox_move(x);
}

void ebox_vis(t_ebox* x, int vis)
{
    vis = (int)pd_clip_minmax(vis, 0, 1);
    if (x->b_visible != vis) {
        x->b_visible = (char)vis;
        if (x->b_visible && x->b_ready_to_draw && x->b_obj.o_canvas) {
            ebox_redraw(x);
        } else {
            ebox_erase(x);
        }
    }
}

t_pd_err ebox_set_receiveid(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        t_symbol* new_sym = atom_getsymbol(argv);

        if (new_sym == x->b_receive_id)
            return 0; // no change

        // unbind previous
        if (x->b_receive_id != s_null) {
            // replace #n => $d
            t_symbol* sname_dollar = gensym(ceammc_raute2dollar(x->b_receive_id->s_name).c_str());
            t_symbol* sname = canvas_realizedollar(eobj_getcanvas(x), sname_dollar);
            if (sname)
                pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }

        // bind new
        x->b_receive_id = new_sym;
        t_symbol* sname_dollar = gensym(ceammc_raute2dollar(x->b_receive_id->s_name).c_str());
        t_symbol* sname = canvas_realizedollar(eobj_getcanvas(x), sname_dollar);
        pd_bind(&x->b_obj.o_obj.ob_pd, sname);
    } else {
        // unbind
        if (x->b_receive_id != s_null) {
            t_symbol* sname_dollar = gensym(ceammc_raute2dollar(x->b_receive_id->s_name).c_str());
            t_symbol* sname = canvas_realizedollar(eobj_getcanvas(x), sname_dollar);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }

        x->b_receive_id = s_null;
    }

    return 0;
}

t_pd_err ebox_set_sendid(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        x->b_send_id = atom_getsymbol(argv);
    } else {
        x->b_send_id = s_null;
    }

    return 0;
}

t_pd_err ebox_set_label(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        if (x->b_label == s_null) {
            x->b_label = atom_getsymbol(argv);
            // create new label
            if (ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_visible)
                ebox_create_label(x);
        } else {
            x->b_label = atom_getsymbol(argv);

            if (ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_visible) {
                sys_vgui("%s itemconfigure " LABEL_TAG " -text {%s}\n",
                    label_draw_id(x)->s_name,
                    x->b_canvas_id->s_name,
                    x->b_label->s_name);
            }
        }
    } else {
        if (x->b_label != s_null) {
            if (ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow && x->b_visible)
                ebox_erase_label(x);
        }

        x->b_label = s_null;
    }

    return 0;
}

t_pd_err ebox_set_label_align(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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
            pd_error(x, "[%s] invalid @label_align property value: %s", eobj_getclassname(x)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(x)->s_name, values.c_str());
            return 1;
        }

        x->label_align = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_valign(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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
            pd_error(x, "[%s] invalid @label_valign property value: %s", eobj_getclassname(x)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(x)->s_name, values.c_str());
            return 1;
        }

        x->label_valign = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_side(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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
            pd_error(x, "[%s] invalid @label_side property value: %s", eobj_getclassname(x)->s_name, s->s_name);

            std::string values;
            for (t_symbol* it : items) {
                values.push_back(' ');
                values += it->s_name;
            }

            pd_error(x, "[%s] supported values are:%s", eobj_getclassname(x)->s_name, values.c_str());
            return 1;
        }

        x->label_side = s;
        ebox_update_label_pos(x);
    }

    return 0;
}

t_pd_err ebox_set_label_position(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_FLOAT) {
        int pos = (atom_getfloat(argv) != 0) ? 1 : 0;

        if (x->label_inner != pos) {
            const bool is_drawable = ebox_isdrawable(x);

            if (is_drawable)
                ebox_erase_label(x);

            x->label_inner = pos;

            if (is_drawable)
                ebox_create_label(x);
        }
    }

    return 0;
}

t_pd_err ebox_set_label_margins(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc == 2 && argv && atom_gettype(argv) == A_FLOAT) {
        x->label_margins[0] = int(atom_getfloat(argv));
        x->label_margins[1] = int(atom_getfloat(argv + 1));
        ebox_update_label_pos(x);
    } else
        pd_error(x, "[%s] X Y margin pair expected", eobj_getclassname(x)->s_name);

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

t_pd_err ebox_set_presetid(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null) {
        x->b_objpreset_id = atom_getsymbol(argv);
    } else {
        x->b_objpreset_id = s_null;
    }
    return 0;
}

t_pd_err ebox_set_font(t_ebox* x, t_object* attr, int argc, t_atom* argv)
{
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        if (atom_getsymbol(argv) == s_null)
            x->b_font.c_family = gensym(SYM_DEFAULT_FONT_FAMILY);
        else
            x->b_font.c_family = atom_getsymbol(argv);
    } else
        x->b_font.c_family = gensym(SYM_DEFAULT_FONT_FAMILY);

    auto ftname = strdup(x->b_font.c_family->s_name);
    if (!ftname)
        return -1;

    auto ftname_uc = strtok(ftname, " ',.-");
    ftname_uc[0] = (char)toupper(ftname_uc[0]);
    x->b_font.c_family = gensym(ftname_uc);
    free(ftname);

    ebox_update_label_font(x);

    return 0;
}

t_pd_err ebox_set_fontweight(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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

t_pd_err ebox_set_fontslant(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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

t_pd_err ebox_set_fontsize(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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

t_pd_err ebox_size_set(t_ebox* x, t_object* attr, int argc, t_atom* argv)
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
    t_eclass* c = eobj_getclass(x);
    if (s == s_size) {
        if (c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->b_rect);
        ebox_invalidate_all(x);
        if (ebox_isdrawable(x)) {
            sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name,
                (int)(x->b_rect.width * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.),
                (int)(x->b_rect.height * x->b_zoom + x->b_boxparameters.d_borderthickness * 2.));
            canvas_fixlinesfor(x->b_obj.o_canvas, (t_text*)x);

            ebox_update_label_pos(x);
        }
        ebox_redraw(x);
    } else if (s == s_pinned && ebox_isdrawable(x)) {
        if (x->b_pinned) {
            sys_vgui("lower %s\n", x->b_drawing_id->s_name);
        } else {
            sys_vgui("raise %s\n", x->b_drawing_id->s_name);
        }
    }

    return true;
}

void ebox_attr_dump(t_ebox* x)
{
    t_object* xobj = (t_object*)(x);
    t_class* xc = xobj->te_pd;
    const char* name = class_getname(xc);

    // print methods
    for (int i = 0; i < xc->c_nmethod; i++) {
        // ignore property methods
        if (xc->c_methods[i].me_name->s_name[0] == '@')
            continue;

        post("[%s] method: %s", name, xc->c_methods[i].me_name->s_name);
    }

    // print xlets
    post("[%s] inlets: %i", name, obj_ninlets(xobj));
    post("[%s] outlets: %i", name, obj_noutlets(xobj));

    // print properties
    t_eclass* c = eobj_getclass(x);
    for (int i = 0; i < c->c_nattr; i++) {
        int argc = 0;
        t_atom* argv = 0;
        eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
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

    t_eclass* c = eobj_getclass(x);
    int argc = c->c_nattr;
    if (argc < 1) {
        post("[%s] no properties", class_getname(((t_object*)x)->te_pd));
        return;
    }

    char buf[MAXPDSTRING];
    t_atom* argv = (t_atom*)malloc(argc * sizeof(t_atom));
    for (int i = 0; i < c->c_nattr; i++) {
        sprintf(buf, "@%s", c->c_attr[i]->name->s_name);
        atom_setsym(&argv[i], gensym(buf));
    }

    outlet_anything(x->b_obj.o_obj.te_outlet, gensym(SYM_ALL_PROPS), argc, argv);
    free(argv);
}

void ebox_properties(t_ebox* x, t_glist* glist)
{
    t_eclass* c = eobj_getclass(x);
    char temp[MAXPDSTRING];

    sprintf(temp, "pdtk_%s_dialog %%s", c->c_class.c_name->s_name);
    std::string buffer(temp);

    for (int i = 0; i < c->c_nattr; i++) {
        if (!c->c_attr[i]->invisible) {
            t_atom* argv = 0;
            int argc = 0;
            eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
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
    t_eclass* c = eobj_getclass(x);
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
                snprintf(WIDGET_ID, sizeof(WIDGET_ID), "%s.t.w%i",
                    atom_getsymbol(argv)->s_name, attrindex + 1);

                eobj_attr_getvalueof((t_object*)x, c->c_attr[attrindex]->name, &ac, &av);

                if (ac && av) {
                    if (c->c_attr[attrindex]->style == gensym(SYM_CHECKBUTTON)) {
                        if (atom_getfloat(av) == 0)
                            sys_vgui("%s state !selected\n", WIDGET_ID);
                        else
                            sys_vgui("%s state selected\n", WIDGET_ID);
                    } else if (c->c_attr[attrindex]->style == gensym(SYM_COLOR)) {
                        color.red = atom_getfloat(av);
                        color.green = atom_getfloat(av + 1);
                        color.blue = atom_getfloat(av + 2);
                        sys_vgui("%s configure -readonlybackground #%6.6x\n", WIDGET_ID, rgb_to_hex_int(color));
                    } else if (c->c_attr[attrindex]->style == gensym(SYM_MENU)) {
                        atom_string(av, temp, MAXPDSTRING);
                        std::string buffer(temp);
                        for (int i = 1; i < ac; i++) {
                            atom_string(av + i, temp, MAXPDSTRING);
                            buffer.push_back(' ');
                            buffer += temp;
                        }
                        sys_vgui("%s delete 0 end\n", WIDGET_ID);
                        sys_vgui("%s insert 0 \"%s\"\n", WIDGET_ID, buffer.c_str());
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
                        sys_vgui("%s insert 0 [regsub -all {#(\\d+)} \"%s\" {$\\1}]\n",
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
    if ((ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow) || x->b_force_redraw) {
        x->b_force_redraw = 0;
        ebox_invalidate_layer(x, s_eboxbd);
        ebox_invalidate_layer(x, s_eboxio);
        ebox_paint(x);
    }
}

void ebox_redraw_inner(t_ebox* x)
{
    if ((ebox_isdrawable(x) && x->b_obj.o_canvas->gl_havewindow) || x->b_force_redraw) {
        x->b_force_redraw = 0;
        ebox_paint(x);
    }
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
    t_elayer *temp, *graphic;
    for (int i = 0; i < x->b_number_of_layers; i++) {
        graphic = &x->b_layers[i];
        if (graphic->e_name == name) {
            if (graphic->e_state == EGRAPHICS_INVALID) {
                graphic->e_owner = (t_object*)x;

                egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
                graphic->e_line_width = 1.f;
                graphic->e_line_capstyle = ECAPSTYLE_BUTT;
                graphic->e_line_dashstyle = EDASHSTYLE_NONE;
                graphic->e_color = 0;
                graphic->e_rect.x = 0.f;
                graphic->e_rect.y = 0.f;
                graphic->e_rect.height = (float)pd_clip_min(height, 0.);
                graphic->e_rect.width = (float)pd_clip_min(width, 0.);

                for (int j = 0; j < graphic->e_number_objects; j++) {
                    if (graphic->e_objects[j].e_npoints && graphic->e_objects[j].e_points) {
                        free(graphic->e_objects[j].e_points);
                    }
                    graphic->e_objects[j].e_points = NULL;
                    graphic->e_objects[j].e_npoints = 0;
                }
                if (graphic->e_objects) {
                    free(graphic->e_objects);
                    graphic->e_objects = NULL;
                }
                graphic->e_number_objects = 0;

                if (graphic->e_new_objects.e_points) {
                    free(graphic->e_new_objects.e_points);
                }
                graphic->e_new_objects.e_points = NULL;
                graphic->e_new_objects.e_npoints = 0;

                sprintf(text, "%s%" PRIdPTR, name->s_name, (intptr_t)x);
                graphic->e_id = gensym(text);

                graphic->e_new_objects.e_image = NULL;

                graphic->e_state = EGRAPHICS_OPEN;
                return &x->b_layers[i];
            } else {
                return NULL;
            }
        }
    }
    if (x->b_layers == NULL) {
        temp = (t_elayer*)calloc(1, sizeof(t_elayer));
    } else {
        temp = (t_elayer*)realloc(x->b_layers, (size_t)(x->b_number_of_layers + 1) * sizeof(t_elayer));
    }
    if (temp) {
        x->b_layers = temp;
        graphic = x->b_layers + x->b_number_of_layers;
        x->b_number_of_layers++;

        graphic->e_owner = (t_object*)x;

        egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
        graphic->e_line_width = 1.f;
        graphic->e_line_capstyle = ECAPSTYLE_BUTT;
        graphic->e_line_dashstyle = EDASHSTYLE_NONE;
        graphic->e_color = 0;
        graphic->e_rect.x = 0.f;
        graphic->e_rect.y = 0.f;
        graphic->e_rect.height = (float)pd_clip_min(height, 0.);
        graphic->e_rect.width = (float)pd_clip_min(width, 0.);

        graphic->e_number_objects = 0;
        graphic->e_new_objects.e_points = NULL;
        graphic->e_new_objects.e_npoints = 0;
        graphic->e_objects = NULL;

        graphic->e_name = name;
        sprintf(text, "%s%" PRIdPTR, name->s_name, (intptr_t)x);
        graphic->e_state = EGRAPHICS_OPEN;
        graphic->e_id = gensym(text);
        return graphic;
    } else {
        return NULL;
    }
}

t_pd_err ebox_end_layer(t_ebox* x, t_symbol* name)
{
    for (int i = 0; i < x->b_number_of_layers; i++) {
        if (x->b_layers[i].e_name == name) {
            x->b_layers[i].e_state = EGRAPHICS_TODRAW;
            return 0;
        }
    }
    return -1;
}

static inline t_elayer* ebox_get_layer(t_ebox* x, t_symbol const* name)
{
    for (int i = 0; i < x->b_number_of_layers; i++) {
        if (x->b_layers[i].e_name == name)
            return x->b_layers + i;
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
    t_pt const* pt = gobj->e_points;

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
    t_pt const* pt = gobj->e_points;

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
    t_pt const* pt = gobj->e_points;

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
    t_pt const* pt = gobj->e_points;

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
        x_p += x->b_boxparameters.d_borderthickness;
        y_p += x->b_boxparameters.d_borderthickness;
        for (int i = 0; i < g->e_number_objects; i++) {
            t_egobj const* gobj = g->e_objects + i;
            t_pt const* pt = gobj->e_points;
            t_pt start;
            if (gobj->e_type == E_GOBJ_PATH) {
                int mode = E_PATH_MOVE;
                if (gobj->e_filled) {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-fill #%6.6x -width 0 -tags { %s %s }\n",
                        gobj->e_color, g->e_id->s_name, x->b_all_id->s_name);
                } else {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-fill #%6.6x -width %.1f -capstyle %s %s -tags { %s %s }\n",
                        gobj->e_color, gobj->e_width,
                        my_capstylelist[gobj->e_capstyle],
                        my_dashstylelist[gobj->e_dashstyle],
                        g->e_id->s_name, x->b_all_id->s_name);
                }

                for (int j = 0; j < gobj->e_npoints;) {
                    if (pt[0].x == E_PATH_MOVE) {
                        if (mode != E_PATH_MOVE) {
                            sys_vgui("%s", bottom);
                        }
                        sys_vgui("%s", header);
                        sys_vgui("%d %d ", (int)(pt[1].x + x_p), (int)(pt[1].y + y_p));
                        start = pt[1];
                        pt += 2;
                        j += 2;
                        mode = E_PATH_MOVE;
                    } else if (pt[0].x == E_PATH_LINE) {
                        sys_vgui("%d %d ",
                            (int)(pt[1].x + x_p), (int)(pt[1].y + y_p));
                        pt += 2;
                        j += 2;
                        mode = E_PATH_LINE;
                    } else if (pt[0].x == E_PATH_CURVE) {
                        sys_vgui("%d %d %d %d %d %d ",
                            (int)(pt[1].x + x_p), (int)(pt[1].y + y_p),
                            (int)(pt[2].x + x_p), (int)(pt[2].y + y_p),
                            (int)(pt[3].x + x_p), (int)(pt[3].y + y_p));
                        pt += 4;
                        j += 4;
                        mode = E_PATH_CURVE;
                    } else if (pt[0].x == E_PATH_CLOSE) {
                        pt += 1;
                        j += 1;
                        mode = E_PATH_CLOSE;
                    }
                }
                sys_vgui("%s", bottom);
            } else if (gobj->e_type == E_GOBJ_TEXT) {
                int zoom = ebox_getzoom(x);
                sys_vgui("%s create text %d %d -text {%s} -anchor %s -justify %s -font {{%s} %d %s %s} -fill #%6.6x -width %d -tags { %s %s }\n",
                    x->b_drawing_id->s_name,
                    (int)(gobj->e_points[0].x + x_p),
                    (int)(gobj->e_points[0].y + y_p),
                    gobj->e_text,
                    anchor_to_symbol(gobj->e_anchor),
                    justify_to_symbol(gobj->e_justify),
                    gobj->e_font.c_family->s_name,
                    (int)gobj->e_font.c_size * zoom,
                    gobj->e_font.c_weight->s_name,
                    gobj->e_font.c_slant->s_name,
                    gobj->e_color,
                    (int)(gobj->e_points[1].x),
                    g->e_id->s_name,
                    x->b_all_id->s_name);

            } else if (gobj->e_type == E_GOBJ_SHAPE) {
                int type = gobj->e_points[0].x;
                switch (type) {
                case E_SHAPE_RECT:
                    ebox_do_paint_rect(g, x, gobj, x_p, y_p);
                    break;
                case E_SHAPE_OVAL:
                    ebox_do_paint_oval(g, x, gobj, x_p, y_p);
                    break;
                case E_SHAPE_ARC:
                    ebox_do_paint_arc(g, x, gobj, x_p, y_p);
                    break;
                case E_SHAPE_IMAGE:
                    ebox_do_paint_image(g, x, gobj, x_p, y_p);
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
    const float BRD_W = x->b_boxparameters.d_borderthickness;
    const float BOX_W = x->b_rect.width * x->b_zoom;
    const float BOX_H = x->b_rect.height * x->b_zoom;
    t_elayer* g = ebox_start_layer(x, s_eboxbd, BOX_W, BOX_H);

    if (g) {
        if (x->b_selected_box == EITEM_OBJ)
            egraphics_set_color_rgba(g, &rgba_blue);
        else
            egraphics_set_color_rgba(g, &x->b_boxparameters.d_bordercolor);

        egraphics_set_line_width(g, BRD_W * 2);
        egraphics_rectangle(g, 0, 0, BOX_W + BRD_W * 2, BOX_H + BRD_W * 2);
        egraphics_stroke(g);

        ebox_end_layer(x, s_eboxbd);
    }

    ebox_paint_layer(x, s_eboxbd, -BRD_W, -BRD_W);
}

static void ebox_draw_iolets(t_ebox* x)
{
    static const int XLET_W = 7;
    static const float XLET_H = 1;

    const float bdsize = x->b_boxparameters.d_borderthickness;
    const float BOX_W = x->b_rect.width * x->b_zoom;
    const float BOX_H = x->b_rect.height * x->b_zoom;

    t_elayer* g = ebox_start_layer(x, s_eboxio, BOX_W, BOX_H);

    if (g && !x->b_boxparameters.d_hideiolets) {
        if (!x->b_isinsubcanvas) {
            egraphics_set_line_width(g, 1);
            const float XW = XLET_W * x->b_zoom;
            const float XCTRLH = XLET_H;
            const float XSIGH = XCTRLH * x->b_zoom + 1;
            const t_object* obj = reinterpret_cast<t_object*>(x);

            const int N_IN = obj_ninlets(obj);
            for (int i = 0; i < N_IN; i++) {
                int pos_x_inlet = 0;
                if (N_IN != 1)
                    pos_x_inlet = (int)(i / (float)(N_IN - 1) * (BOX_W - (XW + 1)));

                const int is_sig = obj_issignalinlet(obj, i);
                if (is_sig)
                    egraphics_set_color_hex(g, STYLE_AUDIO_XLET_COLOR);
                else
                    egraphics_set_color_hex(g, STYLE_IEM_BORDER_COLOR);

                egraphics_rectangle(g, pos_x_inlet, 0, XW, (is_sig) ? XSIGH : XCTRLH);

                egraphics_stroke_preserve(g);
                egraphics_fill(g);
            }

            const int N_OUT = obj_noutlets(obj);
            for (int i = 0; i < N_OUT; i++) {
                int pos_x_outlet = 0;
                if (N_OUT != 1)
                    pos_x_outlet = (int)(i / (float)(N_OUT - 1) * (BOX_W - (XW + 1)));

                const int is_sig = obj_issignaloutlet(obj, i);
                float outlet_h;
                if (is_sig) {
                    outlet_h = XSIGH;
                    egraphics_set_color_hex(g, STYLE_AUDIO_XLET_COLOR);
                } else {
                    outlet_h = XCTRLH;
                    egraphics_set_color_hex(g, STYLE_IEM_BORDER_COLOR);
                }

                egraphics_rectangle(g, pos_x_outlet, BOX_H - (outlet_h + 1) + bdsize * 2, XW, outlet_h);

                egraphics_stroke_preserve(g);
                egraphics_fill(g);
            }
        }

        ebox_end_layer(x, s_eboxio);
    }
    ebox_paint_layer(x, s_eboxio, 0, -bdsize);
}

static void ebox_invalidate_all(t_ebox* x)
{
    for (int i = 0; i < x->b_number_of_layers; i++) {
        x->b_layers[i].e_state = EGRAPHICS_INVALID;
    }
}

static void layers_erase(t_ebox* x)
{
    for (int i = 0; i < x->b_number_of_layers; i++) {
        if (x->b_layers[i].e_state == EGRAPHICS_INVALID) {
            sys_vgui("%s delete %s\n", x->b_drawing_id->s_name, x->b_layers[i].e_id->s_name);
        }
    }
}

static void ebox_erase(t_ebox* x)
{
    if (x->b_obj.o_canvas && glist_isvisible(x->b_obj.o_canvas) && x->b_obj.o_canvas->gl_havewindow) {
        ebox_erase_label(x);
        // prevent double destroy
        x->b_label = s_null;
        sys_vgui("destroy %s \n", x->b_drawing_id->s_name);
    }
    if (x->b_layers) {
        for (long i = 0; i < x->b_number_of_layers; i++)
            elayer_free_content(x->b_layers[i]);

        free(x->b_layers);
        x->b_layers = NULL;
    }
    x->b_number_of_layers = 0;
}

static void ebox_select(t_ebox* x)
{
    if (glist_isvisible(x->b_obj.o_canvas)) {
        int color = (x->b_selected_box == EITEM_OBJ)
            ? rgba_to_hex_int(rgba_blue)
            : rgba_to_hex_int(x->b_boxparameters.d_bordercolor);

        sys_vgui("%s itemconfigure eboxbd%ld -outline #%6.6x\n",
            x->b_drawing_id->s_name, x, color);
    }
}

static void ebox_move(t_ebox* x)
{
    if (glist_isvisible(x->b_obj.o_canvas)) {
        sys_vgui("%s coords %s %d %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name,
            (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness),
            (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness));

        if (x->b_label != s_null)
            ebox_update_label_pos(x);
    }
    canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
}

void ebox_setzoom(t_ebox* x, float f)
{
    x->b_zoom = f;

    t_eclass* c = eobj_getclass(x);
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
    return x->b_zoom;
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
    for (int i = 0; i < l.e_number_objects; i++) {
        if (l.e_objects[i].e_npoints && l.e_objects[i].e_points) {
            free(l.e_objects[i].e_points);
        }
        l.e_objects[i].e_points = NULL;
        l.e_objects[i].e_npoints = 0;
    }

    if (l.e_objects) {
        free(l.e_objects);
        l.e_objects = NULL;
    }
}

void ebox_free_layer(t_elayer* l)
{
    if (!l)
        return;

    elayer_free_content(*l);
    free(l);
}
