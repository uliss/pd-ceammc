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

FlowCompareBase::FlowCompareBase(const PdArgs& args, FlowCompareFn fn, CompreFnDesc desc)
    : BaseObject(args)
    , cmp_(fn)
    , prop_args_(nullptr)
    , cmp_desc_(desc)
    , ascending_order_(cmp_(1, 2))
{
    prop_args_ = new ListProperty("@values", {}, PropValueAccess::INITONLY);
    prop_args_->setArgIndex(0);
    addProperty(prop_args_);
}

void FlowCompareBase::initDone()
{
    if (prop_args_->value().empty()) {
        usage(false);
        return;
    }

    args_.reserve(prop_args_->value().size());

    auto arg_cmp = ascending_order_ ? [](t_float f0, t_float f1) { return f0 < f1; }
                                    : [](t_float f0, t_float f1) { return f0 > f1; };

    for (auto& a : prop_args_->value()) {
        if (a.isFloat()) {
            auto f = a.asFloat();
            if (args_.empty() || arg_cmp(args_.back(), f)) {
                createOutlet();
                args_.push_back(f);
                char buf[128];
                snprintf(buf, sizeof(buf), "%s %f", cmp_desc_.first, f);
                out_info_.push_back(buf);
            } else {
                OBJ_ERR << "invalid argument order: " << args_.back() << (ascending_order_ ? " < " : " > ") << f << " is expected,";
                OBJ_ERR << "\tabort other arguments processing";
                usage(true);
                break;
            }
        }
    }

    createOutlet();

    char buf[128];
    snprintf(buf, sizeof(buf), "%s %f", cmp_desc_.second, args_.back());
    out_info_.push_back(buf);
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

const char* FlowCompareBase::annotateInlet(size_t n) const
{
    return "float";
}

const char* FlowCompareBase::annotateOutlet(size_t n) const
{
    if (n < out_info_.size())
        return out_info_[n].c_str();
    else
        return nullptr;
}

void FlowCompareBase::usage(bool err)
{
    if (err) {
        OBJ_ERR << "Usage: FLOAT1 [FLOAT2] ... [FLOAT-N] (in " << (ascending_order_ ? "ascending" : "descending") << " order)";
    } else {
        OBJ_DBG << "Usage: FLOAT1 [FLOAT2] ... [FLOAT-N] (in " << (ascending_order_ ? "ascending" : "descending") << " order)";
    }
}
