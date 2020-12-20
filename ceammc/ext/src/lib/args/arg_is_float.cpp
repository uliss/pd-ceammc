/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "arg_is_float.h"
#include "ceammc_atom.h"
#include "ceammc_format.h"
#include "fmt/format.h"

namespace ceammc {

ArgIsFloat::ArgIsFloat()
    : ArgCheckerSingle()
    , a_(0)
    , type_(FLOAT_ANY)
{
    setName("f");
}

void ArgIsFloat::setCheck(ArgIsFloat::CheckType t, t_float a)
{
    type_ = t;
    a_ = a;
}

bool ArgIsFloat::checkAtom(const Atom& a, CheckerContext& ctx) const
{
    if (!a.isFloat()) {
        ctx.error = "not a float: " + to_string(a);
        return false;
    }

    const t_float f = a.asFloat();

    switch (type_) {
    case FLOAT_GREATER_EQUAL: {
        bool rc = (f >= a_);
        if (!rc)
            ctx.error = fmt::format("expected value >={}, got {}", a_, f);

        return rc;
    }
    case FLOAT_GREATER: {
        bool rc = (f > a_);
        if (!rc)
            ctx.error = fmt::format("expected value >{}, got {}", a_, f);

        return rc;
    }
    case FLOAT_LESS: {
        bool rc = (f < a_);
        if (!rc)
            ctx.error = fmt::format("expected value <{}, got {}", a_, f);

        return rc;
    }
    case FLOAT_LESS_EQUAL: {
        bool rc = (f <= a_);
        if (!rc)
            ctx.error = fmt::format("expected value <={}, got {}", a_, f);

        return rc;
    }
    case FLOAT_EQUAL: {
        bool rc = (f == a_);
        if (!rc)
            ctx.error = fmt::format("expected value is {}, got {}", a_, f);

        return rc;
    }
    case FLOAT_NOT_EQUAL: {
        bool rc = (f != a_);
        if (!rc)
            ctx.error = fmt::format("value !={} is expected, got {}", a_, f);

        return rc;
    }
    default:
        return true;
    }
}

std::string ArgIsFloat::name() const
{
    switch (type_) {
    case FLOAT_EQUAL:
        return fmt::format("f={}", a_);
    case FLOAT_NOT_EQUAL:
        return fmt::format("f!={}", a_);
    case FLOAT_GREATER:
        return fmt::format("f>{}", a_);
    case FLOAT_GREATER_EQUAL:
        return fmt::format("f>={}", a_);
    case FLOAT_LESS:
        return fmt::format("f<{}", a_);
    case FLOAT_LESS_EQUAL:
        return fmt::format("f<={}", a_);
    default:
        return "f";
    }
}

}
