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
#include "ceammc_atomlist_view.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "fmt/core.h"

namespace ceammc {

using namespace ceammc::argcheck;

ArgChecker::ArgChecker(const std::string& format)
    : format_(format)
    , os_(&std::cerr)
{
    parse(format_);
}

bool ArgChecker::check(const AtomListView& v) const
{
    CheckerContext ctx;
    auto rc = checker_.check(v, ctx);
    if (!rc.ok) {
        out() << fmt::format(
            "pattern {} check failed on [{}]:\n\t{}\n\n",
            format_, to_string(v), ctx.error);
        return false;
    } else if (rc.matched < v.size()) {
        out() << fmt::format(
            "pattern {} check failed on [{}]:\n"
            "\tnot matched atoms left: [{}]\n\n",
            format_, to_string(v), to_string(v.subView(rc.matched)));
        return false;
    } else
        return true;
}

bool ArgChecker::parse(const std::string& format)
{
    try {
        ArgCheckLexer lexer(format);
        ArgCheckParser parser(lexer, checker_);

        if (parser.parse() != 0) {
            out() << "parse error: " << format << "\n";
            return false;
        }

        return true;
    } catch (std::exception& e) {
        LIB_ERR << "checker error: " << e.what();
        return false;
    }
}
}
