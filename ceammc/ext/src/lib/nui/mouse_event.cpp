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
#include "mouse_event.h"
#include "ceammc_log.h"

#include <cstdint>

namespace ceammc {
namespace ui {

    t_class* MouseBind::class_ = nullptr;

    MouseBind::MouseBind(t_symbol* sym)
        : bind_name_(&s_)
    {
        if (!class_)
            class_init();

        t_object* tmp = reinterpret_cast<t_object*>(pd_new(class_));
        memcpy(&obj_, tmp, sizeof(t_object));
        pd_free((t_pd*)tmp);

        bind(sym);
    }

    MouseBind::~MouseBind()
    {
        unbind();
    }

    void MouseBind::on_mouse_enter(MouseBind* x)
    {
        if (x->mouse_enter_fn_)
            x->mouse_enter_fn_();
    }

    void MouseBind::on_mouse_leave(MouseBind* x)
    {
        if (x->mouse_leave_fn_)
            x->mouse_leave_fn_();
    }

    bool MouseBind::class_init()
    {
        class_ = class_new(gensym("proxy mouse"), 0, 0, sizeof(MouseBind), CLASS_PD, A_NULL);
        class_addmethod(class_, (t_method)&MouseBind::on_mouse_enter, gensym("mouseenter"), A_NULL, 0);
        class_addmethod(class_, (t_method)&MouseBind::on_mouse_leave, gensym("mouseleave"), A_NULL, 0);
        return true;
    }

    void MouseBind::bind(t_symbol* name)
    {
        if (bind_name_ == name)
            return;

        unbind();
        pd_bind(pd(), name);
        bind_name_ = name;
    }

    void MouseBind::unbind()
    {
        if (bind_name_ != &s_) {
            pd_unbind(pd(), bind_name_);
            bind_name_ = &s_;
        }
    }

    bool MouseBind::isBinded() const
    {
        return bind_name_ != &s_;
    }

    void MouseBind::setMouseEnter(MouseEnterFn fn)
    {
        mouse_enter_fn_ = fn;
    }

    void MouseBind::setMouseLeave(MouseLeaveFn fn)
    {
        mouse_leave_fn_ = fn;
    }

    t_symbol* genBindName(void* x)
    {
        char buf[64] = { 0 };
        snprintf(buf, sizeof(buf) - 1, "#%llx", reinterpret_cast<uint64_t>(x));
        return gensym(buf);
    }

}
}
