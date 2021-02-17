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

#include <cstring>

namespace ceammc {
namespace ui {
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

            sys_vgui("nui::widget_mouse_bind %lx %lx %lx %s\n", glist_getcanvas(c), obj, obj, buf);
        }

        void widget_create(t_glist* c, t_object* obj, const Point& pos, const Size& sz, int zoom)
        {
            auto sz0 = sz * zoom;
            sys_vgui("nui::widget_create %lx %lx"
                     " %d %d %d %d 1\n",
                glist_getcanvas(c), obj,
                pos.x(), pos.y(), sz0.width(), sz0.height());
        }

        void widget_erase(t_glist* c, t_object* obj)
        {
            sys_vgui("nui::widget_erase %lx %lx\n", glist_getcanvas(c), obj);
            sys_unqueuegui(obj);
        }

    }
}
}
