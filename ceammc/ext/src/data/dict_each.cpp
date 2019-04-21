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
    : BaseObject(args)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void DictEach::onInlet(size_t n, const AtomList& lst)
{
    if (lst.isData())
        dict_.insert(current_key_, DataAtom(lst[0]));
    else if (lst.isList())
        dict_.insert(current_key_, lst);
    else if (lst.size() == 1)
        dict_.insert(current_key_, lst[0]);
    else
        return; // skip empty values
}

void DictEach::onDataT(const DataTPtr<DataTypeDict>& dptr)
{
    dict_.clear();

    const DataTypeDict::DictMap& dict = dptr->innerData();
    for (auto& kv : dict) {
        current_key_ = kv.first;
        auto val = kv.second;

        if (val.type() == typeid(Atom))
            atomTo(1, boost::get<Atom>(val));
        else if (val.type() == typeid(AtomList))
            listTo(1, boost::get<AtomList>(val));
        else if (val.type() == typeid(DataAtom)) {
            const DataAtom& datom = boost::get<DataAtom>(val);
            to_outlet(outletAt(1), datom);
        }
    }

    dataTo(0, DataTPtr<DataTypeDict>(dict_));
}

void setup_dict_each()
{
    ObjectFactory<DictEach> obj("dict.each");
    obj.processData<DataTypeDict>();
}
