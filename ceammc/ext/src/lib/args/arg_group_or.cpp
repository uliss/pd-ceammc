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
#include "arg_group_or.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_format.h"
#include "fmt/format.h"

namespace ceammc {

ArgGroupOr::ArgGroupOr()
    : ArgGroup()
{
    setRepeats(1);
}

std::string ArgGroupOr::name() const
{
    std::string res("(");
    res.reserve(nodes_.size() * 2);
    for (auto& n : nodes_) {
        res += n->fullName();
        res.push_back('|');
    }

    if (!nodes_.empty())
        res.back() = ')';
    else
        res.push_back(')');

    return res;
}

CheckResult ArgGroupOr::check(const AtomListView& v, CheckerContext& ctx) const
{
    return checkRepeats(v, ctx);
}

CheckResult ArgGroupOr::checkSingle(const AtomListView& v, CheckerContext& ctx) const
{
    std::string err_lst;
    for (auto& n : nodes_) {
        auto res = n->check(v, ctx);
        if (res.ok) {
            return res;
        } else {
            err_lst += " - ";
            err_lst += ctx.error;
            err_lst += "\n";
        }
    }

    if (!err_lst.empty()) {
        err_lst.pop_back();
    }

    ctx.error = fmt::format("group check {1} failed on [{0}], "
                            "none of this patterns matched:\n{2}",
        to_string(v), fullName(), err_lst);
    return { false, 0 };
}

}
