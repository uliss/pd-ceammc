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
#include "../ui/ui_knob.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Knob)

TEST_CASE("ui.knob", "[ui.knob]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestKnob t("ui.knob");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->midiChannel() == 0);
        REQUIRE(t->midiControl() == 0);
        REQUIRE(t->value() == 0);
        REQUIRE(t->realValue() == 0);

        float prop_min, prop_max, f;
        AtomList l;

        HAS_PROPERTY(t, "min");
        HAS_PROPERTY(t, "max");
        HAS_PROPERTY(t, "size");
        HAS_PROPERTY(t, "presetname");
        HAS_PROPERTY(t, "background_color");
        HAS_PROPERTY(t, "border_color");
        HAS_PROPERTY(t, "knob_color");
        HAS_PROPERTY(t, "scale_color");
        HAS_PROPERTY(t, "show_range");
        HAS_PROPERTY(t, "active_scale");
        HAS_PROPERTY(t, "value");
        HAS_PROPERTY(t, "pinned");
        HAS_PROPERTY(t, "midi_channel");
        HAS_PROPERTY(t, "midi_control");
        HAS_PROPERTY(t, "midi_pickup");
        HAS_PROPERTY(t, "send");
        HAS_PROPERTY(t, "receive");

        REQUIRE_FALSE(t->hasProperty(gensym("not-exists")));

        REQUIRE(t->getProperty(gensym("min"), prop_min));
        REQUIRE(prop_min == 0);
        REQUIRE(t->getProperty(gensym("max"), prop_max));
        REQUIRE(prop_max == 1);
        REQUIRE(t->getProperty(gensym("value"), f));
        REQUIRE(f == 0);
        REQUIRE(t->getProperty(gensym("midi_channel"), f));
        REQUIRE(f == 0);
        REQUIRE(t->getProperty(gensym("midi_control"), f));
        REQUIRE(f == 0);
        REQUIRE(t->getProperty(gensym("midi_pickup"), f));
        REQUIRE(f == 1);
        REQUIRE_FALSE(t->getProperty(gensym("size"), f));
        REQUIRE_FALSE(t->getProperty(gensym("presetname"), f));
        REQUIRE_FALSE(t->getProperty(gensym("not-exists"), f));

        REQUIRE(t->getProperty(gensym("size"), l));
        REQUIRE(l == LF(40, 40));
        REQUIRE(t->getProperty(gensym("max"), l));
        REQUIRE(l == LF(1));
        REQUIRE(t->getProperty(gensym("knob_color"), l));
        REQUIRE(l == LA(0.f, 0.75, 1, 1));
        REQUIRE_FALSE(t->getProperty(gensym("unknown???"), l));

        REQUIRE(t->presetId()->s_name == std::string("ui.knob.0"));
        REQUIRE(t->name() == gensym("ui.knob"));

        REQUIRE(t->getProperty(gensym("send"), l));
        REQUIRE(l == LA("(null)"));

        REQUIRE(t->getProperty(gensym("receive"), l));
        REQUIRE(l == LA("(null)"));

        REQUIRE(t->zoom() == 1.f);
        REQUIRE(t->width() == 40);
        REQUIRE(t->height() == 40);
        REQUIRE(t->x() == 0.f);
        REQUIRE(t->y() == 0.f);
        ::t_rect r = t->rect();
        REQUIRE(r.height == 40);
        REQUIRE(r.width == 40);
        REQUIRE(r.x == 0.f);
        REQUIRE(r.y == 0.f);

        pd_float(t->asPd(), 0.6);
        REQUIRE(t->getProperty(gensym("value"), f));
        REQUIRE(f == 0.6f);
        REQUIRE(t->value() == 0.6f);
        REQUIRE(t->range() == 1);
        REQUIRE(t->minValue() == 0);
        REQUIRE(t->maxValue());

        t->setValue(-100);
        REQUIRE(t->value() == 0);
        t->setValue(1.1);
        REQUIRE(t->value() == 1);

        SECTION("new one")
        {
            TestKnob t2("ui.knob");
            REQUIRE(t2->numOutlets() == 1);
            REQUIRE(t2->presetId()->s_name == std::string("ui.knob.1"));
        }

        SECTION("properties")
        {
            SECTION("min/max")
            {
                TestKnob t("ui.knob", LA("@min", -10, "@max", 10));
                REQUIRE(t->minValue() == -10);
                REQUIRE(t->maxValue() == 10);
                REQUIRE(t->range() == 20);
                t->setValue(0.1f);
                REQUIRE(t->value() == 0.1f);
                REQUIRE(t->realValue() == -8.f);

                t->setRealValue(-11);
                REQUIRE(t->realValue() == -10);
                REQUIRE(t->value() == 0);
                t->setRealValue(11);
                REQUIRE(t->realValue() == 10);
                REQUIRE(t->value() == 1);

                SECTION("reversed")
                {
                    TestKnob t("ui.knob", LA("@min", 10, "@max", -10));
                    REQUIRE(t->maxValue() == -10);
                    REQUIRE(t->minValue() == 10);
                    REQUIRE(t->range() == -20);
                    REQUIRE(t->value() == 0.f);
                    REQUIRE(t->realValue() == 10);
                    t->setValue(0.25);
                    REQUIRE(t->value() == 0.25f);
                    REQUIRE(t->realValue() == 5);
                    t->setValue(1);
                    REQUIRE(t->realValue() == -10);

                    t->setRealValue(-11);
                    REQUIRE(t->realValue() == -10);
                    REQUIRE(t->value() == 1);
                    t->setRealValue(12);
                    REQUIRE(t->realValue() == 10);
                    REQUIRE(t->value() == 0);
                }

                SECTION("invalid range")
                {
                    TestKnob t("ui.knob", LA("@min", 0.f, "@max", 0.f));
                    REQUIRE(t->maxValue() == 0);
                    REQUIRE(t->minValue() == 0);
                    REQUIRE(t->range() == 0);
                    REQUIRE(t->value() == 0.f);
                    REQUIRE(t->realValue() == 0.f);
                    t->setValue(0);
                    REQUIRE(t->value() == 0);
                    t->setValue(1);
                    REQUIRE(t->value() == 1);
                    REQUIRE(t->realValue() == 0);
                    t->setRealValue(0);
                    REQUIRE(t->realValue() == 0);
                }
            }

            SECTION("presetname")
            {
                TestKnob t("ui.knob", LA("@presetname", "ABC"));
                REQUIRE(t->presetId()->s_name == std::string("ABC"));

                TestKnob t2("ui.knob", LA("@presetname", "ABC"));
                REQUIRE(t2->presetId()->s_name == std::string("ABC"));

                TestKnob t3("ui.knob", LA("@presetname", "ABC"));
                REQUIRE(t3->presetId()->s_name == std::string("ABC"));
                t3->setProperty(gensym("presetname"), LA("DEF"));
                REQUIRE(t3->presetId()->s_name == std::string("DEF"));

                t3->setProperty(gensym("presetname"), LA("(null)"));
                REQUIRE(t3->presetId()->s_name == std::string("(null)"));

                t3->setProperty(gensym("presetname"), LA("(null)"));
                REQUIRE(t3->presetId()->s_name == std::string("(null)"));

                t3->setProperty(gensym("presetname"), LA("GHJ"));
                REQUIRE(t3->presetId()->s_name == std::string("GHJ"));

                TestKnob t4("ui.knob");
                REQUIRE(t4->presetId()->s_name == std::string("ui.knob.1"));

                TestKnob t5("ui.knob");
                REQUIRE(t5->presetId()->s_name == std::string("ui.knob.2"));

                t4->setProperty(gensym("presetname"), LA("(null)"));

                TestKnob t6("ui.knob");
                REQUIRE(t6->presetId()->s_name == std::string("ui.knob.1"));
            }
        }
    }

    SECTION("preset counter")
    {
        TestKnob t("ui.knob");
        REQUIRE(t->presetId()->s_name == std::string("ui.knob.0"));
    }

    SECTION("methods")
    {
        SECTION("+")
        {
            TestKnob t("ui.knob");
            REQUIRE(t->realValue() == 0.f);
            t.call("+", LF(0.1));
            REQUIRE(t->realValue() == 0.1f);
            t.call("+", LF(10));
            REQUIRE(t->realValue() == 1);
        }

        SECTION("-")
        {
            TestKnob t("ui.knob", LA("@value", 0.5f));
            REQUIRE(t->realValue() == 0.5f);
            t.call("-", LF(0.5));
            REQUIRE(t->realValue() == 0.f);
            t.call("-", LF(0.1));
            REQUIRE(t->realValue() == 0.f);
        }

        SECTION("++")
        {
            TestKnob t("ui.knob", LA("@min", -1, "@max", 2));
            REQUIRE(t->realValue() == -1);
            t.call("++");
            REQUIRE(t->realValue() == Approx(0.f));
            t.call("++");
            REQUIRE(t->realValue() == 1);
            t.call("++");
            REQUIRE(t->realValue() == 2);
            t.call("++");
            REQUIRE(t->realValue() == 2);
            t.call("++");
            REQUIRE(t->realValue() == 2);
        }

        SECTION("--")
        {
            TestKnob t("ui.knob", LA("@min", -1, "@max", 2));
            t->setRealValue(2);
            REQUIRE(t->realValue() == 2);
            t.call("--");
            REQUIRE(t->realValue() == 1);
            t.call("--");
            REQUIRE(t->realValue() == Approx(0.f));
            t.call("--");
            REQUIRE(t->realValue() == -1);
            t.call("--");
            REQUIRE(t->realValue() == -1);
        }

        SECTION("mul")
        {
            TestKnob t("ui.knob", LA("@min", 1, "@max", 12));
            REQUIRE(t->realValue() == 1);
            t.call("*", LF(2));
            REQUIRE(t->realValue() == 2);
            t.call("*", LF(2));
            REQUIRE(t->realValue() == 4);
            t.call("*", LF(2));
            REQUIRE(t->realValue() == 8);
            t.call("*", LF(2));
            REQUIRE(t->realValue() == 12);
            t.call("*", LF(2));
            REQUIRE(t->realValue() == 12);
        }

        SECTION("div")
        {
            TestKnob t("ui.knob", LA("@min", 12, "@max", 1));
            REQUIRE(t->realValue() == Approx(12));
            t.call("/", LF(2));
            REQUIRE(t->realValue() == Approx(6));
            t.call("/", LF(2));
            REQUIRE(t->realValue() == Approx(3));
            t.call("/", LF(2));
            REQUIRE(t->realValue() == Approx(1.5));
            t.call("/", LF(0.f));
        }
    }

    SECTION("onBang")
    {
        TestKnob t("ui.knob");
        pd_bang(t->asPd());
    }

    SECTION("presets")
    {
        TestKnob t("ui.knob", LA("@min", 10, "@max", 100));
        t->setRealValue(20);
        t.call("store", LF(0.f));
        t->setRealValue(40);
        t.call("store", LF(1));
        t->setRealValue(60);
        t.call("store", LF(2));

        t.call("load", LF(0.f));
        REQUIRE(t->realValue() == Approx(20));
        t.call("load", LF(1));
        REQUIRE(t->realValue() == Approx(40));
        t.call("load", LF(2));
        REQUIRE(t->realValue() == Approx(60));
        t.call("load", LF(100));
        REQUIRE(t->realValue() == Approx(10));
        t.call("load");
        REQUIRE(t->realValue() == Approx(20));
    }

    SECTION("onMidi")
    {
        TestKnob t("ui.knob");
        t->setProperty(gensym("midi_pickup"), LF(0.f));
        t->onMidiCtrl(LF(1, 2));

        t->onMidiCtrl(LF(1, 2, 3));
        REQUIRE(t->realValue() == 0);

        t->setProperty(gensym("midi_control"), LF(23));
        t->onMidiCtrl(LF(1, 2, 3));
        REQUIRE(t->realValue() == 0);

        t->onMidiCtrl(LF(23, 64, 3));
        REQUIRE(t->realValue() == Approx(0.50394f));

        t->setProperty(gensym("midi_channel"), LF(2));
        t->onMidiCtrl(LF(23, 12, 3));
        REQUIRE(t->realValue() == Approx(0.50394f));

        t->onMidiCtrl(LF(23, 12, 2));
        REQUIRE(t->realValue() == Approx(0.09449));

        t->onMidiCtrl(LF(1, 22, 2));
        REQUIRE(t->realValue() == Approx(0.09449));
        t->onMidiCtrl(LF(23, 127, 2));
        REQUIRE(t->realValue() == Approx(1));
        t->onMidiCtrl(LF(23, 0.f, 2));
        REQUIRE(t->realValue() == Approx(0));

        SECTION("pickup over min->max")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));
            t2->setRealValue(30);
            REQUIRE(t2->realValue() == Approx(30));

            // not picked up
            t2->onMidiCtrl(LF(10, 2, 1));
            t2->onMidiCtrl(LF(10, 3, 1));
            t2->onMidiCtrl(LF(10, 4, 1));
            REQUIRE(t2->realValue() == Approx(30));

            t2->onMidiCtrl(LF(10, 29, 1));
            // not picked
            REQUIRE(t2->realValue() == Approx(30));
            // picked
            t2->onMidiCtrl(LF(10, 31, 1));
            REQUIRE(t2->realValue() == Approx(31));

            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->realValue() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->realValue() == Approx(10));
        }

        SECTION("pickup over max->min")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));
            t2->setRealValue(30);
            REQUIRE(t2->realValue() == Approx(30));

            // not picked up
            t2->onMidiCtrl(LF(10, 60, 1));
            t2->onMidiCtrl(LF(10, 61, 1));
            t2->onMidiCtrl(LF(10, 62, 1));
            REQUIRE(t2->realValue() == Approx(30));

            t2->onMidiCtrl(LF(10, 31, 1));
            // not picked
            REQUIRE(t2->realValue() == Approx(30));
            // picked
            t2->onMidiCtrl(LF(10, 29, 1));
            REQUIRE(t2->realValue() == Approx(29));

            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->realValue() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->realValue() == Approx(10));
        }

        SECTION("pickup over equal")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));

            // not picked up
            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->realValue() == Approx(0));

            t2->onMidiCtrl(LF(10, 30, 1));
            // not picked
            REQUIRE(t2->realValue() == Approx(0));
            // picked
            t2->onMidiCtrl(LF(10, 0.f, 1));
            REQUIRE(t2->realValue() == Approx(0));
            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->realValue() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->realValue() == Approx(10));
        }
    }

    SECTION("send/receive test")
    {
        pd::External r1("receive", LA("r1"));
        ExternalOutput out;
        REQUIRE(r1.connectTo(0, out.object(), 0));

        TestKnob t("ui.knob", LA("@send", "r1", "@receive", "s1"));
        t->onFloat(0.5);
        REQUIRE(out.msg().atomValue().asFloat() == 0.5f);
        out.reset();

        t->onBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.5f);

        // test receive
        pd::External s1("send", LA("s1"));
        pd::External s2("send", LA("s2"));

        out.reset();
        s1.sendFloat(0.3);
        REQUIRE(out.msg().atomValue().asFloat() == 0.3f);

        // change receiver
        t->setProperty(gensym("receive"), LA("s2"));

        // do not react on old
        s1.sendFloat(0.9);
        REQUIRE(out.msg().atomValue().asFloat() == 0.3f);

        // check new one
        s2.sendFloat(0.8);
        REQUIRE(out.msg().atomValue().asFloat() == 0.8f);
    }

    SECTION("output")
    {
        pd::External knob("ui.knob");
        ExternalOutput out;
        REQUIRE(knob.connectTo(0, out.object(), 0));

        knob.bang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.f);

        knob.sendFloat(0.4);
        REQUIRE(out.msg().atomValue().asFloat() == 0.4f);

        out.reset();
        knob.bang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.4f);

        // list auto convert test
        out.reset();
        knob.sendList(LF(0.1, 0.2));
        REQUIRE(out.msg().atomValue().asFloat() == 0.1f);

        out.reset();
        knob.sendMessage(gensym("@value"), LF(0.5));
        REQUIRE(out.msg().isNone());

        knob.bang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.5f);

        knob.sendMessage(gensym("@value?"));
        REQUIRE(out.msg().listValue() == LF(0.5));

        knob.sendMessage(gensym("@max?"));
        REQUIRE(out.msg().listValue() == LF(1));

        knob.sendMessage(gensym("@send?"));
        REQUIRE(out.msg().listValue() == LA("(null)"));

        knob.sendMessage(gensym("@size?"));
        REQUIRE(out.msg().listValue() == LF(40, 40));

        out.reset();
        knob.sendMessage(gensym("set"), LF(0.1));
        REQUIRE(out.msg().isNone());
        knob.bang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.1f);
    }

    SECTION("send")
    {
        TestExtKnob t("ui.knob", LA("@send", "r1"));
        t.addListener("r1");

        t << 0.5;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 0.5);

        t << BANG;
        REQUIRE_FLOAT_WAS_SEND(t, "r1", 0.5);
    }

    SECTION("props test")
    {
        TestExtKnob t("ui.knob");
        t.call("@*?");
        REQUIRE(t.hasOutput());
        auto props = t.outputAnyAt(0);
        props.sort();
        const auto desired = LA("@*", "@active_scale", "@background_color", "@border_color",
                                 "@fontname", "@fontsize", "@fontslant", "@fontweight",
                                 "@knob_color", "@label", "@label_align", "@label_color")
            + LA("@label_inner", "@label_margins", "@label_side", "@label_valign",
                "@max", "@midi_channel", "@midi_control")
            + LA("@midi_pickup", "@min", "@pinned", "@presetname", "@receive",
                "@scale_color", "@send", "@show_range", "@size", "@value");
        REQUIRE(props == desired);

        t.call("@max?", LA("@min?", "@xxx?", "", "@non", "unknown", 100, "@receive?"));
        REQUIRE(t.outputAnyAt(0) == LA("@max", 1, "@min", 0.f, "@receive", "(null)"));
    }
}
