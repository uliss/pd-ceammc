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
#include "ceammc_property_callback.h"
#include "ceammc_log.h"

#include <cmath>

#define PROP_ERR LIB_ERR << '[' << name()->s_name << "] "
#define PROP_CHECK_ERR(v)                                              \
    {                                                                  \
        if (!err_msg_.empty())                                         \
            PROP_ERR << "check error, " << err_msg_ << ", got: " << v; \
    }

namespace ceammc {
CallbackProperty::CallbackProperty(const std::string& name, PropertyBoolGetter getter, PropertyBoolSetter setter, tag<bool>)
    : Property(PropertyInfo(name, PropValueType::BOOLEAN), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

CallbackProperty::CallbackProperty(const std::string& name, PropertyFloatGetter getter, PropertyFloatSetter setter, tag<t_float>)
    : Property(PropertyInfo(name, PropValueType::FLOAT), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

CallbackProperty::CallbackProperty(const std::string& name, PropertyIntGetter getter, PropertyIntSetter setter, tag<int>)
    : Property(PropertyInfo(name, PropValueType::INTEGER), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

CallbackProperty::CallbackProperty(const std::string& name, PropertySymbolGetter getter, PropertySymbolSetter setter, tag<t_symbol*>)
    : Property(PropertyInfo(name, PropValueType::SYMBOL), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

CallbackProperty::CallbackProperty(const std::string& name, PropertyAtomGetter getter, PropertyAtomSetter setter, tag<Atom>)
    : Property(PropertyInfo(name, PropValueType::ATOM), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

CallbackProperty::CallbackProperty(const std::string& name, PropertyListGetter getter, PropertyListSetter setter, tag<AtomList>)
    : Property(PropertyInfo(name, PropValueType::LIST), setter ? PropValueAccess::READWRITE : PropValueAccess::READONLY)
    , getter_(getter)
    , setter_(setter)
{
}

bool CallbackProperty::setList(const AtomListView& lst)
{
    static auto is_toggle = [](t_symbol* s) {
        return (s->s_name[0] == '~' || s->s_name[0] == '!') && s->s_name[1] == '\0';
    };

    static auto is_plus = [](t_symbol* s) { return s->s_name[0] == '+' && s->s_name[1] == '\0'; };
    static auto is_minus = [](t_symbol* s) { return s->s_name[0] == '-' && s->s_name[1] == '\0'; };
    static auto is_mul = [](t_symbol* s) { return s->s_name[0] == '*' && s->s_name[1] == '\0'; };
    static auto is_div = [](t_symbol* s) { return s->s_name[0] == '/' && s->s_name[1] == '\0'; };

    switch (setter_.type) {
    case Type::LIST:
        if (!hasListCb(SETTER) || !checkList(lst))
            return false;

        if (!setter_.fn_list(lst)) {
            PROP_ERR << "list setter error: " << cb_err_msg_;
            return false;
        } else
            return true;

        break;
    case Type::BOOL:
        if (lst.isBool())
            return setBool(lst[0].asBool());
        else if (lst.isSymbol() && is_toggle(lst[0].asT<t_symbol*>())) {
            bool v = false;

            if (!getBool(v))
                return false;
            else
                return setBool(!v);
        } else
            PROP_ERR << "bool value expected (0|1|true|false), got: " << lst;

        break;
    case Type::FLOAT:
        if (lst.isFloat())
            return setFloat(lst[0].asFloat());
        else if (lst.size() == 2 && lst[0].isSymbol() && lst[1].isFloat()) {
            t_float a = 0;
            if (!getFloat(a))
                return false;

            const auto b = lst[1].asT<t_float>();
            const auto op = lst[0].asT<t_symbol*>();
            if (is_plus(op))
                return setFloat(a + b);
            else if (is_minus(op))
                return setFloat(a - b);
            else if (is_mul(op))
                return setFloat(a * b);
            else if (is_div(op)) {
                if (b == 0) {
                    PROP_ERR << "division by zero";
                    return false;
                } else
                    return setFloat(a / b);
            } else {
                PROP_ERR << "expected +-*/, got: " << lst[0];
                return false;
            }
        } else
            PROP_ERR << "float value expected, got: " << lst;

        break;
    case Type::INT:
        if (lst.isFloat()) {
            t_float v = lst[0].asFloat();

            if (!lst.isInteger()) {
                v = std::round(v);
                PROP_ERR << "int value expected, got: " << lst << ", rounding to " << v;
            }

            return setInt(static_cast<int>(v));
        } else if (lst.size() == 2 && lst[0].isSymbol() && lst[1].isFloat()) {
            int a = 0;
            if (!getInt(a))
                return false;

            const auto b = lst[1].asT<int>();
            const auto op = lst[0].asT<t_symbol*>();
            if (is_plus(op))
                return setInt(a + b);
            else if (is_minus(op))
                return setInt(a - b);
            else if (is_mul(op))
                return setInt(a * b);
            else if (is_div(op)) {
                if (b == 0) {
                    PROP_ERR << "division by zero";
                    return false;
                } else
                    return setInt(a / b);
            } else {
                PROP_ERR << "expected +-*/, got: " << lst[0];
                return false;
            }
        } else
            PROP_ERR << "int value expected, got: " << lst;

        break;
    case Type::SYMBOL:
        if (lst.isSymbol())
            return setSymbol(lst[0].asSymbol());
        else
            PROP_ERR << "symbol value expected, got: " << lst;

        break;
    case Type::ATOM:
        if (lst.isAtom())
            return setAtom(lst[0]);
        else
            PROP_ERR << "single atom value expected, got: " << lst;
        break;
    default:
        PROP_ERR << "unknown property type: " << (int)setter_.type;
        return false;
    }

    return false;
}

bool CallbackProperty::checkPositive()
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::GREATER_THEN, 0);
    else if (isInt())
        return setIntCheck(PropValueConstraints::GREATER_THEN, 0);
    else {
        PROP_ERR << "can't set positive check";
        return false;
    }
}

bool CallbackProperty::checkNegative()
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::LESS_THEN, 0);
    else if (isInt())
        return setIntCheck(PropValueConstraints::LESS_THEN, 0);
    else {
        PROP_ERR << "can't set negative check";
        return false;
    }
}

bool CallbackProperty::checkNonNegative()
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::GREATER_EQUAL, 0);
    else if (isInt())
        return setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);
    else {
        PROP_ERR << "can't set non-negative check";
        return false;
    }
}

void CallbackProperty::updateDefault()
{
    if (isBool() && getter_.fn_bool)
        setDefault(getter_.fn_bool());
    else if (isFloat() && getter_.fn_float)
        setDefault(getter_.fn_float());
    else if (isInt() && getter_.fn_int)
        setDefault(getter_.fn_int());
    else if (isSymbol() && getter_.fn_symbol)
        setDefault(getter_.fn_symbol());
    else if (isAtom() && getter_.fn_atom)
        setDefault(getter_.fn_atom());
    else if (isList() && getter_.fn_list)
        setDefault(getter_.fn_list());
    else
        PROP_ERR << "can't update default info in callback property";
}

bool CallbackProperty::hasBoolCb(CallbackType t) const
{
    if (!isBool()) {
        PROP_ERR << "not a bool property";
        return false;
    } else if (t == GETTER && !getter_.fn_bool) {
        PROP_ERR << "bool getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_bool) {
        PROP_ERR << "bool setter is not set";
        return false;
    } else
        return true;
}

bool CallbackProperty::hasFloatCb(CallbackType t) const
{
    if (!isFloat()) {
        PROP_ERR << "not a float property";
        return false;
    } else if (t == GETTER && !getter_.fn_float) {
        PROP_ERR << "float getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_float) {
        PROP_ERR << "float setter is not set";
        return false;
    } else
        return true;
}

bool CallbackProperty::hasIntCb(CallbackType t) const
{
    if (!isInt()) {
        PROP_ERR << "not a int property";
        return false;
    } else if (t == GETTER && !getter_.fn_int) {
        PROP_ERR << "int getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_int) {
        PROP_ERR << "int setter is not set";
        return false;
    } else
        return true;
}

bool CallbackProperty::hasSymbolCb(CallbackType t) const
{
    if (!isSymbol()) {
        PROP_ERR << "not a symbol property";
        return false;
    } else if (t == GETTER && !getter_.fn_symbol) {
        PROP_ERR << "symbol getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_symbol) {
        PROP_ERR << "symbol setter is not set";
        return false;
    } else
        return true;
}

bool CallbackProperty::hasAtomCb(CallbackType t) const
{
    if (!isAtom()) {
        PROP_ERR << "not a atom property";
        return false;
    } else if (t == GETTER && !getter_.fn_atom) {
        PROP_ERR << "atom getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_atom) {
        PROP_ERR << "atom setter is not set";
        return false;
    } else
        return true;
}

bool CallbackProperty::hasListCb(CallbackType t) const
{
    if (!isList()) {
        PROP_ERR << "not a list property";
        return false;
    } else if (t == GETTER && !getter_.fn_list) {
        PROP_ERR << "list getter is not set";
        return false;
    } else if (t == SETTER && !setter_.fn_list) {
        PROP_ERR << "list setter is not set";
        return false;
    } else
        return true;
}

AtomList CallbackProperty::get() const
{
#define CHECKED_GETTER_CALL(m)               \
    {                                        \
        if (!getter_.m) {                    \
            PROP_ERR << "getter is not set"; \
            return AtomList();               \
        } else {                             \
            return atomFrom(getter_.m());    \
        }                                    \
    }

    switch (getter_.type) {
    case Type::BOOL:
        CHECKED_GETTER_CALL(fn_bool);
    case Type::FLOAT:
        CHECKED_GETTER_CALL(fn_float);
    case Type::INT:
        CHECKED_GETTER_CALL(fn_int);
    case Type::SYMBOL:
        CHECKED_GETTER_CALL(fn_symbol);
    case Type::ATOM:
        CHECKED_GETTER_CALL(fn_atom);
    case Type::LIST:
        if (!getter_.fn_list) {
            PROP_ERR << "getter is not set";
            return AtomList();
        } else {
            return getter_.fn_list();
        }
    default:
        PROP_ERR << "unknown property type: " << (int)getter_.type;
        return {};
    }

#undef CHECKED_GETTER_CALL
}

bool CallbackProperty::getBool(bool& b) const
{
    if (!hasBoolCb(GETTER))
        return false;

    b = getter_.fn_bool();
    return true;
}

bool CallbackProperty::getFloat(t_float& f) const
{
    if (!hasFloatCb(GETTER))
        return false;

    f = getter_.fn_float();
    return true;
}

bool CallbackProperty::getInt(int& i) const
{
    if (!hasIntCb(GETTER))
        return false;

    i = getter_.fn_int();
    return true;
}

bool CallbackProperty::getSymbol(t_symbol*& s) const
{
    if (!hasSymbolCb(GETTER))
        return false;

    s = getter_.fn_symbol();
    return true;
}

bool CallbackProperty::getAtom(Atom& a) const
{
    if (!hasAtomCb(GETTER))
        return false;

    a = getter_.fn_atom();
    return true;
}

bool CallbackProperty::getList(AtomList& l) const
{
    if (!hasListCb(GETTER))
        return false;

    l = getter_.fn_list();
    return true;
}

#define CHECKED_SETTER_CALL(m, v)                                                   \
    {                                                                               \
        if (!setter_.m(v)) {                                                        \
            PROP_ERR << "error, while setting value: " << v << ": " << cb_err_msg_; \
            return false;                                                           \
        } else                                                                      \
            return true;                                                            \
    }

bool CallbackProperty::setBool(bool b)
{
    if (!hasBoolCb(SETTER))
        return false;

    CHECKED_SETTER_CALL(fn_bool, b)
}

bool CallbackProperty::setFloat(t_float f)
{
    if (!hasFloatCb(SETTER) || !checkFloat(f))
        return false;

    CHECKED_SETTER_CALL(fn_float, f)
}

bool CallbackProperty::setInt(int i)
{
    if (!hasIntCb(SETTER) || !checkInt(i))
        return false;

    CHECKED_SETTER_CALL(fn_int, i)
}

bool CallbackProperty::setSymbol(t_symbol* s)
{
    if (!hasSymbolCb(SETTER) || !checkSymbol(s))
        return false;

    CHECKED_SETTER_CALL(fn_symbol, s)
}

bool CallbackProperty::setAtom(const Atom& a)
{
    if (!hasAtomCb(SETTER) || !checkAtom(a))
        return false;

    CHECKED_SETTER_CALL(fn_atom, a)
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertyBoolSetter& f)
    : type(Type::BOOL)
{
    new (&fn_bool) PropertyBoolSetter(f);
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertyFloatSetter& f)
    : type(Type::FLOAT)
{
    new (&fn_float) PropertyFloatSetter(f);
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertyIntSetter& f)
    : type(Type::INT)
{
    new (&fn_int) PropertyIntSetter(f);
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertySymbolSetter& f)
    : type(Type::SYMBOL)
{
    new (&fn_symbol) PropertySymbolSetter(f);
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertyAtomSetter& f)
    : type(Type::ATOM)
{
    new (&fn_atom) PropertyAtomSetter(f);
}

CallbackProperty::FnSetterUnion::FnSetterUnion(const PropertyListSetter& f)
    : type(Type::LIST)
{
    new (&fn_list) PropertyListSetter(f);
}

CallbackProperty::FnSetterUnion::~FnSetterUnion()
{
    switch (type) {
    case Type::BOOL:
        fn_bool.~PropertyBoolSetter();
        break;
    case Type::FLOAT:
        fn_float.~PropertyFloatSetter();
        break;
    case Type::INT:
        fn_int.~PropertyIntSetter();
        break;
    case Type::SYMBOL:
        fn_symbol.~PropertySymbolSetter();
        break;
    case Type::ATOM:
        fn_atom.~PropertyAtomSetter();
        break;
    case Type::LIST:
        fn_list.~PropertyListSetter();
        break;
    }
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertyBoolGetter& f)
    : type(Type::BOOL)
{
    new (&fn_bool) PropertyBoolGetter(f);
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertyFloatGetter& f)
    : type(Type::FLOAT)
{
    new (&fn_float) PropertyFloatGetter(f);
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertyIntGetter& f)
    : type(Type::INT)
{
    new (&fn_int) PropertyIntGetter(f);
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertySymbolGetter& f)
    : type(Type::SYMBOL)
{
    new (&fn_symbol) PropertySymbolGetter(f);
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertyAtomGetter& f)
    : type(Type::ATOM)
{
    new (&fn_atom) PropertyAtomGetter(f);
}

CallbackProperty::FnGetterUnion::FnGetterUnion(const PropertyListGetter& f)
    : type(Type::LIST)
{
    new (&fn_list) PropertyListGetter(f);
}

CallbackProperty::FnGetterUnion::~FnGetterUnion()
{
    switch (type) {
    case Type::BOOL:
        fn_bool.~PropertyBoolGetter();
        break;
    case Type::FLOAT:
        fn_float.~PropertyFloatGetter();
        break;
    case Type::INT:
        fn_int.~PropertyIntGetter();
        break;
    case Type::SYMBOL:
        fn_symbol.~PropertySymbolGetter();
        break;
    case Type::ATOM:
        fn_atom.~PropertyAtomGetter();
        break;
    case Type::LIST:
        fn_list.~PropertyListGetter();
        break;
    }
}
}
