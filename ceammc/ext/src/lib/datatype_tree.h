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
#include "ceammc_either.h"

#include <boost/variant.hpp>
#include <iostream>
#include <memory>

namespace ceammc {

class DataTypeTree;
class DataTypeTreeImpl;

using JsonValue = boost::variant<boost::blank, DataTypeTree, Atom, AtomList>;
using EitherTreeFloat = Either<t_float>;
using EitherTreeSymbol = Either<t_symbol*>;
using EitherTree = Either<DataTypeTree>;

class DataTypeTree : public AbstractData {
    using PimplPtr = std::shared_ptr<DataTypeTreeImpl>;

private:
    PimplPtr pimpl_;
    bool copy_on_write_;

    DataTypeTree(DataTypeTreeImpl* pimpl);

public:
    DataTypeTree();
    DataTypeTree(const DataTypeTree& tree);
    DataTypeTree(DataTypeTree&& tree);
    DataTypeTree(const DataTypeTreeImpl& imp);
    DataTypeTree(DataTypeTreeImpl&& imp);
    DataTypeTree(t_float f);
    DataTypeTree(t_symbol* s);
    DataTypeTree(const char* s);
    DataTypeTree(const std::string& s);
    DataTypeTree(const FloatList& lst);
    DataTypeTree(const AtomList& lst);

    ~DataTypeTree();

    DataTypeTree& operator=(const DataTypeTree& t);

    DataTypeTree* clone() const final;
    DataType type() const final;
    std::string toString() const final;
    bool isEqual(const AbstractData* d) const final;
    bool operator==(const DataTypeTree& d) const;

    size_t size() const;
    bool empty() const;
    bool isArray() const;
    bool isSimpleArray() const;
    bool isNull() const;
    bool isFloat() const;
    bool isString() const;
    bool isObject() const;

    EitherTreeFloat getFloat() const;
    EitherTreeSymbol getSymbol() const;
    EitherTree getList() const;
    EitherTree getObject() const;

    AtomList keys() const;

    void clear();

    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);
    bool addTree(const DataTypeTree& tree);

    void setFloat(t_float f);
    void setSymbol(t_symbol* s);
    bool set(const DataTPtr<DataTypeTree>& ptr);

    DataTPtr<DataTypeTree> match(const char* pattern) const;
    DataTypeTree at(size_t idx) const;
    DataTypeTree at(t_symbol* key) const;

    bool insertFloat(const char* key, t_float f);
    bool insertSymbol(const char* key, t_symbol* s);
    bool insertTree(const char* key, const DataTypeTree& tree);

    void outputTo(t_outlet* o) const;
    Atom asAtom() const;
    AtomList asAtomList() const;
    DataTPtr<DataTypeTree> asDataPtr() const;

public:
    bool parse(const char* str);

public:
    static const DataType dataType;
    static DataTPtr<DataTypeTree> newFromString(const char* str);
    static DataTPtr<DataTypeTree> newFromString(const std::string& str);
    static DataTPtr<DataTypeTree> newEmpty();

    static DataTypeTree fromString(const std::string& str);
    static DataTypeTree fromString(const char* str);

private:
    void detach_pimpl();
    bool is_null() const;
    static DataTypeTree& nullInstance();
};

std::ostream& operator<<(std::ostream& os, const DataTypeTree& t);
}

#endif // DATATYPE_TREE_H
