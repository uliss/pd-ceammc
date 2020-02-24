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

DataTypeSet::DataTypeSet(DataTypeSet&& ds)
    : data_(std::move(ds.data_))
{
}

DataTypeSet::~DataTypeSet()
{
}

void DataTypeSet::add(const Atom& a)
{
    if (a.isData() && contains(a))
        return;

    data_.insert(DataAtom(a));
}

void DataTypeSet::add(const AtomList& l)
{
    for (auto& el : l)
        add(el);
}

void DataTypeSet::remove(const Atom& a)
{
    data_.erase(DataAtom(a));
}

void DataTypeSet::remove(const AtomList& l)
{
    for (auto& el : l)
        remove(el);
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

    for (auto& el : data_) {
        if (!el.isData())
            continue;

        if (el == DataAtom(a))
            return true;
    }

    return false;
}

bool DataTypeSet::contains(const DataAtom& a) const
{
    return data_.find(a) != data_.end();
}

bool DataTypeSet::contains(const AtomList& lst) const
{
    for (auto& a : lst) {
        if (contains(a))
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

    for (auto& el : data_) {
        if (!ds->contains(el.toAtom()))
            return false;
    }

    return true;
}

AtomList DataTypeSet::toList() const
{
    AtomList res;
    res.reserve(size());

    for (auto a : data_)
        res.append(a.toAtom());

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

    for (auto& s0_el : s0.data_) {
        DataAtom elem(s0_el);
        if (s1.contains(elem.asAtom()))
            out.data_.insert(elem);
    }
}

void DataTypeSet::set_union(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (auto& el : s0.data_)
        out.add(el.toAtom());

    for (auto& el : s1.data_)
        out.add(el.toAtom());
}

void DataTypeSet::difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (auto& el : s0.data_) {
        if (!s1.contains(el.toAtom()))
            out.data_.insert(el);
    }
}

void DataTypeSet::sym_difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1)
{
    out.clear();

    for (auto& el : s0.data_) {
        if (!s1.contains(el.toAtom()))
            out.data_.insert(el);
    }

    for (auto& el : s1.data_) {
        if (!s0.contains(el.toAtom()))
            out.data_.insert(el);
    }
}

DataTypeSet::DataTypeSet(const DataTypeSet& ds)
    : data_(ds.data_)
{
}
