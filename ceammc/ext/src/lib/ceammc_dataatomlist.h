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
#ifndef CEAMMC_DATAATOMLIST_H
#define CEAMMC_DATAATOMLIST_H

#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"

#include <boost/iterator/filter_iterator.hpp>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <vector>

namespace ceammc {

class DataAtomList {
    std::vector<DataAtom> list_;

public:
    DataAtomList();
    ~DataAtomList();
    DataAtomList(const Atom& a);
    DataAtomList(const AtomList& lst);
    DataAtomList(std::initializer_list<Atom> lst);
    DataAtomList(std::initializer_list<DataAtom> lst);
    DataAtomList(std::initializer_list<DataPtr> lst);

    // copy/move
    DataAtomList(const DataAtomList& a);
    DataAtomList(DataAtomList&& a);
    DataAtomList& operator=(const DataAtomList& a);
    DataAtomList& operator=(DataAtomList&& a);

    void set(const AtomList& lst);

    bool empty() const;
    size_t size() const;
    DataAtom& operator[](size_t idx);
    const DataAtom& operator[](size_t idx) const;
    void clear();

    void append(const Atom& a);
    void append(const DataPtr& d);
    void append(const DataAtom& data);
    void append(const AtomList& lst);
    void append(const DataAtomList& lst);
    bool insert(size_t pos, const AtomList& lst);
    bool insert(size_t pos, const DataAtomList& lst);
    void prepend(const Atom& a);
    void prepend(const DataPtr& d);
    void prepend(const DataAtom& data);
    void prepend(const AtomList& lst);
    void prepend(const DataAtomList& lst);
    bool pop();
    bool remove(size_t pos);
    void reserve(size_t sz);

    AtomList toList() const;

    bool operator==(const DataAtomList& l) const;

    bool isSingleData() const;
    bool isSingleDataType(DataType t) const;
    template <typename T>
    bool isSingleDataType() const;
    template <typename T>
    const T* asSingle() const;

public:
    static const size_t END = std::numeric_limits<size_t>::max();

public:
    typedef std::vector<DataAtom> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;
    typedef container::value_type value_type;
    typedef bool (*DataAtomPredicate)(const DataAtom&);
    typedef boost::filter_iterator<DataAtomPredicate, iterator> filter_iterator;
    typedef boost::filter_iterator<DataAtomPredicate, const_iterator> const_filter_iterator;

    const_iterator begin() const { return list_.begin(); }
    const_iterator end() const { return list_.end(); }

    iterator begin() { return list_.begin(); }
    iterator end() { return list_.end(); }

    filter_iterator begin_filter(DataAtomPredicate pred);
    filter_iterator end_filter();

    const_filter_iterator begin_filter(DataAtomPredicate pred) const;
    const_filter_iterator end_filter() const;

    bool contains(const DataPtr& p) const;
    bool contains(const DataAtom& p) const;
    bool contains(const Atom& p) const;
    bool contains(const AtomList& p) const;

    long search(const Atom& p, size_t from = 0, size_t to = END) const;
    long search(const DataPtr& p, size_t from = 0, size_t to = END) const;
    long search(const DataAtom& p, size_t from = 0, size_t to = END) const;
    long search(const AtomList& p, size_t from = 0, size_t to = END) const;
};

template <typename T>
const T* DataAtomList::asSingle() const
{
    if (!isSingleDataType<T>())
        return nullptr;

    return list_[0].as<T>();
}

template <typename T>
bool DataAtomList::isSingleDataType() const
{
    return isSingleDataType(T::dataType);
}

std::ostream& operator<<(std::ostream& os, const DataAtomList& l);
}

#endif // CEAMMC_DATAATOMLIST_H
