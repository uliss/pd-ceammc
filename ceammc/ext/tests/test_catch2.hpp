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
#ifndef TEST_CATCH2_HPP
#define TEST_CATCH2_HPP

#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_DISABLE_MATCHERS
#include "Catch2/catch.hpp"

#include "ceammc.h"
#include "ceammc_data.h"
#include "ceammc_format.h"
#include "ceammc_message.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc::literals;

#include <sstream>

struct NoOutput : public ceammc::Exception {
    NoOutput(OutletIdx n)
        : ceammc::Exception(std::string("no output at ") + std::to_string(n.n))
    {
    }
};

struct NotFloat : public ceammc::Exception {
    NotFloat(const std::string& msg)
        : ceammc::Exception("not a float: " + msg)
    {
    }
};

struct NotSymbol : public ceammc::Exception {
    NotSymbol(const std::string& msg)
        : ceammc::Exception("not a symbol: " + msg)
    {
    }
};

struct NotAtom : public ceammc::Exception {
    NotAtom(const std::string& msg)
        : ceammc::Exception("not a atom: " + msg)
    {
    }
};

struct NotData : public ceammc::Exception {
    NotData(const std::string& msg)
        : ceammc::Exception("not a data: " + msg)
    {
    }
};

struct NotAny : public ceammc::Exception {
    NotAny(const std::string& msg)
        : ceammc::Exception("not a message: " + msg)
    {
    }
};

struct NotList : public ceammc::Exception {
    NotList(const std::string& msg)
        : ceammc::Exception("not a list: " + msg)
    {
    }
};

/** float */
template <typename T>
t_float floatAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isFloat())
        throw NotFloat(to_string(msg));

    return msg.atomValue().asFloat();
}

/** symbol */
template <typename T>
std::string symbolAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isSymbol())
        throw NotSymbol(to_string(msg));

    return msg.atomValue().asSymbol()->s_name;
}

/** atom */
template <typename T>
Atom atomAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (msg.isList() || msg.isAny())
        throw NotAtom(to_string(msg));

    return msg.atomValue();
}

/** data */
template <typename T>
Atom dataAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isData())
        throw NotData(to_string(msg));

    return msg.atomValue();
}

/** list */
template <typename T>
AtomList listAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isList())
        throw NotList(to_string(msg));

    return msg.listValue();
}

/** any */
template <typename T>
AtomList anyAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isAny())
        throw NotAny(to_string(msg));

    return msg.anyValue();
}

template <typename T>
class OutputFloatBase : public Catch::MatcherBase<T> {
protected:
    t_float v_;
    size_t outl_;

public:
    OutputFloatBase(t_float v, size_t outlet = 0)
        : v_(v)
        , outl_(outlet)
    {
    }

    std::string describe() const final
    {
        std::ostringstream ss;
        ss << "output " << v_ << " to outlet[" << outl_ << "]";
        return ss.str();
    }
};

template <typename T>
class OutputSymbolBase : public Catch::MatcherBase<T> {
protected:
    t_symbol* v_;
    size_t outl_;

public:
    OutputSymbolBase(const char* v, size_t outlet = 0)
        : v_(gensym(v))
        , outl_(outlet)
    {
    }

    std::string describe() const final
    {
        std::ostringstream ss;
        ss << "output " << v_->s_name << " to outlet[" << outl_ << "]";
        return ss.str();
    }
};

template <class T>
class OutputFloat : public OutputFloatBase<TestExternal<T>> {
public:
    OutputFloat(t_float v, size_t outlet = 0)
        : OutputFloatBase<TestExternal<T>>(v, outlet)
    {
    }

    bool match(const TestExternal<T>& t) const override
    {
        return t.hasNewMessages(this->outl_)
            && t.lastMessage(this->outl_).isFloat()
            && t.lastMessage(this->outl_).atomValue().asFloat() == Approx(this->v_);
    }
};

template <class T>
class OutputFloatExt : public OutputFloatBase<TestPdExternal<T>> {
public:
    OutputFloatExt(t_float v, size_t outlet = 0)
        : OutputFloatBase<TestPdExternal<T>>(v, outlet)
    {
    }

    bool match(const TestPdExternal<T>& t) const override
    {
        return t.hasOutputAt(this->outl_)
            && t.isOutputFloatAt(this->outl_)
            && t.outputFloatAt(this->outl_) == Approx(this->v_);
    }
};

template <class T>
class OutputSymbol : public OutputSymbolBase<TestExternal<T>> {
public:
    OutputSymbol(const char* v, size_t outlet = 0)
        : OutputSymbolBase<TestExternal<T>>(v, outlet)
    {
    }

    bool match(const TestExternal<T>& t) const override
    {
        return t.hasNewMessages(this->outl_)
            && t.lastMessage(this->outl_).isSymbol()
            && t.lastMessage(this->outl_).atomValue().asSymbol() == this->v_;
    }
};

template <class T>
class OutputSymbolExt : public OutputSymbolBase<TestPdExternal<T>> {
public:
    OutputSymbolExt(const char* v, size_t outlet = 0)
        : OutputSymbolBase<TestPdExternal<T>>(v, outlet)
    {
    }

    bool match(const TestPdExternal<T>& t) const override
    {
        return t.hasOutputAt(this->outl_)
            && t.isOutputSymbolAt(this->outl_)
            && t.outputSymbolAt(this->outl_) == this->v_;
    }
};

template <typename T>
class HasProperty : public Catch::MatcherBase<T> {
    std::string name_;
    AtomList value_;

public:
    HasProperty(const char* name, const AtomList& l)
        : name_(name)
        , value_(l)
    {
    }

    std::string describe() const final
    {
        std::ostringstream ss;
        ss << "has property " << name_ << " = " << value_;
        return ss.str();
    }

    bool match(const T& t) const override
    {
        auto prop = t.property(gensym(name_.c_str()));
        return prop && prop->get() == value_;
    }
};

template <class T>
class HasOutput : public Catch::MatcherBase<TestExternal<T>> {
    size_t outl_;

public:
    HasOutput(size_t outlet = 0)
        : outl_(outlet)
    {
    }

    bool match(const TestExternal<T>& t) const override
    {
        return t.hasNewMessages(outl_);
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "has output to outlet[" << outl_ << "]";
        return ss.str();
    }
};

template <class T>
class HasOutputExt : public Catch::MatcherBase<TestPdExternal<T>> {
    size_t outl_;

public:
    HasOutputExt(size_t outlet = 0)
        : outl_(outlet)
    {
    }

    bool match(const TestPdExternal<T>& t) const override
    {
        return t.hasOutputAt(outl_);
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "has output at outlet [" << outl_ << "]";
        return ss.str();
    }
};

// common
template <typename T, typename... Args>
HasProperty<T> hasProperty(T* t, const char* name, Args... args) { return HasProperty<T>(name, AtomList(args...)); }

// Pd object simulation
template <typename T>
OutputFloat<T> outputFloat(TestExternal<T>* t, t_float v, size_t outl = 0) { return OutputFloat<T>(v, outl); }

template <typename T>
OutputFloat<T> outputTrue(TestExternal<T>* t, size_t outl = 0) { return OutputFloat<T>(1.0, outl); }

template <typename T>
OutputFloat<T> outputFalse(TestExternal<T>* t, size_t outl = 0) { return OutputFloat<T>(0.0, outl); }

template <typename T>
OutputSymbol<T> outputSymbol(TestExternal<T>* t, const char* v, size_t outl = 0) { return OutputSymbol<T>(v, outl); }

template <typename T>
HasOutput<T> hasOutput(TestExternal<T>* t, size_t outl = 0) { return HasOutput<T>(outl); }

// real Pd external
template <typename T>
OutputFloatExt<T> outputFloat(TestPdExternal<T>* t, t_float v, size_t outl = 0) { return OutputFloatExt<T>(v, outl); }

template <typename T>
OutputFloatExt<T> outputTrue(TestPdExternal<T>* t, size_t outl = 0) { return OutputFloatExt<T>(1.0, outl); }

template <typename T>
OutputFloatExt<T> outputFalse(TestPdExternal<T>* t, size_t outl = 0) { return OutputFloatExt<T>(0.0, outl); }

template <typename T>
OutputSymbolExt<T> outputSymbol(TestPdExternal<T>* t, const char* v, size_t outl = 0) { return OutputSymbolExt<T>(v, outl); }

template <typename T>
HasOutputExt<T> hasOutput(TestPdExternal<T>* t, size_t outl = 0) { return HasOutputExt<T>(outl); }

static std::ostream& operator<<(std::ostream& os, const BaseObject& b)
{
    os << "[" << b.className()->s_name << "]";
    return os;
}

static std::ostream& operator<<(std::ostream& os, const External& e)
{
    os << "[" << e.className()->s_name << "]";
    return os;
}

#endif // TEST_CATCH2_HPP
