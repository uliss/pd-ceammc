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
    createOutlet();

    OBJ_DBG << "only iOS mode is supported";

    bindReceive(gensym("#keyname"));
}

void HwCuvaveCubeTurner::onList(const AtomList& lst)
{
    const bool ok = (lst.size() == 2) && lst[0].isInteger() && lst[1].isSymbol();
    if (!ok)
        return;

    auto ch = lst[1].asT<t_symbol*>()->s_name[0];

    switch (ch) {
    case 'U':
    case 'L':
        floatTo(0, lst[0].asT<int>());
        break;
    case 'D':
    case 'R':
        floatTo(1, lst[0].asT<int>());
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
