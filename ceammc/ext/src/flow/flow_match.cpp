/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "flow_match.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_regexp.h"

FlowMatch::FlowMatch(const PdArgs& args)
    : BaseObject(args)
    , cut_(nullptr)
{
    cut_ = new BoolProperty("@cut", false);
    createProperty(cut_);

    auto N = positionalArguments().size();
    for (size_t i = 0; i < N; i++) {
        createInlet();
        createOutlet();

        re_.emplace_back(Re2Ptr(new re2::RE2(regexp::escape(to_string(positionalArgument(i))))));
    }

    createOutlet();
}

void FlowMatch::onInlet(size_t idx, const AtomList& l)
{
    if (idx < re_.size())
        re_[idx].reset(new re2::RE2(regexp::escape(to_string(l))));
}

void FlowMatch::onSymbol(t_symbol* s)
{
    const size_t N = re_.size();
    assert(N == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(s->s_name, *re_[i])) {
            if (cut_->value())
                bangTo(i);
            else
                symbolTo(i, s);

            return;
        }
    }

    symbolTo(N, s);
}

void FlowMatch::onAny(t_symbol* s, const AtomList& l)
{
    const size_t N = re_.size();
    assert(N == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(s->s_name, *re_[i])) {
            if (cut_->value())
                anyTo(i, l);
            else
                anyTo(i, s, l);

            return;
        }
    }

    anyTo(N, s, l);
}

void FlowMatch::onDataT(const DataTPtr<DataTypeString>& data)
{
    const size_t N = re_.size();
    assert(N == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(data.data()->str(), *re_[i])) {
            dataTo(i, data);
            return;
        }
    }

    dataTo(N, data);
}

void setup_flow_match()
{
    ObjectFactory<FlowMatch> obj("flow.match");
    obj.processData<DataTypeString>();
}
