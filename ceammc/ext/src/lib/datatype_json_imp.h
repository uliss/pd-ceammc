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
#ifndef DATATYPE_JSON_IMP_H
#define DATATYPE_JSON_IMP_H

#include "ceammc_atom.h"
#include "datatype_json.h"
#include "json/json.hpp"

namespace ceammc {

class DataTypeJsonImpl {
    nlohmann::json json_;

public:
    DataTypeJsonImpl();
    DataTypeJsonImpl(const DataTypeJsonImpl& imp);
    DataTypeJsonImpl(const nlohmann::json& json);
    DataTypeJsonImpl(nlohmann::json&& json);
    std::string toString() const;
    bool parse(const char* str);
    bool append(const Atom& a);
    size_t size() const;
    bool empty() const;
    std::string stringAt(size_t idx) const;
    bool isArray() const;
    bool isNull() const;

    void clear();

    bool operator==(const DataTypeJsonImpl& json) const;

    JsonValue valueAt(size_t idx) const;

    bool addFloat(t_float f);
    bool addSymbol(t_symbol* s);
    bool addList(const AtomList& l);
};
}

#endif // DATATYPE_JSON_IMP_H
