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
#include "ceammc_crc32.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "lex/parser_props.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#include "fmt/format.h"
#pragma clang diagnostic pop

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <random>

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()
#define PROP_CHECK_ERR(v)                                                \
    {                                                                    \
        if (!err_msg_.empty())                                           \
            PROP_ERR() << "check error, " << err_msg_ << ", got: " << v; \
    }

#define PROP_USED (void)

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

Property::Property(const PropertyInfo& info, PropValueAccess access)
    : info_(info)
    , owner_(nullptr)
{
    info_.setAccess(access);
}

bool Property::set(const AtomListView& lst)
{
    if (!writeCheck())
        return false;

    auto res = setList(lst);
    if (res && ok_fn_ptr_ && *ok_fn_ptr_)
        (*ok_fn_ptr_)(this);

    return res;
}

bool Property::setInit(const AtomListView& view)
{
    if (!initCheck())
        return false;

    auto res = setList(view);
    if (res && ok_fn_ptr_ && *ok_fn_ptr_)
        (*ok_fn_ptr_)(this);

    return res;
}

bool Property::setArgIndex(int8_t idx)
{
    if (isReadOnly()) {
        PROP_ERR() << "setting positional arg index to readonly property";
        return false;
    }

    info_.setArgIndex(idx);
    return true;
}

bool Property::setArgIndexNext(Property* p)
{
    if (!p || p == this)
        return false;

    return setArgIndex(p->argIndex() + 1);
}

bool Property::getDefault(bool& b) const
{
    return info_.getDefault(b);
}

bool Property::getDefault(t_float& f) const
{
    return info_.getDefault(f);
}

bool Property::getDefault(int& i) const
{
    return info_.getDefault(i);
}

bool Property::getDefault(t_symbol*& s) const
{
    return info_.getDefault(s);
}

bool Property::getDefault(Atom& a) const
{
    return info_.getDefault(a);
}

bool Property::getDefault(AtomList& l) const
{
    return info_.getDefault(l);
}

void Property::updateDefault()
{
    // empty call
}

bool Property::reset()
{
    if (!isReadWrite()) {
        PROP_ERR() << "can't reset readonly property";
        return false;
    }

    if (info_.noDefault()) {
        PROP_ERR() << "no default value. Can't reset";
        return false;
    }

    switch (type()) {
    case PropValueType::BOOLEAN:
        return setBool(info().defaultBool());
    case PropValueType::FLOAT:
        return setFloat(info().defaultFloat());
    case PropValueType::INTEGER:
        return setInt(info().defaultInt());
    case PropValueType::SYMBOL:
        return setSymbol(info().defaultSymbol());
    case PropValueType::ATOM:
        return setAtom(info().defaultAtom());
    case PropValueType::LIST:
        return setList(info().defaultList());
    default:
        return false;
    }
}

bool Property::setFloatCheckFn(Property::PropFloatCheckFn fn, const std::string& err)
{
    if (!isFloat() && !isList()) {
        PROP_ERR() << "not float or list property";
        return false;
    }

    if (isReadOnly())
        return false;

    if (!check_fn_ptr_)
        check_fn_ptr_.reset(new CheckFnTuple { fn, nullptr, nullptr, nullptr, nullptr });
    else
        std::get<0>(*check_fn_ptr_) = fn;

    err_msg_ = err;
    return true;
}

bool Property::setIntCheckFn(Property::PropIntCheckFn fn, const std::string& err)
{
    if (!isInt() && !isList()) {
        PROP_ERR() << "not int or list property";
        return false;
    }

    if (isReadOnly())
        return false;

    if (!check_fn_ptr_)
        check_fn_ptr_.reset(new CheckFnTuple { nullptr, fn, nullptr, nullptr, nullptr });
    else
        std::get<1>(*check_fn_ptr_) = fn;

    if (fn) {
        int def;
        if (getDefault(def)) {
            if (!fn(def))
                PROP_ERR() << "default value is not in checked range: " << def;
        }
    }

    err_msg_ = err;
    return true;
}

bool Property::setSymbolCheckFn(Property::PropSymbolCheckFn fn, const std::string& err)
{
    if (!isSymbol() && !isList()) {
        PROP_ERR() << "not symbol or list property";
        return false;
    }

    if (isReadOnly())
        return false;

    if (!check_fn_ptr_)
        check_fn_ptr_.reset(new CheckFnTuple { nullptr, nullptr, fn, nullptr, nullptr });
    else
        std::get<2>(*check_fn_ptr_) = fn;

    err_msg_ = err;
    return true;
}

bool Property::setAtomCheckFn(Property::PropAtomCheckFn fn, const std::string& err)
{
    if (!isAtom() && !isList()) {
        PROP_ERR() << "not atom or list property";
        return false;
    }

    if (isReadOnly())
        return false;

    if (!check_fn_ptr_)
        check_fn_ptr_.reset(new CheckFnTuple { nullptr, nullptr, nullptr, fn, nullptr });
    else
        std::get<3>(*check_fn_ptr_) = fn;

    err_msg_ = err;
    return true;
}

bool Property::setListCheckFn(Property::PropListCheckFn fn, const std::string& err)
{
    if (!isList()) {
        PROP_ERR() << "not list property";
        return false;
    }

    if (isReadOnly())
        return false;

    if (!check_fn_ptr_)
        check_fn_ptr_.reset(new CheckFnTuple { nullptr, nullptr, nullptr, nullptr, fn });
    else
        std::get<4>(*check_fn_ptr_) = fn;

    err_msg_ = err;
    return true;
}

bool Property::setSuccessFn(Property::PropSuccessFn fn)
{
    if (isReadOnly()) {
        PROP_ERR() << "can't set success fn for readonly property";
        return false;
    }

    if (!fn)
        ok_fn_ptr_.reset();
    else if (!ok_fn_ptr_)
        ok_fn_ptr_.reset(new PropSuccessFn(fn));

    return true;
}

bool Property::setFloatCheck(PropValueConstraints type, t_float a, t_float b)
{
    if (!isFloat() && !isList()) {
        PROP_ERR() << "float check function for non float or list";
        return false;
    }

    switch (type) {
    case PropValueConstraints::NON_ZERO:
        setFloatCheckFn(
            [](t_float f) { return !std::equal_to<t_float>()(f, 0); },
            "non-zero value expected");

        info().setConstraints(type);
        return true;
    case PropValueConstraints::GREATER_EQUAL:
        setFloatCheckFn(
            [a](t_float f) { return f >= a; },
            fmt::format("value >= {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMinFloat(a);
        return true;
    case PropValueConstraints::GREATER_THEN:
        setFloatCheckFn(
            [a](t_float f) { return f > a; },
            fmt::format("value > {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMinFloat(a);
        return true;
    case PropValueConstraints::LESS_EQUAL:
        setFloatCheckFn(
            [a](t_float f) { return f <= a; },
            fmt::format("value <= {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMaxFloat(a);
        return true;
    case PropValueConstraints::LESS_THEN:
        setFloatCheckFn(
            [a](t_float f) { return f < a; },
            fmt::format("value < {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMaxFloat(a);
        return true;
    case PropValueConstraints::CLOSED_RANGE:
        setFloatCheckFn(
            [a, b](t_float f) { return a <= f && f <= b; },
            fmt::format("expected value in [{0} ... {1}] range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeFloat(a, b);
        return true;
    case PropValueConstraints::OPEN_RANGE:
        setFloatCheckFn(
            [a, b](t_float f) { return a < f && f < b; },
            fmt::format("expected value in ({0} ... {1}) range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeFloat(a, b);
        return true;
    case PropValueConstraints::OPEN_CLOSED_RANGE:
        setFloatCheckFn(
            [a, b](t_float f) { return a < f && f <= b; },
            fmt::format("expected value in ({0} ... {1}] range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeFloat(a, b);
        return true;
    case PropValueConstraints::CLOSED_OPEN_RANGE:
        setFloatCheckFn(
            [a, b](t_float f) { return a <= f && f < b; },
            fmt::format("expected value in [{0} ... {1}) range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeFloat(a, b);
        return true;
    case PropValueConstraints::NONE:
        info().setConstraints(type);
        info().clearRangeFloat();
        setFloatCheckFn(nullptr);
        err_msg_.clear();
        return true;
    default:
        return false;
    }
}

bool Property::setIntCheck(PropValueConstraints type, int a, int b)
{
    if (!isInt()) {
        PROP_ERR() << "attempt to set int check function for non-int type";
        return false;
    }

    switch (type) {
    case PropValueConstraints::NON_ZERO:
        setIntCheckFn(
            [](int i) { return i != 0; },
            "non-zero value expected");

        info().setConstraints(type);
        return true;
    case PropValueConstraints::GREATER_EQUAL:
        setIntCheckFn(
            [a](int i) { return i >= a; },
            fmt::format("value >= {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMinInt(a);
        return true;
    case PropValueConstraints::GREATER_THEN:
        setIntCheckFn(
            [a](int i) { return i > a; },
            fmt::format("value > {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMinInt(a);
        return true;
    case PropValueConstraints::LESS_EQUAL:
        setIntCheckFn(
            [a](int i) { return i <= a; },
            fmt::format("value <= {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMaxInt(a);
        return true;
    case PropValueConstraints::LESS_THEN:
        setIntCheckFn(
            [a](int i) { return i < a; },
            fmt::format("value < {} expected", a));

        info().setConstraints(type);
        PROP_USED info().setMaxInt(a);
        return true;
    case PropValueConstraints::CLOSED_RANGE:
        setIntCheckFn(
            [a, b](int i) { return a <= i && i <= b; },
            fmt::format("expected value in [{0} ... {1}] range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeInt(a, b);
        return true;
    case PropValueConstraints::OPEN_RANGE:
        setIntCheckFn(
            [a, b](int i) { return a < i && i < b; },
            fmt::format("expected value in ({0} ... {1}) range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeInt(a, b);
        return true;
    case PropValueConstraints::OPEN_CLOSED_RANGE:
        setIntCheckFn(
            [a, b](int i) { return a < i && i <= b; },
            fmt::format("expected value in ({0} ... {1}] range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeInt(a, b);
        return true;
    case PropValueConstraints::CLOSED_OPEN_RANGE:
        setIntCheckFn(
            [a, b](int i) { return a <= i && i < b; },
            fmt::format("expected value in [{0} ... {1}) range", a, b));

        info().setConstraints(type);
        PROP_USED info().setRangeInt(a, b);
        return true;
    case PropValueConstraints::NONE:
        info().setConstraints(type);
        info().clearRangeInt();
        setIntCheckFn(nullptr);
        err_msg_.clear();
        return true;
    default:
        return false;
    }
}

bool Property::setSymbolEnumCheck(std::initializer_list<t_symbol*> l)
{
    if (!isSymbol()) {
        PROP_ERR() << "attempt to set enum check for non symbol type";
        return false;
    }

    info_.setConstraints(PropValueConstraints::ENUM);
    info_.clearEnum();
    for (auto s : l)
        PROP_USED info_.addEnum(s);

    setSymbolCheckFn([this](t_symbol* s) -> bool { return info_.enumContains(s); });
    return true;
}

bool Property::setSymbolEnumCheck(std::initializer_list<const char*> l)
{
    if (!isSymbol()) {
        PROP_ERR() << "attempt to set enum check for non symbol type";
        return false;
    }

    info_.setConstraints(PropValueConstraints::ENUM);
    info_.clearEnum();
    for (auto s : l)
        PROP_USED info_.addEnum(gensym(s));

    setSymbolCheckFn([this](t_symbol* s) -> bool { return info_.enumContains(s); });
    return true;
}

void Property::setCheckErrorMsg(const std::string& str)
{
    err_msg_ = str;
}

bool Property::checkPositive()
{
    PROP_ERR() << "positive check is not implemented";
    return false;
}

bool Property::checkNegative()
{
    PROP_ERR() << "negative check is not implemented";
    return false;
}

bool Property::checkNonNegative()
{
    PROP_ERR() << "non-negative check is not implemented";
    return false;
}

bool Property::getBool(bool&) const
{
    return false;
}

bool Property::getFloat(t_float&) const
{
    return false;
}

bool Property::getInt(int&) const
{
    return false;
}

bool Property::getSymbol(t_symbol*&) const
{
    return false;
}

bool Property::getAtom(Atom&) const
{
    return false;
}

bool Property::getList(AtomList&) const
{
    return false;
}

bool Property::setBool(bool)
{
    return false;
}

bool Property::setFloat(t_float)
{
    return false;
}

bool Property::setInt(int)
{
    return false;
}

bool Property::setSymbol(t_symbol*)
{
    return false;
}

bool Property::setAtom(const Atom&)
{
    return false;
}

bool Property::writeCheck() const
{
    if (isReadOnly()) {
        PROP_ERR() << "write check: readonly property";
        return false;
    } else if (isInitOnly()) {
        PROP_ERR() << "write check: initonly property";
        return false;
    } else
        return true;
}

bool Property::initCheck() const
{
    if (isReadOnly()) {
        PROP_ERR() << "init check: readonly property";
        return false;
    }

    return true;
}

bool Property::emptyCheck(const AtomListView& v) const
{
    if (v.empty()) {
        PROP_ERR() << "empty value given";
        return false;
    }

    return true;
}

bool Property::checkFloat(t_float v) const
{
    if (check_fn_ptr_) {
        auto fn = std::get<0>(*check_fn_ptr_);
        if (fn && !fn(v)) {
            PROP_CHECK_ERR(v);
            return false;
        }
    }

    if (!std::isnormal(v) && v != 0) {
        PROP_ERR() << "ignore denormal value: " << v;
        return false;
    }

    return true;
}

bool Property::checkInt(int v) const
{
    if (check_fn_ptr_) {
        auto fn = std::get<1>(*check_fn_ptr_);
        if (fn && !fn(v)) {
            PROP_CHECK_ERR(v);
            return false;
        }
    }

    return true;
}

bool Property::checkSymbol(t_symbol* s) const
{
    if (check_fn_ptr_) {
        auto fn = std::get<2>(*check_fn_ptr_);
        if (fn && !fn(s)) {
            PROP_CHECK_ERR(s);
            return false;
        }
    }

    return true;
}

bool Property::checkAtom(const Atom& a) const
{
    if (check_fn_ptr_) {
        auto fn = std::get<3>(*check_fn_ptr_);
        if (fn && !fn(a)) {
            PROP_CHECK_ERR(a);
            return false;
        }
    }

    return true;
}

bool Property::checkList(const AtomListView& l) const
{
    if (!check_fn_ptr_)
        return true;

    PropListCheckFn list_fn = std::get<4>(*check_fn_ptr_);
    if (list_fn) {
        if (!list_fn(l)) {
            PROP_CHECK_ERR(l);
            return false;
        } else
            return true;
    }

    PropFloatCheckFn float_fn = std::get<0>(*check_fn_ptr_);
    if (float_fn) {
        // check for all floats
        for (auto& a : l) {
            if (!a.isFloat()) {
                LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "list of floats expected, got: " << l;
                return false;
            }

            if (!float_fn(a.asFloat())) {
                PROP_CHECK_ERR(l);
                return false;
            }
        }

        return true;
    }

    PropIntCheckFn int_fn = std::get<1>(*check_fn_ptr_);
    if (int_fn) {
        // check for all ints
        for (auto& a : l) {
            if (!a.isInteger()) {
                LogPdObject(owner(), LogLevel::LOG_ERROR).stream()
                    << errorPrefix() << "list of integers expected, got: " << l;
                return false;
            }

            if (!int_fn(a.asInt())) {
                PROP_CHECK_ERR(l);
                return false;
            }
        }

        return true;
    }

    PropSymbolCheckFn sym_fn = std::get<2>(*check_fn_ptr_);
    if (sym_fn) {
        // check for all symbols
        for (auto& a : l) {
            if (!a.isSymbol()) {
                LogPdObject(owner(), LogLevel::LOG_ERROR).stream()
                    << errorPrefix() << "list of symbols expected, got: " << l;
                return false;
            }

            if (!sym_fn(a.asSymbol())) {
                PROP_CHECK_ERR(l);
                return false;
            }
        }

        return true;
    }

    return true;
}

std::string Property::errorPrefix() const
{
    constexpr int N = 64;
    char buf[N];
    snprintf(buf, N, "[%s] ", name()->s_name);
    return std::string(buf);
}

AtomProperty::AtomProperty(const std::string& name, const Atom& def, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::ATOM), access)
    , v_(def)
{
    info().setDefault(def);
}

bool AtomProperty::setList(const AtomListView& lst)
{
    if (!emptyCheck(lst))
        return false;

    if (lst.size() > 1) {
        PROP_ERR() << "single atom value expected, got: " << lst;
        return false;
    }

    return setValue(lst[0]);
}

AtomList AtomProperty::get() const
{
    return { v_ };
}

bool AtomProperty::getAtom(Atom& a) const
{
    a = value();
    return true;
}

bool AtomProperty::setBool(bool b)
{
    return setValue(Atom(b ? 1 : 0));
}

bool AtomProperty::setFloat(t_float f)
{
    return setValue(Atom(f));
}

bool AtomProperty::setInt(int i)
{
    return setValue(Atom(i));
}

bool AtomProperty::setSymbol(t_symbol* s)
{
    return setValue(Atom(s));
}

bool AtomProperty::setAtom(const Atom& a)
{
    return setValue(a);
}

bool AtomProperty::setValue(const Atom& v)
{
    if (!checkAtom(v))
        return false;

    v_ = v;
    return true;
}

Atom AtomProperty::defaultValue() const
{
    return info().defaultAtom();
}

ListProperty::ListProperty(const std::string& name, const AtomList& init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::LIST), access)
    , lst_(init)
{
    info().setDefault(init);
}

bool ListProperty::setList(const AtomListView& l)
{
    return setValue(l);
}

bool ListProperty::getList(AtomList& l) const
{
    l = lst_;
    return true;
}

bool ListProperty::setValue(const AtomList& l)
{
    AtomList args = l;

    if (filter_)
        args = args.filtered(filter_);

    if (map_)
        args = args.map(map_);

    if (!checkList(args))
        return false;

    lst_ = args;
    return true;
}

const AtomList& ListProperty::defaultValue() const
{
    return info().defaultList();
}

bool ListProperty::checkNonNegative()
{
    return setFloatCheck(PropValueConstraints::GREATER_EQUAL, 0);
}

bool ListProperty::checkPositive()
{
    return setFloatCheck(PropValueConstraints::GREATER_THEN, 0);
}

bool ListProperty::checkNegative()
{
    return setFloatCheck(PropValueConstraints::LESS_THEN, 0);
}

bool ListProperty::checkMinElementCount(size_t n)
{
    bool res = setListCheckFn([this, n](const AtomList& l) -> bool {
        if (l.size() < n) {
            PROP_ERR() << fmt::format("list is too short, min {} elements expected, got: {}", n, l.size());
            return false;
        } else
            return true;
    });

    if (res) {
        info().setConstraints(PropValueConstraints::MIN_ELEMENT_COUNT);
        PROP_USED info().setMinElementCount(n);
    }

    return res;
}

bool ListProperty::checkMaxElementCount(size_t n)
{
    bool res = setListCheckFn([this, n](const AtomList& l) -> bool {
        if (l.size() > n) {
            PROP_ERR() << fmt::format("list is too long, max {} elements expected, got: {}", n, l.size());
            return false;
        } else
            return true;
    });

    if (res) {
        info().setConstraints(PropValueConstraints::MAX_ELEMENT_COUNT);
        PROP_USED info().setMaxElementCount(n);
    }

    return res;
}

bool ListProperty::checkRangeElementCount(size_t min, size_t max)
{
    if (min > max) {
        PROP_ERR() << "min > max";
        return false;
    }

    bool res = setListCheckFn([this, min, max](const AtomList& l) -> bool {
        if (l.size() < min || l.size() > max) {

            if (min == max)
                PROP_ERR() << fmt::format("expected list size {}, got: {}", min, l.size());
            else
                PROP_ERR() << fmt::format("expected list size in [{}-{}] range, got: {}", min, max, l.size());

            return false;
        } else
            return true;
    });

    if (res) {
        info().setConstraints(PropValueConstraints::RANGE_ELEMENT_COUNT);
        PROP_USED info().setRangeElementCount(min, max);
    }

    return res;
}

void ListProperty::acceptFloats()
{
    setFilterAtomFn([this](const Atom& a) {
        bool rc = a.isFloat();
        if (!rc)
            PROP_ERR() << "reject non-float value: " << a;

        return rc;
    });
}

void ListProperty::acceptIntegers()
{
    setFilterAtomFn([this](const Atom& a) {
        bool rc = a.isInteger();
        if (!rc)
            PROP_ERR() << "reject non-integer value: " << a;

        return rc;
    });
}

void ListProperty::acceptSymbols()
{
    setFilterAtomFn([this](const Atom& a) {
        bool rc = a.isSymbol();
        if (!rc)
            PROP_ERR() << "reject non-symbol value: " << a;

        return rc;
    });
}

void ListProperty::roundFloats()
{
    setMapAtomFn([](const Atom& a) { return a.isFloat() ? atomFrom(std::round(a.asFloat())) : a; });
}

void ListProperty::truncateFloats()
{
    setMapAtomFn([](const Atom& a) { return a.isFloat() ? atomFrom(std::trunc(a.asFloat())) : a; });
}

void ListProperty::setFilterAtomFn(Property::PropAtomCheckFn fn)
{
    filter_ = fn;
}

void ListProperty::setMapAtomFn(AtomMapFunction fn)
{
    map_ = fn;
}

AtomList ListProperty::get() const
{
    return lst_;
}

FloatProperty::FloatProperty(const std::string& name, t_float init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::FLOAT), access)
    , v_(init)
{
    info().setDefault(init);
}

AtomList FloatProperty::get() const
{
    return { v_ };
}

static inline bool is_op(const char* sym, char op)
{
    return sym[0] == op && sym[1] == '\0';
}

bool FloatProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    using namespace parser;

    t_float res = 0;
    auto rc = numeric_prop_calc<t_float>(value(), info(), lv, res);
    switch (rc) {
    case PropParseRes::OK:
        return setValue(res);
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
}

bool FloatProperty::setFloat(t_float v)
{
    return setValue(v);
}

bool FloatProperty::setInt(int v)
{
    return setValue(v);
}

bool FloatProperty::getFloat(t_float& v) const
{
    v = value();
    return true;
}

bool FloatProperty::setValue(t_float v)
{
    if (!checkFloat(v))
        return false;

    v_ = v;
    return true;
}

bool FloatProperty::setValue(const Atom& a)
{
    if (!a.isFloat()) {
        PROP_ERR() << "float value expected, got: " << a;
        return false;
    }

    return setValue(a.asFloat());
}

t_float FloatProperty::defaultValue() const
{
    return info().defaultFloat(v_);
}

BoolProperty::BoolProperty(const std::string& name, bool init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::BOOLEAN), access)
    , v_(init)
{
    info().setDefault(init);
}

AtomList BoolProperty::get() const
{
    return AtomList({ v_ ? 1.f : 0.f });
}

bool BoolProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.size() != 1) {
        PROP_ERR() << "bool value (0|1|true|false|~|!|random) expected, got: " << lv;
        return false;
    }

    return setValue(lv[0]);
}

bool BoolProperty::setBool(bool b)
{
    return setValue(b);
}

bool BoolProperty::getBool(bool& b) const
{
    b = value();
    return true;
}

bool BoolProperty::setValue(bool v)
{
    v_ = v;
    return true;
}

bool BoolProperty::setValue(const Atom& a)
{
    using namespace parser;

    static std::random_device rnd;

    bool res = false;
    auto err = bool_prop_calc(v_, info().defaultBool(), AtomListView(a), res);
    if (err != PropParseRes::OK) {
        PROP_ERR() << bool_prop_expected() << " expected, got: " << a;
        return false;
    }

    v_ = res;
    return true;
}

bool BoolProperty::defaultValue() const
{
    return info().defaultBool();
}

IntProperty::IntProperty(const std::string& name, int init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::INTEGER), access)
    , v_(init)
{
    info().setDefault(init);
}

bool IntProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.size() == 1 && lv[0].isFloat())
        return setValue(lv[0]);
    else if (lv[0].isSymbol()) {
        using namespace parser;

        auto sym = lv[0].asT<t_symbol*>();
        auto op = parse_numeric_prop_op(sym->s_name);
        switch (op) {
        case NumericPropOp::DEFAULT: { // default
            return setValue(info().defaultInt());
        } break;
        case NumericPropOp::RANDOM: { // random
            int new_min, new_max;

            switch (info().constraints()) {
            case PropValueConstraints::CLOSED_RANGE:
                new_min = info().minInt();
                new_max = info().maxInt();
                break;
            case PropValueConstraints::OPEN_RANGE:
                new_min = info().minInt() + 1;
                new_max = info().maxInt() - 1;
                break;
            case PropValueConstraints::OPEN_CLOSED_RANGE:
                new_min = info().minInt() + 1;
                new_max = info().maxInt();
                break;
            case PropValueConstraints::CLOSED_OPEN_RANGE:
                new_min = info().minInt();
                new_max = info().maxInt() - 1;
                break;
            default:
                PROP_ERR() << "property should have range for setting it random";
                return false;
            }

            if (lv.size() == 1) { /* ok */
            } else if (lv.size() == 3 && lv[1].isInteger() && lv[2].isInteger()) {
                new_min = std::max<int>(new_min, lv[1].asT<int>());
                new_max = std::min<int>(new_max, lv[2].asT<int>());
            } else {
                PROP_ERR() << "random [MIN MAX]? expected, got: " << lv;
                return false;
            }

            std::random_device rnd;
            std::uniform_int_distribution<int> dist(new_min, new_max);
            return setValue(dist(rnd));
        } break;
        case NumericPropOp::UNKNOWN: { // error
            PROP_ERR() << "expected +|-|*|/|%|random|default, got: " << lv[0];
            return false;
        } break;
        default: { // math
            if (lv.size() == 2 && lv[1].isFloat()) { // basic math
                const auto val = lv[1].asT<int>();
                switch (op) {
                case NumericPropOp::ADD:
                    return setValue(value() + val);
                case NumericPropOp::SUB:
                    return setValue(value() - val);
                case NumericPropOp::MUL:
                    return setValue(value() * val);
                case NumericPropOp::DIV: {
                    if (val == 0) {
                        PROP_ERR() << "division by zero";
                        return false;
                    }
                    return setValue(value() / val);
                case NumericPropOp::MOD: {
                    if (val == 0) {
                        PROP_ERR() << "division by zero";
                        return false;
                    }
                    return setValue(value() % val);
                }
                default:
                    PROP_ERR() << "unknown op: " << (int)op;
                    return false;
                }
                }
            }
            break;
        }
        } // end switch
    } else {
        PROP_ERR() << "expected +|-|*|/|%|random|default, got: " << lv[0];
    }

    return false;
}

bool IntProperty::setInt(int v)
{
    return setValue(v);
}

bool IntProperty::getInt(int& v) const
{
    v = value();
    return true;
}

bool IntProperty::setValue(int v)
{
    if (!checkInt(v))
        return false;

    v_ = v;
    return true;
}

AtomList IntProperty::get() const
{
    return { t_float(v_) };
}

bool IntProperty::setValue(t_float f)
{
    int i = static_cast<int>(std::round(f));
    auto d = std::ceil(f) - f;

    if (!std::equal_to<t_float>()(d, 0)) { // just warning
        LIB_DBG << "integer value expected for " << name() << ", got " << f << ", rounding to " << i;
    }

    return setValue(i);
}

bool IntProperty::setValue(const Atom& a)
{
    if (!a.isFloat()) {
        PROP_ERR() << "integer value expected, got " << a;
        return false;
    }

    return setValue(a.asFloat());
}

int IntProperty::defaultValue() const
{
    return info().defaultInt(v_);
}

SizeTProperty::SizeTProperty(const std::string& n, size_t init, PropValueAccess access)
    : Property(PropertyInfo(n, PropValueType::INTEGER), access)
    , v_(init)
{
    if (!info().setConstraints(PropValueConstraints::GREATER_EQUAL)) {
        PROP_ERR() << "can't set constraints";
        return;
    }

    if (!info().setMinInt(0)) {
        PROP_ERR() << "can't set min constraints";
        return;
    }

    info().setDefault(int(init));
}

bool SizeTProperty::setList(const AtomListView& lst)
{
    if (!emptyCheck(lst))
        return false;

    if (lst.size() == 1)
        return setValue(lst[0]);
    else if (lst.size() == 2 && lst[0].isSymbol() && lst[1].isFloat()) {
        const auto val = lst[1].asT<int>();
        const auto op = lst[0].asT<t_symbol*>()->s_name;
        if (is_op(op, '+'))
            return setValue(value() + val);
        else if (is_op(op, '-'))
            return setValue(value() - val);
        else if (is_op(op, '*'))
            return setValue(value() * val);
        else if (is_op(op, '/')) {
            if (val == 0) {
                PROP_ERR() << "division by zero";
                return false;
            } else
                return setValue(value() / val);
        } else {
            PROP_ERR() << "expected +-*/, got: " << lst[0];
            return false;
        }
    } else {
        PROP_ERR() << "unsigned value expected, got " << lst;
        return false;
    }
}

bool SizeTProperty::getInt(int& v) const
{
    v = static_cast<int>(v_);
    return true;
}

bool SizeTProperty::setValue(size_t v)
{
    if (!checkInt(static_cast<int>(v)))
        return false;

    v_ = v;
    return true;
}

AtomList SizeTProperty::get() const
{
    return { t_float(v_) };
}

bool SizeTProperty::setValue(t_float f)
{
    if (f < 0) {
        PROP_ERR() << "unsigned value expected, got: " << f;
        return false;
    }

    auto diff = std::ceil(f) - f;
    if (!std::equal_to<t_float>()(diff, 0))
        LIB_DBG << "rounding value to " << std::round(f);

    return setValue(static_cast<size_t>(std::round(f)));
}

bool SizeTProperty::setValue(const Atom& a)
{
    if (!a.isFloat()) {
        PROP_ERR() << "unsigned value expected, got: " << a;
        return false;
    }

    return setValue(a.asFloat());
}

size_t SizeTProperty::defaultValue() const
{
    int v = info().defaultInt(0);
    if (v < 0)
        return 0;

    return static_cast<size_t>(v);
}

bool SizeTProperty::checkPositive()
{
    return setIntCheck(PropValueConstraints::GREATER_THEN, 0);
}

FlagProperty::FlagProperty(const std::string& name)
    : Property(PropertyInfo(name, PropValueType::BOOLEAN), PropValueAccess::INITONLY)
    , v_(false)
{
    info().setDefault(false);
    setPublic();
}

AtomList FlagProperty::get() const
{
    constexpr t_float F_TRUE = 1;
    constexpr t_float F_FALSE = 0;
    return { v_ ? F_TRUE : F_FALSE };
}

bool FlagProperty::getBool(bool& b) const
{
    b = value();
    return true;
}

bool FlagProperty::getInt(int& v) const
{
    v = value() ? 1 : 0;
    return true;
}

bool FlagProperty::setList(const AtomListView&)
{
    v_ = true;
    return true;
}

SymbolProperty::SymbolProperty(const std::string& name, t_symbol* init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::SYMBOL), access)
    , value_(init)
{
    info().setDefault(init);
}

AtomList SymbolProperty::get() const
{
    return Atom(value_);
}

bool SymbolProperty::setList(const AtomListView& lst)
{
    if (!emptyCheck(lst))
        return false;

    if (lst.size() != 1) {
        PROP_ERR() << "symbol expected, got " << lst;
        return false;
    }

    return setValue(lst[0]);
}

bool SymbolProperty::setSymbol(t_symbol* s)
{
    return setValue(s);
}

bool SymbolProperty::getSymbol(t_symbol*& s) const
{
    s = value();
    return true;
}

bool SymbolProperty::setValue(t_symbol* s)
{
    if (!checkSymbol(s))
        return false;

    value_ = s;
    return true;
}

bool SymbolProperty::setValue(const Atom& a)
{
    if (!a.isSymbol()) {
        PROP_ERR() << "symbol expected, got " << a;
        return false;
    }

    return setValue(a.asSymbol());
}

t_symbol* SymbolProperty::defaultValue() const
{
    return info().defaultSymbol(&s_);
}

CombinedProperty::CombinedProperty(const std::string& name, std::initializer_list<Property*> props)
    : Property(PropertyInfo(name, PropValueType::LIST), PropValueAccess::READONLY)
    , props_(props)
{
    setHidden();
}

bool CombinedProperty::setList(const AtomListView&)
{
    return true;
}

AtomList CombinedProperty::get() const
{
    AtomList res;
    res.reserve(props_.size());

    for (auto p : props_) {
        if (p)
            res.append(p->get());
    }

    return res;
}

bool CombinedProperty::getList(AtomList& l) const
{
    l = get();
    return true;
}
}
