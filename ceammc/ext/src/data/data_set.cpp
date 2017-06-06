/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "data_set.h"
#include "ceammc_factory.h"

DataSet::DataSet(const PdArgs& a)
    : BaseObject(a)
    , set_(new DataTypeSet())
{
    createOutlet();

    set_.data()->add(positionalArguments());
}

void DataSet::dump() const
{
    BaseObject::dump();
    OBJ_DBG << set_.data()->toString();
}

void DataSet::onBang()
{
    dataTo(0, set_.toAtom());
}

void DataSet::onFloat(float f)
{
    set_.data()->add(Atom(f));
}

void DataSet::onSymbol(t_symbol* s)
{
    set_.data()->add(Atom(s));
}

void DataSet::onData(const BaseData* d)
{
    DataTypeSet* ds = d->cloneT<DataTypeSet>();
    if (ds) {
        set_.set(*ds);
    } else {
    }
}

void DataSet::onList(const AtomList& l)
{
    set_.data()->add(l);
}

void DataSet::m_toList(t_symbol*, const AtomList&)
{
    listTo(0, set_.data()->toList());
}

void DataSet::m_clear(t_symbol*, const AtomList&)
{
    set_.data()->clear();
}

void DataSet::m_add(t_symbol*, const AtomList& l)
{
    set_.data()->add(l);
}

void DataSet::m_remove(t_symbol*, const AtomList& l)
{
    set_.data()->remove(l);
}

extern "C" void setup_data0x2eset()
{
    ObjectFactory<DataSet> obj("data.set");
    obj.addMethod("to_list", &DataSet::m_toList);
    obj.addMethod("clear", &DataSet::m_clear);
    obj.addMethod("add", &DataSet::m_add);
    obj.addMethod("remove", &DataSet::m_remove);
}
