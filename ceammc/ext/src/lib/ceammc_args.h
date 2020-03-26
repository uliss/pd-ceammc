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
#ifndef CEAMMC_ARGS_H
#define CEAMMC_ARGS_H

#include "args/arg_group_or.h"

namespace ceammc {

class AtomCheckerAndGroup : public ArgGroup {
public:
    AtomCheckerAndGroup();
    CheckResult check(const AtomListView& v, CheckerContext& ctx) const override;
    CheckResult checkSingle(const AtomListView& v, CheckerContext& ctx) const override;
    bool insertChild(const ArgCheckerNodePtr& node) override;
};

class ArgChecker {
    std::string format_;
    AtomCheckerAndGroup checker_;

public:
    ArgChecker(const std::string& format);
    bool check(const AtomListView& v) const;

private:
    bool parse(const std::string& format);
};
}

#endif // CEAMMC_ARGS_H
