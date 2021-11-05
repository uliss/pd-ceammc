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
#include "ui_knob.h"

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
        REQUIRE(t->knobPhase() == 0);
        REQUIRE(t->value() == 0);

        t_float prop_min, prop_max, f;
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

        REQUIRE(t->zoom() == 1.);
        REQUIRE(t->width() == 40);
        REQUIRE(t->height() == 40);
        REQUIRE(t->x() == 0.);
        REQUIRE(t->y() == 0.);
        ::t_rect r = t->rect();
        REQUIRE(r.height == 40);
        REQUIRE(r.width == 40);
        REQUIRE(r.x == 0.);
        REQUIRE(r.y == 0.);

        pd_float(t->asPd(), 0.625);
        REQUIRE(t->getProperty(gensym("value"), f));
        REQUIRE(f == 0.625);
        REQUIRE(t->knobPhase() == 0.625);
        REQUIRE(t->range() == 1);
        REQUIRE(t->minValue() == 0);
        REQUIRE(t->maxValue());

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
                t->setValue(-7.5);
                REQUIRE(t->knobPhase() == 0.125);
                REQUIRE(t->value() == -7.5);

                t->setValue(-11);
                REQUIRE(t->value() == -10);
                REQUIRE(t->knobPhase() == 0);
                t->setValue(11);
                REQUIRE(t->value() == 10);
                REQUIRE(t->knobPhase() == 1);

                SECTION("reversed")
                {
                    TestKnob t("ui.knob", LA("@min", 10, "@max", -10));
                    REQUIRE(t->maxValue() == -10);
                    REQUIRE(t->minValue() == 10);
                    REQUIRE(t->range() == -20);
                    REQUIRE(t->knobPhase() == 0.f);
                    REQUIRE(t->value() == 10);
                    t->setValue(5);
                    REQUIRE(t->knobPhase() == 0.25f);
                    REQUIRE(t->value() == 5);
                    t->setValue(-10);
                    REQUIRE(t->knobPhase() == 1);
                    REQUIRE(t->value() == -10);

                    t->setValue(-11);
                    REQUIRE(t->value() == -10);
                    REQUIRE(t->knobPhase() == 1);
                    t->setValue(12);
                    REQUIRE(t->value() == 10);
                    REQUIRE(t->knobPhase() == 0);
                }

                SECTION("invalid range")
                {
                    TestKnob t("ui.knob", LA("@min", 0.f, "@max", 0.f));
                    REQUIRE(t->maxValue() == 0);
                    REQUIRE(t->minValue() == 0);
                    REQUIRE(t->range() == 0);
                    REQUIRE(t->knobPhase() == 0.f);
                    REQUIRE(t->value() == 0.f);
                    t->setValue(0);
                    REQUIRE(t->value() == 0);
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
            REQUIRE(t->value() == 0.);
            t.call("+", LF(0.125));
            REQUIRE(t->value() == 0.125);
            t.call("+", LF(10));
            REQUIRE(t->value() == 1);
        }

        SECTION("-")
        {
            TestKnob t("ui.knob");
            t->setValue(0.5);
            t.call("-", LF(0.5));
            REQUIRE(t->value() == 0.);
            t.call("-", LF(0.1));
            REQUIRE(t->value() == 0.);
        }

        SECTION("++")
        {
            TestKnob t("ui.knob", LA("@min", -1, "@max", 2));
            REQUIRE(t->value() == -1);
            t.call("++");
            REQUIRE(t->value() == Approx(0.f));
            t.call("++");
            REQUIRE(t->value() == 1);
            t.call("++");
            REQUIRE(t->value() == 2);
            t.call("++");
            REQUIRE(t->value() == 2);
            t.call("++");
            REQUIRE(t->value() == 2);
        }

        SECTION("--")
        {
            TestKnob t("ui.knob", LA("@min", -1, "@max", 2));
            t->setValue(2);
            REQUIRE(t->value() == 2);
            t.call("--");
            REQUIRE(t->value() == 1);
            t.call("--");
            REQUIRE(t->value() == Approx(0.f));
            t.call("--");
            REQUIRE(t->value() == -1);
            t.call("--");
            REQUIRE(t->value() == -1);
        }

        SECTION("mul")
        {
            TestKnob t("ui.knob", LA("@min", 1, "@max", 12));
            REQUIRE(t->value() == 1);
            t.call("*", LF(2));
            REQUIRE(t->value() == 2);
            t.call("*", LF(2));
            REQUIRE(t->value() == 4);
            t.call("*", LF(2));
            REQUIRE(t->value() == 8);
            t.call("*", LF(2));
            REQUIRE(t->value() == 12);
            t.call("*", LF(2));
            REQUIRE(t->value() == 12);
        }

        SECTION("div")
        {
            TestKnob t("ui.knob", LA("@min", 12, "@max", 1));
            REQUIRE(t->value() == Approx(12));
            t.call("/", LF(2));
            REQUIRE(t->value() == Approx(6));
            t.call("/", LF(2));
            REQUIRE(t->value() == Approx(3));
            t.call("/", LF(2));
            REQUIRE(t->value() == Approx(1.5));
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
        t->setValue(20);
        t.call("store", LF(0.f));
        t->setValue(40);
        t.call("store", LF(1));
        t->setValue(60);
        t.call("store", LF(2));

        t.call("load", LF(0.f));
        REQUIRE(t->value() == Approx(20));
        t.call("load", LF(1));
        REQUIRE(t->value() == Approx(40));
        t.call("load", LF(2));
        REQUIRE(t->value() == Approx(60));
        t.call("load", LF(100));
        REQUIRE(t->value() == Approx(10));
        t.call("load");
        REQUIRE(t->value() == Approx(20));
    }

    SECTION("onMidi")
    {
        TestKnob t("ui.knob");
        t->setProperty(gensym("midi_pickup"), LF(0.f));
        t->onMidiCtrl(LF(1, 2));

        t->onMidiCtrl(LF(1, 2, 3));
        REQUIRE(t->value() == 0);

        t->setProperty(gensym("midi_control"), LF(23));
        t->onMidiCtrl(LF(1, 2, 3));
        REQUIRE(t->value() == 0);

        t->onMidiCtrl(LF(23, 64, 3));
        REQUIRE(t->value() == Approx(0.50394f));

        t->setProperty(gensym("midi_channel"), LF(2));
        t->onMidiCtrl(LF(23, 12, 3));
        REQUIRE(t->value() == Approx(0.50394f));

        t->onMidiCtrl(LF(23, 12, 2));
        REQUIRE(t->value() == Approx(0.09449f).epsilon(0.0001));

        t->onMidiCtrl(LF(1, 22, 2));
        REQUIRE(t->value() == Approx(0.09449f).epsilon(0.0001));
        t->onMidiCtrl(LF(23, 127, 2));
        REQUIRE(t->value() == Approx(1));
        t->onMidiCtrl(LF(23, 0.f, 2));
        REQUIRE(t->value() == Approx(0));

        SECTION("pickup over min->max")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));
            t2->setValue(30);
            REQUIRE(t2->value() == Approx(30));

            // not picked up
            t2->onMidiCtrl(LF(10, 2, 1));
            t2->onMidiCtrl(LF(10, 3, 1));
            t2->onMidiCtrl(LF(10, 4, 1));
            REQUIRE(t2->value() == Approx(30));

            t2->onMidiCtrl(LF(10, 29, 1));
            // not picked
            REQUIRE(t2->value() == Approx(30));
            // picked
            t2->onMidiCtrl(LF(10, 31, 1));
            REQUIRE(t2->value() == Approx(31));

            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->value() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->value() == Approx(10));
        }

        SECTION("pickup over max->min")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));
            t2->setValue(30);
            REQUIRE(t2->value() == Approx(30));

            // not picked up
            t2->onMidiCtrl(LF(10, 60, 1));
            t2->onMidiCtrl(LF(10, 61, 1));
            t2->onMidiCtrl(LF(10, 62, 1));
            REQUIRE(t2->value() == Approx(30));

            t2->onMidiCtrl(LF(10, 31, 1));
            // not picked
            REQUIRE(t2->value() == Approx(30));
            // picked
            t2->onMidiCtrl(LF(10, 29, 1));
            REQUIRE(t2->value() == Approx(29));

            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->value() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->value() == Approx(10));
        }

        SECTION("pickup over equal")
        {
            TestKnob t2("ui.knob", LA("@midi_control", 10, "@max", 127));

            // not picked up
            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->value() == Approx(0));

            t2->onMidiCtrl(LF(10, 30, 1));
            // not picked
            REQUIRE(t2->value() == Approx(0));
            // picked
            t2->onMidiCtrl(LF(10, 0.f, 1));
            REQUIRE(t2->value() == Approx(0));
            t2->onMidiCtrl(LF(10, 60, 1));
            REQUIRE(t2->value() == Approx(60));
            t2->onMidiCtrl(LF(10, 10, 1));
            REQUIRE(t2->value() == Approx(10));
        }
    }

    SECTION("send/receive test")
    {
        pd::External r1("receive", LA("r1"));
        LogExternalOutput out;
        REQUIRE(r1.connectTo(0, out.object(), 0));

        TestKnob t("ui.knob", LA("@send", "r1", "@receive", "s1"));
        t->onFloat(0.5);
        REQUIRE(out.msg().atomValue().asFloat() == 0.5);
        out.reset();

        t->onBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.5);

        // test receive
        pd::External s1("send", LA("s1"));
        pd::External s2("send", LA("s2"));

        out.reset();
        s1.sendFloat(0.375);
        REQUIRE(out.msg().atomValue().asFloat() == 0.375);

        // change receiver
        t->setProperty(gensym("receive"), LA("s2"));

        // do not react on old
        s1.sendFloat(0.9);
        REQUIRE(out.msg().atomValue().asFloat() == 0.375);

        // check new one
        s2.sendFloat(0.875);
        REQUIRE(out.msg().atomValue().asFloat() == 0.875);
    }

    SECTION("output")
    {
        pd::External knob("ui.knob");
        LogExternalOutput out;
        REQUIRE(knob.connectTo(0, out.object(), 0));

        knob.sendBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.f);

        knob.sendFloat(0.25);
        REQUIRE(out.msg().atomValue().asFloat() == 0.25);

        out.reset();
        knob.sendBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.25);

        // list auto convert test
        out.reset();
        knob.sendList(LF(0.125, 0.25));
        REQUIRE(out.msg().atomValue().asFloat() == 0.125);

        out.reset();
        knob.sendMessage(gensym("@value"), LF(0.5));
        REQUIRE(out.msg().isNone());

        knob.sendBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.5);

        knob.sendMessage(gensym("@value?"));
        REQUIRE(out.msg().listValue() == LF(0.5));

        knob.sendMessage(gensym("@max?"));
        REQUIRE(out.msg().listValue() == LF(1));

        knob.sendMessage(gensym("@send?"));
        REQUIRE(out.msg().listValue() == LA("(null)"));

        knob.sendMessage(gensym("@size?"));
        REQUIRE(out.msg().listValue() == LF(40, 40));

        out.reset();
        knob.sendMessage(gensym("set"), LF(0.125));
        REQUIRE(out.msg().isNone());
        knob.sendBang();
        REQUIRE(out.msg().atomValue().asFloat() == 0.125);
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
            + LA("@midi_pickup", "@min", "@mouse_events", "@pinned", "@presetname", "@receive",
                "@scale", "@scale_color", "@send", "@show_range", "@show_value", "@size", "@value");
        REQUIRE(props == desired);

        t.call("@max?", LA("@min?", "@xxx?", "", "@non", "unknown", 100, "@receive?"));
        REQUIRE(t.outputAnyAt(0) == LA("@max", 1, "@min", 0.f, "@receive", "(null)"));
    }

    SECTION("prop +")
    {
        TestExtKnob t("ui.knob");
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);

        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(40, 40));
        t.call("@size", LA("+", 5));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(45, 45));
        t.call("@size", LA("+", 0.0));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(45, 45));
        t.call("@size", LA("-", 15));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));
        t.call("@size", LA("-", 0.0));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));
        t.call("@size", LA("*", 1));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));
        t.call("@size", LA("*", 2));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(60, 60));
        t.call("@size", LA("*", 2.5));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(150, 150));
        t.call("@size", LA("/", 5));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));
        t.call("@size", LA("/", 1));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));

        // invalid
        t.call("@size", LA("/", 0.0));
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(30, 30));
        t.call("@size", LA("/", -1)); // set to minimal size
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(20, 20));
        t.call("@size", LA("*", -1)); // set to minimal size
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(20, 20));
        t.call("@size", LA("+", -100)); // set to minimal size
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(20, 20));
        t.call("@size", LA("-", 100)); // set to minimal size
        REQUIRE_UI_LIST_PROPERTY(t, "size", LA(20, 20));
    }

    SECTION("prop math")
    {
        TestExtKnob t("ui.knob");
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 0);

        t.call("@midi_channel", LA(2));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 2);

        t.call("@midi_channel", LA("+", 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 5);

        t.call("@midi_channel", LA("+", 32));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 16);

        t.call("@midi_channel", LA("-", 4));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 12);

        t.call("@midi_channel", LA("-", 44));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 12);

        t.call("@midi_channel", LA("/", 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 4);

        t.call("@midi_channel", LA("/", 0.));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 4);

        t.call("@midi_channel", LA("*", 2));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 8);

        t.call("@midi_channel", LA("*", 200));
        REQUIRE_UI_FLOAT_PROPERTY(t, "midi_channel", 16);
    }

    SECTION("prop math @max")
    {
        TestExtKnob t("ui.knob");
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);

        t.call("@max", LA(2));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 2);

        t.call("@max", LA("+", 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5);

        t.call("@max", LA("+", 32));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 37);

        t.call("@max", LA("-", 4));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", 33);

        t.call("@max", LA("-", 45));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", -12);

        t.call("@max", LA("/", 3));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", -4);

        t.call("@max", LA("/", 0.));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", -4);

        t.call("@max", LA("*", 2));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", -8);

        t.call("@max", LA("*", 20));
        REQUIRE_UI_FLOAT_PROPERTY(t, "max", -160);
    }

    SECTION("prop @border_color")
    {
        TestExtKnob t("ui.knob");

        t.call("@border_color", LF(0.125, 0.25, 0.5, 1));
        REQUIRE_UI_LIST_PROPERTY(t, "border_color", LF(0.125, 0.25, 0.5, 1));

        t.call("@border_color", LF(0.75, 0.5, 0.25));
        REQUIRE_UI_LIST_PROPERTY(t, "border_color", LF(0.75, 0.5, 0.25, 1));
    }

    SECTION("pos args")
    {
        SECTION("min")
        {
            TestExtKnob t("ui.knob", LF(-10));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -10);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 1);
        }

        SECTION("minmax")
        {
            TestExtKnob t("ui.knob", LF(-10, 5.5));
            REQUIRE_UI_FLOAT_PROPERTY(t, "min", -10);
            REQUIRE_UI_FLOAT_PROPERTY(t, "max", 5.5);
        }
    }

    SECTION("random")
    {
        TestExtKnob t("ui.knob");

        auto v = t->value();

        t.call("random");
        REQUIRE(v != t->value());
        REQUIRE(t.outputFloatAt(0) == t->value());

        v = t->value();

        t.clearAll();
        t.call("set", LA("random"));
        REQUIRE(v != t->value());
        REQUIRE_FALSE(t.hasOutput());
    }
}
