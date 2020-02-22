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
#ifndef DATATYPE_TREE_IMP_H
#define DATATYPE_TREE_IMP_H

#include "ceammc_atomlist.h"
#include "ceammc_either.h"

#include "json/json.hpp"

#include <vector>

namespace ceammc {

using EitherImpTreeFloat = Either<t_float>;
using EitherImpTreeString = Either<std::string>;
using EitherImpTree = Either<nlohmann::json>;
using TreeImpKeyList = std::vector<std::string>;

class DataTypeTreeImpl {
    nlohmann::json json_;

public:
    DataTypeTreeImpl();
    ~DataTypeTreeImpl();

    DataTypeTreeImpl(const DataTypeTreeImpl& imp);
    DataTypeTreeImpl(DataTypeTreeImpl&& imp);
    DataTypeTreeImpl(const nlohmann::json& json);
    DataTypeTreeImpl(nlohmann::json&& json);
    explicit DataTypeTreeImpl(t_float f);
    explicit DataTypeTreeImpl(t_symbol* s);
    explicit DataTypeTreeImpl(const char* s);
    explicit DataTypeTreeImpl(const FloatList& l);
    explicit DataTypeTreeImpl(const AtomList& l);

    std::string toString() const;

    bool parse(const char* str);
    size_t size() const;
    bool empty() const;

    bool isArray() const;
    bool isSimpleArray() const;
    bool isNull() const;
    bool isFloat() const;
    bool isString() const;
    bool isObject() const;

    const char* typeName() const;

    t_float asFloat() const;
    t_symbol* asSymbol() const;
    AtomList asAtomList() const;

    EitherImpTreeFloat getFloat() const;
    EitherImpTreeString getString() const;
    EitherImpTree getArray() const;
    EitherImpTree getObject() const;

    TreeImpKeyList keys() const;

    void clear();

    bool operator==(const DataTypeTreeImpl& json) const;
    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);
    bool addTree(const DataTypeTreeImpl& impl);

    void setFloat(t_float f);
    void setSymbol(t_symbol* s);
    void set(const DataTypeTreeImpl& imp);

    DataTypeTreeImpl match(const char* pattern) const;
    DataTypeTreeImpl at(size_t idx) const;
    DataTypeTreeImpl at(const char* key) const;

    bool insertFloat(const char* key, t_float f);
    bool insertSymbol(const char* key, t_symbol* s);
    bool insertTree(const char* key, const DataTypeTreeImpl& tree);

public:
    static DataTypeTreeImpl fromString(const char* str);
    static std::string treeToJson(const char* tree_str);
    static std::string jsonToTree(const std::string& json_str);

    static AtomList toList(const nlohmann::json& json);
    static Atom toAtom(const nlohmann::json& json);
};
}

#endif // DATATYPE_TREE_IMP_H
