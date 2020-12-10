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
#include "proto_xtouch_ext.h"
#include "test_proto_base.h"

PD_COMPLETE_TEST_SETUP(XTouchExtender, proto, xtouch_ext)

TEST_CASE("proto.xtouch_ext", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        SECTION("default")
        {
            TExt t("proto.xtouch_ext");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("display")
    {
        Display d;

        d.setUpperAlign(d.ALIGN_CENTER);
        d.setUpperText("ABC");
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == 'B');
        REQUIRE(d.upperCharAt(2) == 'C');
        REQUIRE(d.upperCharAt(3) == '\0');
        REQUIRE(d.upperCharAt(4) == '\0');
        REQUIRE(d.upperCharAt(5) == '\0');
        REQUIRE(d.upperCharAt(6) == '\0');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("ABCDEFGHIJ");
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == 'B');
        REQUIRE(d.upperCharAt(2) == 'C');
        REQUIRE(d.upperCharAt(3) == 'D');
        REQUIRE(d.upperCharAt(4) == 'E');
        REQUIRE(d.upperCharAt(5) == 'F');
        REQUIRE(d.upperCharAt(6) == 'G');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperAlign(d.ALIGN_LEFT);
        d.setUpperText("ABC");
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == 'B');
        REQUIRE(d.upperCharAt(2) == 'C');
        REQUIRE(d.upperCharAt(3) == ' ');
        REQUIRE(d.upperCharAt(4) == ' ');
        REQUIRE(d.upperCharAt(5) == ' ');
        REQUIRE(d.upperCharAt(6) == ' ');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("ABCDEFGHIJ");
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == 'B');
        REQUIRE(d.upperCharAt(2) == 'C');
        REQUIRE(d.upperCharAt(3) == 'D');
        REQUIRE(d.upperCharAt(4) == 'E');
        REQUIRE(d.upperCharAt(5) == 'F');
        REQUIRE(d.upperCharAt(6) == 'G');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperAlign(d.ALIGN_RIGHT);
        d.setUpperText("ABC");
        REQUIRE(d.upperCharAt(0) == ' ');
        REQUIRE(d.upperCharAt(1) == ' ');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == ' ');
        REQUIRE(d.upperCharAt(4) == 'A');
        REQUIRE(d.upperCharAt(5) == 'B');
        REQUIRE(d.upperCharAt(6) == 'C');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("ABCDEFGHIJ");
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == 'B');
        REQUIRE(d.upperCharAt(2) == 'C');
        REQUIRE(d.upperCharAt(3) == 'D');
        REQUIRE(d.upperCharAt(4) == 'E');
        REQUIRE(d.upperCharAt(5) == 'F');
        REQUIRE(d.upperCharAt(6) == 'G');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperAlign(d.ALIGN_JUSTIFY);
        d.setUpperText("n: 1%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == ' ');
        REQUIRE(d.upperCharAt(4) == ' ');
        REQUIRE(d.upperCharAt(5) == '1');
        REQUIRE(d.upperCharAt(6) == '%');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("n: 12%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == ' ');
        REQUIRE(d.upperCharAt(4) == '1');
        REQUIRE(d.upperCharAt(5) == '2');
        REQUIRE(d.upperCharAt(6) == '%');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("n: 123%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == '1');
        REQUIRE(d.upperCharAt(4) == '2');
        REQUIRE(d.upperCharAt(5) == '3');
        REQUIRE(d.upperCharAt(6) == '%');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("n: 123%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == '1');
        REQUIRE(d.upperCharAt(4) == '2');
        REQUIRE(d.upperCharAt(5) == '3');
        REQUIRE(d.upperCharAt(6) == '%');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("n: 1234%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == '1');
        REQUIRE(d.upperCharAt(3) == '2');
        REQUIRE(d.upperCharAt(4) == '3');
        REQUIRE(d.upperCharAt(5) == '4');
        REQUIRE(d.upperCharAt(6) == '%');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText("n: 12345%");
        REQUIRE(d.upperCharAt(0) == 'n');
        REQUIRE(d.upperCharAt(1) == ':');
        REQUIRE(d.upperCharAt(2) == '1');
        REQUIRE(d.upperCharAt(3) == '2');
        REQUIRE(d.upperCharAt(4) == '3');
        REQUIRE(d.upperCharAt(5) == '4');
        REQUIRE(d.upperCharAt(6) == '5');
        REQUIRE(d.upperCharAt(7) == '\0');
    }

    SECTION("setUpperText")
    {
        Display d;

        d.setUpperText(LA("A", "B"));
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == ' ');
        REQUIRE(d.upperCharAt(2) == ' ');
        REQUIRE(d.upperCharAt(3) == ' ');
        REQUIRE(d.upperCharAt(4) == ' ');
        REQUIRE(d.upperCharAt(5) == ' ');
        REQUIRE(d.upperCharAt(6) == 'B');
        REQUIRE(d.upperCharAt(7) == '\0');

        d.setUpperText(LA("A B"));
        REQUIRE(d.upperCharAt(0) == 'A');
        REQUIRE(d.upperCharAt(1) == ' ');
        REQUIRE(d.upperCharAt(2) == 'B');
        REQUIRE(d.upperCharAt(3) == '\0');
        REQUIRE(d.upperCharAt(4) == '\0');
        REQUIRE(d.upperCharAt(5) == '\0');
        REQUIRE(d.upperCharAt(6) == '\0');
        REQUIRE(d.upperCharAt(7) == '\0');
    }

    SECTION("align")
    {
        TExt t("proto.xtouch_ext");

        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_AUTO);

        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_AUTO);

        t.call("lcd_align");
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_AUTO);

        t.call("lcd_align", LA("all"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_AUTO);

        t.call("lcd_align", LA("all", "left"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_LEFT);

        t.call("lcd_align", LA("scene", "right"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(8).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(8).lowerAlign() == Display::ALIGN_LEFT);

        t.call("lcd_align", LA(2, "auto"));
        REQUIRE(t->display(1).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(1).lowerAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(2).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(2).lowerAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(3).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(3).lowerAlign() == Display::ALIGN_RIGHT);

        t.call("lcd_align", LA(0., "left", "right", "center", "left", "right", "center", "left", "right", "auto"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(1).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(2).upperAlign() == Display::ALIGN_CENTER);
        REQUIRE(t->display(3).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(4).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(5).upperAlign() == Display::ALIGN_CENTER);
        REQUIRE(t->display(6).upperAlign() == Display::ALIGN_LEFT);

        t.call("lcd_align", LA(4, "auto", "auto", "auto", "auto"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(1).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(2).upperAlign() == Display::ALIGN_CENTER);
        REQUIRE(t->display(3).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(4).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(5).upperAlign() == Display::ALIGN_AUTO);
        REQUIRE(t->display(6).upperAlign() == Display::ALIGN_AUTO);

        t.call("lcd_align", LA("all", "left"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_LEFT);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_LEFT);

        t.call("lcd_align0", LA("all", "right"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_LEFT);

        t.call("lcd_align1", LA("all", "auto"));
        REQUIRE(t->display(0).upperAlign() == Display::ALIGN_RIGHT);
        REQUIRE(t->display(0).lowerAlign() == Display::ALIGN_AUTO);
    }

    SECTION("colors")
    {
        TExt t("proto.xtouch_ext");

        REQUIRE(t->display(0).color() == Display::CYAN);
        REQUIRE(t->display(1).color() == Display::CYAN);
        REQUIRE(t->display(3).color() == Display::CYAN);

        t.call("lcd_color", "all", "red");
        REQUIRE(t->display(0).color() == Display::RED);
        REQUIRE(t->display(1).color() == Display::RED);
        REQUIRE(t->display(3).color() == Display::RED);

        t.call("lcd_color", "all", 2);
        REQUIRE(t->display(0).color() == Display::GREEN);
        REQUIRE(t->display(1).color() == Display::GREEN);
        REQUIRE(t->display(3).color() == Display::GREEN);

        t.call("lcd_color", 1, "black");
        REQUIRE(t->display(0).color() == Display::GREEN);
        REQUIRE(t->display(1).color() == Display::BLACK);
        REQUIRE(t->display(3).color() == Display::GREEN);

        t.call("lcd_color", 9, "blue");
        REQUIRE(t->display(8).color() == Display::GREEN);
        REQUIRE(t->display(9).color() == Display::BLUE);
        REQUIRE(t->display(10).color() == Display::GREEN);
    }

    SECTION("enum")
    {
        TExt t("proto.xtouch_ext");

        t.call("lcd_enum0");
        REQUIRE(t->display(0).upperText() == "0");
        REQUIRE(t->display(1).upperText() == "1");

        t.call("lcd_enum1", "ch:");
        REQUIRE(t->display(0).upperText() == "0");
        REQUIRE(t->display(1).upperText() == "1");
        REQUIRE(t->display(0).lowerText() == "ch:0");
        REQUIRE(t->display(1).lowerText() == "ch:1");
    }

    SECTION("rec")
    {
        SECTION("toggle")
        {
            TExt t("proto.xtouch_ext");

            t.call("rec", LF(0, 1, 1, 0, 0));
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
            REQUIRE(t->sceneByIdx(0).rec(1).state() == 1);
            REQUIRE(t->sceneByIdx(0).rec(2).state() == 0);
            REQUIRE(t->sceneByIdx(0).rec(2).state() == 0);

            t.call("rec", LA(0., "!", -1, "~", "~"));
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
            REQUIRE(t->sceneByIdx(0).rec(1).state() == -1);
            REQUIRE(t->sceneByIdx(0).rec(2).state() == 1);
            REQUIRE(t->sceneByIdx(0).rec(2).state() == 1);

            t.call("rec", LA(1, "!"));
            REQUIRE(t->sceneByIdx(0).rec(1).state() == 1);
        }

        SECTION("midi")
        {
            TExt t("proto.xtouch_ext");
            t->sceneByIdx(0).rec(0).setMidi(127);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
            t->sceneByIdx(0).rec(0).setMidi(0);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
            t->sceneByIdx(0).rec(0).setMidi(127);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
            t->sceneByIdx(0).rec(0).setMidi(0);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
            t->sceneByIdx(0).rec(0).setMidi(127);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);

            t->sceneByIdx(0).rec(0).setMode(Button::BUTTON);
            t->sceneByIdx(0).rec(0).setMidi(127);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
            t->sceneByIdx(0).rec(0).setMidi(0);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
            t->sceneByIdx(0).rec(0).setMidi(127);
            REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
        }
    }

    SECTION("any")
    {
        TExt t("proto.xtouch_ext");

        t.sendMessage("rec");
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
        t.sendMessage("rec0");
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
        t.sendMessage("rec0", LA(1, 2, 3));
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
        t.sendMessage("rec0", LF(1));
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
        t.sendMessage("rec0", LF(1));
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 1);
        t.sendMessage("rec0", LF(0));
        REQUIRE(t->sceneByIdx(0).rec(0).state() == 0);
        t.sendMessage("rec0", LF(-1));
        REQUIRE(t->sceneByIdx(0).rec(0).state() == -1);

        t.sendMessage("solo11", LF(-1));
        REQUIRE(t->sceneByIdx(1).solo(3).state() == -1);

        t.sendMessage("mute8", LF(-1));
        REQUIRE(t->sceneByIdx(1).mute(0).state() == -1);

        t.sendMessage("select5", LF(-1));
        REQUIRE(t->sceneByIdx(0).select(5).state() == -1);

        t.sendMessage("fader0", LF(0.5));
        REQUIRE(t->sceneByIdx(0).fader(0).value() == 0.5);
        t.sendMessage("knob1", LF(0.75));
        REQUIRE(t->sceneByIdx(0).knob(1).value() == 0.75);
    }
}
