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

#include "catch.hpp"

#include "../class-wrapper/src/wrapper_macros.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "test_external.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace wrapper;

class WrapperInt : public DataIFace {
    int v_;

public:
    WrapperInt(int v = 0)
        : v_(v)
    {
    }

    bool operator==(const WrapperInt& d) const
    {
        return v_ == d.v_;
    }

    std::string toString() const
    {
        return std::string("int: ") + std::to_string(v_);
    }

    Result setFromFloat(t_float v) override
    {
        v_ = static_cast<decltype(v_)>(v);
        return ok();
    }

    Result setFromList(const ceammc::AtomList& l) override
    {
        if (l.isFloat()) {
            v_ = static_cast<decltype(v_)>(l[0].asFloat());
            return ok();
        } else
            return error(std::string("float expected, got: ") + to_string(l));
    }

    static const char* typeName()
    {
        return "DataInt";
    }

    int get() const { return v_; }

public:
    static AbstractData* initFromList(const AtomList& l)
    {
        WrapperInt wint;
        if (wint.setFromList(l))
            return new AbstractDataWrapper<WrapperInt>(wint);
        else
            return nullptr;
    }

    static WrapperInt parse(const std::string& str)
    {
        return WrapperInt(strtol(str.c_str(), nullptr, 0));
    }

    static std::tuple<int, std::string> intstr(int v)
    {
        return std::make_tuple(v, std::to_string(v));
    }

    static std::vector<std::string> str(const std::vector<int>& v)
    {
        std::vector<std::string> res;
        res.reserve(v.size());
        for (auto& e : v)
            res.push_back(std::to_string(e));

        return res;
    }

    static AtomList dup2(int v) { return AtomList(Atom(v), Atom(v + 1)); }
    static Atom toAtom(int v) { return Atom(v); }
    static int fromAtom(const Atom& a)
    {
        if (a.isFloat())
            return a.asFloat();
        else if (a.isSymbol())
            return std::strtol(a.asSymbol()->s_name, nullptr, 0);
        else
            return 0;
    }

    static int lsize(const AtomList& l) { return l.size(); }
    static t_symbol* test_msg() { return gensym("test_msg"); }
    static int slen(t_symbol* s) { return strlen(s->s_name); }
};

TEST_CASE("wrapper static method", "[class-wrapper]")
{
    SECTION("parse")
    {
        using DataType = AbstractDataWrapper<WrapperInt>;
        using mtype = decltype(&WrapperInt::parse);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "int.parse", method_id, mtype, &WrapperInt::parse);
        }

        SECTION("empty args")
        {
            TestType t("int.parse");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << BANG;
            REQUIRE(t.hasOutput());

            t << 123;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0).asD<DataType>()->value().get() == 123);

            t << "0xFF";
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0).asD<DataType>()->value().get() == 255);

            t << LA("122");
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0).asD<DataType>()->value().get() == 122);
        }
    }

    SECTION("parse")
    {
        using mtype = decltype(&WrapperInt::intstr);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "int.intstr", method_id, mtype, &WrapperInt::intstr);
        }

        SECTION("empty args")
        {
            TestType t("int.intstr");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            t << 144;
            REQUIRE(t.hasOutput());
            REQUIRE(t.isOutputSymbolAt(1));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputSymbolAt(1)->s_name == std::string("144"));
            REQUIRE(t.outputFloatAt(0) == 144);
        }
    }

    SECTION("str")
    {
        using mtype = decltype(&WrapperInt::str);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "int.str", method_id, mtype, &WrapperInt::str);
        }

        SECTION("empty args")
        {
            TestType t("int.str");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << LF(1, 2, 3, 4, 5);
            REQUIRE(t.hasOutput());
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LA("1", "2", "3", "4", "5"));

            t.sendMessage(gensym("dump"));
        }
    }

    SECTION("dup2")
    {
        using mtype = decltype(&WrapperInt::dup2);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "int.dup2", method_id, mtype, &WrapperInt::dup2);
        }

        SECTION("empty args")
        {
            TestType t("int.dup2");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << 10;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(10, 11));
        }
    }

    SECTION("toAtom")
    {
        using mtype = decltype(&WrapperInt::toAtom);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "int->atom", method_id, mtype, &WrapperInt::toAtom);
        }

        SECTION("empty args")
        {
            TestType t("int->atom");
            REQUIRE_PROPERTY(t, @args);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << 10;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 10);
        }

        SECTION("float arg")
        {
            TestType t("int->atom", LF(125));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @args, 125);

            t << BANG;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 125);
        }
    }

    SECTION("fromAtom")
    {
        using mtype = decltype(&WrapperInt::fromAtom);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "atom->int", method_id, mtype, &WrapperInt::fromAtom);
        }

        SECTION("empty args")
        {
            TestType t("atom->int");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << LA("0xFF");
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 255);

            t << LA(155);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 155);
        }
    }

    SECTION("lsize")
    {
        using mtype = decltype(&WrapperInt::lsize);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "lsize", method_id, mtype, &WrapperInt::lsize);
        }

        SECTION("empty args")
        {
            TestType t("lsize");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << BANG;
            REQUIRE(t.outputFloatAt(0) == 0);
            t << 123;
            REQUIRE(t.outputFloatAt(0) == 1);
            t << "ABC";
            REQUIRE(t.outputFloatAt(0) == 1);
            t << LA(1, 2, 3, "4");
            REQUIRE(t.outputFloatAt(0) == 4);
        }
    }

    SECTION("test_msg")
    {
        using mtype = decltype(&WrapperInt::test_msg);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "test_msg", method_id, mtype, &WrapperInt::test_msg);
        }

        SECTION("empty args")
        {
            TestType t("test_msg");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << BANG;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == gensym("test_msg"));
            t << 123;
            REQUIRE_FALSE(t.hasOutput());
        }
    }

    SECTION("strlen")
    {
        using mtype = decltype(&WrapperInt::slen);
        using ExternalType = wrapper::ClassStaticMethod<WrapperInt, mtype>;
        using TestType = TestPdExternal<ExternalType>;

        SECTION("register")
        {
            WRAP_STATIC_METHOD(WrapperInt, "strlen", method_id, mtype, &WrapperInt::slen);
        }

        SECTION("empty args")
        {
            TestType t("strlen");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            t << "ABC";
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 3);

            t << LA("A", "B", "C");
            REQUIRE_FALSE(t.hasOutputAt(0));

            t << 1000;
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.outputFloatAt(0) == 3);

            t << BANG;
            REQUIRE(t.outputFloatAt(0) == 3);
        }
    }

    SECTION("parse string")
    {
        using DataInt = AbstractDataWrapper<WrapperInt>;

        {
            auto lst = parseDataString("DataInt(255)");
            REQUIRE(lst);
            REQUIRE(lst.result().asD<DataInt>()->value().get() == 255);
        }

        {
            auto lst = parseDataString("DataInt(1 2)");
            REQUIRE_FALSE(lst);
            REQUIRE(lst.result().empty());
        }
    }
}
