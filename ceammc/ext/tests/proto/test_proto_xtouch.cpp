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
        DisplayData d;

        d.setAlign(d.ALIGN_CENTER);
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

        d.setAlign(d.ALIGN_LEFT);
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

        d.setAlign(d.ALIGN_RIGHT);
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

        d.setAlign(d.ALIGN_JUSTIFY);
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
}
