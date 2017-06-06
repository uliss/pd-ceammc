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
    void add(const Atom& a);
    void add(const AtomList& l);
    void remove(const Atom& a);
    void remove(const AtomList& l);
    void clear();
    size_t size() const;

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

public:
    static const DataType dataType;
    static DataTypeSet intersection(const DataTypeSet& s0, const DataTypeSet& s1);

private:
    DataTypeSet(const DataTypeSet& ds);
};

#endif // DATATYPE_SET_H
