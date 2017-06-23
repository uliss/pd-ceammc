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

namespace ceammc {

DataAtomList::DataAtomList()
{
}

DataAtomList::~DataAtomList()
{
    clear();
}

DataAtomList::DataAtomList(const Atom& a)
    : list_(1, new DataAtom(a))
{
}

DataAtomList::DataAtomList(const AtomList& lst)
{
    set(lst);
}

void DataAtomList::set(const AtomList& lst)
{
    list_.clear();
    list_.reserve(lst.size());

    for (size_t i = 0; i < lst.size(); i++)
        list_.push_back(new DataAtom(lst[i]));
}

size_t DataAtomList::size() const
{
    return list_.size();
}

const DataAtom& DataAtomList::operator[](size_t idx) const
{
    return *list_.at(idx);
}

DataAtom& DataAtomList::operator[](size_t idx)
{
    return *list_.at(idx);
}

void DataAtomList::clear()
{
    for (size_t i = 0; i < list_.size(); i++)
        delete list_[i];

    list_.clear();
}

void DataAtomList::append(const Atom& a)
{
    list_.push_back(new DataAtom(a));
}

void DataAtomList::append(const AbstractData* d)
{
    list_.push_back(new DataAtom(d));
}

AtomList DataAtomList::toList() const
{
    AtomList res;
    res.reserve(list_.size());

    for (size_t i = 0; i < list_.size(); i++)
        res.append(list_[i]->toAtom());

    return res;
}

bool DataAtomList::operator==(const DataAtomList& l) const
{
    if (size() != l.size())
        return false;

    for (size_t i = 0; i < size(); i++) {
        if (!list_[i]->isEqual(l.list_[i]->toAtom()))
            return false;
    }

    return true;
}
}
