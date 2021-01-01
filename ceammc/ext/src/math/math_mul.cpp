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
#include "math_mul.h"
#include "ceammc_factory.h"

MathMul::MathMul(const PdArgs& args)
    : BaseObject(args)
    , mul_(nullptr)
{
    mul_ = new FloatProperty("@mul", 1);
    mul_->setArgIndex(0);
    addProperty(mul_);

    createInlet();
    createOutlet();
}

void MathMul::onInlet(size_t n, const AtomListView& lst)
{
    if (n != 1)
        return;

    mul_->set(lst);
}

void MathMul::onFloat(t_float v)
{
    floatTo(0, v * mul_->value());
}

void MathMul::onList(const AtomList& lst)
{
    AtomList res(lst);

    for (size_t i = 0; i < res.size(); i++) {
        Atom& a = res[i];

        a *= mul_->value();
    }

    listTo(0, res);
}

void setup_math_mul()
{
    ObjectFactory<MathMul> obj("math.mul");
    obj.addAlias("math.*");
}
