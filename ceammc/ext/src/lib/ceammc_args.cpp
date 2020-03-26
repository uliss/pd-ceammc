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
#include "ceammc_args.h"
#include "args/argcheck.lexer.h"
#include "args/argcheck.parser.hpp"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "fmt/format.h"

namespace ceammc {

ArgChecker::ArgChecker(const std::string& format)
    : format_(format)
{
    parse(format_);
}

bool ArgChecker::check(const AtomListView& v) const
{
    CheckerContext ctx;
    auto rc = checker_.check(v, ctx);
    if (!rc.ok) {
        std::cerr << fmt::format(
            "pattern {} check failed on [{}]:\n\t{}\n\n",
            format_, to_string(v), ctx.error);
        return false;
    } else if (rc.matched < v.size()) {
        std::cerr << fmt::format(
            "pattern {} check failed on [{}]:\n"
            "\tnot matched atoms left: [{}]\n\n",
            format_, to_string(v), to_string(v.subView(rc.matched)));
        return false;
    } else
        return true;
}

bool ArgChecker::parse(const std::string& format)
{
    ArgCheckLexer lexer(format);
    ArgCheckParser parser(lexer, checker_);

    if (parser.parse() != 0) {
        std::cerr << "parse error: " << format << "\n";
        return false;
    }

    return true;
}

AtomCheckerAndGroup::AtomCheckerAndGroup()
{
    setRepeats(1);
}

CheckResult AtomCheckerAndGroup::check(const AtomListView& v, CheckerContext& ctx) const
{
    return checkSingle(v, ctx);
}

CheckResult AtomCheckerAndGroup::checkSingle(const AtomListView& v, CheckerContext& ctx) const
{
    size_t offset = 0;
    for (size_t i = 0; i < nodes_.size(); i++) {
        auto& n = nodes_[i];
        if (offset + n->minRepeats() > v.size()) {
            if (nodes_.size() > 1)
                ctx.error = fmt::format("unmatched rule {} at {}, from `{}`", n->fullName(), i, fullName());
            else
                ctx.error = fmt::format("unmatched rule {}", fullName());

            return { false, 0 };
        }

        //        std::cout << "checking " << v.subView(offset) << "\n";
        auto res = n->check(v.subView(offset), ctx);
        if (!res.ok)
            return res;
        else
            offset += res.matched;
    }

    return { true, offset };
}

bool AtomCheckerAndGroup::insertChild(const ArgCheckerNodePtr& node)
{
    return ArgGroup::insertChild(node);
}

}
