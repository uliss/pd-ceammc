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
#include "arg_checker_node.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_format.h"
#include "fmt/format.h"

namespace ceammc {

constexpr size_t REPEAT_INF = 1025;

ArgCheckerNode::ArgCheckerNode()
    : min_(0)
    , max_(0)
{
}

ArgCheckerNode::ArgCheckerNode(const ArgCheckerNode& n)
    : min_(n.min_)
    , max_(n.max_)
    , name_(n.name_)
{
}

bool ArgCheckerNode::insertChild(const ArgCheckerNodePtr& /*node*/)
{
    return false;
}

void ArgCheckerNode::setRepeats(size_t n)
{
    min_ = n;
    max_ = n;
}

void ArgCheckerNode::setInfinitePattern()
{
    setMaxRepeats(REPEAT_INF);
}

bool ArgCheckerNode::infinitePattern() const
{
    return max_ == REPEAT_INF;
}

std::string ArgCheckerNode::name() const
{
    return name_;
}

std::string ArgCheckerNode::repeats() const
{
    if (constRepeat()) {
        if (min_ == 1)
            return "";
        else
            return fmt::format("[{}]", min_);
    } else {
        if (infinitePattern()) {
            if (min_ == 0)
                return "*";
            else if (min_ == 1)
                return "+";
            else
                return fmt::format("[{}..]", min_);
        } else if (min_ == 0 && max_ == 1)
            return "?";
        else
            return fmt::format("[{}..{}]", min_, max_);
    }
}

std::string ArgCheckerNode::fullName() const
{
    return name() + repeats();
}

CheckResult ArgCheckerNode::checkRepeats(const AtomListView& v, CheckerContext& ctx) const
{
    if (min_ > max_) {
        ctx.error = fmt::format("invalid repeat range: [{}]", repeats());
        ctx.error_atom_idx = 0;
        return { false, 0 };
    }

    if (v.size() < min_) {
        ctx.error = fmt::format("too short sequence: [{}], expected at least {} elements", to_string(v), min_);
        ctx.error_atom_idx = 0;
        return { false, 0 };
    } else {
        const auto N = std::min(v.size(), max_);
        size_t idx = 0;
        while (idx < N) {
            auto res = checkSingle(v.subView(idx), ctx);
            // partial match
            if (!res.ok) {
                if (idx < min_) {
                    return { false, idx };
                } else {
                    ctx.error.clear();
                    return { true, idx };
                }
            } else if (res.matched > 0)
                idx += res.matched;
            else
                return { false, 10000 };
        }

        // full match
        ctx.error.clear();
        return { true, idx };
    }
}
}
