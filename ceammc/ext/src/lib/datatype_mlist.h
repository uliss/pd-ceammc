/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef DATATYPE_MLIST_H
#define DATATYPE_MLIST_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"

#include <boost/optional.hpp>
#include <iostream>

namespace ceammc {

class DataTypeMList : public AbstractData {
    AtomList data_;

public:
    using iterator = AtomList::iterator;
    using const_iterator = AtomList::const_iterator;
    using filter_iterator = AtomList::atom_filter_iterator;
    using const_filter_iterator = AtomList::const_atom_filter_iterator;

public:
    /**
     * create empty list
     */
    DataTypeMList() noexcept;

    /**
     * create with elements from given list
     */
    DataTypeMList(const AtomList& lst);
    DataTypeMList(AtomList&& lst) noexcept;

    /**
     * Creates list with variable args
     */
    template <typename... Args>
    explicit DataTypeMList(Args... args)
        : DataTypeMList(AtomList(args...))
    {
    }

    // copy/move
    DataTypeMList(const DataTypeMList& mlist);
    DataTypeMList(DataTypeMList&& mlist) noexcept;
    DataTypeMList& operator=(const DataTypeMList& mlist);
    DataTypeMList& operator=(DataTypeMList&& mlist);

    int type() const noexcept final;
    DataTypeMList* clone() const final;
    bool isEqual(const AbstractData* cmp) const noexcept final;
    std::string toString() const final;
    std::string valueToJsonString() const override;

    AtomList& data() noexcept { return data_; }
    const AtomList& data() const noexcept { return data_; }
    template <class Fn>
    AtomList toList(Fn pred) const;

    // main
    /**
     * Checks if list is empty
     */
    bool empty() const noexcept { return data_.empty(); }

    /**
     * Number of elements in list
     */
    size_t size() const noexcept { return data_.size(); }

    const Atom& at(size_t n) const { return data_[n]; }
    Atom& at(size_t n) { return data_[n]; }
    const Atom& operator[](size_t n) const { return data_[n]; }
    Atom& operator[](size_t n) { return data_[n]; }

    void append(const Atom& a);
    void append(const AtomList& lst);
    void clear();
    bool insert(size_t idx, const AtomList& lst);
    void prepend(const Atom& a);
    void prepend(const AtomList& lst);
    bool pop();
    bool remove(size_t idx);
    void reserve(size_t n);
    void setRaw(const AtomList& lst);
    void setParsed(const AtomList& lst);

    const_iterator begin() const { return data_.begin(); }
    iterator begin() { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    iterator end() { return data_.end(); }

    const_filter_iterator begin_filter(AtomPredicate pred) const { return data_.begin_atom_filter(pred); }
    filter_iterator begin_filter(AtomPredicate pred) { return data_.begin_atom_filter(pred); }
    const_filter_iterator end_filter() const { return data_.end_atom_filter(); }
    filter_iterator end_filter() { return data_.end_atom_filter(); }

    // fn
    DataTypeMList rotateLeft(int steps) const;
    DataTypeMList flatten() const;
    DataTypeMList slice(int start, int end = -1, size_t step = 1) const;
    // sort only floats and symbols
    void sort();
    void reverse();
    void shuffle();
    bool contains(const Atom& a) const;
    bool contains(const DataTypeMList& l) const;

    bool operator==(const DataTypeMList& ml) const { return data_ == ml.data_; }
    bool operator!=(const DataTypeMList& ml) const { return !operator==(ml); }

public:
    using MaybeList = boost::optional<DataTypeMList>;

public:
    static int dataType;
    static MaybeList parse(const AtomList& lst);
    static MaybeList parse(const std::string& lst);
};

template <class Fn>
AtomList DataTypeMList::toList(Fn pred) const
{
    AtomList res;
    res.reserve(size());

    for (auto& el : data_) {
        if (pred(el))
            res.append(el);
    }

    return res;
}

std::ostream& operator<<(std::ostream& os, const DataTypeMList& d);

}

#endif // DATATYPE_MLIST_H
