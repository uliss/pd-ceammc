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
#include "flow_compare_base.h"

FlowCompareBase::FlowCompareBase(const PdArgs& args, FlowCompareFn fn)
    : BaseObject(args)
    , cmp_(fn)
    , ascending_order_(cmp_(1, 2))
{
    const AtomList& pos_args = positionalArguments();

    if (pos_args.empty())
        usage(false);

    args_.reserve(pos_args.size());

    auto arg_cmp = ascending_order_ ? [](t_float f0, t_float f1) { return f0 < f1; }
                                    : [](t_float f0, t_float f1) { return f0 > f1; };

    for (auto& a : pos_args) {
        if (a.isFloat()) {
            auto f = a.asFloat();
            if (args_.empty() || arg_cmp(args_.back(), f)) {
                createOutlet();
                args_.push_back(f);
            } else {
                OBJ_ERR << "invalid argument order: " << args_.back() << (ascending_order_ ? " < " : " > ") << f << " is expected,";
                OBJ_ERR << "\tabort other arguments processing";
                usage(true);
                createOutlet();
                return;
            }
        }
    }

    createOutlet();
}

void FlowCompareBase::onFloat(t_float f)
{
    if (args_.empty())
        return;

    for (size_t i = 0; i < args_.size(); i++) {
        if (cmp_(f, args_[i]))
            return floatTo(i, f);
    }

    floatTo(numOutlets() - 1, f);
}

void FlowCompareBase::usage(bool err)
{
    if (err) {
        OBJ_ERR << "Usage: FLOAT1 [FLOAT2] ... [FLOAT-N] (in " << (ascending_order_ ? "ascending" : "descending") << " order)";
    } else {
        OBJ_DBG << "Usage: FLOAT1 [FLOAT2] ... [FLOAT-N] (in " << (ascending_order_ ? "ascending" : "descending") << " order)";
    }
}
