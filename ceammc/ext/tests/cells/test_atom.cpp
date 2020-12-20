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
#include "catch.hpp"
#include "ceammc_atom.h"

#include "test_base.h"
#include "test_external.h"
#include "test_macro.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>
#include <string>

using namespace ceammc;

t_symbol* toUpper(t_symbol* s)
{
    std::string str(s->s_name);
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return gensym(str.c_str());
}

TEST_CASE("Atom", "[core]")
{
    SECTION("None")
    {
        Atom a;
        REQUIRE(a.isNone());
        REQUIRE_FALSE(a.isBool());
        REQUIRE_FALSE(a.isData());
        REQUIRE_FALSE(a.isFloat());
        REQUIRE_FALSE(a.isSymbol());
        REQUIRE_FALSE(a.isInteger());
        REQUIRE_FALSE(a.isProperty());
        REQUIRE_FALSE(a.isDataType(100));
        REQUIRE(a.dataType() == 0);

        REQUIRE(a.type() == Atom::NONE);

        t_float f;
        t_symbol* s;

        REQUIRE_FALSE(a.getFloat(&f));
        REQUIRE_FALSE(a.getSymbol(&s));
        REQUIRE_FALSE(a.setFloat(100));
        REQUIRE_FALSE(a.setSymbol(&s_list));
        REQUIRE(a.isNone());

        REQUIRE(a.asBool(true));
        REQUIRE(a.asBool(false) == false);
        REQUIRE(a.asFloat(100) == 100);
        REQUIRE(a.asSymbol(&s_symbol) == &s_symbol);
        REQUIRE(a.asInt(-10) == -10);
        REQUIRE(a.asSizeT(3) == 3);
        REQUIRE(a.asData() == nullptr);
        REQUIRE(a.asDataT<IntData>() == nullptr);

        REQUIRE_FALSE(a < a);
        REQUIRE_FALSE(a < 0);
        REQUIRE_FALSE(a <= 0);
        REQUIRE_FALSE(a > 0);
        REQUIRE_FALSE(a >= 0);

        REQUIRE(a == a);
        REQUIRE_FALSE(a != a);
        REQUIRE(a == Atom());
        REQUIRE_FALSE(a != Atom());

        REQUIRE_FALSE(a == 0);
        REQUIRE(a != 0);

        REQUIRE((a += 10).isNone());
        REQUIRE((a -= 10).isNone());
        REQUIRE((a *= 10).isNone());
        REQUIRE((a /= 10).isNone());
    }

    SECTION("float")
    {
        Atom a(0.5);
        REQUIRE_FALSE(a.isNone());
        REQUIRE_FALSE(a.isBool());
        REQUIRE_FALSE(a.isData());
        REQUIRE(a.isFloat());
        REQUIRE_FALSE(a.isSymbol());
        REQUIRE_FALSE(a.isInteger());
        REQUIRE_FALSE(a.isProperty());
        REQUIRE_FALSE(a.isDataType(100));
        REQUIRE(a.dataType() == 0);

        REQUIRE(a.type() == Atom::FLOAT);

        t_float f;
        t_symbol* s;

        REQUIRE(a.getFloat(&f));
        REQUIRE(f == 0.5);
        REQUIRE_FALSE(a.getSymbol(&s));
        REQUIRE(a.setFloat(2.5));
        REQUIRE_FALSE(a.setSymbol(&s_list));
        REQUIRE(a.isFloat());

        REQUIRE(a.asBool(true));
        REQUIRE(a.asBool(false));
        REQUIRE(a.asFloat(100) == 2.5);
        REQUIRE(a.asSymbol(&s_symbol) == &s_symbol);
        REQUIRE(a.asInt(-10) == 2);
        REQUIRE(a.asSizeT(3) == 2);
        REQUIRE(a.asData() == nullptr);
        REQUIRE(a.asDataT<IntData>() == nullptr);

        REQUIRE_FALSE(a < a);
        REQUIRE_FALSE(a < 0);
        REQUIRE_FALSE(a <= 0);
        REQUIRE(a > 0);
        REQUIRE(a >= 0);

        REQUIRE(a == a);
        REQUIRE_FALSE(a != a);
        REQUIRE(a == Atom(2.5));
        REQUIRE_FALSE(a != Atom(2.5));
        REQUIRE(a != Atom(2));
        REQUIRE_FALSE(a == Atom(2));
        REQUIRE(a != Atom());
        REQUIRE_FALSE(a == Atom());

        REQUIRE_FALSE(a == 0);
        REQUIRE(a != 0);

        REQUIRE((a += 10) == 12.5);
        REQUIRE((a -= 10) == 2.5);
        REQUIRE((a *= 10) == 25);
        REQUIRE((a /= 10) == 2.5);
    }

    SECTION("symbol")
    {
        Atom a(SYM("abc"));
        REQUIRE_FALSE(a.isNone());
        REQUIRE_FALSE(a.isBool());
        REQUIRE_FALSE(a.isData());
        REQUIRE_FALSE(a.isFloat());
        REQUIRE(a.isSymbol());
        REQUIRE_FALSE(a.isInteger());
        REQUIRE_FALSE(a.isProperty());
        REQUIRE_FALSE(a.isDataType(100));
        REQUIRE(a.dataType() == 0);

        REQUIRE(a.type() == Atom::SYMBOL);

        t_float f;
        t_symbol* s;

        REQUIRE_FALSE(a.getFloat(&f));
        REQUIRE(a.getSymbol(&s));
        REQUIRE(s == SYM("abc"));
        REQUIRE_FALSE(a.setFloat(2.5));
        REQUIRE(a.setSymbol(&s_list));
        REQUIRE(a.isSymbol());

        REQUIRE(a.asBool(true));
        REQUIRE(a.asBool(false) == false);
        REQUIRE(a.asFloat(100) == 100);
        REQUIRE(a.asSymbol(&s_symbol) == &s_list);
        a.setSymbol(SYM("abc"));
        REQUIRE(a.asInt(-10) == -10);
        REQUIRE(a.asSizeT(3) == 3);
        REQUIRE(a.asData() == nullptr);
        REQUIRE(a.asDataT<IntData>() == nullptr);

        REQUIRE_FALSE(a < a);
        REQUIRE_FALSE(a < 0);
        REQUIRE_FALSE(a <= 0);
        REQUIRE_FALSE(a > 0);
        REQUIRE_FALSE(a >= 0);

        REQUIRE(a == a);
        REQUIRE_FALSE(a != a);
        REQUIRE(a == Atom(SYM("abc")));
        REQUIRE_FALSE(a != Atom(SYM("abc")));
        REQUIRE(a != Atom(SYM("abcd")));
        REQUIRE_FALSE(a == Atom(SYM("abcd")));
        REQUIRE(a != Atom());
        REQUIRE_FALSE(a == Atom());
        REQUIRE(a != Atom(1));
        REQUIRE_FALSE(a == Atom(1));

        REQUIRE_FALSE(a == 0);
        REQUIRE(a != 0);

        REQUIRE((a += 10).isSymbol());
        REQUIRE((a -= 10).isSymbol());
        REQUIRE((a *= 10).isSymbol());
        REQUIRE((a /= 10).isSymbol());
    }

    SECTION("Atom attr")
    {
        t_atom a;
        SETSYMBOL(&a, gensym("@attr"));
        Atom atom(a);
        REQUIRE(atom.isProperty());
        REQUIRE_FALSE(notProperty(atom));
        REQUIRE(atom.isSymbol());
        REQUIRE(atom.asSymbol() == gensym("@attr"));

        atom.setFloat(1.f, true);
        REQUIRE(!atom.isProperty());

        atom.setSymbol(gensym(""), true);
        REQUIRE(!atom.isProperty());

        Atom p(gensym("a"));
        REQUIRE(!p.isProperty());
        REQUIRE(p.isSymbol());
    }

    SECTION("Float atom tests")
    {
        Atom fatom(1.99f);
        REQUIRE(fatom.isFloat());
        REQUIRE(fatom.asFloat() == 1.99f);
        t_float v;
        REQUIRE(fatom.getFloat(&v));
        REQUIRE_FALSE(fatom.getFloat(nullptr));
        t_symbol* s;
        REQUIRE_FALSE(fatom.getSymbol(&s));
        REQUIRE_FALSE(fatom.getSymbol(nullptr));
    }

    SECTION("Type replace")
    {
        t_atom a;
        a.a_type = A_DEFFLOAT;
        Atom fatom(a);
        REQUIRE(fatom.isFloat());
        SETSYMBOL(&a, gensym("a"));
        REQUIRE(a.a_type == A_SYMBOL);
        a.a_type = A_DEFSYMBOL;
        fatom = Atom(a);
        REQUIRE(fatom.isSymbol());

        a.a_type = A_NULL;
        REQUIRE(Atom(a).type() == Atom::NONE);
    }

    SECTION("Symbol atom tests")
    {
        Atom satom(gensym("test"));
        REQUIRE_FALSE(satom.isFloat());
        REQUIRE(satom.type() == Atom::SYMBOL);
        REQUIRE(satom.asSymbol() == gensym("test"));
        t_symbol* s;
        REQUIRE(satom.getSymbol(&s));
        REQUIRE_FALSE(satom.getSymbol(NULL));
        t_float v;
        REQUIRE_FALSE(satom.getFloat(NULL));
        REQUIRE_FALSE(satom.getFloat(&v));
    }

    SECTION("set tests")
    {
        Atom a1(1.2f);
        REQUIRE(a1.setFloat(2.0f));
        REQUIRE(a1.isFloat());
        REQUIRE(a1.setFloat(3.0f, true));
        REQUIRE(a1.isFloat());
        REQUIRE_FALSE(a1.setSymbol(0));
        REQUIRE(a1.isFloat());
        // setting null pointer
        REQUIRE(a1.setSymbol(0, true));
        REQUIRE_FALSE(a1.isSymbol());
        REQUIRE(a1.setSymbol(gensym("a"), true));
        REQUIRE(a1.isSymbol());

        Atom a2(gensym("test"));
        REQUIRE(a2.setSymbol(gensym("test2")));
        REQUIRE(a2.isSymbol());
        REQUIRE(a2.setSymbol(gensym("test2"), true));
        REQUIRE(a2.isSymbol());

        REQUIRE_FALSE(a2.setFloat(1));
        REQUIRE(a2.isSymbol());
        REQUIRE(a2.setFloat(1, true));
        REQUIRE(a2.isFloat());

        Atom int100(new IntData(100));
    }

    SECTION("equal tests")
    {
        Atom a1(1.2f);
        Atom a2(1.2f);

        REQUIRE(a1 == a1);
        REQUIRE_FALSE(a1 != a1);
        REQUIRE(a1 == a2);
        REQUIRE_FALSE(a1 != a2);

        a2.setFloat(1.3f);
        REQUIRE(a1 != a2);

        Atom a3(a2);
        REQUIRE(a3 == a2);

        Atom a4(1.0f);
        a4 = a2;
        REQUIRE(a4 == a2);

        Atom s1(gensym("test"));
        Atom s2(gensym("test"));
        REQUIRE(s1 == s2);

        Atom s3(gensym("test2"));
        REQUIRE(s1 != s3);

        REQUIRE(s1 != a1);

        t_atom p;
        p.a_type = A_NULL;
        Atom u1(p);
        Atom u2(p);
        REQUIRE(u1 == u2);

        SECTION("symbol")
        {
            t_atom a1;
            t_atom a2;

            SETSYMBOL(&a1, gensym("a"));
            SETSYMBOL(&a2, gensym("a"));

            REQUIRE(Atom(a1) == Atom(a2));
            REQUIRE_FALSE(Atom(a1) != Atom(a2));
        }
    }

    SECTION("compare tests")
    {
        Atom a1(1.0);

        REQUIRE_FALSE(a1 < a1);
        Atom a2(1.0);
        REQUIRE_FALSE(a1 < a2);
        a2.setFloat(1.0001);
        REQUIRE(a1 < a2);

        Atom s1(gensym("test"));
        REQUIRE(a1 < s1);

        Atom s2(gensym("test"));
        REQUIRE_FALSE(s1 < s2);

        Atom s3(gensym("a"));
        Atom s4(gensym("b"));
        REQUIRE(s3 < s4);

        t_atom r1;
        r1.a_type = A_NULL;
        t_atom r2;
        r2.a_type = A_NULL;
        REQUIRE_FALSE(Atom(r1) < Atom(r2));
    }

    SECTION("test output")
    {
        {
            Atom a1;
            std::ostringstream ss;
            ss << a1;
            REQUIRE(ss.str() == "NONE");
        }

        {
            Atom a1(1.23f);
            std::ostringstream ss;
            ss << a1;
            REQUIRE(ss.str() == "1.23");
        }

        {
            Atom a1(gensym("test"));
            std::ostringstream ss;
            ss << a1;
            REQUIRE(ss.str() == "test");
        }
    }

    SECTION("test as int")
    {
        Atom a1(1.34f);
        REQUIRE(a1.asInt(0) == 1);

        Atom a2(gensym("a"));
        REQUIRE(a2.asInt(-30) == -30);
    }

    SECTION("test as size_t")
    {
        Atom a1(21.34f);
        REQUIRE(a1.asSizeT(0) == 21);

        Atom a2(gensym("a"));
        REQUIRE(a2.asSizeT(30) == 30);

        Atom a3(-132.f);
        REQUIRE(a3.asSizeT(10) == 10);
    }

    SECTION("test isInteger")
    {
        REQUIRE(Atom(0.f).isInteger());
        REQUIRE(Atom(-0.f).isInteger());
        REQUIRE(Atom(1.f).isInteger());
        REQUIRE(Atom(-1.f).isInteger());
        REQUIRE(Atom(100000.f).isInteger());
        REQUIRE(Atom(-100000.f).isInteger());

        REQUIRE_FALSE(Atom().isInteger());
        REQUIRE_FALSE(Atom(gensym("a")).isInteger());

        REQUIRE_FALSE(Atom(0.0000001f).isInteger());
        REQUIRE_FALSE(Atom(1.0000001f).isInteger());
        REQUIRE_FALSE(Atom(-0.0000001f).isInteger());
        REQUIRE_FALSE(Atom(-1.0000001f).isInteger());
    }

    SECTION("test float operators")
    {
        SECTION("")
        {
            REQUIRE((Atom(1.f) + 10.0) == 11.f);
            REQUIRE((Atom(1.f) - 10.0) == -9.f);
            REQUIRE((Atom(2.f) * 10.0) == 20.f);
            REQUIRE((Atom(20.f) / 10.0) == 2.f);
            REQUIRE((Atom(20.f) / 0.0) == 20.f);

            REQUIRE((Atom(gensym("a")) + 10.0) == Atom(gensym("a")));
            REQUIRE((Atom(gensym("a")) - 10.0) == Atom(gensym("a")));
            REQUIRE((Atom(gensym("a")) * 10.0) == Atom(gensym("a")));
            REQUIRE((Atom(gensym("a")) / 10.0) == Atom(gensym("a")));
            REQUIRE((Atom(gensym("a")) / 0.0) == Atom(gensym("a")));
        }

        SECTION("test assign")
        {
            Atom a(1.f);
            REQUIRE((a += 10.0) == 11.f);
            REQUIRE((a -= 2.0) == 9.f);
            REQUIRE((a *= 10.0) == 90.f);
            REQUIRE((a /= 3.0) == 30.f);
            REQUIRE((a /= 0.0) == 30.f);

            Atom b(gensym("a"));
            const Atom c(b);

            REQUIRE((b += 10.0) == c);
            REQUIRE((b -= 2.0) == c);
            REQUIRE((b *= 10.0) == c);
            REQUIRE((b /= 3.0) == c);
            REQUIRE((b /= 0.0) == c);
        }
    }

    SECTION("bool")
    {
        REQUIRE(Atom(1).isBool());
        REQUIRE(Atom(t_float(0)).isBool());
        REQUIRE(S("true").isBool());
        REQUIRE_FALSE(Atom(t_float(0.1)).isBool());
        REQUIRE_FALSE(Atom(t_float(-1)).isBool());

        REQUIRE(Atom().asBool() == false);
        REQUIRE(Atom(true).asBool() == true);
        REQUIRE(Atom(1).asBool() == true);
        REQUIRE(Atom(-1).asBool() == true);
        REQUIRE(Atom(0.f).asBool() == false);
        REQUIRE(Atom(SYM("true")).asBool() == true);
        REQUIRE(Atom(SYM("false")).asBool() == false);
        REQUIRE(Atom(SYM("???")).asBool(true) == true);
        REQUIRE(Atom(SYM("???")).asBool(false) == false);
    }

    SECTION("test apply")
    {
        SECTION("float std::function")
        {
            Atom a;
            REQUIRE_FALSE(a.applyFloat([](t_float f) { return f + 2; }));
            REQUIRE(a.isNone());

            a.setFloat(0, true);
            REQUIRE(a.applyFloat([](t_float f) { return f + 2; }));
            REQUIRE(a.asFloat() == 2);

            a.setSymbol(gensym("a"), true);
            REQUIRE_FALSE(a.applyFloat([](t_float f) { return f + 2; }));
            REQUIRE(a.isSymbol());
            REQUIRE(a.asSymbol() == gensym("a"));
        }

        SECTION("float ptr")
        {
            Atom a;
            REQUIRE_FALSE(a.applyFloat(cosf));
            REQUIRE(a.isNone());

            a.setFloat(0, true);
            REQUIRE(a.applyFloat(cosf));
            REQUIRE(a.asFloat() == 1);

            a.setSymbol(gensym("a"), true);
            REQUIRE_FALSE(a.applyFloat(cosf));
            REQUIRE(a.isSymbol());
            REQUIRE(a.asSymbol() == gensym("a"));
        }

        SECTION("symbol")
        {
            Atom a;
            REQUIRE_FALSE(a.applySymbol(toUpper));
            REQUIRE(a.isNone());

            a.setSymbol(gensym("a"), true);
            REQUIRE(a.applySymbol(toUpper));
            REQUIRE(a.isSymbol());
            REQUIRE(a.asSymbol() == gensym("A"));

            a.setFloat(123, true);
            REQUIRE_FALSE(a.applySymbol(toUpper));
            REQUIRE(a.asFloat() == 123);
        }
    }
}
