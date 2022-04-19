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
#include "ceammc_outlet.h"

extern "C" {
#include "m_imp.h"

// Note: keep in sync with m_obj.c
struct _outconnect {
    struct _outconnect* oc_next;
    t_pd* oc_to;
};

// Note: keep in sync with m_obj.c
struct _outlet {
    t_object* o_owner;
    struct _outlet* o_next;
    t_outconnect* o_connections;
    t_symbol* o_sym;
};
}

namespace ceammc {

OutletIterator::OutletIterator(const t_object* obj)
    : obj_(obj)
    , outlet_(nullptr)
{
    reset();
}

bool OutletIterator::isSignal() const
{
    return util::outlet_is_signal(outlet_);
}

void OutletIterator::setSignal(bool v)
{
    return util::outlet_set_signal(outlet_, v);
}

void OutletIterator::reset()
{
    outlet_ = obj_ ? obj_->te_outlet : nullptr;
}

OutletIterator& OutletIterator::next()
{
    outlet_ = (obj_ && outlet_) ? outlet_->o_next : nullptr;
    return *this;
}

OutletIterator::operator bool()
{
    return obj_ && outlet_;
}

bool util::outlet_is_signal(t_outlet* x)
{
    return x ? (x->o_sym) == &s_signal : false;
}

void util::outlet_set_signal(t_outlet* x, bool v)
{
    if (x)
        x->o_sym = v ? &s_signal : &s_list;
}

}
