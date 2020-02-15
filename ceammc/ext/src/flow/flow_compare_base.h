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
#ifndef FLOW_COMPARE_BASE_H
#define FLOW_COMPARE_BASE_H

#include "ceammc_object.h"
using namespace ceammc;

typedef bool (*FlowCompareFn)(t_float, t_float);

class FlowCompareBase : public BaseObject {
    FlowCompareFn cmp_;
    std::vector<t_float> args_;
    const char* cmp_str_;

public:
    FlowCompareBase(const PdArgs& args, FlowCompareFn fn, const char* cmp_str);

    void onFloat(t_float f) final;

private:
    void usage(bool err = false);
};

#endif // FLOW_COMPARE_BASE_H
