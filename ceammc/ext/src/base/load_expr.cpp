/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "load_expr.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"

LoadExpr::LoadExpr(const PdArgs& args)
    : LoadMsg(args)
{
}

void LoadExpr::output()
{
    for (auto& m : messages()) {
        messageTo(0, m);
    }
}

//void LoadExpr::doOutput(const AtomListView& lv)
//{
//    auto res = parseDataList(lv);
//    if (!res) {
//        OBJ_ERR << res.err();
//        return;
//    } else {
//        const auto& msg = res.result();

//        if (msg.empty())
//            bangTo(0);
//        else if (msg.isFloat())
//            floatTo(0, msg[0].asT<t_float>()); //
//        else if (msg.size() > 1 && msg[0] == &s_float) // explicit float: float 123
//            floatTo(0, msg[1].asFloat());
//        else if (msg.size() > 1 && msg[0] == &s_symbol) // symbol A B C
//            symbolTo(0, msg[1].asSymbol());
//        else if (msg.size() > 1 && msg[0].isFloat()) // implicit list: 1 2 3
//            listTo(0, msg);
//        else if (msg.size() > 1 && msg[0] == &s_list) // explicit list: list 1 2 3
//            listTo(0, msg.view(1));
//        else if (msg.isData())
//            atomTo(0, msg[0]);
//        else
//            anyTo(0, msg);
//    }
//}

void setup_load_expr()
{
    ObjectFactory<LoadExpr> obj("loadexpr", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.useClick();
    obj.useLoadBang();
    obj.noPropsDispatch();
    obj.noArgsAndPropsParse();

    obj.setDescription("send message when patch loads");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "message", "loadbang", "onload" });
    obj.setCategory("msg");
    obj.setSinceVersion(0, 9);
}
