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
#include "dict_each.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

DictEach::DictEach(const PdArgs& args)
    : DictBase(args)
    , current_key_(&s_)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void DictEach::onInlet(size_t n, const AtomListView& lst)
{
    if (lst.isData())
        dict_->insert(current_key_, lst[0]);
    else if (lst.size() > 1)
        dict_->insert(current_key_, lst);
    else if (lst.isAtom())
        dict_->insert(current_key_, lst[0]);
    else
        return; // skip empty values
}

void DictEach::onDataT(const DictAtom& dict)
{
    dict_->clear();

    for (auto& kv : *dict) {
        current_key_ = kv.first;
        listTo(1, kv.second);
    }

    atomTo(0, dict_);
}

void setup_dict_each()
{
    ObjectFactory<DictEach> obj("dict.each");
    obj.processData<DataTypeDict>();

    obj.setDescription("map each value in dict");
    obj.setCategory("data");
    obj.setKeywords({"each", "iterate", "dictionary"});
}
