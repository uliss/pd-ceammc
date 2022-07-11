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
#include "ceammc_property_info.h"

using namespace ceammc;

class DataTypeProperty : public AbstractData {
public:
    DataTypeProperty(t_symbol* name);
    DataTypeProperty(const DataTypeProperty& p);

    DataTypeId type() const noexcept override;
    DataTypeProperty* clone() const override;
    std::string toString() const override;

    void setTypeFloat(t_float def = 0);
    void setTypeInt(int def = 0);
    void setTypeBool(bool def);
    void setTypeSymbol(t_symbol* def);
    void setTypeList(const AtomListView& def);

    bool setBool(bool v);
    bool setFloat(t_float f);
    bool setInt(int v);
    bool setSymbol(t_symbol* s);
    bool setList(const AtomListView& lv);
    bool setFromPdArgs(const AtomListView& lv);

    void restoreDefault();

    bool getFloat(t_float& out) const;
    bool getInt(int &out) const;
    bool getBool(bool& out) const;
    bool getSymbol(t_symbol** s) const;
    bool getList(AtomList& out) const;

    bool setFloatRange(t_float min, t_float max);
    bool setIntRange(int min, int max);
    bool setEnumValues(const AtomList& lst);

    bool isFloat() const { return type_ == PropValueType::FLOAT; }
    bool isInt() const { return type_ == PropValueType::INTEGER; }
    bool isBool() const { return type_ == PropValueType::BOOLEAN; }
    bool isSymbol() const { return type_ == PropValueType::SYMBOL; }
    bool isList() const { return type_ == PropValueType::LIST; }

    t_symbol* name() const { return name_; }
    PropValueType propertyType() const { return type_; }
    std::string propertyStrValue() const;
    std::string propertyStrMinValue() const;
    std::string propertyStrMaxValue() const;
    std::pair<t_float, t_float> floatRange() const { return { fmin_, fmax_ }; }
    std::pair<int, int> intRange() const { return { lmin_, lmax_ }; }
    const AtomList& enumValues() const { return enum_; }

    bool hasMinValue() const;
    bool hasMaxValue() const;
    bool hasEnumValues() const;

    PropertyInfo info() const;

    std::string toListStringContent() const final;
    std::string toDictStringContent() const final;
    bool set(const AbstractData* d) noexcept final;


public:
    static const DataTypeId dataType;

private:
    void updateAll();

private:
    t_symbol* name_;
    PropValueType type_;
    PropertyValue value_;
    PropertyValue default_;
    t_float fmin_, fmax_;
    int lmin_, lmax_;
    AtomList enum_;
};

class PropertyStorage {
    PropertyStorage();

public:
    typedef NamedDataDict<DataTypeProperty> Dict;

public:
    static Dict& storage();
    static t_symbol* makeFullName(const std::string& name, t_canvas* cnv);
    static t_symbol* makeFullName(t_symbol* name, t_canvas* cnv);
};

class PropertyPtr {
    t_symbol* name_;
    DataTypeProperty* prop_;

public:
    PropertyPtr(t_symbol* name);
    ~PropertyPtr();

    DataTypeProperty* operator->()
    {
        return prop_;
    }

    operator bool() const
    {
        return prop_ != nullptr;
    }
};

#endif // DATATYPE_PROPERTY_H
