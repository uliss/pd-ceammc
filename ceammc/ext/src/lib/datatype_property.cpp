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
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "lex/parser_props.h"

#include <cstring>
#include <limits>
#include <sstream>

const int DataTypeProperty::dataType = data::DATA_PROPERTY;

constexpr auto min_fdefault = std::numeric_limits<t_float>::lowest();
constexpr auto max_fdefault = std::numeric_limits<t_float>::max();
constexpr auto min_idefault = std::numeric_limits<int>::lowest();
constexpr auto max_idefault = std::numeric_limits<int>::max();

#define CHECK_DECLTYPE(cons, member) static_assert(std::is_same<std::remove_const<decltype(cons)>::type, decltype(member)>::value, "");

#define PROP_ERR() LIB_ERR << name_ << ' '

DataTypeProperty::DataTypeProperty(t_symbol* name)
    : name_(name)
    , type_(PropValueType::FLOAT)
    , value_(0.f)
    , default_(0.f)
    , fmin_(min_fdefault)
    , fmax_(max_fdefault)
    , lmin_(min_idefault)
    , lmax_(max_idefault)
{
    CHECK_DECLTYPE(min_fdefault, fmin_);
    CHECK_DECLTYPE(max_fdefault, fmax_);
    CHECK_DECLTYPE(max_idefault, lmax_);
    CHECK_DECLTYPE(max_idefault, lmax_);
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

int DataTypeProperty::type() const noexcept
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
       << "  type:  " << to_string(type_) << "\n"
       << "  value: " << propertyStrValue() << "\n";

    return ss.str();
}

void DataTypeProperty::setTypeFloat(t_float def)
{
    type_ = PropValueType::FLOAT;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeInt(int def)
{
    type_ = PropValueType::INTEGER;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeBool(bool def)
{
    type_ = PropValueType::BOOLEAN;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeSymbol(t_symbol* def)
{
    type_ = PropValueType::SYMBOL;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeList(const AtomList& def)
{
    type_ = PropValueType::LIST;
    default_ = def;
    value_ = default_;
}

bool DataTypeProperty::setBool(bool v)
{
    if (type_ != PropValueType::BOOLEAN)
        return false;

    value_ = v;
    updateAll();
    return true;
}

bool DataTypeProperty::setFloat(t_float f)
{
    if (type_ != PropValueType::FLOAT)
        return false;

    value_ = clip<t_float>(f, fmin_, fmax_);
    updateAll();
    return true;
}

bool DataTypeProperty::setInt(int v)
{
    if (type_ != PropValueType::INTEGER)
        return false;

    value_ = clip<int>(v, lmin_, lmax_);
    updateAll();
    return true;
}

bool DataTypeProperty::setSymbol(t_symbol* s)
{
    if (type_ != PropValueType::SYMBOL)
        return false;

    if (enum_.size() > 0 && !enum_.contains(Atom(s)))
        return false;

    value_ = s;
    updateAll();
    return true;
}

bool DataTypeProperty::setList(const AtomList& lst)
{
    if (type_ != PropValueType::LIST)
        return false;

    value_ = lst;
    updateAll();
    return true;
}

bool DataTypeProperty::setFromPdArgs(const AtomListView& lv)
{
    if (isFloat()) {
        using namespace parser;

        t_float res = false;
        t_float prev = false;
        if (!getFloat(prev))
            return false;

        auto rc = numeric_prop_calc<t_float>(prev, info(), lv, res);
        switch (rc) {
        case PropParseRes::OK:
            return setFloat(res);
        case PropParseRes::DIVBYZERO:
            PROP_ERR() << "division by zero: " << lv;
            return false;
        case PropParseRes::NORANGE:
            PROP_ERR() << "property without range, can't set random";
            return false;
        case PropParseRes::INVALID_RANDOM_ARGS:
            PROP_ERR() << "random [MIN MAX]? expected, got: " << lv;
            return false;
        case PropParseRes::UNKNOWN:
        default:
            PROP_ERR() << float_prop_expected() << " expected, got: " << lv;
            return false;
        }

    } else if (isInt()) {
        using namespace parser;

        int res = false;
        int prev = false;
        if (!getInt(prev))
            return false;

        auto rc = numeric_prop_calc<int>(prev, info(), lv, res);
        switch (rc) {
        case PropParseRes::OK:
            return setInt(res);
        case PropParseRes::DIVBYZERO:
            PROP_ERR() << "division by zero: " << lv;
            return false;
        case PropParseRes::NORANGE:
            PROP_ERR() << "property without range, can't set random";
            return false;
        case PropParseRes::INVALID_RANDOM_ARGS:
            PROP_ERR() << "random [MIN MAX]? expected, got: " << lv;
            return false;
        case PropParseRes::UNKNOWN:
        default:
            PROP_ERR() << int_prop_expected() << " expected, got: " << lv;
            return false;
        }

    } else if (isBool()) {
        using namespace parser;

        bool res = false;
        bool prev = false;
        if (!getBool(prev))
            return false;

        auto err = bool_prop_calc(prev, info().defaultBool(), lv, res);
        if (err != PropParseRes::OK) {
            PROP_ERR() << bool_prop_expected() << " expected, got: " << lv;
            return false;
        }

        return setBool(res);
    } else if (isSymbol()) {
        if (lv.isSymbol())
            setSymbol(lv[0].asSymbol());
        else {
            PROP_ERR() << "symbol is expected: " << lv;
            return false;
        }
    } else if (isList()) {
        setList(lv);
    } else {
        PROP_ERR() << "unhandled property type: " << to_string(propertyType());
        return false;
    }

    return true;
}

void DataTypeProperty::restoreDefault()
{
    value_ = default_;
    updateAll();
}

bool DataTypeProperty::getFloat(t_float& out) const
{
    if (type_ != PropValueType::FLOAT)
        return false;

    out = boost::get<t_float>(value_);
    return true;
}

bool DataTypeProperty::getInt(int& out) const
{
    if (type_ != PropValueType::INTEGER)
        return false;

    out = boost::get<int>(value_);
    return true;
}

bool DataTypeProperty::getBool(bool& out) const
{
    if (type_ != PropValueType::BOOLEAN)
        return false;

    out = boost::get<bool>(value_);
    return true;
}

bool DataTypeProperty::getSymbol(t_symbol** s) const
{
    if (type_ != PropValueType::SYMBOL)
        return false;

    *s = boost::get<t_symbol*>(value_);
    return true;
}

bool DataTypeProperty::getList(AtomList& out) const
{
    if (type_ != PropValueType::LIST)
        return false;

    out = boost::get<AtomList>(value_);
    return true;
}

bool DataTypeProperty::setFloatRange(t_float min, t_float max)
{
    if (type_ != PropValueType::FLOAT)
        return false;

    auto p = std::minmax(min, max);
    fmin_ = p.first;
    fmax_ = p.second;
    default_ = clip<t_float>(boost::get<t_float>(default_), fmin_, fmax_);
    return true;
}

bool DataTypeProperty::setIntRange(int min, int max)
{
    if (type_ != PropValueType::INTEGER)
        return false;

    auto p = std::minmax(min, max);
    lmin_ = p.first;
    lmax_ = p.second;
    default_ = clip<int>(boost::get<int>(default_), lmin_, lmax_);
    return true;
}

bool DataTypeProperty::setEnumValues(const AtomList& lst)
{
    if (type_ != PropValueType::SYMBOL)
        return false;

    enum_.clear();
    enum_.append(boost::get<t_symbol*>(default_));
    enum_.append(lst.filtered(ceammc::isSymbol));
    return true;
}

std::string DataTypeProperty::propertyStrValue() const
{
    bool vbool = false;
    int vint = 0;
    t_float vfloat = 0;
    t_symbol* vsym = &s_;
    AtomList vlist;

    if (getBool(vbool))
        return vbool ? "true" : "false";
    else if (getInt(vint))
        return std::to_string(vint);
    else if (getFloat(vfloat))
        return std::to_string(vfloat);
    else if (getSymbol(&vsym))
        return vsym->s_name;
    else if (getList(vlist))
        return to_string(vlist);
    else
        return "unknown property value type";
}

std::string DataTypeProperty::propertyStrMinValue() const
{
    if (isFloat())
        return std::to_string(floatRange().first);
    else if (isInt())
        return std::to_string(intRange().first);
    else
        return "";
}

std::string DataTypeProperty::propertyStrMaxValue() const
{
    if (isFloat())
        return std::to_string(floatRange().second);
    else if (isInt())
        return std::to_string(intRange().second);
    else
        return "";
}

bool DataTypeProperty::hasMinValue() const
{
    if (isFloat())
        return floatRange().first != std::numeric_limits<t_float>::lowest();
    else if (isInt())
        return intRange().first != std::numeric_limits<int>::lowest();
    else
        return false;
}

bool DataTypeProperty::hasMaxValue() const
{
    if (isFloat())
        return floatRange().second != std::numeric_limits<t_float>::max();
    else if (isInt())
        return intRange().second != std::numeric_limits<int>::max();
    else
        return false;
}

bool DataTypeProperty::hasEnumValues() const
{
    return !enum_.empty();
}

PropertyInfo DataTypeProperty::info() const
{
    const char* name = "??";
    // skip first '@'
    if (name_->s_name[0] != '@')
        name = name_->s_name;
    else
        name = name_->s_name + 1;

    PropertyInfo res(name, type_);

    if (isFloat()) {
        const bool has_min = fmin_ != min_fdefault;
        const bool has_max = fmax_ != max_fdefault;

        if (has_min && has_max) {
            res.setConstraints(PropValueConstraints::CLOSED_RANGE);
            if (!res.setRangeFloat(fmin_, fmax_))
                res.setConstraints(PropValueConstraints::NONE);

        } else if (has_min && !has_max) {
            res.setConstraints(PropValueConstraints::GREATER_EQUAL);
            if (!res.setMinFloat(fmin_))
                res.setConstraints(PropValueConstraints::NONE);

        } else if (!has_min && has_max) {
            res.setConstraints(PropValueConstraints::LESS_EQUAL);
            if (!res.setMaxFloat(fmax_))
                res.setConstraints(PropValueConstraints::NONE);
        }

        res.setDefault(boost::get<t_float>(default_));
    } else if (isInt()) {
        const bool has_min = lmin_ != min_idefault;
        const bool has_max = lmax_ != max_idefault;

        if (has_min && has_max) {
            res.setConstraints(PropValueConstraints::CLOSED_RANGE);
            if (!res.setRangeInt(lmin_, lmax_))
                res.setConstraints(PropValueConstraints::NONE);

        } else if (has_min && !has_max) {
            res.setConstraints(PropValueConstraints::GREATER_EQUAL);
            if (!res.setMinInt(lmin_))
                res.setConstraints(PropValueConstraints::NONE);

        } else if (!has_min && has_max) {
            res.setConstraints(PropValueConstraints::LESS_EQUAL);
            if (!res.setMaxInt(lmax_))
                res.setConstraints(PropValueConstraints::NONE);
        }

        res.setDefault(boost::get<int>(default_));
    } else if (isBool()) {
        res.setDefault(boost::get<bool>(default_) ? 1 : 0);
    } else if (isSymbol()) {
        res.setDefault(boost::get<t_symbol*>(default_));
    } else if (isList()) {
        res.setDefault(boost::get<AtomList>(default_));
    }

    return res;
}

void DataTypeProperty::updateAll()
{
    if (name_->s_thing)
        pd_bang(name_->s_thing);
}

PropertyStorage::Dict& PropertyStorage::storage()
{
    static Dict dict_;
    return dict_;
}

t_symbol* PropertyStorage::makeFullName(const std::string& name, t_glist* cnv)
{
    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf), "%p:%s", reinterpret_cast<void*>(cnv), name.c_str());
    return gensym(buf);
}

t_symbol* PropertyStorage::makeFullName(t_symbol* name, t_glist* cnv)
{
    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf), "%p:%s", reinterpret_cast<void*>(cnv), name->s_name);
    return gensym(buf);
}

PropertyPtr::PropertyPtr(t_symbol* name)
    : name_(name)
    , prop_(PropertyStorage::storage().acquire(name_))
{
}

PropertyPtr::~PropertyPtr()
{
    if (prop_)
        PropertyStorage::storage().release(name_);
}
