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

char FlowPack::descr_buf_[MAX_INLETS][DESC_LEN] = { { 0 } };

FlowPack::FlowPack(const PdArgs& args)
    : BaseObject(args)
    , num_(nullptr)
    , msg_(nullptr)
{
    num_ = new IntProperty("@n", DEF_INLETS);
    num_->setInitOnly();
    num_->checkClosedRange(MIN_INLETS, MAX_INLETS);
    num_->setArgIndex(0);
    addProperty(num_);

    msg_ = new ListProperty("@init");
    msg_->setInitOnly();
    msg_->setArgIndex(1);
    msg_->checkRangeElementCount(MIN_INLETS, MAX_INLETS);
    addProperty(msg_);
}

void FlowPack::initDone()
{
    msg_->value().resizePad(num_->value(), Atom(0.));

    // (in/out)lets
    for (int i = 1; i < num_->value(); i++)
        createInlet();

    createOutlet();
}

void FlowPack::onBang()
{
    output(0);
}

void FlowPack::onFloat(t_float f)
{
    msg_->value()[0] = Atom(f);
    output(0);
}

void FlowPack::onSymbol(t_symbol* s)
{
    msg_->value()[0] = Atom(s);
    output(0);
}

void FlowPack::onInlet(size_t idx, const AtomListView& l)
{
    const size_t N = msg_->value().size();

    if (!l.empty()) {
        if (idx >= N) {
            OBJ_ERR << "invalid inlet index: " << idx;
            return;
        }

        // spread list values to other inelts
        const size_t NMIN = std::min<size_t>(idx + l.size(), N);
        for (size_t i = idx; i < NMIN; i++)
            msg_->value()[i] = l[i - idx];
    }

    output(idx);
}

void FlowPack::onList(const AtomListView& lv)
{
    const size_t N = msg_->value().size();

    if (lv.size() > N)
        OBJ_ERR << "too many values in list: " << lv.size() << ". Using only first " << N;

    const size_t NMIN = std::min<size_t>(lv.size(), N);
    for (size_t i = 0; i < NMIN; i++)
        msg_->value()[i] = lv[i];

    output(0);
}

void FlowPack::onAny(t_symbol* s, const AtomListView& lv)
{
    const size_t N = msg_->value().size();

    if ((lv.size() + 1) > N)
        OBJ_ERR << "too many atoms in message: " << (lv.size() + 1);

    const size_t NMIN = std::min<size_t>(lv.size() + 1, N);

    msg_->value()[0] = s;
    for (size_t i = 1; i < NMIN; i++)
        msg_->value()[i] = lv[i - 1];

    anyTo(0, msg_->value());
}

void FlowPack::output(size_t inlet_idx)
{
    if (inlet_idx == 0)
        listTo(0, msg_->value());
}

const char* FlowPack::annotateInlet(size_t n) const
{
    if (n == 0)
        return "bang:   output current value as list\n"
               "float:  set first element and output as list\n"
               "symbol: set first element and output as list\n"
               "list:   spread among inlets and output as list\n"
               "any:    spread among inlets and output as message";
    else if (n < MAX_INLETS)
        return descr_buf_[n];
    else
        return nullptr;
}

void FlowPack::initAnnotations()
{
    for (size_t i = 0; i < MAX_INLETS; i++)
        snprintf(descr_buf_[i], DESC_LEN, "atom: set \\[%d\\]\n"
                                          "list: set \\[%d..\\]",
            (int)i, (int)i);
}

void setup_flow_pack()
{
    FlowPack::initAnnotations();

    ObjectFactory<FlowPack> obj("flow.pack");
    obj.noPropsDispatch();
    obj.addOutletInfo("list: packed list\n"
                      "any:  packed message");
}
