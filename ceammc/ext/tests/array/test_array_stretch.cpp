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
#include "array_stretch.h"
#include "test_array_base.h"

PD_COMPLETE_TEST_SETUP(ArrayStretch, array, stretch)

TEST_CASE("array.stretch", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("create")
    {
        ArrayPtr aptr0 = cnv->createArray("array0", 10);
        ArrayPtr aptr1 = cnv->createArray("array_stretch1", 10);

        SECTION("empty")
        {
            TObj t("array.stretch");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @src, "");
            REQUIRE_PROPERTY(t, @dest, "");
            REQUIRE_PROPERTY(t, @pitch, 0.f);
            REQUIRE_PROPERTY(t, @tempo, 0.f);
            REQUIRE_PROPERTY(t, @rate, 1);
            REQUIRE_PROPERTY(t, @sequence, 0.f);
            REQUIRE_PROPERTY(t, @seekwindow, 0.f);
            REQUIRE_PROPERTY(t, @overlap, 8);
            REQUIRE_PROPERTY(t, @antialias, 1);
            REQUIRE_PROPERTY(t, @aalength, 64);
            REQUIRE_PROPERTY(t, @speech, 0.f);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("@speech")
        {
            TObj t("array.stretch", LA("@speech", 1));
            REQUIRE_PROPERTY(t, @sequence, 40);
            REQUIRE_PROPERTY(t, @seekwindow, 15);
            REQUIRE_PROPERTY(t, @overlap, 8);
            REQUIRE_PROPERTY(t, @speech, 1);
        }

        SECTION("errors")
        {
            SECTION("invalid source")
            {
                TObj t("array.stretch", LA("non-exists"));
                REQUIRE_PROPERTY(t, @src, "non-exists");
                REQUIRE_PROPERTY(t, @dest, "");

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }

            SECTION("invalid dest")
            {
                TObj t("array.stretch", LA("array0", "non-exists"));
                REQUIRE_PROPERTY(t, @src, "array0");
                REQUIRE_PROPERTY(t, @dest, "non-exists");

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }

            SECTION("invalid prop source")
            {
                TObj t("array.stretch", LA("@src", "non-exists"));
                REQUIRE_PROPERTY(t, @src, "non-exists");
                REQUIRE_PROPERTY(t, @dest, "");

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }

            SECTION("invalid dest")
            {
                TObj t("array.stretch", LA("@dest", "non-exists"));
                REQUIRE_PROPERTY(t, @src, "");
                REQUIRE_PROPERTY(t, @dest, "non-exists");

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }
        }
    }

    SECTION("props")
    {
        SECTION("@sequence")
        {
            TObj t("array.stretch");
            t.setProperty("@sequence", A(0.f));
            REQUIRE_PROPERTY(t, @sequence, 0.f);
            t.setProperty("@sequence", A(99));
            REQUIRE_PROPERTY(t, @sequence, 99);
            t.setProperty("@sequence", A(100));
            REQUIRE_PROPERTY(t, @sequence, 100);
            // clip
            t.setProperty("@sequence", A(101));
            REQUIRE_PROPERTY(t, @sequence, 100);
            t.setProperty("@sequence", A(-1));
            REQUIRE_PROPERTY(t, @sequence, 100.f);
        }

        SECTION("@seekwindow")
        {
            TObj t("array.stretch");
            t.setProperty("@seekwindow", A(0.f));
            REQUIRE_PROPERTY(t, @seekwindow, 0.f);
            t.setProperty("@seekwindow", A(99));
            REQUIRE_PROPERTY(t, @seekwindow, 99);
            t.setProperty("@seekwindow", A(100));
            REQUIRE_PROPERTY(t, @seekwindow, 100);
            // clip
            t.setProperty("@seekwindow", A(101));
            REQUIRE_PROPERTY(t, @seekwindow, 100);
            t.setProperty("@seekwindow", A(-1));
            REQUIRE_PROPERTY(t, @seekwindow, 100.f);
        }

        SECTION("@overlap")
        {
            TObj t("array.stretch");
            t.setProperty("@overlap", A(1));
            REQUIRE_PROPERTY(t, @overlap, 1);
            t.setProperty("@overlap", A(100));
            REQUIRE_PROPERTY(t, @overlap, 100);
            // clip
            t.setProperty("@overlap", A(0.f));
            REQUIRE_PROPERTY(t, @overlap, 100);
            t.setProperty("@overlap", A(101));
            REQUIRE_PROPERTY(t, @overlap, 100);
        }

        SECTION("@aalength")
        {
            TObj t("array.stretch");
            t.setProperty("@aalength", A(8));
            REQUIRE_PROPERTY(t, @aalength, 8);
            t.setProperty("@aalength", A(128));
            REQUIRE_PROPERTY(t, @aalength, 128);
            // clip
            t.setProperty("@aalength", A(7));
            REQUIRE_PROPERTY(t, @aalength, 128);
            t.setProperty("@aalength", A(129));
            REQUIRE_PROPERTY(t, @aalength, 128);
        }
    }

    SECTION("array_stretch1")
    {
        TObj t("array.bpm", LA("array0", "array_stretch1"));
        ArrayPtr aptr0 = cnv->createArray("array0", 10);
        ArrayPtr aptr1 = cnv->createArray("array_stretch1", 10);

        // array created
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        // crash check on small buffer sizes
        REQUIRE(aptr0->resize(1023));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        REQUIRE(aptr0->resize(1024));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        aptr0->resize(1025);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        // crash check on small buffer sizes
        REQUIRE(aptr0->resize(4095));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        REQUIRE(aptr0->resize(4096));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);

        REQUIRE(aptr0->resize(4097));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, t);
    }
}
