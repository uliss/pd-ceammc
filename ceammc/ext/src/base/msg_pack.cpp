/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "msg_pack.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/core.h"

constexpr int NUM_DEF = 2;
constexpr int NUM_MIN = 1;
constexpr int NUM_MAX = 32;

MsgPack::MsgPack(const PdArgs& args)
    : BaseObject(args)
{
    n_ = new IntProperty("@n", NUM_DEF);
    n_->checkClosedRange(NUM_MIN, NUM_MAX);
    n_->setInitOnly();
    n_->setArgIndex(0);
    addProperty(n_);

    prefix_ = new ListProperty("@msg");
    prefix_->setSuccessFn([this](Property*) { updateMessage(); });
    prefix_->setArgIndex(1);
    addProperty(prefix_);

    sync_ = new BoolProperty("@sync", false);
    addProperty(sync_);

    createOutlet();
}

void MsgPack::initDone()
{
    updateMessage();

    for (int i = 1; i < n_->value(); i++)
        createInlet();
}

void MsgPack::onBang()
{
    output();
}

void MsgPack::onFloat(t_float f)
{
    if (checkIndex(0)) {
        at(0) = f;
        output();
    }
}

void MsgPack::onSymbol(t_symbol* s)
{
    if (checkIndex(0)) {
        at(0) = s;
        output();
    }
}

void MsgPack::onInlet(size_t n, const AtomListView& lv)
{
    if (checkIndex(n) && lv.isAtom()) {
        at(n) = lv[0];
        if (sync_->value())
            output();
    } else
        OBJ_ERR << fmt::format("single atom expected to {} inlet, got: {}", n, to_string(lv));
}

void MsgPack::updateMessage()
{
    int PREFIX_SZ = prefix_->value().size();
    int SZ = PREFIX_SZ + n_->value();
    int OLD_SZ = msg_.size();
    int DIFF = SZ - OLD_SZ;

    if (DIFF > 0 && (DIFF <= PREFIX_SZ))
        msg_.insert(PREFIX_SZ - DIFF, AtomList::zeroes(DIFF)); // move data
    else
        msg_.resizePad(SZ, 0.0);

    // update prefix
    std::copy(prefix_->value().begin(), prefix_->value().end(), msg_.begin());
}

void MsgPack::output()
{
    if (msg_.isBang() || msg_.isMsgBang())
        bangTo(0);
    else if (msg_.isMsgSymbol())
        symbolTo(0, msg_.asMsgSymbol());
    else if (msg_.isMsgFloat())
        floatTo(0, msg_.asMsgFloat());
    else if (msg_.isFloat())
        floatTo(0, msg_[0].asFloat());
    else if (msg_.isLogicList())
        listTo(0, msg_.asLogicList());
    else if (msg_.isMsgAny())
        anyTo(0, msg_);
    else
        OBJ_ERR << "unsupported message: " << msg_;
}

void setup_base_msg_pack()
{
    ObjectFactory<MsgPack> obj("msg.pack");
    obj.noPropsDispatch();
}
