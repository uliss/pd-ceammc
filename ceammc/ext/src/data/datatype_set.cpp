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
#include "datatype_set.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

const DataType DataTypeSet::dataType = 2;

DataTypeSet::DataTypeSet()
{
    LIB_DBG << "set created";
}

DataTypeSet::~DataTypeSet()
{
    LIB_DBG << "set destroyed";
}

void DataTypeSet::add(const Atom& a)
{
    data_.insert(a);
}

void DataTypeSet::add(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++)
        add(l[i]);
}

void DataTypeSet::remove(const Atom& a)
{
    data_.erase(a);
}

void DataTypeSet::remove(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++)
        remove(l[i]);
}

void DataTypeSet::clear()
{
    data_.clear();
}

size_t DataTypeSet::size() const
{
    return data_.size();
}

bool DataTypeSet::contains(const Atom& a) const
{
    return contains(DataAtom(a));
}

bool DataTypeSet::contains(const DataAtom& a) const
{
    DataSet::const_iterator it = data_.find(a);
    return it != data_.end();
}

std::string DataTypeSet::toString() const
{
    return std::string("Set ") + to_string(toList());
}

DataType DataTypeSet::type() const
{
    return dataType;
}

bool DataTypeSet::isEqual(const AbstractData* d) const
{
    if (!d || d->type() != dataType)
        return false;

    const DataTypeSet* ds = static_cast<const DataTypeSet*>(d);
    if (size() != ds->size())
        return false;

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        if (!ds->contains(*it))
            return false;
    }

    return true;
}

AtomList DataTypeSet::toList() const
{
    AtomList res;
    res.reserve(size());

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        res.append(it->toAtom());
    }

    return res;
}

AbstractData* DataTypeSet::clone() const
{
    return new DataTypeSet(*this);
}

DataTypeSet::DataTypeSet(const DataTypeSet& ds)
    : data_(ds.data_)
{
}
