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
#include "../env/env_env.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<Envelope> EnvelopeTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

#define REQUIRE_ENV_OUTPUT(t, env)                                   \
    {                                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                            \
        const DataTypeEnv* env0 = t.typedLastDataAt<DataTypeEnv>(0); \
        REQUIRE(env0 != 0);                                          \
        REQUIRE(*env0 == env);                                       \
        t.cleanAllMessages();                                        \
    }

TEST_CASE("env.env", "[externals]")
{
    SECTION("init")
    {
        setup_envelope();

        SECTION("empty")
        {
            EnvelopeTest t("env");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @npoints, 0.f);
            REQUIRE_PROPERTY(t, @length, 0.f);
            REQUIRE_PROPERTY_LIST(t, @values, AtomList());
            REQUIRE_PROPERTY_LIST(t, @points, AtomList());
        }

        SECTION("adsr")
        {
            EnvelopeTest t("env", L5("adsr", 10, 20, 90, 10));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 4);
            REQUIRE_PROPERTY_FLOAT(t, @length, 40);
            REQUIRE_PROPERTY_LIST(t, @values, L4(0.f, 1, 0.9, 0.f));
            REQUIRE_PROPERTY_LIST(t, @points, L4(0.f, 10, 30, 40));
        }

        SECTION("asr")
        {
            EnvelopeTest t("env", L3("asr", 10, 20));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 3);
            REQUIRE_PROPERTY_FLOAT(t, @length, 30.f);
            REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
            REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        }

        SECTION("ar")
        {
            EnvelopeTest t("env", L3("ar", 10, 20));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 3);
            REQUIRE_PROPERTY_FLOAT(t, @length, 30);
            REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
            REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        }

        SECTION("invalid")
        {

#define INVALID_INIT(lst)                   \
    {                                       \
        EnvelopeTest t("env", lst);         \
        REQUIRE_PROPERTY(t, @npoints, 0.f); \
    }

            INVALID_INIT(L1("adsr"));
            INVALID_INIT(L4("adsr", 10, 20, 90));
            INVALID_INIT(L5("adsrd", 10, 20, 90, 10));
            INVALID_INIT(L5("adsr", -1, 20, 90, 10));
            INVALID_INIT(L5("adsr", "A", 20, 90, 10));
            INVALID_INIT(L5("adsr", 10, -1, 90, 10));
            INVALID_INIT(L5("adsr", 10, "B", 90, 10));
            INVALID_INIT(L5("adsr", 10, 20, "C", 10));
            INVALID_INIT(L5("adsr", 10, 20, -1, 10));
            INVALID_INIT(L5("adsr", 10, 20, 101, 10));
            INVALID_INIT(L5("adsr", 10, 20, 100, "D"));
            INVALID_INIT(L5("adsr", 10, 20, 100, -1));
            INVALID_INIT(L5("asr", 10, 20, 100, 10));
            INVALID_INIT(L2("asr", 10));
            INVALID_INIT(L1("asr"));
            INVALID_INIT(L3("asr", -1, 20));
            INVALID_INIT(L3("asr", "A", 20));
            INVALID_INIT(L3("asr", 20, -1));
            INVALID_INIT(L3("asr", 20, "D"));
            INVALID_INIT(L2("???", 10));
            INVALID_INIT(L5("ar", 10, 20, 100, 10));
            INVALID_INIT(L2("ar", 10));
            INVALID_INIT(L1("ar"));
            INVALID_INIT(L3("ar", -1, 20));
            INVALID_INIT(L3("ar", "A", 20));
            INVALID_INIT(L3("ar", 20, -1));
            INVALID_INIT(L3("ar", 20, "D"));
        }
    }

    SECTION("on bang")
    {
        EnvelopeTest t("env");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, DataTypeEnv());

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, t.envelope());

        t.m_ADSR(gensym("adsr"), L4(10, 20, 90, 100));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, EnvelopeTest("env", L5("adsr", 10, 20, 90, 100)).envelope());
    }

    SECTION("on data")
    {
        EnvelopeTest t("env");
        WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
        REQUIRE_ENV_OUTPUT(t, DataTypeEnv());

        WHEN_SEND_TDATA_TO(0, t, EnvelopeTest("env", L5("adsr", 10, 20, 90, 100)).envelope());
        REQUIRE_ENV_OUTPUT(t, EnvelopeTest("env", L5("adsr", 10, 20, 90, 100)).envelope());
    }

    SECTION("clear")
    {
        EnvelopeTest t("env");
        t.m_ADSR(gensym("adsr"), L4(10, 20, 70, 200));
        REQUIRE(t.p_npoints() == L1(4));

        t.cleanAllMessages();
        WHEN_CALL(t, clear);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(0.f));
    }

    SECTION("asr")
    {
        EnvelopeTest t("env");

        WHEN_CALL_2(t, ASR, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(3));
        REQUIRE(t.p_points() == L3(0.f, 10, 30));

        WHEN_CALL_2(t, ASR, 100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(3));
        REQUIRE(t.p_points() == L3(0.f, 100, 300));

        // invalid
        WHEN_CALL_2(t, ASR, -100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L3(0.f, 100, 300));

        WHEN_CALL_2(t, ASR, 100, -200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L3(0.f, 100, 300));
    }

    SECTION("AR")
    {
        EnvelopeTest t("env");

        WHEN_CALL_2(t, AR, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(3));
        REQUIRE(t.p_points() == L3(0.f, 10, 30));

        WHEN_CALL_2(t, AR, 100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(3));
        REQUIRE(t.p_points() == L3(0.f, 100, 300));

        // invalid
        WHEN_CALL_2(t, AR, -100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L3(0.f, 100, 300));

        WHEN_CALL_2(t, AR, 100, -200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L3(0.f, 100, 300));
    }

    SECTION("ADSR")
    {
        EnvelopeTest t("env");

        WHEN_CALL_4(t, ADSR, 10, 20, 90, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(4));
        REQUIRE(t.p_points() == L4(0.f, 10, 30, 130));

        WHEN_CALL_4(t, ADSR, 1, 2, 80, 10);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_npoints() == L1(4));
        REQUIRE(t.p_points() == L4(0.f, 1, 3, 13));

        // invalid
        WHEN_CALL_4(t, ADSR, -100, 200, 10, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L4(0.f, 1, 3, 13));

        WHEN_CALL_4(t, ADSR, 100, -200, 10, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == L4(0.f, 1, 3, 13));
    }

    SECTION("add point")
    {
        EnvelopeTest t("env");

        t.m_addPoint(0, L1(100));
        REQUIRE(t.p_npoints() == L1(0.f));

        t.m_addPoint(0, L2(10, 200));
        REQUIRE(t.p_npoints() == L1(1));
        REQUIRE(t.p_values() == L1(200));

        t.m_addPoint(0, L2(10, 200));
        REQUIRE(t.p_npoints() == L1(1));
        REQUIRE(t.p_values() == L1(200));

        t.m_addPoint(0, L2(10, 300));
        REQUIRE(t.p_npoints() == L1(1));
        REQUIRE(t.p_values() == L1(300));

        t.m_addPoint(0, L2(9, 300));
        REQUIRE(t.p_npoints() == L1(2));
        REQUIRE(t.p_points() == L2(9, 10));
        REQUIRE(t.p_values() == L2(300, 300));

        t.m_addPoint(0, L2(-1, 1));
        REQUIRE(t.p_npoints() == L1(2));
        REQUIRE(t.p_points() == L2(9, 10));
        REQUIRE(t.p_values() == L2(300, 300));
        REQUIRE(t.envelope().pointAt(0).timeMs() == 9);
        REQUIRE(t.envelope().pointAt(1).timeMs() == 10);
        REQUIRE(!t.envelope().pointAt(0).stop);
        REQUIRE(!t.envelope().pointAt(1).stop);
        REQUIRE(t.envelope().pointAt(0).type == CURVE_LINE);
        REQUIRE(t.envelope().pointAt(1).type == CURVE_LINE);

        t.m_addPoint(0, L3(0.f, 0.f, 1));
        REQUIRE(t.envelope().hasStopPoints());

        t.m_addPoint(0, L4(20, 2, 0.f, "exp"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        t.m_addPoint(0, L4(20, 2, 0.f, "line"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_LINE);
        t.m_addPoint(0, L4(20, 2, 0.f, "step"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_STEP);
        t.m_addPoint(0, L4(20, 2, 0.f, "????"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_LINE);
        t.m_addPoint(0, L4(20, 2, 0.f, "exp"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == 1);
        t.m_addPoint(0, L5(20, 2, 0.f, "exp", -1));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == -1);
        t.m_addPoint(0, L5(20, 2, 0.f, "exp", 100));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == 100);
    }

    SECTION("remove point")
    {
        EnvelopeTest t("env");

        t.m_removePoint(gensym("remove_point"), AtomList());
        t.m_removePoint(gensym("remove_point"), L1("A"));
        t.m_removePoint(gensym("remove_point"), L1(0.f));
        t.m_removePoint(gensym("remove_point"), L1(2));

        t.m_AR(0, L2(10, 20));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), L1("A"));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), L1(-1));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), L1(3));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), L1(2));
        REQUIRE_PROPERTY(t, @npoints, 2);
        REQUIRE_PROPERTY_LIST(t, @points, L2(0.f, 10));
        t.m_removePoint(gensym("remove_point"), L1(0.f));
        REQUIRE_PROPERTY_LIST(t, @points, L1(10));
        t.m_removePoint(gensym("remove_point"), L1(0.f));
        REQUIRE_PROPERTY(t, @npoints, 0.f);
    }

    SECTION("set_point_value")
    {
        EnvelopeTest t("env");
        t.m_setPointValue(gensym("set_point_value"), AtomList());
        t.m_setPointValue(gensym("set_point_value"), L1(1));
        t.m_setPointValue(gensym("set_point_value"), L2(1, 2));

        t.m_AR(0, L2(10, 20));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
        // invalid
        t.m_setPointValue(gensym("set_point_value"), L2("A", 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
        t.m_setPointValue(gensym("set_point_value"), L2(-1, 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
        t.m_setPointValue(gensym("set_point_value"), L2(3, 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
        // ok
        t.m_setPointValue(gensym("set_point_value"), L2(2, 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 2));
        t.m_setPointValue(gensym("set_point_value"), L2(1, 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 2, 2));
        t.m_setPointValue(gensym("set_point_value"), L2(0.f, 2));
        REQUIRE_PROPERTY_LIST(t, @values, L3(2, 2, 2));
    }

    SECTION("set_point_time")
    {
        EnvelopeTest t("env");
        t.m_setPointTime(gensym("set_point_time"), AtomList());
        t.m_setPointTime(gensym("set_point_time"), L1(1));
        t.m_setPointTime(gensym("set_point_time"), L2(1, 2));

        t.m_AR(0, L2(10, 20));
        REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        // invalid
        t.m_setPointTime(gensym("set_point_time"), AtomList());
        REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        t.m_setPointTime(gensym("set_point_time"), L2(-1, 2));
        REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        t.m_setPointTime(gensym("set_point_time"), L2(3, 2));
        REQUIRE_PROPERTY_LIST(t, @points, L3(0.f, 10, 30));
        // ok
        t.m_setPointTime(gensym("set_point_time"), L2(0.f, 5));
        REQUIRE_PROPERTY_LIST(t, @points, L3(5, 10, 30));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 1, 0.f));
        t.m_setPointTime(gensym("set_point_time"), L2(1, 1));
        REQUIRE_PROPERTY_LIST(t, @points, L3(1, 5, 30));
        REQUIRE_PROPERTY_LIST(t, @values, L3(1, 0.f, 0.f));
        t.m_setPointTime(gensym("set_point_time"), L2(0.f, 40));
        REQUIRE_PROPERTY_LIST(t, @points, L3(5, 30, 40));
        REQUIRE_PROPERTY_LIST(t, @values, L3(0.f, 0.f, 1));
    }

    SECTION("set_stop_point")
    {
        EnvelopeTest t("env");
        t.m_setStopPoint(gensym("set_stop_point"), AtomList());
        t.m_setStopPoint(gensym("set_stop_point"), L1(1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(1, 2));

        t.m_ASR(0, L2(10, 20));
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));

        // invalid
        t.m_setStopPoint(gensym("set_stop_point"), AtomList());
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));
        t.m_setStopPoint(gensym("set_stop_point"), L1(1));
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(1, -1));
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(-1, -1));
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(3, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, L1(1));

        // ok
        t.m_setStopPoint(gensym("set_stop_point"), L2(1, 0.f));
        REQUIRE_PROPERTY_LIST(t, @stops, AtomList());

        t.m_setStopPoint(gensym("set_stop_point"), L2(1, 1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(0.f, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, L2(0.f, 1));
        t.m_setStopPoint(gensym("set_stop_point"), L2(2, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, L3(0.f, 1, 2));
    }

    SECTION("set_segment_type")
    {
        EnvelopeTest t("env");
        t.m_setSegmentType(gensym("set_segment_type"), AtomList());
        t.m_setSegmentType(gensym("set_segment_type"), L1(1));
        t.m_setSegmentType(gensym("set_segment_type"), L2(1, "exp"));

        t.m_ASR(0, L2(10, 20));
    }
}
