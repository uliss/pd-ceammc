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
#include "math_approx.h"
#include "ceammc_factory.h"

#include <cmath>

MathApprox::MathApprox(const PdArgs& a)
    : BaseObject(a)
    , value_(0)
    , epsilon_(0)
{
    createInlet();
    createInlet();
    createOutlet();

    value_ = new FloatProperty("@value", 0);
    value_->setArgIndex(0);
    epsilon_ = new FloatProperty("@epsilon", t_float(0.01));
    epsilon_->setArgIndexNext(value_);

    addProperty(value_);
    addProperty(epsilon_);
}

void MathApprox::onFloat(t_float f)
{
    const t_float v = value_->value();
    const t_float e = epsilon_->value();
    floatTo(0, std::fabs(f - v) <= std::fabs(e) ? 1 : 0);
}

void MathApprox::onList(const AtomList& l)
{
    static ArgChecker arg_cheker("f f f?");

    if (!arg_cheker.check(l))
        return;

    if (l.size() == 3) {
        value_->set(l.view(1, 1));
        epsilon_->set(l.view(2, 1));
        onFloat(l[0].asFloat(0));
    }

    if (l.size() == 2) {
        value_->set(l.view(1, 1));
        onFloat(l[0].asFloat(0));
    }
}

void MathApprox::onInlet(size_t n, const AtomList& l)
{
    switch (n) {
    case 1:
        value_->set(l);
        break;
    case 2:
        epsilon_->set(l);
        break;
    }
}

void setup_math_approx()
{
    ObjectFactory<MathApprox> obj("math.approx");
    obj.addAlias("approx");
}
