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

struct Dummy {
};
typedef PdUI<Dummy> UI;

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
        UIElement e(UI_BUTTON, "/ui/test", "test");
        REQUIRE(e.type() == UI_BUTTON);
        REQUIRE(e.typeSymbol() == gensym("button"));
        REQUIRE(e.path() == "/ui/test");
        REQUIRE(e.label() == "test");
        REQUIRE(e.init() == 0.f);
        REQUIRE(e.min() == 0.f);
        REQUIRE(e.max() == 1.0f);
        REQUIRE(e.step() == 0.f);

        REQUIRE(e.getPropertySym() == gensym("@test?"));
        REQUIRE(e.setPropertySym() == gensym("@test"));

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

        e.outputProperty(0);
        e.outputValue(0);

        t_outlet out = outlet();
        e.outputProperty(&out);
        e.outputValue(&out);

        SECTION("testConstrains")
        {
            UIElement e(UI_H_SLIDER, "/ui/hsl", "hsl");
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

        SECTION("testDump")
        {
            t_outlet out = outlet();
            UIElement e(UI_V_SLIDER, "/ui/vsl", "vsl");
            REQUIRE(e.typeSymbol() == gensym("vslider"));
            e.setContraints(2.0, -2, 16, 1);

            e.dump(0);

            t_outlet xlet = outlet();
            e.dump(&xlet);
            t_float v;
            e.setValuePtr(&v);
            e.dump(&xlet);
        }

        SECTION("pathcmp")
        {
            UIElement e(UI_V_SLIDER, "/ui/vsl", "Slider name");
            REQUIRE_FALSE(e.pathcmp(""));
            REQUIRE_FALSE(e.pathcmp(""));
            REQUIRE_FALSE(e.pathcmp("/vsl"));
            REQUIRE_FALSE(e.pathcmp("i/vsl"));
            REQUIRE(e.pathcmp("ui/vsl"));
            REQUIRE(e.pathcmp("/ui/vsl"));
            REQUIRE(e.pathcmp("vsl"));
        }
    }

    SECTION("test static function")
    {
        SECTION("skipOscSegment")
        {
            REQUIRE(skipOscSegment(""));
            REQUIRE(skipOscSegment("0x00"));
            REQUIRE_FALSE(skipOscSegment("abc"));
        }

        SECTION("invalidOscChar")
        {
            REQUIRE(invalidOscChar(' '));
            REQUIRE(invalidOscChar('.'));
            REQUIRE(invalidOscChar('/'));
            REQUIRE(invalidOscChar(','));
            REQUIRE(invalidOscChar(';'));
            REQUIRE(invalidOscChar('?'));
            REQUIRE(invalidOscChar('~'));
            REQUIRE(invalidOscChar(']'));
            REQUIRE(invalidOscChar('['));
            REQUIRE(invalidOscChar('\\'));
            REQUIRE(invalidOscChar('>'));
            REQUIRE(invalidOscChar('<'));
            REQUIRE(invalidOscChar(':'));
            REQUIRE(invalidOscChar('-'));
            REQUIRE(invalidOscChar('!'));
            REQUIRE(invalidOscChar('@'));
            REQUIRE(invalidOscChar('#'));
            REQUIRE(invalidOscChar('$'));
            REQUIRE(invalidOscChar('%'));
            REQUIRE(invalidOscChar('^'));
            REQUIRE(invalidOscChar('&'));
            REQUIRE(invalidOscChar('*'));
            REQUIRE(invalidOscChar('('));
            REQUIRE(invalidOscChar(')'));
            REQUIRE_FALSE(invalidOscChar('_'));
            REQUIRE_FALSE(invalidOscChar('1'));
            REQUIRE_FALSE(invalidOscChar('2'));
            REQUIRE_FALSE(invalidOscChar('a'));
            REQUIRE_FALSE(invalidOscChar('A'));
            REQUIRE_FALSE(invalidOscChar('Z'));
        }

        SECTION("escapeOscSegment")
        {
            REQUIRE(escapeOscSegment("") == "");
            REQUIRE(escapeOscSegment("abc") == "abc");
            REQUIRE(escapeOscSegment("ab_c") == "ab_c");
            REQUIRE(escapeOscSegment("1 2 3 4 5") == "12345");
            REQUIRE(escapeOscSegment("!№%:,.;()") == "");
        }

        SECTION("filterOscSegment")
        {
            typedef std::vector<std::string> SList;
            SList l1, l2;
            REQUIRE(filterOscSegment(l1) == l1);
            l1.push_back("");
            l1.push_back("");
            REQUIRE(filterOscSegment(l1) == l2);
            l1.push_back("a");
            l2.push_back("a");
            REQUIRE(filterOscSegment(l1) == l2);

            l1.push_back("0x00");
            REQUIRE(filterOscSegment(l1) == l2);

            l1.push_back("a b c d");
            l2.push_back("abcd");
            REQUIRE(filterOscSegment(l1) == l2);
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
            UI a("osc.tri~", "osc1");
            REQUIRE(a.fullName() == "osc.tri~ osc1");
            REQUIRE(a.uiCount() == 0);
            REQUIRE(a.oscPath("freq") == "/osc1/osctri/freq");

            UI b("osc.tri~", "");
            REQUIRE(b.fullName() == "osc.tri~");
            REQUIRE(b.uiCount() == 0);
            REQUIRE(b.oscPath("freq") == "/osctri/freq");

            UI c("osc.tri_c~", "");
            REQUIRE(c.fullName() == "osc.tri_c~");
            REQUIRE(c.uiCount() == 0);
            REQUIRE(c.oscPath("freq") == "/osctri_c/freq");
        }

        SECTION("uiAt")
        {
            t_float b;
            UI a("osc.tri~", "osc1");
            REQUIRE(a.uiAt(0) == 0);
            REQUIRE(a.findElementByLabel("run") == 0);

            a.addButton("run", &b);
            REQUIRE(a.uiCount() == 1);
            REQUIRE(a.uiAt(0) != 0);
            REQUIRE(a.uiAt(1) == 0);
            REQUIRE(b == 0.f);
            a.setElementValue("run1", 100.f);
            REQUIRE(b == 0.f);
            REQUIRE(a.findElementByLabel("run") == a.uiAt(0));
            a.setElementValue("run", 1.f);
            REQUIRE(b == 1.f);

            const UI& c = a;
            REQUIRE(c.uiAt(0) != 0);
            REQUIRE(c.uiAt(1) == 0);
            REQUIRE(c.uiAt(0)->label() == "run");
            REQUIRE(c.uiAt(0)->path() == "/osc1/osctri/run");

            REQUIRE(a.findElementByLabel("run") == a.uiAt(0));
        }

        SECTION("ui values")
        {
            t_float v;
            std::vector<t_float> v_empty;
            UI a("test", "id1");
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
        SECTION("asd")
        {
            FAUSTFLOAT v = 0;
            UIElement e(UI_BUTTON, "/ui/test", "test");
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
}
