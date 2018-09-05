/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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

#include "ceammc_property.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include <algorithm>
#include <cctype>

namespace ceammc {

// using singleton to prevent problems with static objects
// intializations in different modules
t_symbol* SYM_DUMP()
{
    static t_symbol* s = gensym("dump");
    return s;
}

t_symbol* SYM_PROPS_ALL()
{
    static t_symbol* s = gensym("@*");
    return s;
}

t_symbol* SYM_PROPS_ALL_Q()
{
    static t_symbol* s = gensym("@*?");
    return s;
}

Property::Property(const PropertyInfo& info, bool readonly)
    : info_(info)
    , readonly_(readonly)
    , visible_(true)
{
}

Property::~Property() {}

bool Property::readonlyCheck() const
{
    if (readonly_) {
        LIB_DBG << "readonly property: " << name();
        return false;
    }
    return true;
}

bool Property::emptyValueCheck(const AtomList& v) const
{
    if (v.empty()) {
        LIB_DBG << "empty value given for property: " << name();
        return false;
    }

    return true;
}

AtomProperty::AtomProperty(const std::string& name, const Atom& def, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::VARIANT), readonly)
    , v_(def)
{
    info().setDefault(def);
}

bool AtomProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    v_ = lst.at(0);
    return true;
}

AtomList AtomProperty::get() const
{
    AtomList res;
    res.append(v_);
    return res;
}

ListProperty::ListProperty(const std::string& name, const AtomList& l, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::LIST), readonly)
    , lst_(l)
{
    info().setDefault(l);
}

bool ListProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    lst_ = lst;
    return true;
}

AtomList ListProperty::get() const
{
    return lst_;
}

const AtomList& ListProperty::value() const
{
    return lst_;
}

AtomList& ListProperty::value()
{
    return lst_;
}

FloatProperty::FloatProperty(const std::string& name, float init, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::FLOAT), readonly)
    , v_(init)
{
    info().setDefault(init);
}

bool FloatProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    Atom v = lst.at(0);
    if (!v.isFloat()) {
        LIB_DBG << "not a float given for property " << name() << ": " << to_string(v);
        return false;
    }

    v_ = v.asFloat();
    return true;
}

AtomList FloatProperty::get() const
{
    return AtomList({ v_ });
}

BoolProperty::BoolProperty(const std::string& name, bool init, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::BOOLEAN), readonly)
    , v_(init)
{
    info().setDefault(init);
}

bool BoolProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    Atom a = lst[0];

    if (a.isFloat()) {
        v_ = (a.asInt(0) == 0) ? false : true;
        return true;
    }

    if (a.isSymbol()) {
        std::string str(a.asSymbol()->s_name);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        if (str == "true")
            v_ = true;
        else if (str == "false")
            v_ = false;
        else
            return false;

        return true;
    }

    return false;
}

AtomList BoolProperty::get() const
{
    return AtomList({ v_ ? 1.f : 0.f });
}

IntProperty::IntProperty(const std::string& name, int init, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::INTEGER), readonly)
    , v_(init)
{
    info().setDefault(init);
}

bool IntProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    Atom v = lst.at(0);
    if (!v.isFloat()) {
        LIB_DBG << "not a number given for property " << name() << ": " << to_string(v);
        return false;
    }

    v_ = static_cast<int>(v.asFloat());
    return true;
}

AtomList IntProperty::get() const
{
    return AtomList::filled(static_cast<float>(v_), 1);
}

SizeTProperty::SizeTProperty(const std::string& name, size_t init, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::INTEGER), readonly)
    , v_(init)
{
    info().setDefault(int(init));
    info().setMin(0);
}

bool SizeTProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    Atom v = lst.at(0);
    t_float f = 0;
    if (!v.getFloat(&f)) {
        LIB_DBG << "not a number given for property " << name() << ": " << to_string(v);
        return false;
    }

    if (f < 0.f) {
        LIB_DBG << "negative value given for property " << name() << ": "
                << to_string(v) << ", ignored.";
        return false;
    }

    v_ = v.asSizeT(0);
    return true;
}

AtomList SizeTProperty::get() const
{
    return AtomList::filled(static_cast<float>(v_), 1);
}

FlagProperty::FlagProperty(const std::string& name)
    : Property(PropertyInfo(name, PropertyInfoType::BOOLEAN), false)
    , v_(false)
{
    info().setDefault(false);
}

AtomList FlagProperty::get() const { return listFrom(v_); }

bool FlagProperty::set(const AtomList&)
{
    v_ = true;
    return true;
}

SymbolProperty::SymbolProperty(const std::string& name, t_symbol* init, bool readonly)
    : Property(PropertyInfo(name, PropertyInfoType::SYMBOL), readonly)
    , value_(init)
{
    info().setDefault(init);
    info().setType(PropertyInfoType::SYMBOL);
}

AtomList SymbolProperty::get() const
{
    return Atom(value_);
}

bool SymbolProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    if (!lst[0].isSymbol()) {
        LIB_DBG << "non symbol given for property " << name() << ": " << to_string(lst) << ", ignored.";
        return false;
    }

    value_ = lst[0].asSymbol();
    return true;
}

t_symbol* SymbolProperty::value() const
{
    return value_;
}

void SymbolProperty::setValue(t_symbol* s)
{
    value_ = s;
}

std::string SymbolProperty::str() const
{
    return value_->s_name;
}

SymbolEnumProperty::~SymbolEnumProperty()
{
}

CombinedProperty::CombinedProperty(const std::string& name, std::initializer_list<Property*> props)
    : Property(PropertyInfo(name, PropertyInfoType::LIST), true)
    , props_(props)
{
}

bool CombinedProperty::set(const AtomList&)
{
    return false;
}

AtomList CombinedProperty::get() const
{
    AtomList res;

    for (auto p : props_) {
        if (p)
            res.append(p->get());
    }

    return res;
}

}
