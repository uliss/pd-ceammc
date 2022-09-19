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

#include <cstdint>
#include <exception>
#include <type_traits>

namespace ceammc {

MathSyncBase::MathSyncBase(FloatBinFn fn, const PdArgs& args, bool create_int_prop)
    : BaseObject(args)
    , v1_(0)
    , v2_(parsedPosArgs().floatAt(0, 0))
    , fn_(fn)
    , prop_int_(nullptr)
{
    if (create_int_prop) {
        prop_int_ = new FlagProperty("@int");
        addProperty(prop_int_);
    }

    createInlet();
    createOutlet();
}

void MathSyncBase::onBang()
{
    try {

        if (prop_int_->value())
            floatTo(0, static_cast<IntType>(fn_(static_cast<IntType>(v1_), static_cast<IntType>(v2_))));
        else
            floatTo(0, fn_(v1_, v2_));

    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }
}

void MathSyncBase::onFloat(t_float f)
{
    v1_ = f;
    onBang();
}

void MathSyncBase::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    if (!lv.empty())
        v2_ = lv[0].asFloat();

    onBang();
}

void MathSyncBase::onList(const AtomListView& lv)
{
    const auto N = lv.size();

    if (N > 0)
        v1_ = lv[0].asFloat();

    if (N > 1)
        v2_ = lv[1].asFloat();

    onBang();
}

MathSyncBool::MathSyncBool(BoolBinFn fn, const PdArgs& args)
    : BaseObject(args)
    , v1_(false)
    , v2_(parsedPosArgs().boolAt(0, false))
    , fn_(fn)
{
    createInlet();
    createOutlet();
}

void MathSyncBool::onBang()
{
    floatTo(0, fn_(v1_, v2_) ? 1 : 0);
}

void MathSyncBool::onFloat(t_float f)
{
    v1_ = checkBool(f);
    onBang();
}

void MathSyncBool::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    if (!lv.empty())
        v2_ = checkBool(lv[0].asFloat());

    onBang();
}

void MathSyncBool::onList(const AtomListView& lv)
{
    const auto N = lv.size();

    if (N > 0)
        v1_ = checkBool(lv[0].asFloat());

    if (N > 1)
        v2_ = checkBool(lv[1].asFloat());

    onBang();
}

bool MathSyncBool::checkBool(t_float f) const
{
    if (std::equal_to<t_float>()(f, 0))
        return false;
    else if (std::equal_to<t_float>()(f, 1))
        return true;
    else {
        OBJ_ERR << "1 or 0 expected";
        return true;
    }
}
}
