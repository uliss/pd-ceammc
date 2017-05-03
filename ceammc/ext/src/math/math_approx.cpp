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

    value_ = new FloatProperty("@value", positionalFloatArgument(0, 0));
    epsilon_ = new FloatProperty("@epsilon", positionalFloatArgument(1, 0.01f));

    createProperty(value_);
    createProperty(epsilon_);
}

void MathApprox::onFloat(float f)
{
    const t_float v = value_->value();
    const t_float e = epsilon_->value();
    floatTo(0, fabsf(f - v) <= fabsf(e) ? 1 : 0);
}

void MathApprox::onList(const AtomList& l)
{
    if (l.size() == 3) {
        value_->set(l[1]);
        epsilon_->set(l[2]);
        onFloat(l[0].asFloat(0));
    }

    if (l.size() == 2) {
        value_->set(l[1]);
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

extern "C" void setup_math0x2eaapprox()
{
    ObjectFactory<MathApprox> obj("math.approx");
    obj.addAlias("approx");
}
