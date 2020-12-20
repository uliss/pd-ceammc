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
#ifndef ARG_GROUP_OR_H
#define ARG_GROUP_OR_H

#include "arg_group.h"

namespace ceammc {

class ArgGroupOr : public ArgGroup {
public:
    ArgGroupOr();

    std::string name() const final;

    CheckResult check(const AtomListView& v, CheckerContext& ctx) const final;
    CheckResult checkSingle(const AtomListView& v, CheckerContext& ctx) const final;
};
}

#endif // ARG_GROUP_OR_H
