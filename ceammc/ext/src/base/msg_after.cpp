/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "msg_after.h"
#include "ceammc_factory.h"

MessageAfter::MessageAfter(const PdArgs& args)
    : BaseObject(args)
    , clock_(this, &MessageAfter::tick)
    , delay_(parsedPosArgs().floatAt(0, 0))
    , msg_(args.args.slice(1))
{
    if (delay_ < 0) {
        OBJ_ERR << "invalid delay value: " << delay_ << ", setting to 0";
        delay_ = 0;
    }

    createInlet(&delay_);
    createOutlet();
}

void MessageAfter::onBang()
{
    bangTo(0);
    run();
}

void MessageAfter::onFloat(t_float f)
{
    floatTo(0, f);
    run();
}

void MessageAfter::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
    run();
}

void MessageAfter::onList(const AtomList& lst)
{
    listTo(0, lst);
    run();
}

void MessageAfter::onAny(t_symbol* s, const AtomListView& lst)
{
    anyTo(0, s, lst);
    run();
}

void MessageAfter::dump() const
{
    OBJ_DBG << "delay: " << delay_;
    BaseObject::dump();
}

void MessageAfter::parseProperties()
{
}

bool MessageAfter::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void MessageAfter::run()
{
    if (delay_ < 0) {
        OBJ_ERR << "invalid delay value: " << delay_;
        return;
    }

    clock_.delay(delay_);
}

void MessageAfter::tick()
{
    if (msg_.size() == 0)
        bangTo(0);
    else {
        if (msg_[0].isSymbol())
            anyTo(0, msg_);
        else
            listTo(0, msg_);
    }
}

void setup_msg_after()
{
    ObjectFactory<MessageAfter> obj("msg.after");

    obj.setDescription("send specified message after incoming message");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "message", "after", "onload" });
    obj.setCategory("msg");
    obj.setSinceVersion(0, 7);
}
