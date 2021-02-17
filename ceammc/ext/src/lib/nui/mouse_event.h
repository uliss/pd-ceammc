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
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include <functional>

#include "m_pd.h"

namespace ceammc {
namespace ui {

    using MouseEnterFn = std::function<void()>;
    using MouseLeaveFn = std::function<void()>;

    t_symbol* genBindName(void* x);

    class MouseBind {
        t_object obj_;
        t_symbol* bind_name_;
        MouseEnterFn mouse_leave_fn_;
        MouseEnterFn mouse_enter_fn_;

    public:
        explicit MouseBind(t_symbol* sym);
        ~MouseBind();

        bool isBinded() const;

        void setMouseEnter(MouseEnterFn fn);
        void setMouseLeave(MouseLeaveFn fn);

    public:
        static void on_mouse_enter(MouseBind* x);
        static void on_mouse_leave(MouseBind* x);

        static bool class_init();

    protected:
        void bind(t_symbol* name);
        void unbind();
        t_pd* pd() { return &obj_.te_g.g_pd; }

    private:
        static t_class* class_;
    };
}
}

#endif // MOUSE_EVENT_H
