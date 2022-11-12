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
#include "env_to_vline.h"
#include "test_env_base.h"

PD_COMPLETE_TEST_SETUP(Env2VLine, env, to_vline)

#define REQUIRE_OUTPUT(obj, n, data)                      \
    {                                                     \
        REQUIRE(obj.hasNewMessages(0));                   \
        REQUIRE(obj.messageAt(n, 0).isList());            \
        REQUIRE(obj.messageAt(n, 0).listValue() == data); \
    }

TEST_CASE("env->vline", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        setup_env_to_vline();

        TObj t("env->vline");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @sync, 0.f);
    }

    SECTION("AR")
    {
        // AR
        EnvAtom env;
        env->setAR(LF(10, 20));

        TObj t("env->vline");
        t.onDataT(env);
        t.onBang();

        REQUIRE_OUTPUT(t, 0, AtomList::zeroes(3));
        REQUIRE_OUTPUT(t, 1, LF(1, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LF(0.f, 20, 10));

        t.cleanAllMessages();
    }

    SECTION("ASR")
    {
        // ASR
        EnvAtom env;
        env->setASR(LF(20, 30));

        TObj t("evn->vline");

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MSG(t);

        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 20, 0.f));

        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 30, 0.f));

        // double release
        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        // send again
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 20, 0.f));

        t.cleanAllMessages();

        // double send
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 20, 0.f));

        t.cleanAllMessages();

        // attack after reset
        WHEN_CALL(t, reset);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 20, 0.f));
        t.cleanAllMessages();

        // release
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 30, 0.f));
        t.cleanAllMessages();

        // release after reset
        WHEN_CALL(t, reset);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
    }

    SECTION("ADSR")
    {
        TObj t("evn->vline");

        // empty envelope
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MSG(t);

        // set ADSR
        EnvAtom env;
        env->setADSR(LF(10, 20, 25, 80));
        t.onDataT(env);
        t.cleanAllMessages();

        // release no attack
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        // attack
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.25, 20, 10.f));
        t.cleanAllMessages();

        // attack again
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.25, 20, 10.f));
        t.cleanAllMessages();

        // release
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LF(0.f, 80, 0.f));

        // double release
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
    }

    SECTION("line")
    {
        EnvAtom env;
        env->setLine(LA(0.5, 10, 1, 20, 0.1));

        TObj t("env->vline");
        t.onDataT(env);
        t.onBang();

        REQUIRE_OUTPUT(t, 0, LA(0.5, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.1, 20, 10));

        env->setLine(LF(0.125, 10, 0.25, 20, 0.375, 30, 0.5, 40, 0.625, 50, 0.75, 60, 0.125));
        env->pointAt(2).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_SEND_BANG_TO(0, t);

        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 20, 10));
        REQUIRE_OUTPUT(t, 3, LA(0.5, 30, 30));
        REQUIRE_OUTPUT(t, 4, LA(0.625, 40, 60));
        REQUIRE_OUTPUT(t, 5, LA(0.75, 50, 100));
        REQUIRE_OUTPUT(t, 6, LA(0.125, 60, 150));

        t.cleanAllMessages();

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 20, 10));

        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 4);
        REQUIRE_OUTPUT(t, 0, LA(0.5, 30, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.625, 40, 30));
        REQUIRE_OUTPUT(t, 2, LA(0.75, 50, 70));
        REQUIRE_OUTPUT(t, 3, LA(0.125, 60, 120));

        t.cleanAllMessages();
        env->pointAt(2).stop = true;
        env->pointAt(3).stop = true;
        env->pointAt(5).stop = true;

        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_CALL(t, reset);
        REQUIRE_NO_MSG(t);
        WHEN_CALL(t, next);
        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 10, 0.f));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 20, 10));
        REQUIRE(t.messageCount() == 3);

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LA(0.5, 30, 0.f));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LA(0.625, 40, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.75, 50, 40));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LA(0.125, 60, 0.f));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 0);
    }

    SECTION("STEP")
    {
        EnvAtom env;
        env->setStep(LA(0.5, 10, 1, 20, 0.1));

        TObj t("env->vline");
        t.onDataT(env);
        t.onBang();

        REQUIRE_OUTPUT(t, 0, LA(0.5, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LF(1, 0.f, 10));
        REQUIRE_OUTPUT(t, 2, LA(0.1, 0.f, 30));

        env->setStep(LF(0.125, 10, 0.25, 20, 0.375, 30, 0.5, 40, 0.625, 50, 0.75, 60, 0.125));
        env->pointAt(2).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        // fixed
        WHEN_SEND_BANG_TO(0, t);

        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 0.f, 10));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 0.f, 30));
        REQUIRE_OUTPUT(t, 3, LA(0.5, 0.f, 60));
        REQUIRE_OUTPUT(t, 4, LA(0.625, 0.f, 100));
        REQUIRE_OUTPUT(t, 5, LA(0.75, 0.f, 150));
        REQUIRE_OUTPUT(t, 6, LA(0.125, 0.f, 210));

        t.cleanAllMessages();

        // attack/release
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 0.f, 10));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 0.f, 30));

        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 4);
        REQUIRE_OUTPUT(t, 0, LA(0.5, 0.f, 30));
        REQUIRE_OUTPUT(t, 1, LA(0.625, 0.f, 70));
        REQUIRE_OUTPUT(t, 2, LA(0.75, 0.f, 120));
        REQUIRE_OUTPUT(t, 3, LA(0.125, 0.f, 180));

        env->pointAt(2).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        // multi segment
        env->pointAt(2).stop = true;
        env->pointAt(3).stop = true;
        env->pointAt(6).stop = true;
        t.onDataT(env);

        t.cleanAllMessages();

        WHEN_CALL(t, reset);
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LA(0.125, 0.f, 0.f));
        REQUIRE_OUTPUT(t, 1, LA(0.25, 0.f, 10));
        REQUIRE_OUTPUT(t, 2, LA(0.375, 0.f, 30));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 1);
        REQUIRE_OUTPUT(t, 0, LA(0.5, 0.f, 30));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LA(0.625, 0.f, 40));
        REQUIRE_OUTPUT(t, 1, LA(0.75, 0.f, 90));
        REQUIRE_OUTPUT(t, 2, LA(0.125, 0.f, 150));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 0);
    }

    SECTION("Exponential")
    {
        EnvAtom env;
        REQUIRE_FALSE(env->setExponential(L()));
        REQUIRE_FALSE(env->setExponential(LF(0.1, 16, -3, 0.7, 16, -3)));
        REQUIRE(env->setExponential(LF(0.1, 16, -2, 0.7, 20, -2, 0.2, 60, -3, 0.f)));
        REQUIRE(env->numPoints() == 4);

        TObj t("env->vline");
        t.onDataT(env);

        t.cleanAllMessages();

        // fixed
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.messageCount() == 11);

        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.538635, 8, 0));
        REQUIRE_OUTPUT(t, 2, LX(0.7, 8, 8));
        REQUIRE_OUTPUT(t, 3, LX(0.334471, 10, 16));
        REQUIRE_OUTPUT(t, 4, LX(0.2, 10, 26));
        REQUIRE_OUTPUT(t, 5, LX(0.117183, 10, 36));
        REQUIRE_OUTPUT(t, 6, LX(0.0669518, 10, 46));
        REQUIRE_OUTPUT(t, 7, LX(0.0364851, 10, 56));
        REQUIRE_OUTPUT(t, 8, LX(0.0180061, 10, 66));
        REQUIRE_OUTPUT(t, 9, LX(0.00679804, 10, 76));
        REQUIRE_OUTPUT(t, 10, LX(0, 10, 86));

        // attack/release
        env->pointAt(1).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        // attack
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.538635, 8, 0));
        REQUIRE_OUTPUT(t, 2, LX(0.7, 8, 8));

        // release
        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 8);
        REQUIRE_OUTPUT(t, 0, LX(0.334471, 10, 0).margin(0.00001));
        REQUIRE_OUTPUT(t, 1, LX(0.2, 10, 10));
        REQUIRE_OUTPUT(t, 2, LX(0.117183, 10, 20));
        REQUIRE_OUTPUT(t, 3, LX(0.0669518, 10, 30));
        REQUIRE_OUTPUT(t, 4, LX(0.0364851, 10, 40));
        REQUIRE_OUTPUT(t, 5, LX(0.0180061, 10, 50));
        REQUIRE_OUTPUT(t, 6, LX(0.00679804, 10, 60));
        REQUIRE_OUTPUT(t, 7, LX(0, 10, 70));

        // multi segment
        env->pointAt(2).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_CALL(t, reset);
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.538635, 8, 0));
        REQUIRE_OUTPUT(t, 2, LX(0.7, 8, 8));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 2);
        REQUIRE_OUTPUT(t, 0, LX(0.334471f, 10, 0).margin(0.00001));
        REQUIRE_OUTPUT(t, 1, LX(0.2, 10, 10));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 6);
        REQUIRE_OUTPUT(t, 0, LX(0.117183f, 10, 0).margin(0.000001));
        REQUIRE_OUTPUT(t, 1, LX(0.0669518f, 10, 10));
        REQUIRE_OUTPUT(t, 2, LX(0.0364851f, 10, 20));
        REQUIRE_OUTPUT(t, 3, LX(0.0180061f, 10, 30));
        REQUIRE_OUTPUT(t, 4, LX(0.00679804, 10, 40));
        REQUIRE_OUTPUT(t, 5, LX(0, 10, 50));
    }

    SECTION("Sin2")
    {
        EnvAtom env;
        REQUIRE_FALSE(env->setSin2(L()));
        REQUIRE_FALSE(env->setSin2(LA(0.1, 2)));
        REQUIRE(env->setSin2(LA(0.1, 20, 1, 40, 0.f)));
        REQUIRE(env->numPoints() == 3);

        TObj t("env->vline");
        t.onDataT(env);

        t.cleanAllMessages();

        // fixed
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.messageCount() == 7);

        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));
        REQUIRE_OUTPUT(t, 3, LX(0.853553, 10, 20));
        REQUIRE_OUTPUT(t, 4, LX(0.5, 10, 30));
        REQUIRE_OUTPUT(t, 5, LX(0.146447, 10, 40));
        REQUIRE_OUTPUT(t, 6, LX(0, 10, 50));

        // attack/release
        env->pointAt(1).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        // attack
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));

        // release
        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 4);
        REQUIRE_OUTPUT(t, 0, LX(0.853553, 10, 0).margin(0.00001));
        REQUIRE_OUTPUT(t, 1, LX(0.5, 10, 10));
        REQUIRE_OUTPUT(t, 2, LX(0.146447, 10, 20));
        REQUIRE_OUTPUT(t, 3, LX(0, 10, 30));

        // multi segment
        env->pointAt(1).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_CALL(t, reset);
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 4);
        REQUIRE_OUTPUT(t, 0, LX(0.853553, 10, 0).margin(0.000001));
        REQUIRE_OUTPUT(t, 1, LX(0.5, 10, 10));
        REQUIRE_OUTPUT(t, 2, LX(0.146447, 10, 20));
        REQUIRE_OUTPUT(t, 3, LX(0, 10, 30));
    }

    SECTION("Sigmoid")
    {
        EnvAtom env;
        REQUIRE_FALSE(env->setSigmoid(L()));
        REQUIRE_FALSE(env->setSigmoid(LA(0.1, 2)));
        REQUIRE(env->setSigmoid(LA(0.1, 20, 0.f, 1, 25, 1, 0.2, 40, 1, 0.1, 6, -1, 0.f)));
        REQUIRE(env->numPoints() == 5);

        TObj t("env->vline");
        t.onDataT(env);

        t.cleanAllMessages();

        // fixed
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.messageCount() == 11);

        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));
        REQUIRE_OUTPUT(t, 3, LX(0.633256, 8.33333, 20));
        REQUIRE_OUTPUT(t, 4, LX(0.566744, 8.33333, 28.3333));
        REQUIRE_OUTPUT(t, 5, LX(0.2, 8.33333, 36.6667));
        REQUIRE_OUTPUT(t, 6, LX(0.156218, 10, 45));
        REQUIRE_OUTPUT(t, 7, LX(0.15, 10, 55));
        REQUIRE_OUTPUT(t, 8, LX(0.143782, 10, 65));
        REQUIRE_OUTPUT(t, 9, LX(0.1, 10, 75));
        REQUIRE_OUTPUT(t, 10, LX(0, 6, 85));

        // attack/release
        env->pointAt(1).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        // attack
        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));

        // release
        t.cleanAllMessages();
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE(t.messageCount() == 8);
        REQUIRE_OUTPUT(t, 0, LX(0.633256, 8.33333, 0).margin(0.000001));
        REQUIRE_OUTPUT(t, 1, LX(0.566744, 8.33333, 8.3333));
        REQUIRE_OUTPUT(t, 2, LX(0.2, 8.33333, 16.6667));
        REQUIRE_OUTPUT(t, 3, LX(0.156218, 10, 25));
        REQUIRE_OUTPUT(t, 4, LX(0.15, 10, 35));
        REQUIRE_OUTPUT(t, 5, LX(0.143782, 10, 45));
        REQUIRE_OUTPUT(t, 6, LX(0.1, 10, 55));
        REQUIRE_OUTPUT(t, 7, LX(0, 6, 65));

        // multi segment
        env->pointAt(1).stop = true;
        env->pointAt(2).stop = true;
        t.onDataT(env);
        t.cleanAllMessages();

        WHEN_CALL(t, reset);
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.1, 0, 0));
        REQUIRE_OUTPUT(t, 1, LX(0.55, 10, 0));
        REQUIRE_OUTPUT(t, 2, LX(1, 10, 10));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 3);
        REQUIRE_OUTPUT(t, 0, LX(0.633256, 8.33333, 0).margin(0.000001));
        REQUIRE_OUTPUT(t, 1, LX(0.566744, 8.33333, 8.3333));
        REQUIRE_OUTPUT(t, 2, LX(0.2, 8.33333, 16.6667));

        t.cleanAllMessages();
        WHEN_CALL(t, next);
        REQUIRE(t.messageCount() == 5);
        REQUIRE_OUTPUT(t, 0, LX(0.156218, 10, 0).margin(0.00001));
        REQUIRE_OUTPUT(t, 1, LX(0.15, 10, 10));
        REQUIRE_OUTPUT(t, 2, LX(0.143782, 10, 20));
        REQUIRE_OUTPUT(t, 3, LX(0.1, 10, 30));
        REQUIRE_OUTPUT(t, 4, LX(0, 6, 40));
    }

    SECTION("Data @sync")
    {
        EnvAtom env;
        env->setAR(100, 200);

        TObj t("env->vline");
        REQUIRE_PROPERTY(t, @sync, 0.f);
        t.cleanAllMessages();
        t.storeAllMessageCount();

        t.onDataT(env);
        REQUIRE_NO_MSG(t);

        t.setProperty("@sync", LF(1));
        t.onDataT(env);
        REQUIRE(t.messageCount() == 3);
        t.cleanAllMessages();
        t.storeAllMessageCount();

        t.setProperty("@sync", LF(0.f));
        REQUIRE_NO_MSG(t);
    }
}
