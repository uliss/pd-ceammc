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

#include "ceammc_property_info.h"

#include <algorithm>
#include <numeric>

namespace ceammc {

static PropertyInfoView defaultView(PropertyInfoType type)
{
    switch (type) {
    case PropertyInfoType::INTEGER:
        return PropertyInfoView::NUMBOX;
    case PropertyInfoType::FLOAT:
        return PropertyInfoView::SLIDER;
    case PropertyInfoType::BOOLEAN:
        return PropertyInfoView::TOGGLE;
    case PropertyInfoType::SYMBOL:
        return PropertyInfoView::ENTRY;
    case PropertyInfoType::VARIANT:
        return PropertyInfoView::ENTRY;
    case PropertyInfoType::LIST:
        return PropertyInfoView::ENTRY;
    default:
        return PropertyInfoView::ENTRY;
    }
}

PropertyInfo::PropertyInfo(const std::string& name, PropertyInfoType type, bool readonly)
    : name_(name)
    , type_(type)
    , view_(defaultView(type))
    , min_(std::numeric_limits<decltype(min_)>::min())
    , max_(std::numeric_limits<decltype(max_)>::max())
    , step_(0)
    , readonly_(readonly)
{
}

bool PropertyInfo::hasMinLimit() const
{
    return min_ != std::numeric_limits<decltype(min_)>::min();
}

bool PropertyInfo::hasMaxLimit() const
{
    return max_ != std::numeric_limits<decltype(max_)>::max();
}

bool PropertyInfo::hasEnumLimit() const
{
    return !enum_.empty();
}

bool PropertyInfo::hasStep() const
{
    return step_ > 0;
}

bool PropertyInfo::readonly() const
{
    return readonly_;
}

void PropertyInfo::setReadonly(bool v)
{
    readonly_ = v;
}

void PropertyInfo::setDefault(bool v)
{
    default_ = PropertySingleValue(v);
}

void PropertyInfo::setDefault(int v)
{
    default_ = PropertySingleValue(v);
}

void PropertyInfo::setDefault(size_t v)
{
    setDefault(int(v));
}

void PropertyInfo::setDefault(float v)
{
    default_ = PropertySingleValue(v);
}

void PropertyInfo::setDefault(t_symbol* s)
{
    default_ = PropertySingleValue(s);
}

void PropertyInfo::setDefault(const Atom& a)
{
    default_ = PropertySingleValue(a);
}

void PropertyInfo::setDefault(const PropertySingleValue& v)
{
    default_ = v;
}

void PropertyInfo::setDefault(const AtomList& lst)
{
    default_ = lst;
}

void PropertyInfo::setDefault(const PropertyValue& v)
{
    default_ = v;
}

bool PropertyInfo::setMin(float v)
{
    if (v > max_)
        return false;

    min_ = v;
    return true;
}

bool PropertyInfo::setMax(float v)
{
    if (v < min_)
        return false;

    max_ = v;
    return true;
}

void PropertyInfo::setRange(float min, float max)
{
    auto p = std::minmax(min, max);
    min_ = p.first;
    max_ = p.second;
}

bool PropertyInfo::setStep(float step)
{
    if (step < 0)
        return false;

    step_ = step;
    return true;
}

bool PropertyInfo::setView(PropertyInfoView v)
{
    switch (type_) {
    case PropertyInfoType::BOOLEAN:
        switch (v) {
        case PropertyInfoView::TOGGLE:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropertyInfoType::INTEGER:
    case PropertyInfoType::FLOAT:
        switch (v) {
        case PropertyInfoView::TOGGLE:
        case PropertyInfoView::NUMBOX:
        case PropertyInfoView::KNOB:
        case PropertyInfoView::SLIDER:
        case PropertyInfoView::SPINBOX:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropertyInfoType::SYMBOL:
        switch (v) {
        case PropertyInfoView::ENTRY:
        case PropertyInfoView::MENU:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropertyInfoType::VARIANT:
        switch (v) {
        case PropertyInfoView::ENTRY:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropertyInfoType::LIST:
        switch (v) {
        case PropertyInfoView::ENTRY:
        case PropertyInfoView::COLOR:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    }
}

void PropertyInfo::addEnum(int v)
{
    enum_.append(Atom(v));
}

void PropertyInfo::addEnum(const char* s)
{
    addEnum(gensym(s));
}

void PropertyInfo::addEnum(t_symbol* s)
{
    enum_.append(Atom(s));
}

void PropertyInfo::setType(PropertyInfoType t)
{
    type_ = t;
    view_ = defaultView(t);
}

void PropertyInfo::setUnits(const std::string& s)
{
    units_ = s;
}

bool PropertyInfo::defaultBool(bool def) const
{
    if (default_.type() != typeid(PropertySingleValue))
        return def;

    auto& val = boost::get<PropertySingleValue>(default_);
    if (val.type() != typeid(bool))
        return def;

    return boost::get<bool>(val);
}

int PropertyInfo::defaultInt(int def) const
{
    if (default_.type() != typeid(PropertySingleValue))
        return def;

    auto& val = boost::get<PropertySingleValue>(default_);
    if (val.type() != typeid(int))
        return def;

    return boost::get<int>(val);
}

float PropertyInfo::defaultFloat(float def) const
{
    if (default_.type() != typeid(PropertySingleValue))
        return def;

    auto& val = boost::get<PropertySingleValue>(default_);
    if (val.type() != typeid(float))
        return def;

    return boost::get<float>(val);
}

t_symbol* PropertyInfo::defaultSymbol(t_symbol* def) const
{
    if (default_.type() != typeid(PropertySingleValue))
        return def;

    auto& val = boost::get<PropertySingleValue>(default_);
    if (val.type() != typeid(t_symbol*))
        return def;

    return boost::get<t_symbol*>(val);
}

Atom PropertyInfo::defaultAtom(const Atom& def) const
{
    if (default_.type() != typeid(PropertySingleValue))
        return def;

    auto& val = boost::get<PropertySingleValue>(default_);
    if (val.type() != typeid(Atom))
        return def;

    return boost::get<Atom>(val);
}

AtomList PropertyInfo::defaultList() const
{
    if (default_.type() != typeid(AtomList))
        return AtomList();

    return boost::get<AtomList>(default_);
}
}
