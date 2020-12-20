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
#include "arg_is_int.h"
#include "ceammc_atom.h"
#include "ceammc_format.h"
#include "fmt/format.h"

namespace ceammc {

ArgIsInt::ArgIsInt()
    : ArgCheckerSingle()
    , a_(0)
    , b_(0)
    , type_(INT_ANY)
{
    setName("i");
}

void ArgIsInt::setCheck(ArgIsInt::CheckType t, int a, int b)
{
    type_ = t;
    a_ = a;
    b_ = b;
}

bool ArgIsInt::checkAtom(const Atom& a, CheckerContext& ctx) const
{
    if (!a.isInteger()) {
        ctx.error = "not a int: " + to_string(a);
        return false;
    }

    const int v = a.asInt();

    switch (type_) {
    case INT_EQUAL: {
        bool rc = (v == a_);
        if (!rc)
            ctx.error = fmt::format("integer {} is expected, got {}", a_, v);

        return rc;
    }
    case INT_NOT_EQUAL: {
        bool rc = (v != a_);
        if (!rc)
            ctx.error = fmt::format("integer is expected != {}", v);

        return rc;
    }
    case INT_GREATER: {
        bool rc = (v > a_);
        if (!rc)
            ctx.error = fmt::format("integer >{} is expected, got {}", a_, v);

        return rc;
    }
    case INT_GREATER_EQUAL: {
        bool rc = (v >= a_);
        if (!rc)
            ctx.error = fmt::format("integer >={} is expected, got {}", a_, v);

        return rc;
    }
    case INT_LESS: {
        bool rc = (v < a_);
        if (!rc)
            ctx.error = fmt::format("integer <{} is expected, got {}", a_, v);

        return rc;
    }
    case INT_LESS_EQUAL: {
        bool rc = (v <= a_);
        if (!rc)
            ctx.error = fmt::format("integer <={} is expected, got {}", a_, v);

        return rc;
    }
    case INT_MOD: {
        if (a_ == 0) {
            ctx.error = fmt::format("division by zero in % check");
            return false;
        }

        bool rc = (v % a_ == b_);
        if (!rc)
            ctx.error = fmt::format("remainder {} is expected, got {}", b_, (v % a_));

        return rc;
    }
    case INT_RANGE: {
        if (a_ > b_) {
            ctx.error = fmt::format("invalid range specified: {} should be <= {}", a_, b_);
            return false;
        }

        bool rc = (a_ <= v && v <= b_);
        if (!rc)
            ctx.error = fmt::format("{} not in range {}..{}", v, a_, b_);

        return rc;
    }
    case INT_POWER_OF_TWO: {
        bool rc = (v > 0 && ((v & (v - 1)) == 0));
        if (!rc)
            ctx.error = fmt::format("{} is not power of 2", v);

        return rc;
    }
    case INT_ANY:
    default:
        return true;
    }
}

std::string ArgIsInt::name() const
{
    switch (type_) {
    case INT_EQUAL:
        return fmt::format("i={}", a_);
    case INT_NOT_EQUAL:
        return fmt::format("i!={}", a_);
    case INT_GREATER:
        return fmt::format("i>{}", a_);
    case INT_GREATER_EQUAL:
        return fmt::format("i>={}", a_);
    case INT_LESS:
        return fmt::format("i<{}", a_);
    case INT_LESS_EQUAL:
        return fmt::format("i<={}", a_);
    case INT_MOD:
        return fmt::format("i%{}={}", a_, b_);
    case INT_RANGE:
        return fmt::format("i{}..{}", a_, b_);
    case INT_POWER_OF_TWO:
        return fmt::format("i^2");
    default:
        return "i";
    }
}

}
