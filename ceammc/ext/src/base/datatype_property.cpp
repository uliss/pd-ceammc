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
#include "datatype_property.h"
#include "ceammc_convert.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"

#include <limits>
#include <sstream>

const DataType DataTypeProperty::dataType = data::DATA_PROPERTY;

DataTypeProperty::DataTypeProperty(t_symbol* name)
    : name_(name)
    , type_(T_FLOAT)
    , value_(0.f)
    , default_(0.f)
    , fmin_(std::numeric_limits<t_float>::lowest())
    , fmax_(std::numeric_limits<t_float>::max())
    , lmin_(std::numeric_limits<long>::lowest())
    , lmax_(std::numeric_limits<long>::max())
{
}

DataTypeProperty::DataTypeProperty(const DataTypeProperty& p)
    : name_(p.name_)
    , type_(p.type_)
    , value_(p.value_)
    , fmin_(p.fmin_)
    , fmax_(p.fmax_)
    , lmin_(p.lmin_)
    , lmax_(p.lmax_)
    , enum_(p.enum_)
{
}

DataType DataTypeProperty::type() const
{
    return dataType;
}

DataTypeProperty* DataTypeProperty::clone() const
{
    return new DataTypeProperty(*this);
}

std::string DataTypeProperty::toString() const
{
    std::ostringstream ss;
    ss << "Property: " << name_ << "\n"
       << "  type: " << type_ << "\n";

    return ss.str();
}

void DataTypeProperty::setTypeFloat(t_float def)
{
    type_ = T_FLOAT;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeInt(long def)
{
    type_ = T_INT;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeBool(bool def)
{
    type_ = T_BOOL;
    default_ = def;
    value_ = default_;
}

bool DataTypeProperty::setBool(bool v)
{
    if (type_ != T_BOOL)
        return false;

    value_ = v;
    return true;
}

bool DataTypeProperty::setFloat(t_float f)
{
    if (type_ != T_FLOAT)
        return false;

    value_ = clip<t_float>(f, fmin_, fmax_);
    return true;
}

bool DataTypeProperty::setInt(long v)
{
    if (type_ != T_INT)
        return false;

    value_ = clip<long>(v, lmin_, lmax_);
    return true;
}

bool DataTypeProperty::setSymbol(t_symbol* s)
{
    if (type_ != T_SYMBOL)
        return false;

    if (enum_.size() > 0 && !enum_.contains(Atom(s)))
        return false;

    value_ = s;
    return true;
}

bool DataTypeProperty::setList(const AtomList& lst)
{
    if (type_ != T_LIST)
        return false;

    value_ = lst;
    return true;
}

bool DataTypeProperty::getFloat(t_float& out) const
{
    if (type_ != T_FLOAT)
        return false;

    out = boost::get<t_float>(value_);
    return true;
}

bool DataTypeProperty::getInt(long& out) const
{
    if (type_ != T_INT)
        return false;

    out = boost::get<long>(value_);
    return true;
}

bool DataTypeProperty::getBool(bool& out) const
{
    if (type_ != T_BOOL)
        return false;

    out = boost::get<bool>(value_);
    return true;
}

bool DataTypeProperty::setFloatRange(t_float min, t_float max)
{
    if (type_ != T_FLOAT)
        return false;

    LIB_DBG << "fmin: " << min << ",  fmax: " << max;

    fmin_ = min;
    fmax_ = max;
    default_ = clip<t_float>(boost::get<t_float>(default_), fmin_, fmax_);
    return true;
}

bool DataTypeProperty::setIntRange(long min, long max)
{
    if (type_ != T_INT)
        return false;

    lmax_ = max;
    lmin_ = min;
    default_ = clip<long>(boost::get<long>(default_), lmin_, lmax_);
    return true;
}

PropertyStorage::Dict& PropertyStorage::storage()
{
    static Dict dict;
    return dict;
}

std::string PropertyStorage::makeFullName(const std::string& name, _glist* cnv)
{
    std::string res;
    char buf[30];
    snprintf(buf, sizeof(buf), "%p:", (void*)cnv);
    res += buf;
    res += name;
    return res;
}
