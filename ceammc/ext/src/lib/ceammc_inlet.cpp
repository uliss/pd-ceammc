/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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

#include "ceammc_inlet.h"

extern "C" {
#include "m_imp.h"

// Note: keep in sync with m_obj.c
union inletunion {
    t_symbol* iu_symto;
    t_gpointer* iu_pointerslot;
    t_float* iu_floatslot;
    t_symbol** iu_symslot;
    t_float iu_floatsignalvalue;
};

// Note: keep in sync with m_obj.c
struct _inlet {
    t_pd i_pd;
    struct _inlet* i_next;
    t_object* i_owner;
    t_pd* i_dest;
    t_symbol* i_symfrom;
    union inletunion i_un;
};
}

namespace ceammc {
namespace util {

    size_t inlet_count(t_object* x)
    {
        if (!x)
            return 0;

        return obj_ninlets(x);
    }

    t_inlet* inlet_next(t_inlet* x)
    {
        return x ? x->i_next : nullptr;
    }

    t_object* inlet_owner(t_inlet* x)
    {
        return x ? x->i_owner : nullptr;
    }

    t_pd* inlet_dest(t_inlet* x)
    {
        return x ? x->i_dest : nullptr;
    }

    bool inlet_is_signal(t_inlet* x)
    {
        if (!x)
            return false;

        return x->i_symfrom == &s_signal;
    }

    t_pd* inlet_object(t_inlet* x)
    {
        return x ? &x->i_pd : nullptr;
    }

}

InletIterator::InletIterator(const t_object* obj)
    : obj_(obj)
    , inlet_(nullptr)
{
    reset();
}

bool InletIterator::isSignal() const
{
    return util::inlet_is_signal(inlet_);
}

void InletIterator::reset()
{
    if (obj_)
        inlet_ = obj_->te_inlet;
}

InletIterator& InletIterator::next()
{
    inlet_ = util::inlet_next(inlet_);
    return *this;
}

InletIterator::operator bool()
{
    return inlet_ != nullptr;
}

t_pd* InletIterator::object()
{
    return util::inlet_object(inlet());
}

}
