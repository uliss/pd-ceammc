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
#include "hw_cuvave_cubeturner.h"
#include "ceammc_factory.h"

HwCuvaveCubeTurner::HwCuvaveCubeTurner(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    OBJ_DBG << "only iOS mode is supported";

    bindReceive(gensym("#keyname"));
}

void HwCuvaveCubeTurner::onList(const AtomList& lst)
{
    if (!checkArgs(lst.view(), ARG_INT, ARG_SYMBOL))
        return;

    auto ch = lst[1].asT<t_symbol*>()->s_name[0];
    Atom data[2];

    switch (ch) {
    case 'U':
        data[0] = 0.0;
        data[1] = lst[0].asT<int>();
        anyTo(0, gensym("p"), AtomListView(data, 2));
        break;
    case 'D':
        data[0] = 1.0;
        data[1] = lst[0].asT<int>();
        anyTo(0, gensym("p"), AtomListView(data, 2));
        break;
    case 'L':
        data[0] = 0.0;
        data[1] = lst[0].asT<int>();
        anyTo(0, gensym("l"), AtomListView(data, 2));
        break;
    case 'R':
        data[0] = 1.0;
        data[1] = lst[0].asT<int>();
        anyTo(0, gensym("l"), AtomListView(data, 2));
        break;
    default:
        break;
    }
}

void setup_hw_cuvave_cubeturner()
{
    ObjectFactory<HwCuvaveCubeTurner> obj("hw.cubeturner",
        OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_DEFAULT_INLET);
}
