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
#include "ceammc_convert.h"
#include "ceammc_log.h"

#include <algorithm>
#include <numeric>

#define PROP_LOG() LIB_ERR << "[" << name()->s_name << "] "

namespace ceammc {

constexpr t_float FLOAT_INF_MIN = std::numeric_limits<t_float>::lowest();
constexpr t_float FLOAT_INF_MAX = std::numeric_limits<t_float>::max();
constexpr int INT_INF_MIN = std::numeric_limits<int>::lowest();
constexpr int INT_INF_MAX = std::numeric_limits<int>::max();

static PropertyValue defaultInitValue(PropValueType type)
{
    switch (type) {
    case PropValueType::INTEGER:
        return PropertySingleValue(int(0));
    case PropValueType::FLOAT:
        return PropertySingleValue(t_float(0));
    case PropValueType::BOOLEAN:
        return PropertySingleValue(true);
    case PropValueType::SYMBOL:
        return PropertySingleValue(&s_);
    case PropValueType::VARIANT:
        return PropertySingleValue(Atom());
    case PropValueType::LIST:
        return AtomList();
    default:
        return PropertySingleValue(int(0));
    }
}

static PropValueView defaultView(PropValueType type)
{
    switch (type) {
    case PropValueType::INTEGER:
        return PropValueView::NUMBOX;
    case PropValueType::FLOAT:
        return PropValueView::SLIDER;
    case PropValueType::BOOLEAN:
        return PropValueView::TOGGLE;
    case PropValueType::SYMBOL:
        return PropValueView::ENTRY;
    case PropValueType::VARIANT:
        return PropValueView::ENTRY;
    case PropValueType::LIST:
        return PropValueView::ENTRY;
    default:
        return PropValueView::ENTRY;
    }
}

PropertyInfo::PropertyInfo(t_symbol* name, PropValueType type, PropValueAccess access)
    : name_(name)
    , default_(defaultInitValue(type))
    , min_(FLOAT_INF_MIN)
    , max_(FLOAT_INF_MAX)
    , step_(0)
    , arg_index_(-1)
    , type_(type)
    , units_(PropValueUnits::UNKNOWN)
    , view_(defaultView(type))
    , access_(access)
    , vis_(PropValueVis::PUBLIC)
    , constraints_(PropValueConstraints::NONE)
{
    if (isInt())
        clearRangeInt();
    else if (isFloat())
        clearRangeFloat();
}

PropertyInfo::PropertyInfo(const std::string& name, PropValueType type, PropValueAccess access)
    : PropertyInfo(gensym(name.c_str()), type, access)
{
}

PropertyInfo::PropertyInfo(const PropertyInfo& info)
    : name_(info.name_)
    , default_(info.default_)
    , enum_(info.enum_.get() ? new AtomList(*info.enum_) : nullptr)
    , min_(info.min_)
    , max_(info.max_)
    , step_(info.step_)
    , arg_index_(info.arg_index_)
    , type_(info.type_)
    , units_(info.units_)
    , view_(info.view_)
    , access_(info.access_)
    , vis_(info.vis_)
    , constraints_(info.constraints_)
{
}

PropertyInfo::~PropertyInfo()
{
    validate();
}

bool PropertyInfo::enumContains(int v) const
{
    if (!enum_)
        return false;

    return enum_->contains(Atom(v));
}

bool PropertyInfo::enumContains(t_symbol* s) const
{
    if (!enum_)
        return false;

    return enum_->contains(Atom(s));
}

bool PropertyInfo::hasStep() const
{
    if (!isNumeric())
        return false;

    return step_ > 0;
}

void PropertyInfo::setDefault(bool v)
{
    if (isBool())
        default_ = PropertySingleValue(v);
}

void PropertyInfo::setDefault(int v)
{
    if (isInt())
        default_ = PropertySingleValue(v);
    else if (isFloat())
        setDefault(static_cast<t_float>(v));
}

void PropertyInfo::setDefault(size_t v)
{
    setDefault(int(v));
}

void PropertyInfo::setDefault(float v)
{
    if (isFloat())
        default_ = PropertySingleValue(v);
}

void PropertyInfo::setDefault(double v)
{
    if (isFloat())
        default_ = PropertySingleValue(static_cast<t_float>(v));
}

void PropertyInfo::setDefault(t_symbol* s)
{
    if (isSymbol())
        default_ = PropertySingleValue(s);
}

void PropertyInfo::setDefault(const Atom& a)
{
    if (isVariant())
        default_ = PropertySingleValue(a);
}

void PropertyInfo::setDefault(const PropertySingleValue& v)
{
    if (!isList())
        default_ = v;
}

void PropertyInfo::setDefault(const AtomList& lst)
{
    if (isList())
        default_ = lst;
}

void PropertyInfo::setDefault(const PropertyValue& v)
{
    default_ = v;
}

bool PropertyInfo::setMinFloat(t_float v)
{
    if (!isFloat()) {
        PROP_LOG() << "not float property: setMinFloat failed";
        return false;
    }

    if (!hasConstraintsMin()) {
        PROP_LOG() << "no min constraints is set";
        return false;
    }

    if (v >= max_.f)
        return false;

    min_.f = v;
    enum_.reset();
    return true;
}

bool PropertyInfo::setMaxFloat(t_float v)
{
    if (!isFloat()) {
        PROP_LOG() << "not float property: setMinFloat failed";
        return false;
    }

    if (!hasConstraintsMax()) {
        PROP_LOG() << "no max constraints is set";
        return false;
    }

    if (v <= min_.f)
        return false;

    max_.f = v;
    enum_.reset();
    return true;
}

bool PropertyInfo::setMinInt(int new_min)
{
    if (!isInt()) {
        PROP_LOG() << "not int property: setMinInt failed";
        return false;
    }

    if (!hasConstraintsMin()) {
        PROP_LOG() << "no min constraints is set";
        return false;
    }

    if (new_min >= max_.i)
        return false;

    min_.i = new_min;
    enum_.reset();
    return true;
}

bool PropertyInfo::setMaxInt(int new_max)
{
    if (!isInt()) {
        PROP_LOG() << "not int property: setMaxInt failed";
        return false;
    }

    if (!hasConstraintsMax()) {
        PROP_LOG() << "no max constraints is set";
        return false;
    }

    if (new_max <= min_.i)
        return false;

    max_.i = new_max;
    enum_.reset();
    return true;
}

bool PropertyInfo::setRangeInt(int min, int max)
{
    if (!isInt()) {
        PROP_LOG() << "not int property: setRangeInt failed";
        return false;
    }

    if (!(hasConstraintsMin() && hasConstraintsMax())) {
        PROP_LOG() << "no min/max constraints is set";
        return false;
    }

    auto p = std::minmax(min, max);
    min_.i = p.first;
    max_.i = p.second;
    enum_.reset();
    return true;
}

void PropertyInfo::clearMinInt()
{
    min_.i = INT_INF_MIN;
}

void PropertyInfo::clearMaxInt()
{
    max_.i = INT_INF_MAX;
}

void PropertyInfo::clearRangeInt()
{
    clearMinInt();
    clearMaxInt();
}

bool PropertyInfo::enumContains(t_float v) const
{
    if (!enum_)
        return false;
    else
        return enum_->contains(Atom(v));
}

const AtomList& PropertyInfo::enumValues() const
{
    static AtomList def;
    if (!enum_)
        return def;
    else
        return *enum_;
}

size_t PropertyInfo::enumCount() const
{
    if (!enum_)
        return 0;
    else
        return enum_->size();
}

Atom PropertyInfo::enumAt(size_t idx) const
{
    if (!enum_ || idx >= enum_->size())
        return Atom();
    else
        return enum_->at(idx);
}

bool PropertyInfo::setRangeFloat(t_float min, t_float max)
{
    if (!isFloat()) {
        PROP_LOG() << "not float property: setRangeFloat failed";
        return false;
    }

    if (!(hasConstraintsMin() && hasConstraintsMax())) {
        PROP_LOG() << "no min/max constraints is set";
        return false;
    }

    auto p = std::minmax(min, max);
    min_.f = p.first;
    max_.f = p.second;
    enum_.reset();
    return true;
}

bool PropertyInfo::setStep(t_float step)
{
    if (!isNumeric())
        return false;

    if (step < 0)
        return false;

    step_ = step;
    return true;
}

bool PropertyInfo::hasConstraintsMin() const
{
    switch (constraints_) {
    case PropValueConstraints::CLOSED_OPEN_RANGE:
    case PropValueConstraints::CLOSED_RANGE:
    case PropValueConstraints::OPEN_RANGE:
    case PropValueConstraints::OPEN_CLOSED_RANGE:
    case PropValueConstraints::GREATER_THEN:
    case PropValueConstraints::GREATER_EQUAL:
        return true;
    default:
        return false;
    }
}

bool PropertyInfo::hasConstraintsMax() const
{
    switch (constraints_) {
    case PropValueConstraints::CLOSED_OPEN_RANGE:
    case PropValueConstraints::CLOSED_RANGE:
    case PropValueConstraints::OPEN_RANGE:
    case PropValueConstraints::OPEN_CLOSED_RANGE:
    case PropValueConstraints::LESS_EQUAL:
    case PropValueConstraints::LESS_THEN:
        return true;
    default:
        return false;
    }
}

bool PropertyInfo::hasEnumLimit() const
{
    return constraints_ == PropValueConstraints::ENUM;
}

void PropertyInfo::clearMinFloat()
{
    min_.f = FLOAT_INF_MIN;
}

void PropertyInfo::clearMaxFloat()
{
    max_.f = FLOAT_INF_MAX;
}

void PropertyInfo::clearRangeFloat()
{
    clearMinFloat();
    clearMaxFloat();
}

bool PropertyInfo::setView(PropValueView v)
{
    switch (type_) {
    case PropValueType::BOOLEAN:
        switch (v) {
        case PropValueView::TOGGLE:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropValueType::INTEGER:
        switch (v) {
        case PropValueView::KNOB:
        case PropValueView::MENU:
        case PropValueView::NUMBOX:
        case PropValueView::SLIDER:
        case PropValueView::SPINBOX:
        case PropValueView::TOGGLE:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropValueType::FLOAT:
        switch (v) {
        case PropValueView::TOGGLE:
        case PropValueView::KNOB:
        case PropValueView::NUMBOX:
        case PropValueView::SLIDER:
        case PropValueView::SPINBOX:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropValueType::SYMBOL:
        switch (v) {
        case PropValueView::ENTRY:
        case PropValueView::MENU:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropValueType::VARIANT:
        switch (v) {
        case PropValueView::ENTRY:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    case PropValueType::LIST:
        switch (v) {
        case PropValueView::ENTRY:
        case PropValueView::COLOR:
            view_ = v;
            return true;
        default:
            return false;
        }
        break;
    }
}

bool PropertyInfo::addEnum(int v)
{
    if (!hasEnumLimit()) {
        PROP_LOG() << "no enum constraints";
        return false;
    }

    if (isInt() || isVariant() || isList()) {
        if (!enum_)
            enum_.reset(new AtomList);

        enum_->append(Atom(v));
        return true;
    } else {
        PROP_LOG() << "should not have int in enum";
        return false;
    }
}

bool PropertyInfo::addEnum(const char* s)
{
    return addEnum(gensym(s));
}

bool PropertyInfo::addEnum(t_symbol* s)
{
    if (!hasEnumLimit()) {
        PROP_LOG() << "no enum constraints";
        return false;
    }

    if (isSymbol() || isVariant() || isList()) {
        if (!enum_)
            enum_.reset(new AtomList);

        enum_->append(Atom(s));
        return true;
    } else {
        PROP_LOG() << "should not have symbol in enum";
        return false;
    }
}

bool PropertyInfo::addEnums(std::initializer_list<int> i_list)
{
    if (!hasEnumLimit()) {
        PROP_LOG() << "no enum constraints";
        return false;
    }

    if (isInt() || isVariant() || isList()) {
        if (!enum_)
            enum_.reset(new AtomList);

        enum_->reserve(i_list.size() + 1);
        for (auto& i : i_list)
            enum_->append(Atom(i));

    } else {
        PROP_LOG() << "should not have ints in enum";
        return false;
    }

    return true;
}

bool PropertyInfo::addEnums(std::initializer_list<t_symbol*> s_list)
{
    if (!hasEnumLimit()) {
        PROP_LOG() << "no enum constraints";
        return false;
    }

    if (isSymbol() || isVariant() || isList()) {
        if (!enum_)
            enum_.reset(new AtomList);

        enum_->reserve(s_list.size() + 1);
        for (auto s : s_list)
            enum_->append(Atom(s));

        return true;
    } else {
        PROP_LOG() << "should not have symbol in enum";
        return false;
    }
}

bool PropertyInfo::addEnums(std::initializer_list<const char*> c_list)
{
    if (!hasEnumLimit()) {
        PROP_LOG() << "no enum constraints";
        return false;
    }

    if (isSymbol() || isVariant() || isList()) {
        if (!enum_)
            enum_.reset(new AtomList);

        enum_->reserve(c_list.size() + 1);
        for (auto c : c_list)
            enum_->append(Atom(gensym(c)));

        return true;
    } else {
        PROP_LOG() << "should not have symbol in enum";
        return false;
    }
}

void PropertyInfo::clearEnum()
{
    enum_.reset();
}

void PropertyInfo::setType(PropValueType t)
{
    type_ = t;
    constraints_ = PropValueConstraints::NONE;
    units_ = PropValueUnits::UNKNOWN;
    view_ = defaultView(t);

    if (isInt())
        clearRangeInt();
    else if (isFloat())
        clearRangeFloat();
}

bool PropertyInfo::setUnits(PropValueUnits u)
{
    if (isInt() || isFloat() || u == PropValueUnits::UNKNOWN) {
        units_ = u;
        return true;
    } else {
        PROP_LOG() << "invalid type " << propTypeToSymbol(type()) << " for setting units: " << unitToSymbol(u);
        return false;
    }
}

void PropertyInfo::setAccess(PropValueAccess a)
{
    access_ = a;
}

void PropertyInfo::setVisibility(PropValueVis v)
{
    vis_ = v;
}

bool PropertyInfo::setConstraints(PropValueConstraints c)
{
    switch (type_) {
    case PropValueType::BOOLEAN:
        return false;
    case PropValueType::INTEGER:
        constraints_ = c;
        return true;
    case PropValueType::FLOAT:
        switch (c) {
        case PropValueConstraints::ENUM:
            return false;
        default:
            constraints_ = c;
            return true;
        }
        break;
    case PropValueType::SYMBOL:
        switch (c) {
        case PropValueConstraints::NONE:
        case PropValueConstraints::ENUM:
            constraints_ = c;
            return true;
        default:
            return false;
        }
    case PropValueType::LIST:
    case PropValueType::VARIANT:
    default:
        constraints_ = c;
        return true;
    }
}

void PropertyInfo::setArgIndex(int8_t idx)
{
    arg_index_ = idx;
}

bool PropertyInfo::defaultBool(bool def) const
{
    bool res;
    if (getT<bool>(default_, res))
        return res;
    else
        return def;
}

int PropertyInfo::defaultInt(int def) const
{
    int res;
    if (getT<int>(default_, res))
        return res;
    else
        return def;
}

t_float PropertyInfo::defaultFloat(t_float def) const
{
    t_float res;
    if (getT<t_float>(default_, res))
        return res;
    else
        return def;
}

t_symbol* PropertyInfo::defaultSymbol(t_symbol* def) const
{
    t_symbol* res;
    if (getT<t_symbol*>(default_, res))
        return (res == nullptr) ? def : res;
    else
        return def;
}

Atom PropertyInfo::defaultAtom(const Atom& def) const
{
    Atom res;
    if (getT<Atom>(default_, res))
        return res;
    else
        return def;
}

const AtomList& PropertyInfo::defaultList() const
{
    static const AtomList def;

    if (default_.type() != typeid(AtomList))
        return def;

    return boost::get<AtomList>(default_);
}

bool PropertyInfo::validate() const
{
    bool ok = true;

    if (isBool()) {
        if (hasStep()) {
            ok = false;
            PROP_LOG() << "should not have step";
        }

        if (hasEnumLimit()) {
            ok = false;
            PROP_LOG() << "should not have enum limits";
        }

        if (constraints_ != PropValueConstraints::NONE) {
            ok = false;
            PROP_LOG() << "should not have value constraints";
        }

        if (units_ != PropValueUnits::UNKNOWN) {
            ok = false;
            PROP_LOG() << "should not have units";
        }

        if (view_ != PropValueView::TOGGLE) {
            ok = false;
            PROP_LOG() << "should be a toggle";
        }

        if (enum_ && !enum_->empty()) {
            ok = false;
            PROP_LOG() << "non empty enum";
        }
    } else if (isInt()) {
        if (hasStep() && (std::ceil(step_) - step_ != 0)) {
            ok = false;
            PROP_LOG() << "step should be integer";
        }
    } else if (isFloat()) {
        if (hasEnumLimit()) {
            ok = false;
            PROP_LOG() << "should not have enum limits";
        }
    } else if (isSymbol()) {
        //        if (hasEnumLimit() && enum_.empty()) {
        //            ok = false;
        //            PROP_LOG() << "enum is empty";
        //        }

        //        if (!hasEnumLimit() && !enum_.empty()) {
        //            ok = false;
        //            PROP_LOG() << "enum is not set";
        //        }

        if (hasConstraintsMax()) {
            ok = false;
            PROP_LOG() << "max constraints is set";
        }

        if (hasConstraintsMax()) {
            ok = false;
            PROP_LOG() << "min constraints is set";
        }
    }

    return ok;
}

t_symbol* unitToSymbol(PropValueUnits u)
{
    static t_symbol* tab[] = {
        gensym(""),
        gensym("ms"),
        gensym("s"),
        gensym("samp"),
        gensym("db"),
        gensym("deg"),
        gensym("rad"),
        gensym("hz"),
        gensym("%"),
        gensym("cents"),
        gensym("semitone"),
        gensym("tone")
    };

    constexpr size_t N = sizeof(tab) / sizeof(tab[0]) - 1;
    static_assert(N == static_cast<size_t>(PropValueUnits::TONE), "invalid units tab size");

    return tab[static_cast<size_t>(u)];
}

t_symbol* propTypeToSymbol(PropValueType t)
{
    static t_symbol* tab[] = {
        gensym("bool"),
        gensym("int"),
        &s_float,
        &s_symbol,
        gensym("atom"),
        &s_list
    };

    constexpr size_t N = sizeof(tab) / sizeof(tab[0]) - 1;
    static_assert(N == static_cast<size_t>(PropValueType::LIST), "invalid types tab size");

    return tab[static_cast<size_t>(t)];
}

}
