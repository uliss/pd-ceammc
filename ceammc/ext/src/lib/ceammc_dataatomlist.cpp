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
#include "ceammc_dataatomlist.h"
#include "ceammc_format.h"

#include <algorithm>

namespace ceammc {

DataAtomList::DataAtomList()
{
}

DataAtomList::~DataAtomList()
{
    clear();
}

DataAtomList::DataAtomList(const Atom& a)
    : list_(1, DataAtom(a))
{
}

DataAtomList::DataAtomList(const AtomList& lst)
{
    set(lst);
}

DataAtomList::DataAtomList(std::initializer_list<Atom> lst)
    : list_(lst.begin(), lst.end())
{
}

DataAtomList::DataAtomList(std::initializer_list<DataAtom> lst)
    : list_(lst.begin(), lst.end())
{
}

DataAtomList::DataAtomList(std::initializer_list<DataPtr> lst)
    : list_(lst.begin(), lst.end())
{
}

DataAtomList::DataAtomList(const DataAtomList& a)
    : list_(a.list_)
{
}

DataAtomList::DataAtomList(DataAtomList&& a)
    : list_(std::move(a.list_))
{
}

DataAtomList& DataAtomList::operator=(const DataAtomList& a)
{
    if (this == &a)
        return *this;

    list_ = a.list_;
    return *this;
}

DataAtomList& DataAtomList::operator=(DataAtomList&& a)
{
    if (this == &a)
        return *this;

    list_ = std::move(a.list_);
    return *this;
}

void DataAtomList::set(const AtomList& lst)
{
    list_.clear();
    list_.reserve(lst.size());

    for (auto& el : lst)
        list_.push_back(DataAtom(el));
}

bool DataAtomList::empty() const
{
    return list_.empty();
}

size_t DataAtomList::size() const
{
    return list_.size();
}

const DataAtom& DataAtomList::operator[](size_t idx) const
{
    return list_.at(idx);
}

DataAtom& DataAtomList::operator[](size_t idx)
{
    return list_.at(idx);
}

void DataAtomList::clear()
{
    list_.clear();
}

void DataAtomList::append(const Atom& a)
{
    list_.push_back(DataAtom(a));
}

void DataAtomList::append(const DataPtr& d)
{
    list_.push_back(DataAtom(d));
}

void DataAtomList::append(const DataAtom& data)
{
    list_.push_back(data);
}

void DataAtomList::append(const AtomList& lst)
{
    append(DataAtomList(lst));
}

void DataAtomList::append(const DataAtomList& lst)
{
    list_.insert(list_.end(), lst.begin(), lst.end());
}

bool DataAtomList::insert(size_t pos, const AtomList& lst)
{
    return insert(pos, DataAtomList(lst));
}

bool DataAtomList::insert(size_t pos, const DataAtomList& lst)
{
    if (pos > list_.size())
        return false;

    list_.insert(list_.begin() + pos, lst.begin(), lst.end());
    return true;
}

void DataAtomList::prepend(const Atom& a)
{
    list_.insert(list_.begin(), DataAtom(a));
}

void DataAtomList::prepend(const DataPtr& d)
{
    list_.insert(list_.begin(), DataAtom(d));
}

void DataAtomList::prepend(const DataAtom& data)
{
    list_.insert(list_.begin(), data);
}

void DataAtomList::prepend(const AtomList& lst)
{
    prepend(DataAtomList(lst));
}

void DataAtomList::prepend(const DataAtomList& lst)
{
    list_.insert(list_.begin(), lst.begin(), lst.end());
}

bool DataAtomList::pop()
{
    if (list_.empty())
        return false;

    list_.pop_back();
    return true;
}

bool DataAtomList::remove(size_t pos)
{
    if (pos >= list_.size())
        return false;

    list_.erase(list_.begin() + pos);
    return true;
}

void DataAtomList::reserve(size_t sz)
{
    list_.reserve(sz);
}

AtomList DataAtomList::toList() const
{
    AtomList res;
    res.reserve(list_.size());

    for (size_t i = 0; i < list_.size(); i++)
        res.append(list_[i].toAtom());

    return res;
}

bool DataAtomList::operator==(const DataAtomList& l) const
{
    if (size() != l.size())
        return false;

    for (size_t i = 0; i < size(); i++) {
        if (!(list_[i] == l.list_[i]))
            return false;
    }

    return true;
}

bool DataAtomList::isSingleData() const
{
    return list_.size() == 1 && list_[0].isData();
}

bool DataAtomList::isSingleDataType(DataType t) const
{
    return list_.size() == 1 && list_[0].isDataType(t);
}

DataAtomList::filter_iterator DataAtomList::begin_filter(DataAtomList::DataAtomPredicate pred)
{
    return filter_iterator(pred, list_.begin(), list_.end());
}

DataAtomList::filter_iterator DataAtomList::end_filter()
{
    return filter_iterator(nullptr, list_.end(), list_.end());
}

DataAtomList::const_filter_iterator DataAtomList::begin_filter(DataAtomList::DataAtomPredicate pred) const
{
    return const_filter_iterator(pred, list_.begin(), list_.end());
}

DataAtomList::const_filter_iterator DataAtomList::end_filter() const
{
    return const_filter_iterator(nullptr, list_.end(), list_.end());
}

bool DataAtomList::contains(const DataPtr& p) const
{
    return search(p) >= 0;
}

bool DataAtomList::contains(const DataAtom& p) const
{
    return search(p) >= 0;
}

bool DataAtomList::contains(const Atom& p) const
{
    return search(p) >= 0;
}

bool DataAtomList::contains(const AtomList& p) const
{
    return search(p) >= 0;
}

template <class T, class U>
long search_list(const DataAtomList::container& lst, const T& needle, size_t from, size_t to, U pred)
{
    if (from >= lst.size())
        return -1;

    to = std::min<size_t>(to, lst.size());

    if (from >= to)
        return -1;

    auto start = lst.begin() + from;
    auto end = lst.begin() + to;
    auto it = std::find_if(start, end, pred);
    // distance from very beginning or -1
    return it != end ? std::distance(lst.begin(), it) : -1;
}

long DataAtomList::search(const Atom& p, size_t from, size_t to) const
{
    return search_list(list_, p, from, to, [&](const value_type& v) { return v == DataAtom(p); });
}

long DataAtomList::search(const DataPtr& p, size_t from, size_t to) const
{
    return search_list(list_, p, from, to, [&](const value_type& v) { return v == DataAtom(p); });
}

long DataAtomList::search(const DataAtom& p, size_t from, size_t to) const
{
    return search_list(list_, p, from, to, [&](const value_type& v) { return v == p; });
}

long DataAtomList::search(const AtomList& p, size_t from, size_t to) const
{
    if (p.empty())
        return -1;

    if (from >= list_.size())
        return -1;

    to = std::min<size_t>(to, list_.size());

    if (from >= to)
        return -1;

    auto b = begin() + from;
    auto e = begin() + to;

    auto it = std::search(b, e, p.begin(), p.end(),
        [](const value_type& v1, const Atom& v2) { return v1 == DataAtom(v2); });
    return it != e ? std::distance(begin(), it) : -1;
}

std::ostream& operator<<(std::ostream& os, const DataAtomList& l)
{
    for (size_t i = 0; i < l.size(); i++) {
        if (i != 0)
            os << " ";

        os << to_string(l[i].toAtom());
    }

    return os;
}
}
