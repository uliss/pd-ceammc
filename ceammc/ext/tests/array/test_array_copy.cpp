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
#include "array_copy.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayCopy, array, copy)

#define RESET_DATA() \
    b.resize(3);     \
    a[0] = 1;        \
    a[1] = 2;        \
    a[2] = 3;        \
    a[3] = 4;        \
    a[4] = 5;        \
    b[0] = 20;       \
    b[1] = 19;       \
    b[2] = 18;

TEST_CASE("array.copy", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("empty")
    {
        TObj t("array.copy");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @resize, 0.f);
    }

    SECTION("copy simple")
    {
        TObj t("array.copy");
        cnv->createArray("a", 5);
        cnv->createArray("b", 3);

        Array a("a");
        Array b("b");

        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        a[4] = 5;

        b[0] = 20;
        b[1] = 19;
        b[2] = 18;

        WHEN_CALL_N(t, copy, "a", "b");
        REQUIRE_BANG_AT_OUTLET(0, t);

        REQUIRE(a[0] == 1.f);
        REQUIRE(a[1] == 2.f);
        REQUIRE(a[2] == 3.f);
        REQUIRE(a[3] == 4.f);
        REQUIRE(a[4] == 5.f);

        REQUIRE(b[0] == 1.f);
        REQUIRE(b[1] == 2.f);
        REQUIRE(b[2] == 3.f);
        REQUIRE(b.size() == 3);

        SECTION("invalid source")
        {
            WHEN_CALL_N(t, copy, "invalid", "b");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid destination")
        {
            WHEN_CALL_N(t, copy, "a", "invalid");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid both")
        {
            WHEN_CALL_N(t, copy, "invalid1", "invalid2");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }
    }

    SECTION("copy simple resize")
    {
        TObj t("array.copy");
        t.setProperty("@resize", LF(1));
        REQUIRE_PROPERTY(t, @resize, 1.f);

        Array a("a");
        Array b("b");

        REQUIRE(a.size() == 5);
        REQUIRE(b.size() == 3);

        WHEN_CALL_N(t, copy, "a", "b");
        REQUIRE_BANG_AT_OUTLET(0, t);

        b.update();
        REQUIRE(a.size() == 5);
        REQUIRE(b.size() == 5);

        REQUIRE(b[0] == 1.f);
        REQUIRE(b[1] == 2.f);
        REQUIRE(b[2] == 3.f);
        REQUIRE(b[3] == 4.f);
        REQUIRE(b[4] == 5.f);
    }

    SECTION("range copy 4 args")
    {
        SECTION("invalid arrays")
        {
            TObj t("array.copy");
            WHEN_CALL_N(t, copy, "a", 0.f, 4, "???");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "???", 0.f, 4, "???");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "???", 0.f, 4, "b");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid range")
        {
            TObj t("array.copy");
            WHEN_CALL_N(t, copy, "a", 5, 6, "b");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "a", 3, 2, "b");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("no resize")
        {
            TObj t("array.copy");
            Array a("a");
            Array b("b");

            RESET_DATA();

            WHEN_CALL_N(t, copy, "a", 0.f, 4, "b");
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 1.f);
            REQUIRE(b[1] == 2.f);
            REQUIRE(b[2] == 3.f);

            WHEN_CALL_N(t, copy, "a", 2, 4, "b");
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 3.f); // changed
            REQUIRE(b[1] == 4.f);
            REQUIRE(b[2] == 3.f); // same

            RESET_DATA();
            WHEN_CALL_N(t, copy, "a", 2, 16, "b");
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 3.f); // changed
            REQUIRE(b[1] == 4.f);
            REQUIRE(b[2] == 5.f);
        }

        SECTION("resize")
        {
            TObj t("array.copy", LA("@resize", 1));
            Array a("a");
            Array b("b");

            RESET_DATA();

            WHEN_CALL_N(t, copy, "a", 0.f, 10, "b");
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 5);
            REQUIRE(b[0] == 1.f);
            REQUIRE(b[1] == 2.f);
            REQUIRE(b[2] == 3.f);
            REQUIRE(b[3] == 4.f);
            REQUIRE(b[4] == 5.f);

            RESET_DATA();
        }
    }

    SECTION("range copy 5 args")
    {
        SECTION("invalid arrays")
        {
            TObj t("array.copy");
            WHEN_CALL_N(t, copy, "a", 0.f, 4, "???", 1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "???", 0.f, 4, "???", 1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "???", 0.f, 4, "b", 1);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid range")
        {
            TObj t("array.copy");

            // wrong src
            WHEN_CALL_N(t, copy, "a", 5, 6, "b", 2);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // negative range
            WHEN_CALL_N(t, copy, "a", 3, 2, "b", 2);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            // wrong dest
            WHEN_CALL_N(t, copy, "a", 1, 5, "b", 3);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("no resize")
        {
            TObj t("array.copy");
            Array a("a");
            Array b("b");

            RESET_DATA();

            WHEN_CALL_N(t, copy, "a", 0.f, 4, "b", 1);
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 20.f);
            REQUIRE(b[1] == 1.f);
            REQUIRE(b[2] == 2.f);

            RESET_DATA();
            WHEN_CALL_N(t, copy, "a", 0.f, 16, "b", 2);
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 20.f);
            REQUIRE(b[1] == 19.f);
            REQUIRE(b[2] == 1.f);

            RESET_DATA();
            WHEN_CALL_N(t, copy, "a", 4, 16, "b", 1);
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 20.f);
            REQUIRE(b[1] == 5.f);
            REQUIRE(b[2] == 18.f);

            RESET_DATA();
            WHEN_CALL_N(t, copy, "a", 0.f, 5, "b", 0.f);
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 3);
            REQUIRE(b[0] == 1.f);
            REQUIRE(b[1] == 2.f);
            REQUIRE(b[2] == 3.f);
        }

        SECTION("resize")
        {
            TObj t("array.copy", LA("@resize", 1));
            Array a("a");
            Array b("b");

            RESET_DATA();

            WHEN_CALL_N(t, copy, "a", 0.f, 4, "b", 4);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_CALL_N(t, copy, "a", 0.f, 3, "b", 3);
            REQUIRE_BANG_AT_OUTLET(0, t);

            b.update();
            REQUIRE(b.size() == 6);
            REQUIRE(b[0] == 20.f);
            REQUIRE(b[1] == 19.f);
            REQUIRE(b[2] == 18.f);
            REQUIRE(b[3] == 1.f);
            REQUIRE(b[4] == 2.f);
            REQUIRE(b[5] == 3.f);
        }
    }

    SECTION("range copy 3 args")
    {
        TObj t("array.copy");
        Array a("a");
        Array b("b");

        RESET_DATA();

        WHEN_CALL_N(t, copy, "a", 3, "b");
        b.update();
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 4.f);
        REQUIRE(b[1] == 5.f);
        REQUIRE(b[2] == 18.f);

        RESET_DATA();

        WHEN_CALL_N(t, copy, "a", 1, "b");
        b.update();
        REQUIRE(b.size() == 3);
        REQUIRE(b[0] == 2.f);
        REQUIRE(b[1] == 3.f);
        REQUIRE(b[2] == 4.f);
    }
}
