/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#include "msg_unpack.h"
#include "ceammc_factory.h"

constexpr int NUM_DEF = 1;
constexpr int NUM_MIN = 1;
constexpr int NUM_MAX = 32;

MsgUnpack::MsgUnpack(const PdArgs& args)
    : BaseObject(args)
{
    n_ = new IntProperty("@n", NUM_DEF);
    n_->checkClosedRange(NUM_MIN, NUM_MAX);
    n_->setInitOnly();
    n_->setArgIndex(0);
    addProperty(n_);
}

void MsgUnpack::initDone()
{
    for (int i = 0; i < n_->value(); i++)
        createOutlet();
}

void MsgUnpack::onBang()
{
    symbolTo(0, &s_bang);
}

void MsgUnpack::onFloat(t_float f)
{
    if (checkOutlet(1))
        floatTo(1, f);

    symbolTo(0, &s_float);
}

void MsgUnpack::onSymbol(t_symbol* s)
{
    if (checkOutlet(1))
        symbolTo(1, s);

    symbolTo(0, &s_symbol);
}

void MsgUnpack::onList(const AtomListView& lv)
{
    outputOther(lv);
    symbolTo(0, &s_list);
}

void MsgUnpack::onAny(t_symbol* s, const AtomListView& lv)
{
    outputOther(lv);
    symbolTo(0, s);
}

void MsgUnpack::outputOther(const AtomListView& lv)
{
    const size_t NOUT = n_->value();
    const auto MAX_OUT = std::min<size_t>(NOUT - 1, lv.size());

    for (size_t i = MAX_OUT; i > 0; i--)
        atomTo(i, lv[i - 1]);
}

void setup_base_msg_unpack()
{
    ObjectFactory<MsgUnpack> obj("msg.unpack");
    obj.noPropsDispatch();
}
