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
#include "flow_pack.h"
#include "ceammc_factory.h"

#include <algorithm>

FlowPack::FlowPack(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    n_ = new IntPropertyMinEq("@n", positionalFloatArgument(0, 1), 1);
    createProperty(n_);
}

void FlowPack::parseProperties()
{
    BaseObject::parseProperties();

    for (int i = 1; i < n_->value(); i++)
        createInlet();

    createOutlet();

    // fill all with zeroes
    msg_.fill(Atom(0.f), n_->value());

    // fill default values from positiona arguments (starting from index 1)
    const auto& args = positionalArguments();
    if (!args.empty()) {
        const size_t N = std::min<size_t>(msg_.size(), args.size() - 1);
        for (size_t i = 0; i < N; i++)
            msg_[i] = args[i + 1];
    }
}

void FlowPack::onBang()
{
    listTo(0, msg_);
}

void FlowPack::onFloat(t_float f)
{
    msg_[0] = Atom(f);
    onBang();
}

void FlowPack::onSymbol(t_symbol* s)
{
    msg_[0] = Atom(s);
    onBang();
}

void FlowPack::onInlet(size_t idx, const AtomList& l)
{
    if (idx >= msg_.size()) {
        OBJ_ERR << "invalid inlet index: " << idx;
        return;
    }

    if (l.empty())
        return onBang();
    else if (l.size() != 1) {
        OBJ_ERR << "only single element expected at " << idx << " inlet";
        return;
    }

    msg_[idx] = l[0];
    onBang();
}

void FlowPack::onList(const AtomList& l)
{
    if (l.size() > msg_.size()) {
        OBJ_ERR << "too many values in list: " << l.size();
        return;
    }

    for (size_t i = 0; i < l.size(); i++)
        msg_[i] = l[i];

    onBang();
}

void setup_flow_pack()
{
    ObjectFactory<FlowPack> obj("flow.pack");
}
