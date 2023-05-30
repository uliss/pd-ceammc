/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_faust.h"
#include "test_external.h"

using namespace ceammc::faust;

struct _outlet {
    t_object* o_owner;
    struct _outlet* o_next;
    t_outconnect* o_connections;
    t_symbol* o_sym;
};

struct Dummy : public FaustExternalBase {
    Dummy()
        : FaustExternalBase(PdArgs { {}, &s_, 0, &s_ }, "faust_test")
    {
    }
    void processBlock(const t_sample** in, t_sample** out) { }
};

t_outlet outlet()
{
    t_outlet res;
    res.o_connections = 0;
    return res;
}

TEST_CASE("Faust", "[ceammc::faust]")
{
    PureData::instance();
    SECTION("UIElement")
    {
        UIElement e(UI_BUTTON, "test");
        REQUIRE(e.type() == UI_BUTTON);
        REQUIRE(e.typeSymbol() == gensym("button"));
        REQUIRE(e.label() == SYM("test"));
        REQUIRE(e.init() == 0.f);
        REQUIRE(e.min() == 0.f);
        REQUIRE(e.max() == 1.0f);
        REQUIRE(e.step() == 0.f);

        REQUIRE(e.getPropertySym() == SYM("@test?"));
        REQUIRE(e.setPropertySym() == SYM("@test"));

        REQUIRE(e.value() == 0.f);
        REQUIRE(e.value(1.f) == 1.f);
        e.setValue(2.0);
        REQUIRE(e.value() == 0.f);
        e.setValue(2.0, true);
        REQUIRE(e.value() == 0.f);

        t_float v = 40.f;
        e.setValuePtr(&v);
        REQUIRE(e.valuePtr() == &v);
        REQUIRE(e.value() == 0.f);
        e.setValue(100.f);
        REQUIRE(e.value() == 0.f);
        e.setValue(-100.f);
        REQUIRE(e.value() == 0.f);
        e.setValue(-20.f, true);
        REQUIRE(e.value() == 0.f);
        e.setValue(20.f, true);
        REQUIRE(e.value() == 1.f);
        e.setValue(-20.f, false);
        REQUIRE(e.value() == 1.f);
        e.setValue(0.5f, false);
        REQUIRE(e.value() == 0.5f);
        e.setValue(20.f, false);
        REQUIRE(e.value() == 0.5f);

        SECTION("testConstrains")
        {
            UIElement e(UI_H_SLIDER, "hsl");
            REQUIRE(e.typeSymbol() == gensym("hslider"));
            e.setContraints(2.0, -2, 16, 1);
            REQUIRE(e.init() == 2.0f);
            REQUIRE(e.min() == -2.0f);
            REQUIRE(e.max() == 16.f);
            REQUIRE(e.step() == 1.0f);

            t_float v;
            e.setValuePtr(&v);
            REQUIRE(v == 2.0f);
        }
    }

    SECTION("test static function")
    {
        SECTION("makeOscPath")
        {
            REQUIRE(makeOscPath(SYM("test"), { &s_ }, &s_) == "/test");
            REQUIRE(makeOscPath(SYM("test"), { SYM("0x00") }, &s_) == "/test");
            REQUIRE(makeOscPath(SYM("test"), { SYM("0x00"), SYM("A") }, &s_) == "/A/test");
            REQUIRE(makeOscPath(SYM("test"), { SYM("B"), SYM("A") }, &s_) == "/B/A/test");
            REQUIRE(makeOscPath(SYM("test"), { SYM("B ./,:?][<:"), SYM("A") }, &s_) == "/B/A/test");
            REQUIRE(makeOscPath(SYM("test"), { SYM("0123456789A_"), SYM("A") }, &s_) == "/0123456789A_/A/test");
        }

        SECTION("isGetAllProperties")
        {
            REQUIRE_FALSE(isGetAllProperties(gensym("")));
            REQUIRE_FALSE(isGetAllProperties(gensym("1")));
            REQUIRE_FALSE(isGetAllProperties(gensym("ab")));
            REQUIRE_FALSE(isGetAllProperties(gensym("@1")));
            REQUIRE(isGetAllProperties(gensym("@*?")));
        }

        SECTION("isGetProperty")
        {
            REQUIRE_FALSE(isGetProperty(gensym("")));
            REQUIRE_FALSE(isGetProperty(gensym("a")));
            REQUIRE_FALSE(isGetProperty(gensym("@")));
            REQUIRE_FALSE(isGetProperty(gensym("@!")));
            REQUIRE(isGetProperty(gensym("@?")));
            REQUIRE(isGetProperty(gensym("@longproperty?")));
        }
    }

    SECTION("testPdUI")
    {
        SECTION("construct")
        {
            PdUI a("osc.tri~");
            REQUIRE(a.uiCount() == 0);

            PdUI b("osc.tri~");
            REQUIRE(b.uiCount() == 0);

            PdUI c("osc.tri_c~");
            REQUIRE(c.uiCount() == 0);
        }

        SECTION("uiAt")
        {
            t_float b;
            PdUI a("osc.tri~");
            REQUIRE(a.uiAt(0) == 0);

            a.addButton("run", &b);
            REQUIRE(a.uiCount() == 1);
            REQUIRE(a.uiAt(0));
            REQUIRE_FALSE(a.uiAt(1));
            REQUIRE(b == 0.f);

            const PdUI& c = a;
            REQUIRE(c.uiAt(0) != 0);
            REQUIRE(c.uiAt(1) == 0);
            REQUIRE(c.uiAt(0)->label() == SYM("run"));
        }

        SECTION("ui values")
        {
            t_float v;
            std::vector<t_float> v_empty;
            PdUI a("test");
            REQUIRE(a.uiValues() == v_empty);
            a.addVerticalSlider("vsl", &v, 5.f, -20, 20, 0.0f);
            REQUIRE(a.uiCount() == 1);
            REQUIRE(a.uiAt(0)->typeSymbol() == gensym("vslider"));
            REQUIRE(v == 5);

            t_float v1;
            a.addNumEntry("num", &v1, 1.f, 0, 10, 0.1f);
            REQUIRE(v1 == 1.f);
            REQUIRE(a.uiAt(1)->typeSymbol() == gensym("nentry"));

            std::vector<t_float> vals1;
            vals1.push_back(5);
            vals1.push_back(1);
            REQUIRE(a.uiValues() == vals1);
        }
    }

    SECTION("UIProperty")
    {
        SECTION("setList ops")
        {
            FAUSTFLOAT v = 0;
            UIElement e(UI_BUTTON, "test");
            e.setValuePtr(&v);
            UIProperty p(&e);

            REQUIRE_FALSE(p.setList({}));
            REQUIRE(p.setList(LF(1)));
            REQUIRE(v == 1);
            REQUIRE(p.value() == 1);
            REQUIRE(p.setList(LF(0.5)));
            REQUIRE(v == 0.5);
            REQUIRE(p.setList(LA("+", 0.5)));
            REQUIRE(v == 1);
            REQUIRE(p.setList(LA("-", 0.25)));
            REQUIRE(v == 0.75);
            REQUIRE(p.setList(LA("/", 3)));
            REQUIRE(v == 0.25);
            REQUIRE_FALSE(p.setList(LA("/", 0.)));
            REQUIRE(v == 0.25);
            REQUIRE(p.setList(LA("*", 2)));
            REQUIRE(v == 0.5);
            REQUIRE_FALSE(p.setList(LA("?", 2)));
            REQUIRE_FALSE(p.setList(LA("+")));

            REQUIRE(p.setList(LA("random")));
            REQUIRE(v != 0.5);
        }
    }

    SECTION("to_units")
    {
        REQUIRE(to_units("db") == PropValueUnits::DB);
        REQUIRE(to_units("dbfs") == PropValueUnits::DB);
        REQUIRE(to_units("sec") == PropValueUnits::SEC);
        REQUIRE(to_units("ms") == PropValueUnits::MSEC);
        REQUIRE(to_units("msec") == PropValueUnits::MSEC);
        REQUIRE(to_units("Hz") == PropValueUnits::HZ);
        REQUIRE(to_units("hz") == PropValueUnits::HZ);
        REQUIRE(to_units("samp") == PropValueUnits::SAMP);
        REQUIRE(to_units("%") == PropValueUnits::PERCENT);
        REQUIRE(to_units("perc") == PropValueUnits::PERCENT);
        REQUIRE(to_units("bpm") == PropValueUnits::BPM);
        REQUIRE(to_units("cent") == PropValueUnits::CENT);
        REQUIRE(to_units("semitone") == PropValueUnits::SEMITONE);
    }
}
