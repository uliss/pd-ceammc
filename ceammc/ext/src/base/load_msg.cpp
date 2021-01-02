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
#include "load_msg.h"
#include "ceammc_factory.h"

LoadMsg::LoadMsg(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void LoadMsg::output()
{
    const auto msg = binbufArgs();

    if (msg.empty())
        bangTo(0);
    else if (msg.isFloat())
        floatTo(0, msg[0].asT<t_float>());
    else {
        auto do_output = [this](const AtomListView& lv) {
            if (lv.empty())
                bangTo(0);
            else if (lv.isFloat())
                floatTo(0, lv[0].asT<t_float>()); //
            else if (lv.size() > 1 && lv[0] == &s_float) // explicit float: float 123
                floatTo(0, lv[1].asFloat());
            else if (lv.size() > 1 && lv[0] == &s_symbol) // symbol A B C
                symbolTo(0, lv[1].asSymbol());
            else if (lv.size() > 1 && lv[0].isFloat()) // implicit list: 1 2 3
                listTo(0, lv);
            else if (lv.size() > 1 && lv[0] == &s_list) // explicit list: list 1 2 3
                listTo(0, lv.subView(1));
            else
                anyTo(0, lv);
        };

        size_t msg_start = 0;
        for (size_t i = 0; i < msg.size(); i++) {
            auto& a = msg[i];
            if (a.isComma()) {
                const auto mpart = msg.subView(msg_start, i - msg_start);
                do_output(mpart);
                msg_start = i + 1;
            }
        }

        if (msg_start < msg.size())
            do_output(msg.subView(msg_start, msg.size() - msg_start));
    }
}

void LoadMsg::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/, t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    output();
}

void LoadMsg::onLoadBang()
{
    output();
}

void setup_load_msg()
{
    ObjectFactory<LoadMsg> obj("msg.onload", OBJECT_FACTORY_NO_DEFAULT_INLET);
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
