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
#include "json/json.hpp"

namespace ceammc {

class DataTypeTreeImpl {
    nlohmann::json json_;

public:
    DataTypeTreeImpl();
    ~DataTypeTreeImpl();

    DataTypeTreeImpl(const DataTypeTreeImpl& imp);
    DataTypeTreeImpl(const nlohmann::json& json);
    DataTypeTreeImpl(nlohmann::json&& json);
    DataTypeTreeImpl(t_float f);
    DataTypeTreeImpl(t_symbol* s);
    DataTypeTreeImpl(const char* str);
    DataTypeTreeImpl(const FloatList& l);
    std::string toString() const;
    bool parse(const char* str);
    size_t size() const;
    bool empty() const;

    bool isArray() const;
    bool isNull() const;

    void clear();

    bool operator==(const DataTypeTreeImpl& json) const;
    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);
    bool addJson(const DataTypeTreeImpl& impl);

    void setFloat(t_float f);
    void setSymbol(t_symbol* s);
    void set(const DataTypeTreeImpl& imp);

    DataTypeTreeImpl match(const char* pattern) const;
    DataTypeTreeImpl at(size_t idx) const;
    DataTypeTreeImpl at(const char* key) const;

    bool insertFloat(const char* key, t_float f);
    bool insertSymbol(const char* key, t_symbol* s);
    bool insertJson(const char* key, const DataTypeTreeImpl& json);
};
}

#endif // DATATYPE_TREE_IMP_H
