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

    set_->add(positionalArguments());
}

void DataSet::dump() const
{
    BaseObject::dump();
    OBJ_DBG << set_->toString();
}

void DataSet::onBang()
{
    dataTo(0, set_);
}

void DataSet::onFloat(float f)
{
    set_->add(Atom(f));
}

void DataSet::onSymbol(t_symbol* s)
{
    set_->add(Atom(s));
}

void DataSet::onList(const AtomList& l)
{
    set_.data()->add(l);
}

void DataSet::onDataT(const DataTypeSet& s)
{
    set_.setData(s.clone());
    onBang();
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
    obj.processData<DataTypeSet>();
    obj.addMethod("clear", &DataSet::m_clear);
    obj.addMethod("add", &DataSet::m_add);
    obj.addMethod("remove", &DataSet::m_remove);
}
