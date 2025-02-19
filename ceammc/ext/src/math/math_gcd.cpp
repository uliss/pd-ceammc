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
#include "math_gcd.h"
#include "ceammc_factory.h"

#include <boost/integer/common_factor.hpp>

MathGCD::MathGCD(const PdArgs& a)
    : BaseObject(a)
    , b_(parsedPosArgs().intAt(0, 0))
{
    createInlet(&b_);
    createOutlet();
}

void MathGCD::onFloat(t_float f)
{
    floatTo(0, boost::integer::gcd(int(f), int(b_)));
}

void MathGCD::onList(const AtomListView& lv)
{
    if (lv.size() < 1)
        return;

    if (lv.size() == 1)
        return onFloat(lv[0].asFloat());

    b_ = lv[1].asFloat();
    onFloat(lv[0].asFloat());
}

void setup_math_gcd()
{
    ObjectFactory<MathGCD> obj("math.gcd");

    obj.setDescription("calculate greatest common divisor");
    obj.setCategory("math");
}
