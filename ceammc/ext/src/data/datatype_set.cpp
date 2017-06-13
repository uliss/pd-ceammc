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
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

#include <algorithm>

const DataType DataTypeSet::dataType = ceammc::data::DATA_SET;

DataTypeSet::DataTypeSet()
{
}

DataTypeSet::DataTypeSet(const Atom& a)
{
    add(a);
}

DataTypeSet::DataTypeSet(const AtomList& l)
{
    add(l);
}

DataTypeSet::~DataTypeSet()
{
}

void DataTypeSet::add(const Atom& a)
{
    data_.insert(DataAtom(a));
}

void DataTypeSet::add(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++)
        add(l[i]);
}

void DataTypeSet::remove(const Atom& a)
{
    data_.erase(DataAtom(a));
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
    if (!a.isData())
        return contains(DataAtom(a));

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        if (!it->isData())
            continue;

        if (it->isEqual(a))
            return true;
    }

    return false;
}

bool DataTypeSet::contains(const DataAtom& a) const
{
    DataSet::const_iterator it = data_.find(a);
    return it != data_.end();
}

bool DataTypeSet::contains(const AtomList& lst) const
{
    for (size_t i = 0; i < lst.size(); i++) {
        if (contains(lst[i]))
            return true;
    }

    return false;
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

    const DataTypeSet* ds = d->as<DataTypeSet>();
    if (size() != ds->size())
        return false;

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        if (!ds->contains(it->toAtom()))
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

bool DataTypeSet::operator==(const DataTypeSet& s) const
{
    return isEqual(&s);
}

void DataTypeSet::operator=(const DataTypeSet& s)
{
    if (this == &s)
        return;

    data_ = s.data_;
}

void DataTypeSet::intersection(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    if (s0.size() > s1.size())
        return intersection(out, s1, s0);

    out.clear();
    for (DataSet::const_iterator it = s0.data_.begin(); it != s0.data_.end(); ++it) {
        DataAtom elem(it->toAtom());
        if (s1.data_.find(elem) != s0.data_.end())
            out.data_.insert(elem);
    }
}

void DataTypeSet::set_union(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (DataSet::const_iterator it = s0.data_.begin(); it != s0.data_.end(); ++it)
        out.add(it->toAtom());

    for (DataSet::const_iterator it = s1.data_.begin(); it != s1.data_.end(); ++it)
        out.add(it->toAtom());
}

void DataTypeSet::set_difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (DataSet::const_iterator it = s0.data_.begin(); it != s0.data_.end(); ++it) {
        if (!s1.contains(it->toAtom()))
            out.add(it->toAtom());
    }
}

void DataTypeSet::set_sym_difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (DataSet::const_iterator it = s0.data_.begin(); it != s0.data_.end(); ++it) {
        if (!s1.contains(it->toAtom()))
            out.add(it->toAtom());
    }

    for (DataSet::const_iterator it = s1.data_.begin(); it != s1.data_.end(); ++it) {
        if (!s0.contains(it->toAtom()))
            out.add(it->toAtom());
    }
}

DataTypeSet::DataTypeSet(const DataTypeSet& ds)
    : data_(ds.data_)
{
}
