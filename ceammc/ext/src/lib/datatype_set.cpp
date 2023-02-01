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
#include "ceammc_containers.h"
#include "ceammc_datastorage.h"
#include "ceammc_string.h"
#include "fmt/core.h"

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/functional/hash.hpp>
#include <ctime>
#include <random>

namespace {
using namespace ceammc;

constexpr const char* TYPE_NAME = "Set";

DataTypeId initType()
{
    DataTypeId id = DataStorage::instance().typeByName(TYPE_NAME);
    if (id == data::DATA_INVALID)
        id = DataStorage::instance().registerNewType(TYPE_NAME,
            [](const AtomListView& lv) -> Atom { return new DataTypeSet(lv); });

    return id;
}

size_t hash_value(const Atom& a) noexcept
{
    size_t res = a.type();

    if (a.isFloat())
        boost::hash_combine(res, boost::hash_value(a.asFloat()));
    else if (a.isSymbol())
        boost::hash_combine(res, boost::hash_value(a.asSymbol()));
    else if (a.isData())
        boost::hash_combine(res, boost::hash_value(a.dataType()));

    return res;
}

}

namespace ceammc {

const DataTypeId DataTypeSet::dataType = initType();

DataTypeSet::DataTypeSet()
    : data_(0, hash_value)
{
}

DataTypeSet::DataTypeSet(const Atom& a)
    : data_(0, hash_value)
{
    add(a);
}

DataTypeSet::DataTypeSet(const AtomListView& lv)
    : data_(0, hash_value)
{
    add(lv);
}

DataTypeSet::DataTypeSet(const AtomList& lst)
    : data_(0, hash_value)
{
    add(lst.view());
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

void DataTypeSet::add(const AtomListView& lv)
{
    for (auto& x : lv)
        add(x);
}

bool DataTypeSet::remove(const Atom& a)
{
    return data_.erase(a);
}

void DataTypeSet::remove(const AtomListView& lv)
{
    for (auto& x : lv)
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

bool DataTypeSet::contains_any_of(const AtomListView& lv) const noexcept
{
    for (auto& a : lv) {
        if (contains(a))
            return true;
    }

    return false;
}

bool DataTypeSet::choose(Atom& res) const noexcept
{
    auto N = data_.size();
    if (N < 1)
        return false;

    std::mt19937 gen(time(0));
    const auto offset = std::uniform_int_distribution<size_t>(0, N - 1)(gen);
    auto it = data_.begin();

    for (size_t i = 0; i < offset; i++)
        ++it;

    res = *it;
    return true;
}

DataTypeId DataTypeSet::type() const noexcept
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

std::string DataTypeSet::toListStringContent() const noexcept
{
    SmallAtomListN<16> lst;
    for (auto& a : data_)
        lst.push_back(a);

    std::sort(lst.begin(), lst.end());

    string::MediumString str;
    string::parsed_list_to_string(lst.view(), str);
    return std::string(str.data(), str.size());
}

std::string DataTypeSet::toDictStringContent() const noexcept
{
    return fmt::format("items: {}", toListStringContent());
}

bool DataTypeSet::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeSet>(d);
}

bool DataTypeSet::looksLikeCtor(const AtomListView& lv) noexcept
{
    if (lv.empty() || !lv[0].isSymbol())
        return false;

    auto name = lv[0].asT<t_symbol*>()->s_name;
    return boost::starts_with(name, TYPE_NAME);
}

std::ostream& operator<<(std::ostream& os, const DataTypeSet& set)
{
    os << set.toString();
    return os;
}

}
