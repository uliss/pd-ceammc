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
#include "datatype_env.h"
#include "test_env_base.h"
#include "test_sound.h"

class EnvAr;
extern void setup_env_ar_tilde();

PD_COMPLETE_TEST_SETUP(EnvAr, env, ar_tilde)

#define PROPERTY_REQUEST(obj, n, ...)                       \
    {                                                       \
        obj.sendMessage(gensym(n "?"));                     \
        REQUIRE(obj.hasOutputAt(0));                        \
        REQUIRE(obj.isOutputAnyAt(0));                      \
        REQUIRE(obj.outputAnyAt(0) == LAX(n, __VA_ARGS__)); \
        obj.clearAll();                                     \
    }

#define SEND_ENV(t, env)      \
    {                         \
        EnvAtom enva(env);    \
        t.sendList(LA(enva)); \
    }

using EnvAtom = DataAtom<DataTypeEnv>;

TEST_CASE("env.ar~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        setTestSampleRate(44100);

        TExt t("env.ar~", LA(4, 5));
        REQUIRE(t.object());
        PROPERTY_REQUEST(t, "@ar", 4, 5);
        t.clearAll();

        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        t.sendMessage(SYM("@attack"), LA(10));
        t.sendMessage(SYM("@release"), LA(20));
        REQUIRE(!t.hasOutput());

        PROPERTY_REQUEST(t, "@attack", 10);
        PROPERTY_REQUEST(t, "@release", 20);
        PROPERTY_REQUEST(t, "@ar", 10, 20);
        PROPERTY_REQUEST(t, "@length", 30);
        PROPERTY_REQUEST(t, "@gate", 0.f);

        t.sendMessage(SYM("@attack"), LA(30));
        PROPERTY_REQUEST(t, "@attack", 30);
        PROPERTY_REQUEST(t, "@ar", 30, 20);
        PROPERTY_REQUEST(t, "@length", 50);

        // read-only
        t.sendMessage(SYM("@ar"), LA(5, 10));
        PROPERTY_REQUEST(t, "@ar", 30, 20);

        t.sendBang();
        PROPERTY_REQUEST(t, "@gate", 1);
        t.schedTicks(20);
        PROPERTY_REQUEST(t, "@gate", 0.f);
        t.schedTicks(60);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputBangAt(1));

        DataTypeEnv env;
        env.setAR(10 * 1000, 15 * 1000);
        REQUIRE(env.isAR());
        SEND_ENV(t, env);

        PROPERTY_REQUEST(t, "@ar", 10, 15);

        env.setASR(11 * 1000, 16 * 1000);
        REQUIRE(env.isAR());
        SEND_ENV(t, env);
        PROPERTY_REQUEST(t, "@ar", 11, 16);

        env.setEAR(12 * 1000, 0, 17 * 1000, 0);
        REQUIRE_FALSE(env.isAR());
        SEND_ENV(t, env);
        PROPERTY_REQUEST(t, "@ar", 11, 16);

        t.sendList(LA("abc"));
        PROPERTY_REQUEST(t, "@ar", 11, 16);

        t.sendList(LF(0, 0));
        PROPERTY_REQUEST(t, "@ar", 0.f, 0.f);
        t.sendList(LF(1, 2));
        PROPERTY_REQUEST(t, "@ar", 1, 2);
        t.sendList(LF(1, -1));
        PROPERTY_REQUEST(t, "@ar", 1, 2);
        t.sendList(LF(-100, -1));
        PROPERTY_REQUEST(t, "@ar", 1, 2);

        t.sendList(LF(100000, 100000));
        PROPERTY_REQUEST(t, "@ar", 100000, 100000);
    }

    SECTION("@gate")
    {
        TExt t("env.ar~", LA(10, 20));

        t.sendMessage(SYM("@gate"), LA(1));
        t.schedTicks(10);
        REQUIRE(!t.hasOutputAt(1));
        t.sendMessage(SYM("@gate"), LF(0));
        t.schedTicks(40);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputBangAt(1));
    }

    SECTION("reset")
    {
        TExt t("env.ar~");
        PROPERTY_REQUEST(t, "@ar", 10, 300);

        t.sendMessage(SYM("@gate"), LA(1));
        t.schedTicks(100);
        REQUIRE(!t.hasOutputAt(1));
        t.sendMessage(SYM("reset"));
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(400);
        REQUIRE(!t.hasOutputAt(1));
    }

    SECTION("play")
    {
        TExt t("env.ar~", LA(10, 20));
        t.sendMessage(SYM("play"), LF(500));
        t.schedTicks(40);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputBangAt(1));
    }
}
