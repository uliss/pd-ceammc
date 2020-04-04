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
#include "list_contains.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListContains::ListContains(const PdArgs& args)
    : BaseObject(args)
    , needle_(positionalArguments())
{
    createInlet();
    createOutlet();
}

void ListContains::onList(const AtomList& lst)
{
    auto it = std::search(lst.begin(), lst.end(), needle_.begin(), needle_.end());
    boolTo(0, it != lst.end());
}

void ListContains::onDataT(const MListAtom& ml)
{
    LIB_ERR << ml;
    auto it = std::search(ml->begin(), ml->end(), needle_.begin(), needle_.end());
    boolTo(0, it != ml->end());
}

void ListContains::onInlet(size_t n, const AtomList& lst)
{
    needle_ = lst;
}

void setup_list_contains()
{
    ObjectFactory<ListContains> obj("list.contains");
    obj.processData<DataTypeMList>();
}
