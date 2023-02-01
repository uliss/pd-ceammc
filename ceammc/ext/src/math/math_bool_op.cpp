/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "math_bool_op.h"

#include <algorithm>

constexpr const size_t MIN_ARGS = 3;
constexpr const size_t MAX_ARGS = 16;

MathBoolOp::MathBoolOp(const PdArgs& a)
    : BaseObject(a)
    , sync_(0)
    , n_(nullptr)
{
    createOutlet();

    n_ = new IntProperty("@n", MIN_ARGS, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_ARGS, MAX_ARGS);
    n_->setArgIndex(0);
    addProperty(n_);

    sync_ = new FlagProperty("@sync");
    addProperty(sync_);

    createCbProperty("@state", &MathBoolOp::p_state);
}

void MathBoolOp::initDone()
{
    BaseObject::initDone();

    for (int i = 1; i < n_->value(); i++)
        createInlet();

    vars_.assign(n_->value(), false);
}

void MathBoolOp::onFloat(t_float f)
{
    vars_[0] = (f == 1.f);
    floatTo(0, operate());
}

void MathBoolOp::onInlet(size_t n, const AtomListView& l)
{
    if (l.empty())
        return;

    if (n < n_->value())
        vars_[n] = l[0].asSizeT(0) != 0;

    if (sync_->value())
        floatTo(0, operate());
}

void MathBoolOp::m_reset(t_symbol* /*m*/, const AtomListView&)
{
    vars_.assign(n_->value(), false);
}

int MathBoolOp::operate() const
{
    return (std::find(begin(), end(), false) == vars_.end()) ? 1 : 0;
}

AtomList MathBoolOp::p_state() const
{
    AtomList res;
    res.reserve(n_->value());

    for (auto b : vars_)
        res.append(b ? 1 : 0);

    return res;
}
