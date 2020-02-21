/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef DATATYPE_TREE_H
#define DATATYPE_TREE_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"

#include <boost/variant.hpp>

namespace ceammc {

class DataTypeTree;
typedef boost::variant<boost::blank, DataTypeTree, Atom, AtomList> JsonValue;
class DataTypeTreeImpl;

class DataTypeTree : public AbstractData {
    DataTypeTreeImpl* pimpl_;

public:
    DataTypeTree();
    ~DataTypeTree();
    DataTypeTree(const DataTypeTree& tree);
    DataTypeTree(DataTypeTree&& tree);
    DataTypeTree(const DataTypeTreeImpl& imp);
    DataTypeTree(t_float f);
    DataTypeTree(t_symbol* s);
    DataTypeTree(const char* s);
    DataTypeTree(const std::string& s);
    DataTypeTree(const FloatList& lst);

    DataTypeTree* clone() const final;
    DataType type() const final;
    std::string toString() const final;
    bool isEqual(const AbstractData* d) const final;
    bool operator==(const DataTypeTree& d) const;

    size_t size() const;
    bool empty() const;
    bool isArray() const;
    bool isNull() const;

    void clear();

    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);
    bool addTree(const DataTypeTree& tree);

    void setFloat(t_float f);
    void setSymbol(t_symbol* s);
    bool set(const DataTPtr<DataTypeTree>& ptr);

    DataTPtr<DataTypeTree> match(const char* pattern) const;
    DataTPtr<DataTypeTree> at(size_t idx) const;
    DataTPtr<DataTypeTree> at(t_symbol* key) const;

    bool insertFloat(const char* key, t_float f);
    bool insertSymbol(const char* key, t_symbol* s);
    bool insertTree(const char* key, const DataTypeTree& tree);

public:
    bool parse(const char* str);

public:
    static const DataType dataType;
    static DataTPtr<DataTypeTree> newFromString(const char* str);
    static DataTPtr<DataTypeTree> newFromString(const std::string& str);
    static DataTPtr<DataTypeTree> newEmpty();

    static DataTypeTree fromString(const std::string& str);
    static DataTypeTree fromString(const char* str);
};
}

#endif // DATATYPE_TREE_H
