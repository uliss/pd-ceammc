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
#include "msg_onload.h"
#include "ceammc_factory.h"

MsgOnLoad::MsgOnLoad(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void MsgOnLoad::initDone()
{
    AtomList raw_args;
    binbufArgs().restorePrimitives(raw_args);
    raw_args.expandDollarArgs(canvas());
    raw_args.view().split(Atom::comma(),
        [this](const AtomListView& lv) {
            appendMessage(lv);
        });
}

void MsgOnLoad::output()
{
    for (auto& m : msg_)
        messageTo(0, m);
}

void MsgOnLoad::appendMessage(const AtomListView& lv)
{
    msg_.push_back(Message::makeTyped(lv));
}

void MsgOnLoad::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/, t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    output();
}

void MsgOnLoad::onLoadBang()
{
    output();
}

void setup_msg_onload()
{
    ObjectFactory<MsgOnLoad> obj("msg.onload", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.addAlias("loadmsg");
    obj.useClick();
    obj.useLoadBang();
    obj.noPropsDispatch();
    obj.noArgsAndPropsParse();

    obj.setDescription("send message when patch loads");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "message", "loadbang", "onload" });
    obj.setCategory("msg");
    obj.setSinceVersion(0, 7);
}
