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
#include "mlist_flatten.h"
#include "ceammc_factory.h"

MListFlatten::MListFlatten(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void MListFlatten::onList(const AtomListView& lv)
{
    DataTypeMList ml(lv);
    listTo(0, ml.flatten().data());
}

void MListFlatten::onDataT(const MListAtom& ml)
{
    atomTo(0, MListAtom(ml->flatten()));
}

void setup_mlist_flatten()
{
    ObjectFactory<MListFlatten> obj("mlist.flatten");
    obj.processData<DataTypeMList>();

    obj.setDescription("flatten nested mlist");
    obj.setCategory("data");
}
