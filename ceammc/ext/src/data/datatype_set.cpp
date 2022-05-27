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
#include "ceammc_abstractdata.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

#include <algorithm>
#include <boost/functional/hash.hpp>

namespace {

Atom newFromList(const AtomList& lst)
{
    return new DataTypeSet(lst);
}
}

const int DataTypeSet::dataType = DataStorage::instance().registerNewType("Set", newFromList);

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

DataTypeSet::DataTypeSet(DataTypeSet&& ds) noexcept
    : data_(std::move(ds.data_))
{
}

DataTypeSet::~DataTypeSet() noexcept = default;

void DataTypeSet::add(const Atom& a)
{
    data_.insert(a);
}

void DataTypeSet::add(const AtomList& l)
{
    for (auto& x : l)
        add(x);
}

void DataTypeSet::remove(const Atom& a)
{
    data_.erase(a);
}

void DataTypeSet::remove(const AtomList& l)
{
    for (auto& x : l)
        remove(x);
}

void DataTypeSet::clear() noexcept
{
    data_.clear();
}

size_t DataTypeSet::size() const noexcept
{
    return data_.size();
}

bool DataTypeSet::contains(const Atom& a) const noexcept
{
    return data_.find(a) != data_.end();
}

bool DataTypeSet::contains_any_of(const AtomList& lst) const noexcept
{
    for (auto& a : lst) {
        if (contains(a))
            return true;
    }

    return false;
}

std::string DataTypeSet::toString() const
{
    return to_string(toList());
}

int DataTypeSet::type() const noexcept
{
    return dataType;
}

bool DataTypeSet::isEqual(const AbstractData* d) const noexcept
{
    if (!d || d->type() != dataType)
        return false;

    const DataTypeSet* ds = d->as<DataTypeSet>();
    if (size() != ds->size())
        return false;

    return operator==(*ds);
}

AtomList DataTypeSet::toList(bool sorted) const
{
    AtomList res;
    res.reserve(size());

    for (auto& a : data_)
        res.append(a);

    if (sorted)
        res.sort();

    return res;
}

AbstractData* DataTypeSet::clone() const
{
    return new DataTypeSet(*this);
}

bool DataTypeSet::operator==(const DataTypeSet& s) const noexcept
{
    return data_ == s.data_;
}

void DataTypeSet::operator=(const DataTypeSet& s)
{
    if (this == &s)
        return;

    data_ = s.data_;
}

DataTypeSet DataTypeSet::intersection(const DataTypeSet& s0, const DataTypeSet& s1)
{
    if (s0.size() > s1.size())
        return intersection(s1, s0);

    DataTypeSet out;

    for (auto& x : s0.data_) {
        if (s1.contains(x))
            out.data_.insert(x);
    }

    return out;
}

DataTypeSet DataTypeSet::set_union(const DataTypeSet& s0, const DataTypeSet& s1)
{
    DataTypeSet out;
    out.data_ = s0.data_;
    out.data_.insert(s1.data_.begin(), s1.data_.end());
    return out;
}

DataTypeSet DataTypeSet::difference(const DataTypeSet& s0, const DataTypeSet& s1)
{
    DataTypeSet out;

    for (auto& x : s0.data_) {
        if (!s1.contains(x))
            out.data_.insert(x);
    }

    return out;
}

DataTypeSet DataTypeSet::sym_difference(const DataTypeSet& s0, const DataTypeSet& s1)
{
    DataTypeSet out;

    for (auto& x : s0.data_) {
        if (!s1.contains(x))
            out.data_.insert(x);
    }

    for (auto& x : s1.data_) {
        if (!s0.contains(x))
            out.data_.insert(x);
    }

    return out;
}

DataTypeSet::DataTypeSet(const DataTypeSet& ds)
    : data_(ds.data_)
{
}

size_t hash_value(const Atom& a) noexcept
{
    size_t res = a.type();

    if (a.isFloat())
        boost::hash_combine(res, boost::hash_value(a.asFloat()));
    else if (a.isSymbol())
        boost::hash_combine(res, boost::hash_value(a.asSymbol()));
    else if (a.isData()) {
        boost::hash_combine(res, boost::hash_value(a.dataType()));
        boost::hash_combine(res, boost::hash_value(a.asData()));
    }

    return res;
}
