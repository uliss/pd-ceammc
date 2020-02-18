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
#ifndef DATATYPE_JSON_H
#define DATATYPE_JSON_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"

#include <boost/variant.hpp>

namespace ceammc {

class DataTypeJson;
typedef boost::variant<boost::blank, DataTypeJson, Atom, AtomList> JsonValue;
class DataTypeJsonImpl;

class DataTypeJson : public AbstractData {
    DataTypeJsonImpl* pimpl_;

public:
    DataTypeJson();
    ~DataTypeJson();
    DataTypeJson(const DataTypeJson& json);
    DataTypeJson(DataTypeJson&& json);
    DataTypeJson(const DataTypeJsonImpl& imp);

    DataTypeJson* clone() const final;
    DataType type() const final;
    std::string toString() const final;
    bool isEqual(const AbstractData* d) const final;
    bool operator==(const DataTypeJson& d) const;

    size_t size() const;
    bool empty() const;
    bool isArray() const;
    bool isNull() const;

    JsonValue valueAt(size_t idx) const;
    void clear();

    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);

public:
    bool parse(const char* str);

public:
    static const DataType dataType;
};
}

#endif // DATATYPE_JSON_H
