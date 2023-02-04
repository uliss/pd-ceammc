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
#include "lex/parser_props.h"

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

bool CallbackProperty::setList(const AtomListView& lv)
{
    switch (setter_.type) {
    case Type::LIST:
        if (!hasListCb(SETTER) || !checkList(lv))
            return false;

        if (!setter_.fn_list(lv)) {
            PROP_ERR << "list setter error: " << cb_err_msg_;
            return false;
        } else
            return true;

        break;
    case Type::BOOL: {
        using namespace parser;

        if (!emptyCheck(lv))
            return false;

        bool res = false;
        bool prev = false;
        if (!getBool(prev))
            return false;

        auto err = bool_prop_calc(prev, info().defaultBool(), lv, res);
        if (err != PropParseRes::OK) {
            PROP_ERR << bool_prop_expected() << " expected, got: " << lv;
            return false;
        }

        return setBool(res);
    } break;
    case Type::FLOAT: {
        using namespace parser;

        if (!emptyCheck(lv))
            return false;

        t_float res = false;
        t_float value = false;
        if (!getFloat(value))
            return false;

        auto rc = numeric_prop_calc<t_float>(value, info(), lv, res);
        switch (rc) {
        case PropParseRes::OK:
            return setFloat(res);
        case PropParseRes::DIVBYZERO:
            PROP_ERR << "division by zero: " << lv;
            return false;
        case PropParseRes::NORANGE:
            PROP_ERR << "property without range, can't set random";
            return false;
        case PropParseRes::INVALID_RANDOM_ARGS:
            PROP_ERR << "random [MIN MAX]? expected, got: " << lv;
            return false;
        case PropParseRes::UNKNOWN:
        default:
            PROP_ERR << float_prop_expected() << " expected, got: " << lv;
            return false;
        }

    } break;
    case Type::INT: {

        using namespace parser;

        if (!emptyCheck(lv))
            return false;

        int res = false;
        int value = false;
        if (!getInt(value))
            return false;

        auto rc = numeric_prop_calc<int>(value, info(), lv, res);
        switch (rc) {
        case PropParseRes::OK:
            return setInt(res);
        case PropParseRes::DIVBYZERO:
            PROP_ERR << "division by zero: " << lv;
            return false;
        case PropParseRes::NORANGE:
            PROP_ERR << "property without range, can't set random";
            return false;
        case PropParseRes::INVALID_RANDOM_ARGS:
            PROP_ERR << "random [MIN MAX]? expected, got: " << lv;
            return false;
        case PropParseRes::UNKNOWN:
        default:
            PROP_ERR << int_prop_expected() << " expected, got: " << lv;
            return false;
        }

    } break;
    case Type::SYMBOL:
        if (lv.isSymbol())
            return setSymbol(lv[0].asSymbol());
        else
            PROP_ERR << "symbol value expected, got: " << lv;

        break;
    case Type::ATOM:
        if (lv.isAtom())
            return setAtom(lv[0]);
        else
            PROP_ERR << "single atom value expected, got: " << lv;
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

bool CallbackProperty::checkMin(t_float v)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::GREATER_THEN, v);
    else if (isInt())
        return setIntCheck(PropValueConstraints::GREATER_THEN, v);
    else {
        PROP_ERR << "can't set greater-then check";
        return false;
    }
}

bool CallbackProperty::checkMinEq(t_float v)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::GREATER_EQUAL, v);
    else if (isInt())
        return setIntCheck(PropValueConstraints::GREATER_EQUAL, v);
    else {
        PROP_ERR << "can't set greater-eq check";
        return false;
    }
}

bool CallbackProperty::checkMax(t_float v)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::LESS_THEN, v);
    else if (isInt())
        return setIntCheck(PropValueConstraints::LESS_THEN, v);
    else {
        PROP_ERR << "can't set less-then check";
        return false;
    }
}

bool CallbackProperty::checkMaxEq(t_float v)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::LESS_EQUAL, v);
    else if (isInt())
        return setIntCheck(PropValueConstraints::LESS_EQUAL, v);
    else {
        PROP_ERR << "can't set less-equal check";
        return false;
    }
}

bool CallbackProperty::checkClosedRange(t_float a, t_float b)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::CLOSED_RANGE, a, b);
    else if (isInt())
        return setIntCheck(PropValueConstraints::CLOSED_RANGE, a, b);
    else {
        PROP_ERR << "can't set closed range check";
        return false;
    }
}

bool CallbackProperty::checkOpenedRange(t_float a, t_float b)
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::OPEN_RANGE, a, b);
    else if (isInt())
        return setIntCheck(PropValueConstraints::OPEN_RANGE, a, b);
    else {
        PROP_ERR << "can't set open range check";
        return false;
    }
}

bool CallbackProperty::checkNonZero()
{
    if (isFloat())
        return setFloatCheck(PropValueConstraints::NON_ZERO);
    else if (isInt())
        return setIntCheck(PropValueConstraints::NON_ZERO);
    else {
        PROP_ERR << "can't set non-zero check";
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
