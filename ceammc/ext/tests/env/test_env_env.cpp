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
#include "env_env.h"
#include "test_env_base.h"

PD_COMPLETE_TEST_SETUP(Envelope, envelope, env)

#define REQUIRE_ENV_OUTPUT(t, env)        \
    {                                     \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t); \
        REQUIRE(dataAt(t) == EnvA(env));  \
        t.cleanAllMessages();             \
    }

using EnvAtom = DataAtom<DataTypeEnv>;

TEST_CASE("env.env", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("env");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @npoints, 0.f);
            REQUIRE_PROPERTY(t, @length, 0.f);
            REQUIRE_PROPERTY_LIST(t, @values, L());
            REQUIRE_PROPERTY_LIST(t, @points, L());
        }

        SECTION("adsr")
        {
            TObj t("env", LA("adsr", 10, 20, 75, 10));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 4);
            REQUIRE_PROPERTY_FLOAT(t, @length, 40);
            REQUIRE_PROPERTY_LIST(t, @values, LX(0, 1, 0.75, 0));
            REQUIRE_PROPERTY_LIST(t, @points, LX(0, 10, 30, 40));
        }

        SECTION("asr")
        {
            TObj t("env", LA("asr", 10, 20));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 3);
            REQUIRE_PROPERTY_FLOAT(t, @length, 30.f);
            REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
            REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        }

        SECTION("ar")
        {
            TObj t("env", LA("ar", 10, 20));
            REQUIRE_PROPERTY_FLOAT(t, @npoints, 3);
            REQUIRE_PROPERTY_FLOAT(t, @length, 30);
            REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
            REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        }

        SECTION("invalid")
        {

#define INVALID_INIT(lst)                   \
    {                                       \
        TObj t("env", lst);                 \
        REQUIRE_PROPERTY(t, @npoints, 0.f); \
    }

            INVALID_INIT(LA("adsr"));
            INVALID_INIT(LA("adsr", 10, 20, 90));
            INVALID_INIT(LA("adsrd", 10, 20, 90, 10));
            INVALID_INIT(LA("adsr", -1, 20, 90, 10));
            INVALID_INIT(LA("adsr", "A", 20, 90, 10));
            INVALID_INIT(LA("adsr", 10, -1, 90, 10));
            INVALID_INIT(LA("adsr", 10, "B", 90, 10));
            INVALID_INIT(LA("adsr", 10, 20, "C", 10));
            INVALID_INIT(LA("adsr", 10, 20, -1, 10));
            INVALID_INIT(LA("adsr", 10, 20, 101, 10));
            INVALID_INIT(LA("adsr", 10, 20, 100, "D"));
            INVALID_INIT(LA("adsr", 10, 20, 100, -1));
            INVALID_INIT(LA("asr", 10, 20, 100, 10));
            INVALID_INIT(LA("asr", 10));
            INVALID_INIT(LA("asr"));
            INVALID_INIT(LA("asr", -1, 20));
            INVALID_INIT(LA("asr", "A", 20));
            INVALID_INIT(LA("asr", 20, -1));
            INVALID_INIT(LA("asr", 20, "D"));
            INVALID_INIT(LA("???", 10));
            INVALID_INIT(LA("ar", 10, 20, 100, 10));
            INVALID_INIT(LA("ar", 10));
            INVALID_INIT(LA("ar"));
            INVALID_INIT(LA("ar", -1, 20));
            INVALID_INIT(LA("ar", "A", 20));
            INVALID_INIT(LA("ar", 20, -1));
            INVALID_INIT(LA("ar", 20, "D"));
        }
    }

    SECTION("on bang")
    {
        TObj t("env");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, DataTypeEnv());

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, t.envelope());

        t.m_ADSR(gensym("adsr"), LF(10, 20, 90, 100));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_ENV_OUTPUT(t, TObj("env", LA("adsr", 10, 20, 90, 100)).envelope());
    }

    SECTION("on data")
    {
        TObj t("env");
        WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
        REQUIRE_ENV_OUTPUT(t, DataTypeEnv());

        WHEN_SEND_TDATA_TO(0, t, TObj("env", LA("adsr", 10, 20, 90, 100)).envelope());
        REQUIRE_ENV_OUTPUT(t, TObj("env", LA("adsr", 10, 20, 90, 100)).envelope());
    }

    SECTION("clear")
    {
        TObj t("env");
        t.m_ADSR(gensym("adsr"), LF(10, 20, 70, 200));
        REQUIRE_PROPERTY(t, @npoints, 4);

        t.cleanAllMessages();
        WHEN_CALL(t, clear);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 0.f);
    }

    SECTION("asr")
    {
        TObj t("env");

        WHEN_CALL_N(t, ASR, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 3);
        REQUIRE(t.p_points() == LF(0.f, 10, 30));

        WHEN_CALL_N(t, ASR, 100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 3);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));

        // invalid
        WHEN_CALL_N(t, ASR, -100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));

        WHEN_CALL_N(t, ASR, 100, -200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));
    }

    SECTION("AR")
    {
        TObj t("env");

        WHEN_CALL_N(t, AR, 10, 20);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 3);
        REQUIRE(t.p_points() == LF(0.f, 10, 30));

        WHEN_CALL_N(t, AR, 100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 3);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));

        // invalid
        WHEN_CALL_N(t, AR, -100, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));

        WHEN_CALL_N(t, AR, 100, -200);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LF(0.f, 100, 300));
    }

    SECTION("ADSR")
    {
        TObj t("env");

        WHEN_CALL_N(t, ADSR, 10, 20, 90, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 4);
        REQUIRE(t.p_points() == LA(0.f, 10, 30, 130));

        WHEN_CALL_N(t, ADSR, 1, 2, 80, 10);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY(t, @npoints, 4);
        REQUIRE(t.p_points() == LA(0.f, 1, 3, 13));

        // invalid
        WHEN_CALL_N(t, ADSR, -100, 200, 10, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LA(0.f, 1, 3, 13));

        WHEN_CALL_N(t, ADSR, 100, -200, 10, 100);
        REQUIRE_NO_MSG(t);
        REQUIRE(t.p_points() == LA(0.f, 1, 3, 13));
    }

    SECTION("add point")
    {
        TObj t("env");

        t.m_addPoint(0, LF(100));
        REQUIRE_PROPERTY(t, @npoints, 0.f);

        t.m_addPoint(0, LF(10, 200));
        REQUIRE_PROPERTY(t, @npoints, 1);
        REQUIRE(t.p_values() == LF(200));

        t.m_addPoint(0, LF(10, 200));
        REQUIRE_PROPERTY(t, @npoints, 1);
        REQUIRE(t.p_values() == LF(200));

        t.m_addPoint(0, LF(10, 300));
        REQUIRE_PROPERTY(t, @npoints, 1);
        REQUIRE(t.p_values() == LF(300));

        t.m_addPoint(0, LF(9, 300));
        REQUIRE_PROPERTY(t, @npoints, 2);
        REQUIRE(t.p_points() == LF(9, 10));
        REQUIRE(t.p_values() == LF(300, 300));

        t.m_addPoint(0, LA(-1, 1));
        REQUIRE_PROPERTY(t, @npoints, 2);
        REQUIRE(t.p_points() == LF(9, 10));
        REQUIRE(t.p_values() == LF(300, 300));
        REQUIRE(t.envelope().pointAt(0).timeMs() == 9);
        REQUIRE(t.envelope().pointAt(1).timeMs() == 10);
        REQUIRE(!t.envelope().pointAt(0).stop);
        REQUIRE(!t.envelope().pointAt(1).stop);
        REQUIRE(t.envelope().pointAt(0).type == CURVE_LINE);
        REQUIRE(t.envelope().pointAt(1).type == CURVE_LINE);

        t.m_addPoint(0, LF(0.f, 0.f, 1));
        REQUIRE(t.envelope().hasStopPoints());

        t.m_addPoint(0, LA(20, 2, 0.f, "exp"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        t.m_addPoint(0, LA(20, 2, 0.f, "line"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_LINE);
        t.m_addPoint(0, LA(20, 2, 0.f, "step"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_STEP);
        t.m_addPoint(0, LA(20, 2, 0.f, "????"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_LINE);
        t.m_addPoint(0, LA(20, 2, 0.f, "exp"));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == 1);
        t.m_addPoint(0, LA(20, 2, 0.f, "exp", -1));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == -1);
        t.m_addPoint(0, LA(20, 2, 0.f, "exp", 100));
        REQUIRE(t.envelope().pointAt(3).type == CURVE_EXP);
        REQUIRE(t.envelope().pointAt(3).data == 100);
    }

    SECTION("remove point")
    {
        TObj t("env");

        t.m_removePoint(gensym("remove_point"), L());
        t.m_removePoint(gensym("remove_point"), LA("A"));
        t.m_removePoint(gensym("remove_point"), LF(0.f));
        t.m_removePoint(gensym("remove_point"), LF(2));

        t.m_AR(0, LF(10, 20));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), LA("A"));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), LF(-1));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), LF(3));
        REQUIRE_PROPERTY(t, @npoints, 3);
        t.m_removePoint(gensym("remove_point"), LF(2));
        REQUIRE_PROPERTY(t, @npoints, 2);
        REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10));
        t.m_removePoint(gensym("remove_point"), LF(0.f));
        REQUIRE_PROPERTY_LIST(t, @points, LF(10));
        t.m_removePoint(gensym("remove_point"), LF(0.f));
        REQUIRE_PROPERTY(t, @npoints, 0.f);
    }

    SECTION("set_point_value")
    {
        TObj t("env");
        t.m_setPointValue(gensym("set_point_value"), L());
        t.m_setPointValue(gensym("set_point_value"), LF(1));
        t.m_setPointValue(gensym("set_point_value"), LF(1, 2));

        t.m_AR(0, LF(10, 20));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
        // invalid
        t.m_setPointValue(gensym("set_point_value"), LA("A", 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
        t.m_setPointValue(gensym("set_point_value"), LA(-1, 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
        t.m_setPointValue(gensym("set_point_value"), LF(3, 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
        // ok
        t.m_setPointValue(gensym("set_point_value"), LF(2, 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 2));
        t.m_setPointValue(gensym("set_point_value"), LF(1, 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 2, 2));
        t.m_setPointValue(gensym("set_point_value"), LF(0.f, 2));
        REQUIRE_PROPERTY_LIST(t, @values, LF(2, 2, 2));
    }

    SECTION("set_point_time")
    {
        TObj t("env");
        t.m_setPointTime(gensym("set_point_time"), L());
        t.m_setPointTime(gensym("set_point_time"), LF(1));
        t.m_setPointTime(gensym("set_point_time"), LF(1, 2));

        t.m_AR(0, LF(10, 20));
        REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        // invalid
        t.m_setPointTime(gensym("set_point_time"), L());
        REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        t.m_setPointTime(gensym("set_point_time"), LA(-1, 2));
        REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        t.m_setPointTime(gensym("set_point_time"), LF(3, 2));
        REQUIRE_PROPERTY_LIST(t, @points, LF(0.f, 10, 30));
        // ok
        t.m_setPointTime(gensym("set_point_time"), LF(0.f, 5));
        REQUIRE_PROPERTY_LIST(t, @points, LF(5, 10, 30));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 1, 0.f));
        t.m_setPointTime(gensym("set_point_time"), LF(1, 1));
        REQUIRE_PROPERTY_LIST(t, @points, LF(1, 5, 30));
        REQUIRE_PROPERTY_LIST(t, @values, LF(1, 0.f, 0.f));
        t.m_setPointTime(gensym("set_point_time"), LF(0.f, 40));
        REQUIRE_PROPERTY_LIST(t, @points, LF(5, 30, 40));
        REQUIRE_PROPERTY_LIST(t, @values, LF(0.f, 0.f, 1));
    }

    SECTION("set_stop_point")
    {
        TObj t("env");
        t.m_setStopPoint(gensym("set_stop_point"), L());
        t.m_setStopPoint(gensym("set_stop_point"), LF(1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(1, 2));

        t.m_ASR(0, LF(10, 20));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));

        // invalid
        t.m_setStopPoint(gensym("set_stop_point"), L());
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(1, -1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));
        t.m_setStopPoint(gensym("set_stop_point"), LA(-1, -1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(3, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(1));

        // ok
        t.m_setStopPoint(gensym("set_stop_point"), LF(1, 0.f));
        REQUIRE_PROPERTY_LIST(t, @stops, L());

        t.m_setStopPoint(gensym("set_stop_point"), LF(1, 1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(0.f, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(0.f, 1));
        t.m_setStopPoint(gensym("set_stop_point"), LF(2, 1));
        REQUIRE_PROPERTY_LIST(t, @stops, LF(0.f, 1, 2));
    }

    SECTION("set_segment_type")
    {
        TObj t("env");
        t.m_setSegmentType(gensym("set_segment_type"), L());
        t.m_setSegmentType(gensym("set_segment_type"), LF(1));
        t.m_setSegmentType(gensym("set_segment_type"), LA(1, "exp"));

        t.m_ASR(0, LF(10, 20));
    }
}
