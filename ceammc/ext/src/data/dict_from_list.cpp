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
#include "dict_from_list.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"

constexpr int MIN_STEP = 2;
constexpr int DEFAULT_STEP = 2;

DictFromList::DictFromList(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    n_ = new IntProperty("@step", DEFAULT_STEP);
    n_->setArgIndex(0);
    n_->checkMinEq(MIN_STEP);
    addProperty(n_);

    createOutlet();
}

void DictFromList::onList(const AtomListView& lv)
{
    DictAtom dict;
    *dict = DataTypeDict::fromList(lv, n_->value());
    atomTo(0, dict);
}

void DictFromList::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_dict_from_list()
{
    ObjectFactory<DictFromList> obj("dict.from_list");
    obj.addAlias("list->dict");
    obj.processData<DataTypeMList>();

    obj.setDescription("creates dictionary from list");
    obj.setCategory("data");
    obj.setKeywords({"dictionary"});
}
