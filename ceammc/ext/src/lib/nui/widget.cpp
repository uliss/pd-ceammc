/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "nui/widget.h"
#include "nui/tk_view_impl.h"

extern "C" {
#include "g_canvas.h"
}

#include <array>
#include <cstring>

namespace ceammc {
namespace ui {

    namespace {
        static std::array<const char*, CURSOR_MAX_COUNT> cursor_table = {
            "left_ptr",
            "center_ptr",
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

        static const char* cursorTypeToStr(CursorFlags c)
        {
            return cursor_table[c];
        }
    }

    namespace utils {
        int canvas_zoom(const t_glist* c)
        {
            return c->gl_zoom;
        }

        Point object_pos(t_text* x)
        {
            return { x->te_xpix, x->te_ypix };
        }

        Point object_abs_pos(t_text* x, const t_glist* parent)
        {
            return { text_xpix(x, (t_glist*)parent), text_ypix(x, (t_glist*)parent) };
        }

        bool canvas_is_visible(const t_glist* c)
        {
            return glist_isvisible((t_glist*)c);
        }

        void object_move(t_text* x, int dx, int dy)
        {
            x->te_xpix += dx;
            x->te_ypix += dy;
        }

        void object_bind(t_text* x)
        {
            char buf[32];
            snprintf(buf, sizeof(buf) - 1, "#%llx", reinterpret_cast<uint64_t>(x));
            pd_bind(&x->te_g.g_pd, gensym(buf));
        }

        void canvas_update_object_lines(const t_glist* c, t_text* x)
        {
            canvas_fixlinesfor((t_glist*)c, x);
        }

        void canvas_delete_object_lines(const t_glist* c, t_text* x)
        {
            canvas_deletelinesfor((t_glist*)c, x);
        }

        bool canvas_is_edit(const t_glist* c)
        {
            return c->gl_edit;
        }

        void widget_bind_mouse(t_glist* c, t_object* obj, UIFactoryFlags flags)
        {
            char buf[128] = "";

            if (flags & UI_FACTORY_FLAG_MOUSE_ENTER)
                strcat(buf, " enter");
            if (flags & UI_FACTORY_FLAG_MOUSE_LEAVE)
                strcat(buf, " leave");
            if (flags & UI_FACTORY_FLAG_MOUSE_MOVE)
                strcat(buf, " move");
            if (flags & UI_FACTORY_FLAG_MOUSE_DOWN)
                strcat(buf, " down");
            if (flags & UI_FACTORY_FLAG_MOUSE_UP)
                strcat(buf, " up");

            sys_vgui("nui::widget_mouse_bind %lx %lx %lx %s\n", c, obj, obj, buf);
        }

        void widget_create(t_glist* c, t_object* obj, const Point& pos, const Size& sz, int zoom)
        {
            auto sz0 = sz * zoom;
            sys_vgui("nui::widget_create %lx %lx"
                     " %d %d %d %d 1\n",
                c, obj,
                pos.x(), pos.y(), sz0.width(), sz0.height());
        }

        void widget_erase(t_glist* c, t_object* obj)
        {
            sys_vgui("nui::widget_erase %lx %lx\n", c, obj);
            sys_unqueuegui(obj);
        }

        void widget_focus(t_glist* c, t_object* obj)
        {
            sys_vgui("nui::widget_focus %lx %lx\n", c, obj);
        }

        t_glist* object_get_draw_canvas(t_glist* c)
        {
            return glist_getcanvas(c);
        }

        bool is_platform_control(uint32_t mod)
        {
#ifdef __WIN32
            return mod & KEY_MOD_CTRL;
#else
            return mod & KEY_MOD_CMD;
#endif
        }

        bool is_toplevel(t_glist* x)
        {
            return glist_istoplevel(x);
        }

        void set_cursor(t_glist* c, t_object* x, CursorFlags cursor)
        {
            sys_vgui("nui::widget_cursor %lx %lx %s\n", c, x, cursorTypeToStr(cursor));
        }

    }
}
}
