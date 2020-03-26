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
#include "arg_group_and.h"
#include "ceammc_atomlist_view.h"
#include "fmt/format.h"

namespace ceammc {

ArgGroupAnd::ArgGroupAnd()
{
    setRepeats(1);
}

std::string ArgGroupAnd::name() const
{
    std::string res;
    res.reserve(nodes_.size() * 4);

    for (auto& n : nodes_) {
        res += n->fullName();
        res.push_back(' ');
    }

    if (!nodes_.empty())
        res.pop_back();

    return res;
}

CheckResult ArgGroupAnd::check(const AtomListView& v, CheckerContext& ctx) const
{
    return checkSingle(v, ctx);
}

CheckResult ArgGroupAnd::checkSingle(const AtomListView& v, CheckerContext& ctx) const
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

        auto res = n->check(v.subView(offset), ctx);
        if (!res.ok)
            return res;
        else
            offset += res.matched;
    }

    return { true, offset };
}

}
