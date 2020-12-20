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
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

MathBoolOp::MathBoolOp(const PdArgs& a)
    : BaseObject(a)
    , sync_(0)
    , arg_num_(positionalConstant<MIN_ARGS, MIN_ARGS, MAX_ARGS>(0))
{
    vars_.assign(arg_num_, false);

    sync_ = new FlagProperty("@sync");
    addProperty(sync_);
    createCbProperty("@state", &MathBoolOp::p_state);

    for (size_t i = 1; i < arg_num_; i++)
        createInlet();

    createOutlet();
}

void MathBoolOp::onFloat(t_float f)
{
    vars_[0] = (f == 1.f);
    floatTo(0, operate());
}

void MathBoolOp::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        return;

    if (n < arg_num_)
        vars_[n] = l[0].asSizeT(0) != 0;

    if (sync_->value())
        floatTo(0, operate());
}

void MathBoolOp::m_reset(t_symbol* /*m*/, const AtomListView&)
{
    vars_.assign(arg_num_, false);
}

int MathBoolOp::operate() const
{
    return (std::find(vars_.begin(), vars_.begin() + long(arg_num_), false) == vars_.end()) ? 1 : 0;
}

AtomList MathBoolOp::p_state() const
{
    AtomList res;
    res.reserve(arg_num_);
    for (size_t i = 0; i < arg_num_; i++)
        res.append(Atom(vars_[i] ? 1 : 0));

    return res;
}
