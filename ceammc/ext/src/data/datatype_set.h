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

#include "ceammc_atomlist.h"
#include "ceammc_data.h"
#include "ceammc_dataatom.h"

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_set.hpp>

using namespace ceammc;

class DataTypeSet : public AbstractData {
private:
    typedef boost::unordered_set<DataAtom> DataSet;
    DataSet data_;

public:
    DataTypeSet();
    DataTypeSet(const Atom& a);
    DataTypeSet(const AtomList& l);
    ~DataTypeSet();

    /**
     * Add element to set
     */
    void add(const Atom& a);
    void add(const AtomList& l);

    /**
     * Remove element from set
     */
    void remove(const Atom& a);

    /**
     * Removes all list elements from set
     */
    void remove(const AtomList& l);

    /**
     * Removes all elements from set
     */
    void clear();

    /**
     * Returns number of elements in set
     */
    size_t size() const;

    /**
     * Checks if set contains element
     */
    bool contains(const Atom& a) const;
    bool contains(const DataAtom& a) const;

    /**
     * Returns true if set contains *one* of given values
     */
    bool contains(const AtomList& lst) const;
    std::string toString() const;
    DataType type() const;
    bool isEqual(const AbstractData* d) const;
    AtomList toList() const;
    AbstractData* clone() const;
    bool operator==(const DataTypeSet& s) const;

    /**
      * Assign set
      */
    void operator=(const DataTypeSet& s);

public:
    static const DataType dataType;

    /**
     * Set intersection
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static void intersection(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set union
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static void set_union(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set difference
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static void set_difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1);

    /**
     * Set symmetric difference
     * @param out - result
     * @param s0 - first set
     * @param s1 - second set
     */
    static void set_sym_difference(DataTypeSet& out, const DataTypeSet& s0, const DataTypeSet& s1);

private:
    DataTypeSet(const DataTypeSet& ds);
};

#endif // DATATYPE_SET_H
