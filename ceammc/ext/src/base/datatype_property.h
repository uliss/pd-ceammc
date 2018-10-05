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
#ifndef DATATYPE_PROPERTY_H
#define DATATYPE_PROPERTY_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_globaldata.h"

#include <boost/variant.hpp>

using namespace ceammc;

class DataTypeProperty : public AbstractData {
public:
    enum Type {
        T_FLOAT = 0,
        T_INT,
        T_BOOL,
        T_SYMBOL,
        T_LIST
    };

    typedef boost::variant<t_float, long, t_symbol*, bool, AtomList> PropertyValue;

public:
    DataTypeProperty(t_symbol* name);
    DataTypeProperty(const DataTypeProperty& p);

    DataType type() const override;
    DataTypeProperty* clone() const override;
    std::string toString() const override;

    void setTypeFloat(t_float def = 0);
    void setTypeInt(long def = 0);
    void setTypeBool(bool def);

    bool setBool(bool v);
    bool setFloat(t_float f);
    bool setInt(long v);
    bool setSymbol(t_symbol* s);
    bool setList(const AtomList& lst);

    bool getFloat(t_float& out) const;
    bool getInt(long& out) const;
    bool getBool(bool& out) const;

    bool setFloatRange(t_float min, t_float max);
    bool setIntRange(long min, long max);

    bool isFloat() const { return type_ == T_FLOAT; }
    bool isInt() const { return type_ == T_INT; }
    bool isBool() const { return type_ == T_BOOL; }

public:
    static const DataType dataType;

private:
    t_symbol* name_;
    Type type_;
    PropertyValue value_;
    PropertyValue default_;
    t_float fmin_, fmax_;
    long lmin_, lmax_;
    AtomList enum_;
};

class PropertyStorage {
    PropertyStorage();

public:
    typedef NamedDataDict<DataTypeProperty> Dict;

public:
    static Dict& storage();
    static std::string makeFullName(const std::string& name, t_canvas* cnv);
};

#endif // DATATYPE_PROPERTY_H
