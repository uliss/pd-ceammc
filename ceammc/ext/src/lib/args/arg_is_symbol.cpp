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
#include "arg_is_symbol.h"
#include "ceammc_atom.h"
#include "ceammc_format.h"
#include "ceammc_regexp.h"
#include "ceammc_string.h"

#include "fmt/format.h"
#include "re2/re2.h"

namespace ceammc {

ArgIsSymbol::ArgIsSymbol()
    : ArgCheckerSingle()
    , pattern_(nullptr)
    , check_type_(SYM_ANY)
{
    setName("s");
}

ArgIsSymbol::~ArgIsSymbol() = default;

bool ArgIsSymbol::checkAtom(const Atom& a, CheckerContext& ctx) const
{
    if (!a.isSymbol()) {
        ctx.error = "not a symbol: " + to_string(a);
        return false;
    }

    if (check_type_ == SYM_ANY)
        return true;

    t_symbol* s = a.asSymbol();

    switch (check_type_) {
    case SYM_EQUAL: {
        bool rc = (s == pattern_);
        if (!rc)
            ctx.error = fmt::format("symbol {} is expected, got {}", pattern_->s_name, s->s_name);

        return rc;
    }
    case SYM_NOT_EQUAL: {
        bool rc = (s != pattern_);
        if (!rc)
            ctx.error = fmt::format("symbol != {} is expected", pattern_->s_name);

        return rc;
    }
    case SYM_BEGINS_WITH: {
        bool rc = string::starts_with(s->s_name, pattern_->s_name);
        if (!rc)
            ctx.error = fmt::format("not starts with {}: {}", pattern_->s_name, s->s_name);

        return rc;
    }
    case SYM_ENDS_WITH: {
        bool rc = string::ends_with(s->s_name, pattern_->s_name);
        if (!rc)
            ctx.error = fmt::format("not ends with {}: {}", pattern_->s_name, s->s_name);

        return rc;
    }
    case SYM_CONTAINS: {
        bool rc = string::contains(s->s_name, pattern_->s_name);
        if (!rc)
            ctx.error = fmt::format("not contains {}: {}", pattern_->s_name, s->s_name);

        return rc;
    }
    case SYM_MATCH: {
        bool rc = re2::RE2::FullMatch(s->s_name, *re_);
        if (!rc)
            ctx.error = fmt::format("regexp '{}' match failed on {}", pattern_->s_name, s->s_name);

        return rc;
    }
    default:
        return true;
    }
}

std::string ArgIsSymbol::name() const
{
    switch (check_type_) {
    case SYM_EQUAL:
        return fmt::format("s={}", pattern_->s_name);
    case SYM_NOT_EQUAL:
        return fmt::format("s!={}", pattern_->s_name);
    case SYM_BEGINS_WITH:
        return fmt::format("^{}", pattern_->s_name);
    case SYM_ENDS_WITH:
        return fmt::format("{}$", pattern_->s_name);
    case SYM_CONTAINS:
        return fmt::format("~{}", pattern_->s_name);
    case SYM_MATCH:
        return fmt::format("r'{}'", pattern_->s_name);
    default:
        return ArgCheckerSingle::name();
    }
}

void ArgIsSymbol::setCheck(ArgIsSymbol::CheckType t, const char* s)
{
    check_type_ = t;
    pattern_ = gensym(s);

    if (check_type_ == SYM_MATCH)
        re_.reset(new re2::RE2(regexp::escape(s)));
}

}
