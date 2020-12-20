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
#include "arg_is_bool.h"
#include "ceammc_atom.h"
#include "ceammc_format.h"

namespace ceammc {

ArgIsBool::ArgIsBool()
    : ArgCheckerSingle()
{
    setName("b");
}

bool ArgIsBool::checkAtom(const Atom& a, CheckerContext& ctx) const
{
    bool rc = a.isBool();
    if (!rc)
        ctx.error = "not a bool value: " + to_string(a);

    return rc;
}

}
