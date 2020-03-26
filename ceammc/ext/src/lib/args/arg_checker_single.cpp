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
#include "arg_checker_single.h"
#include "ceammc_atomlist_view.h"

namespace ceammc {

ArgCheckerSingle::ArgCheckerSingle()
{
    setRepeats(1);
}

CheckResult ArgCheckerSingle::checkSingle(const AtomListView& v, CheckerContext& ctx) const
{
    if (v.empty())
        return { false, 0 };
    else
        return checkAtom(v.front(), ctx)
            ? CheckResult({ true, 1 })
            : CheckResult({ false, 0 });
}

bool ArgCheckerSingle::isLeaf() const
{
    return true;
}

bool ArgCheckerSingle::insertChild(const ArgCheckerNodePtr&)
{
    return false;
}

CheckResult ArgCheckerSingle::check(const AtomListView& v, CheckerContext& ctx) const
{
    return checkRepeats(v, ctx);
}

}
