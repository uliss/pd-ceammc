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
#include "math_lcm.h"
#include "ceammc_factory.h"

#include <boost/integer/common_factor.hpp>

MathLCM::MathLCM(const PdArgs& a)
    : BaseObject(a)
    , b_(parsedPosArgs().intAt(0, 0))
{
    createInlet(&b_);
    createOutlet();
}

void MathLCM::onFloat(t_float f)
{
    floatTo(0, boost::integer::lcm(int(f), int(b_)));
}

void MathLCM::onList(const AtomList& l)
{
    if (l.size() < 1)
        return;

    if (l.size() == 1)
        return onFloat(l[0].asFloat());

    b_ = l[1].asFloat();
    onFloat(l[0].asFloat());
}

void setup_math_lcm()
{
    ObjectFactory<MathLCM> obj("math.lcm");
}
