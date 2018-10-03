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

#include "ceammc_data.h"
#include "ceammc_dataatomlist.h"

#include <boost/optional.hpp>

using namespace ceammc;

struct mlist_node;

class DataTypeMList : public AbstractData {
    DataAtomList data_;

public:
    typedef DataAtomList::iterator iterator;
    typedef DataAtomList::const_iterator const_iterator;
    typedef DataAtomList::filter_iterator filter_iterator;
    typedef DataAtomList::const_filter_iterator const_filter_iterator;
    typedef DataAtomList::DataAtomPredicate DataAtomPredicate;

public:
    DataTypeMList();
    DataTypeMList(const std::string& str);
    DataTypeMList(const AtomList& lst);
    DataTypeMList(AtomList&& lst);

    // copy/move
    DataTypeMList(const DataTypeMList& mlist);
    DataTypeMList(DataTypeMList&& mlist);
    DataTypeMList& operator=(const DataTypeMList& mlist);
    DataTypeMList& operator=(DataTypeMList&& mlist);

    DataType type() const final;
    DataTypeMList* clone() const final;
    bool isEqual(const AbstractData* cmp) const final;
    void dump() override;
    std::string toString() const final;

    DataAtomList& data() { return data_; }
    const DataAtomList& data() const { return data_; }
    AtomList toList() const;
    template <class Fn>
    AtomList toList(Fn pred) const;

    // main
    bool empty() const;
    size_t size() const;
    const DataAtom& at(size_t n) const;
    DataAtom& at(size_t n);
    const DataAtom& operator[](size_t n) const;
    DataAtom& operator[](size_t n);
    void append(const Atom& a);
    void append(const DataAtom& a);
    void append(const AtomList& lst);
    void append(const DataAtomList& lst);
    void clear();
    bool insert(size_t idx, const AtomList& lst);
    bool insert(size_t idx, const DataAtomList& lst);
    void prepend(const Atom& a);
    void prepend(const DataAtom& a);
    void prepend(const AtomList& lst);
    void prepend(const DataAtomList& lst);
    bool pop();
    bool remove(size_t idx);
    void reserve(size_t n);
    void set(const AtomList& lst);

    const_iterator begin() const { return data_.begin(); }
    iterator begin() { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    iterator end() { return data_.end(); }

    const_filter_iterator begin_filter(DataAtomPredicate pred) const { return data_.begin_filter(pred); }
    filter_iterator begin_filter(DataAtomPredicate pred) { return data_.begin_filter(pred); }
    const_filter_iterator end_filter() const { return data_.end_filter(); }
    filter_iterator end_filter() { return data_.end_filter(); }

    // fn
    DataTypeMList rotateLeft(int steps) const;
    DataTypeMList flatten() const;
    DataTypeMList slice(int start, int end = -1, size_t step = 1) const;
    // sort only floats and symbols
    void sort();
    void reverse();
    void shuffle();
    bool contains(const Atom& a) const;
    bool contains(const AtomList& l) const;
    bool contains(const DataAtom& d) const;
    bool contains(const DataPtr& ptr) const;
    bool contains(const DataTypeMList& l) const;

public:
    typedef std::vector<DataTypeMList*> MListStack;
    typedef boost::optional<DataTypeMList> MaybeList;

public:
    static DataType dataType;
    static MaybeList parse(const AtomList& lst);
    static MaybeList parse(const std::string& lst);
    static void traverse(mlist_node* node, DataTypeMList* data, MListStack* stack, int act, const char* txt);
};

template <class Fn>
AtomList DataTypeMList::toList(Fn pred) const
{
    AtomList res;
    res.reserve(size());

    for (auto& el : data_) {
        if (pred(el))
            res.append(el.toAtom());
    }

    return res;
}

#endif // DATATYPE_MLIST_H
