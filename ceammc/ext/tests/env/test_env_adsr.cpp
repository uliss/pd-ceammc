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

class EnvAdsr;
extern void setup_env_adsr_tilde();

PD_COMPLETE_TEST_SETUP(EnvAdsr, env, adsr_tilde)

#define PROPERTY_REQUEST(obj, n, ...)                       \
    {                                                       \
        obj.sendMessage(gensym(n "?"));                     \
        REQUIRE(obj.hasOutputAt(0));                        \
        REQUIRE(obj.isOutputAnyAt(0));                      \
        REQUIRE(obj.outputAnyAt(0) == LAX(n, __VA_ARGS__)); \
        obj.clearAll();                                     \
    }

TEST_CASE("env.adsr~", "[externals]")
{
    pd_test_init();
    setTestSampleRate(44100);

    SECTION("create")
    {
        SECTION("default")
        {
            TestExtEnvAdsr t("env.adsr~");
            REQUIRE(t.object());

            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);

            PROPERTY_REQUEST(t, "@attack", 10);
            PROPERTY_REQUEST(t, "@decay", 10);
            PROPERTY_REQUEST(t, "@sustain", 50);
            PROPERTY_REQUEST(t, "@release", 300);
            PROPERTY_REQUEST(t, "@adsr", 10, 10, 50, 300);
            PROPERTY_REQUEST(t, "@gate", 0.f);
        }
    }

    SECTION("init")
    {
        TestExtEnvAdsr t("env.adsr~", LA(4, 90, 5));
        PROPERTY_REQUEST(t, "@adsr", 4, 90, 5, 300);

        t.sendMessage(SYM("@attack"), LA(30));
        PROPERTY_REQUEST(t, "@adsr", 30, 90, 5, 300);
        t.sendMessage(SYM("@decay"), LA(20));
        PROPERTY_REQUEST(t, "@adsr", 30, 20, 5, 300);
        t.sendMessage(SYM("@sustain"), LA(50));
        PROPERTY_REQUEST(t, "@adsr", 30, 20, 50, 300);
        t.sendMessage(SYM("@release"), LA(100));
        PROPERTY_REQUEST(t, "@adsr", 30, 20, 50, 100);

        // read-only
        t.sendMessage(SYM("@adsr"), LA(5, 10, 20, 30));
        PROPERTY_REQUEST(t, "@adsr", 30, 20, 50, 100);

        // gate on
        t.sendMessage(SYM("@gate"), LA(1));
        PROPERTY_REQUEST(t, "@gate", 1);
        t.schedTicks(20);
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(70);

        // attack/decay phase finished - output 1
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputFloatAt(1));
        REQUIRE(t.outputFloatAt(1) == 1);

        // release phase finished
        t.sendMessage(SYM("@gate"), LA(0.f));
        t.schedTicks(150);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputFloatAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);

        DataTypeEnv env;
        env.setADSR(10 * 1000, 45 * 1000, 0.5, 30 * 1000);
        REQUIRE(env.isADSR());
        t.send(EnvA(env));

        PROPERTY_REQUEST(t, "@adsr", 10, 45, 50, 30);

        // exponential envelopes ignored
        env.setEADSR(12 * 1000, 0, 17 * 1000, 0, 40, 10, 0);
        REQUIRE_FALSE(env.isADSR());
        t.send(EnvA(env));
        PROPERTY_REQUEST(t, "@adsr", 10, 45, 50, 30);

        t.sendList(LA("abc"));
        PROPERTY_REQUEST(t, "@adsr", 10, 45, 50, 30);

        t.sendList(LF(0, 0, 0, 0));
        PROPERTY_REQUEST(t, "@adsr", 10, 45, 50, 30);
        t.sendList(LF(0, 0, 0, 10));
        PROPERTY_REQUEST(t, "@adsr", 0.f, 0.f, 0.f, 10);
        t.sendList(LF(1, 15, 2, 3));
        PROPERTY_REQUEST(t, "@adsr", 1, 15, 2, 3);
        t.sendList(LF(1, 10, -1, 3, 4));
        PROPERTY_REQUEST(t, "@adsr", 1, 15, 2, 3);
        t.sendList(LF(100, -8, 1, -3, 1));
        PROPERTY_REQUEST(t, "@adsr", 1, 15, 2, 3);

        t.sendList(LF(100000, 100000, 100, 100000));
        PROPERTY_REQUEST(t, "@adsr", 100000, 100000, 100, 100000);
    }

    SECTION("reset")
    {
        TestExtEnvAdsr t("env.adsr~");

        // reset before sustain
        t.clearAll();
        t.sendMessage(SYM("@gate"), LA(1));
        t.schedTicks(5);
        t.sendMessage(SYM("reset"));
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(500);
        PROPERTY_REQUEST(t, "@gate", 0.f);

        // reset after release
        t.clearAll();
        t.sendMessage(SYM("@gate"), LA(1));
        t.schedTicks(50);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 1);
        t.schedTicks(20);
        t.clearAll();
        t.sendMessage(SYM("reset"));
        t.schedTicks(500);
        REQUIRE(!t.hasOutputAt(1));
        PROPERTY_REQUEST(t, "@gate", 0.f);
    }

    SECTION("play")
    {
        TestExtEnvAdsr t("env.adsr~", LA(10, 15, 100, 20));
        t.sendMessage(SYM("play"), LF(100));
        t.schedTicks(5);
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(20);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 1);
        t.clearAll();
        t.schedTicks(40);
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(60);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);
    }
}
