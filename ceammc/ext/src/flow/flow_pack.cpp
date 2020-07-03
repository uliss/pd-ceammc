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
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

constexpr size_t MIN_INLETS = 1;
constexpr size_t MAX_INLETS = 256;
constexpr size_t DEF_INLETS = 1;

FlowPack::FlowPack(const PdArgs& args)
    : BaseObject(args)
    , n_(positionalConstant<DEF_INLETS, MIN_INLETS, MAX_INLETS>(0))
{
    // (in/out)lets
    for (size_t i = 1; i < n_; i++)
        createInlet();

    createOutlet();

    // fill all with zeroes
    msg_.fill(Atom(0.f), n_);

    // fill default values from positiona arguments (starting from index 1)
    if (!args.args.empty()) {
        const size_t N = std::min<size_t>(msg_.size(), args.args.size() - 1);
        for (size_t i = 0; i < N; i++)
            msg_[i] = args.args[i + 1];
    }
}

void FlowPack::parseProperties()
{
}

void FlowPack::onBang()
{
    output(0);
}

void FlowPack::onFloat(t_float f)
{
    msg_[0] = Atom(f);
    output(0);
}

void FlowPack::onSymbol(t_symbol* s)
{
    msg_[0] = Atom(s);
    output(0);
}

void FlowPack::onInlet(size_t idx, const AtomList& l)
{
    if (!l.empty()) {
        if (idx >= msg_.size()) {
            OBJ_ERR << "invalid inlet index: " << idx;
            return;
        }

        const size_t N = std::min<size_t>(idx + l.size(), msg_.size());
        for (size_t i = idx; i < N; i++)
            msg_[i] = l[i - idx];
    }

    output(idx);
}

void FlowPack::onList(const AtomList& l)
{
    if (l.size() > msg_.size())
        OBJ_ERR << "too many values in list: " << l.size() << ". Using only first " << msg_.size();

    const size_t N = std::min<size_t>(l.size(), msg_.size());
    for (size_t i = 0; i < N; i++)
        msg_[i] = l[i];

    output(0);
}

void FlowPack::onAny(t_symbol* s, const AtomList& l)
{
    if ((l.size() + 1) > msg_.size())
        OBJ_ERR << "too many atoms in message: " << (l.size() + 1);

    const size_t N = std::min<size_t>(l.size() + 1, msg_.size());

    msg_[0] = s;
    for (size_t i = 1; i < N; i++)
        msg_[i] = l[i - 1];

    anyTo(0, msg_);
}

bool FlowPack::processAnyProps(t_symbol* s, const AtomList& l)
{
    return false;
}

void FlowPack::output(size_t inlet_idx)
{
    if (inlet_idx == 0)
        listTo(0, msg_);
}

void setup_flow_pack()
{
    ObjectFactory<FlowPack> obj("flow.pack");
}
