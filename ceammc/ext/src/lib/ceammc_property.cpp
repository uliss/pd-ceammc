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

Property::Property(const std::string& name, bool readonly)
    : name_(name)
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

AtomProperty::AtomProperty(const std::string& name, const Atom& a, bool readonly)
    : Property(name, readonly)
    , v_(a)
{
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
    : Property(name, readonly)
    , lst_(l)
{
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

FloatProperty::FloatProperty(const std::string& name, float init, bool readonly)
    : Property(name, readonly)
    , v_(init)
{
}

bool FloatProperty::set(const AtomList& lst)
{
    if (!readonlyCheck())
        return false;

    if (!emptyValueCheck(lst))
        return false;

    Atom v = lst.at(0);
    if (!v.isFloat()) {
        LIB_DBG << "not a float given for property " << name().c_str() << ": " << to_string(v);
        return false;
    }

    v_ = v.asFloat();
    return true;
}

AtomList FloatProperty::get() const
{
    return AtomList::values(1, v_);
}

BoolProperty::BoolProperty(const std::string& name, bool init, bool readonly)
    : Property(name, readonly)
    , v_(init)
{
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
    return AtomList::values(1, v_ ? 1.f : 0.f);
}

IntProperty::IntProperty(const std::string& name, int init, bool readonly)
    : Property(name, readonly)
    , v_(init)
{
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
    : Property(name, readonly)
    , v_(init)
{
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
    : Property(name, false)
    , v_(false)
{
}

AtomList FlagProperty::get() const { return listFrom(v_); }

bool FlagProperty::set(const AtomList&)
{
    v_ = true;
    return true;
}
}
