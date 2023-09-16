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
#ifndef DATATYPE_SET_H
#define DATATYPE_SET_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_data.h"

#include <iosfwd>
#include <unordered_set>

namespace ceammc {

class DataTypeSet;
using SetAtom = DataAtom<DataTypeSet>;

class DataTypeSet : public AbstractData {
private:
    using DataSet = std::unordered_set<Atom, size_t (*)(const Atom&)>;
    DataSet data_;

public:
    using iterator = DataSet::iterator;
    using const_iterator = DataSet::const_iterator;

public:
    DataTypeSet();
    DataTypeSet(const Atom& a);
    explicit DataTypeSet(const AtomListView& lv);
    explicit DataTypeSet(const AtomList& lst);
    DataTypeSet(DataTypeSet&& ds) noexcept;
    DataTypeSet(const DataTypeSet& ds);
    ~DataTypeSet() noexcept;

    template <typename... Args>
    explicit DataTypeSet(Args... args)
        : DataTypeSet(AtomList(args...))
    {
    }

    /**
     * Add element to set
     */
    void add(const Atom& a);

    /**
     * Add all list elements to set
     */
    void add(const AtomListView& lv);

    /**
     * Remove element from set
     * @return true if element was deleted, otherwise false
     */
    bool remove(const Atom& a);

    /**
     * Removes all list elements from set
     */
    void remove(const AtomListView& lv);

    /**
     * Removes all elements from set
     */
    void clear() noexcept;

    /**
     * Returns number of elements in set
     */
    size_t size() const noexcept;

    /**
     * Checks if set contains element
     */
    bool contains(const Atom& a) const noexcept;

    /**
     * Returns true if set contains at least *one* of given values
     */
    bool contains_any_of(const AtomListView& lv) const noexcept;

    /** choose random element from set */
    bool choose(Atom& res) const noexcept;

    /** pure virtual implementations*/
    DataTypeId type() const noexcept;
    bool isEqual(const AbstractData* d) const noexcept;
    AtomList toList(bool sorted = false) const;
    AbstractData* clone() const;
    std::string toListStringContent() const noexcept final;
    std::string toDictStringContent() const noexcept final;
    bool set(const AbstractData* d) noexcept;

    bool operator==(const DataTypeSet& s) const noexcept;
    bool operator!=(const DataTypeSet& s) const noexcept { return !operator==(s); }

    /**
     * Assign set
     */
    void operator=(const DataTypeSet& s);

    iterator begin() noexcept { return data_.begin(); }
    const_iterator begin() const noexcept { return data_.begin(); }
    iterator end() noexcept { return data_.end(); }
    const_iterator end() const noexcept { return data_.end(); }

public:
    static bool looksLikeCtor(const AtomListView& lv) noexcept;

public:
    static DataTypeId staticType();

    /**
     * Set intersection
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static DataTypeSet intersection(const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set union
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static DataTypeSet set_union(const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set difference
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static DataTypeSet difference(const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set symmetric difference
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static DataTypeSet sym_difference(const DataTypeSet& s0, const DataTypeSet& s1);
};

std::ostream& operator<<(std::ostream& os, const DataTypeSet& set);

}

#endif // DATATYPE_SET_H
