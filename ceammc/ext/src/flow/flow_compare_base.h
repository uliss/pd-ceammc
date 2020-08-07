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

using FlowCompareFn = bool (*)(t_float, t_float);
using CompreFnDesc = std::pair<const char*, const char*>;

class FlowCompareBase : public BaseObject {
    FlowCompareFn cmp_;
    std::vector<t_float> args_;
    std::vector<std::string> out_info_;
    ListProperty* prop_args_;
    CompreFnDesc cmp_desc_;
    const bool ascending_order_;

public:
    FlowCompareBase(const PdArgs& args, FlowCompareFn fn, CompreFnDesc desc);
    void initDone() override;

    void onFloat(t_float f) final;
    const char* annotateOutlet(size_t n) const override;
    const char* annotateInlet(size_t n) const override;

private:
    void usage(bool err = false);
};

#endif // FLOW_COMPARE_BASE_H
