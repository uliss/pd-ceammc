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
#include "../array/array_vline_play.h"
#include "base_extension_test.h"
#include "ceammc_factory.h"
#include "ceammc_pd.h"

#include "catch.hpp"

typedef TestExtension<ArrayVlinePlay> ArrayVPlayTest;

using namespace ceammc;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("array.vplay", "[externals]")
{
    printPdToStdError(true);
    setTestSampleRate(100);

    SECTION("empty")
    {
        ArrayVPlayTest t("array.vplay");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_NONE(t, @array);
        REQUIRE_PROPERTY_FLOAT(t, @speed, 1);
        REQUIRE_PROPERTY(t, @state, "stop");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, -1);
        REQUIRE_PROPERTY_FLOAT(t, @abs_begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @abs_end, 0);

        SECTION("empty")
        {
            WHEN_CALL(t, play);
            REQUIRE_NO_MSG(t);

            WHEN_CALL_2(t, range, 10, "ms");
            REQUIRE_NO_MSG(t);
        }
    }

    SECTION("invalid")
    {
        ArrayVPlayTest t("array.vplay", L1("non-exists"));
        REQUIRE_PROPERTY(t, @array, "non-exists");

        // float
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        // methods
        WHEN_CALL(t, play);
        REQUIRE_NO_MSG(t);
        WHEN_CALL(t, stop);
        REQUIRE_NO_MSG(t);
    }

    SECTION("array1")
    {
#define REQUIRE_VLINE_MSG(t, v0, v1, dur)                              \
    {                                                                  \
        t.cleanAllMessages();                                          \
        WHEN_CALL(t, play);                                            \
        REQUIRE(t.hasNewMessages(0));                                  \
        REQUIRE(t.messageAt(0, 0).atomValue().asFloat() == v0);        \
        REQUIRE(t.messageAt(1, 0).listValue() == ListApprox(v1, dur)); \
        WHEN_CALL(t, stop);                                            \
    }

        ArrayVPlayTest t("array.vplay", L1("array1"));

        // no array yet
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, -1);
        REQUIRE_PROPERTY_FLOAT(t, @abs_begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @abs_end, 0);

        ArrayPtr aptr = cnv->createArray("array1", 100);

        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, -1);
        REQUIRE_PROPERTY_FLOAT(t, @abs_begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @abs_end, 99);

        WHEN_CALL(t, play);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.messageAt(0, 0).atomValue().asFloat() == 0);
        REQUIRE(t.messageAt(1, 0).listValue() == L2(99, 1000));
        REQUIRE_PROPERTY(t, @state, "play");

        // already playing
        WHEN_CALL(t, play);
        REQUIRE_NO_MSG(t);

        // stop
        WHEN_CALL(t, stop);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        // already stopped
        WHEN_CALL(t, stop);
        REQUIRE_NO_MSG(t);

        // different speed
        t.setProperty("@speed", L1(2));
        REQUIRE_VLINE_MSG(t, 0, 99, 500);
        REQUIRE_PROPERTY(t, @speed, 2);
        t.setProperty("@speed", L1(0.001));
        REQUIRE_PROPERTY(t, @speed, 2);

        // reverse playback
        t.setProperty("@begin", L1(-1));
        t.setProperty("@end", L1(0.0));
        REQUIRE_VLINE_MSG(t, 99, 0, 500);

        t.setProperty("@begin", L1(-51));
        REQUIRE_VLINE_MSG(t, 49, 0, 250);

        t.setProperty("@begin", L1(49));
        REQUIRE_VLINE_MSG(t, 49, 0, 250);

        t.setProperty("@speed", L1(1));
        t.setProperty("@begin", L1(0.0));
        t.setProperty("@end", L1(49));
        REQUIRE_VLINE_MSG(t, 0, 49, 500);

        t.setProperty("@begin", L1(25));
        t.setProperty("@end", L1(49));
        REQUIRE_VLINE_MSG(t, 25, 49, 250);

        t.setProperty("@begin", L1(49));
        t.setProperty("@end", L1(25));
        REQUIRE_VLINE_MSG(t, 49, 25, 250);

        t.setProperty("@begin", L1(0.0));
        t.setProperty("@end", L1(9));
        REQUIRE_VLINE_MSG(t, 0, 9, 100);

        t.setProperty("@begin", L1(0.0));
        t.setProperty("@end", L1(1));
        REQUIRE_VLINE_MSG(t, 0, 1, 20);

        t.setProperty("@begin", L1(20));
        t.setProperty("@end", L1(20));
        REQUIRE_VLINE_MSG(t, 20, 20, 10);

        t.setProperty("@begin", L1(-1));
        t.setProperty("@end", L1(-2));
        REQUIRE_VLINE_MSG(t, 99, 98, 20);
    }

    SECTION("range")
    {
#define REQUIRE_PLAY_VLINE_MSG(t, v0, v1, dur)                         \
    {                                                                  \
        t.cleanAllMessages();                                          \
        WHEN_CALL(t, play);                                            \
        REQUIRE(t.hasNewMessages(0));                                  \
        REQUIRE(t.messageAt(0, 0).atomValue().asFloat() == v0);        \
        REQUIRE(t.messageAt(1, 0).listValue() == ListApprox(v1, dur)); \
        WHEN_CALL(t, stop);                                            \
    }

        ArrayVPlayTest t("array.vplay", L1("array2"));
        REQUIRE_PROPERTY(t, @array, "array2");
        ArrayPtr aptr = cnv->createArray("array2", 100);

        WHEN_CALL_4(t, range, 0.0, "sample", 99, "sample");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);
        REQUIRE_NO_MSG(t);
        REQUIRE_PLAY_VLINE_MSG(t, 0, 99, 1000);

        WHEN_CALL_4(t, range, 0.0, "sample", -1, "sample");
        REQUIRE_PLAY_VLINE_MSG(t, 0, 99, 1000);

        WHEN_CALL_4(t, range, -1, "sample", 0.0, "sample");
        REQUIRE_PLAY_VLINE_MSG(t, 99, 0, 1000);

        WHEN_CALL_4(t, range, -2, "sample", -3, "sample");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 98);
        REQUIRE_PROPERTY_FLOAT(t, @end, 97);

        WHEN_CALL_4(t, range, 0.0, "ms", 1000, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "ms", 1001, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "ms", 500, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "ms", -1, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "ms", -2, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "ms", -1000, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 0);

        WHEN_CALL_4(t, range, 0.0, "ms", -2000, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 0);

        WHEN_CALL_4(t, range, 0.0, "ms", -990, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 1);

        WHEN_CALL_4(t, range, 0.0, "ms", -10, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "ms", -20, "ms");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 98);

        WHEN_CALL_4(t, range, 0.0, "sec", 1, "sec");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "sec", 2, "sec");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "sec", 0.5, "sec");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "sec", -0.01, "sec");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "sec", -0.5, "sec");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "sec", 1, "phase");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_4(t, range, 0.0, "sec", 0.0, "phase");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 0);

        WHEN_CALL_4(t, range, 0.0, "sec", 0.5, "phase");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "sec", -0.1, "phase");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "sec", 1.1, "phase");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 50);

        WHEN_CALL_4(t, range, 0.0, "sec", -1, "sample");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        // typo
        WHEN_CALL_4(t, range, 0.0, "sec", 20, "samples");
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);

        WHEN_CALL_3(t, range, 0.0, "sec", 20);
        REQUIRE_PROPERTY_FLOAT(t, @begin, 0);
        REQUIRE_PROPERTY_FLOAT(t, @end, 99);
    }
}
