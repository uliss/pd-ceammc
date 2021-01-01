/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "math_div.h"
#include "ceammc_factory.h"

MathDiv::MathDiv(const PdArgs& args)
    : BaseObject(args)
    , div_(nullptr)
{
    createInlet();
    createOutlet();

    div_ = new FloatProperty("@div", 1);
    div_->setArgIndex(0);
    div_->checkNonZero();
    addProperty(div_);
}

void MathDiv::onFloat(t_float f)
{
    if (!zeroCheck())
        return;

    floatTo(0, f / div_->value());
}

void MathDiv::onList(const AtomList& lst)
{
    if (!zeroCheck())
        return;

    listTo(0, lst / div_->value());
}

bool MathDiv::zeroCheck() const
{
    t_float v = div_->value();
    if (v == 0) {
        OBJ_ERR << "division by zero";
        return false;
    }

    return true;
}

void MathDiv::onInlet(const size_t /*n*/, const AtomListView& lst)
{
    div_->set(lst);
}

void setup_math_div()
{
    ObjectFactory<MathDiv> obj("math.div");
    obj.addAlias("math./");
}
