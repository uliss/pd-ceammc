/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "math_sync_base.h"

MathSyncBase::MathSyncBase(FloatBinFn fn, const PdArgs& args)
    : BaseObject(args)
    , v1_(0)
    , v2_(positionalFloatArgument(0, 0))
    , fn_(fn)
{
    createInlet();
    createOutlet();
}

void MathSyncBase::onBang()
{
    floatTo(0, fn_(v1_, v2_));
}

void MathSyncBase::onFloat(t_float f)
{
    v1_ = f;
    onBang();
}

void MathSyncBase::onInlet(size_t n, const AtomList& lst)
{
    if (n != 1)
        return;

    if (!lst.empty())
        v2_ = lst[0].asFloat();

    onBang();
}

void MathSyncBase::onList(const AtomList& lst)
{
    const auto N = lst.size();

    if (N > 0)
        v1_ = lst[0].asFloat();

    if (N > 1)
        v2_ = lst[1].asFloat();

    onBang();
}
