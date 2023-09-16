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
#include "math_or.h"
#include "ceammc_factory.h"

#include <algorithm>

MathOr::MathOr(const PdArgs& a)
    : MathBoolOp(a)
{
}

int MathOr::operate() const
{
    return (std::find(begin(), end(), true) == end()) ? 0 : 1;
}

void setup_math_or()
{
    ObjectFactory<MathOr> obj("math.or");
    obj.addAlias("or");
    obj.addMethod("reset", &MathOr::m_reset);

    obj.setDescription("operation OR for multiple arguments");
    obj.setCategory("math");
}
