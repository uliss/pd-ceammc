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
#include "list_rlencode.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_containers.h"

ListRLEncode::ListRLEncode(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void ListRLEncode::onList(const AtomListView& lv)
{
    auto rle = list::rleEncode(lv);

    AtomList32 seq, len;
    seq.reserve(rle.size());
    len.reserve(rle.size());

    for (auto& el : rle) {
        seq.push_back(el.first);
        len.push_back(el.second);
    }

    listTo(1, len.view());
    listTo(0, seq.view());
}

void setup_list_rlencode()
{
    ObjectFactory<ListRLEncode> obj("list.rlencode");
    obj.addAlias("list.wrap");
}
