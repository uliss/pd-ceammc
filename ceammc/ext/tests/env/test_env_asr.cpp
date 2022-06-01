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

class EnvAsr;
extern void setup_env_asr_tilde();

PD_COMPLETE_TEST_SETUP(EnvAsr, env, asr_tilde)

#define PROPERTY_REQUEST(obj, n, ...)                       \
    {                                                       \
        obj.sendMessage(gensym(n "?"));                     \
        REQUIRE(obj.hasOutputAt(0));                        \
        REQUIRE(obj.isOutputAnyAt(0));                      \
        REQUIRE(obj.outputAnyAt(0) == LAX(n, __VA_ARGS__)); \
    }

#define SEND_ENV(t, env)      \
    {                         \
        EnvAtom penv(env);    \
        t.sendList(LA(penv)); \
    }

TEST_CASE("env.asr~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        setTestSampleRate(44100);

        TestExtEnvAsr t("env.asr~", LA(4, 90, 5));
        REQUIRE(t.object());
        PROPERTY_REQUEST(t, "@asr", 4, 90, 5);
        t.clearAll();

        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);

        t.sendMessage(SYM("@attack"), LA(10));
        t.sendMessage(SYM("@release"), LA(20));
        REQUIRE(!t.hasOutput());

        PROPERTY_REQUEST(t, "@attack", 10);
        PROPERTY_REQUEST(t, "@sustain", 90);
        PROPERTY_REQUEST(t, "@release", 20);
        PROPERTY_REQUEST(t, "@asr", 10, 90, 20);
        PROPERTY_REQUEST(t, "@gate", 0.f);

        t.sendMessage(SYM("@attack"), LA(30));
        PROPERTY_REQUEST(t, "@attack", 30);
        PROPERTY_REQUEST(t, "@asr", 30, 90, 20);

        // read-only
        t.sendMessage(SYM("@asr"), LA(5, 10));
        PROPERTY_REQUEST(t, "@asr", 30, 90, 20);

        t.sendMessage(SYM("@gate"), LA(1));
        PROPERTY_REQUEST(t, "@gate", 1);
        t.schedTicks(50);
        PROPERTY_REQUEST(t, "@gate", 1);
        t.schedTicks(60);
        PROPERTY_REQUEST(t, "@gate", 1);

        // attack phase finished - output 1
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputFloatAt(1));
        REQUIRE(t.outputFloatAt(1) == 1);

        // release phase finished
        t.sendMessage(SYM("@gate"), LA(0.f));
        t.schedTicks(50);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.isOutputFloatAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);

        DataTypeEnv env;
        env.setAR(10 * 1000, 15 * 1000);
        REQUIRE(env.isAR());
        SEND_ENV(t, env);

        PROPERTY_REQUEST(t, "@asr", 10, 100, 15);

        env.setASR(11 * 1000, 16 * 1000);
        REQUIRE(env.isAR());
        SEND_ENV(t, env);
        PROPERTY_REQUEST(t, "@asr", 11, 100, 16);

        // exponential envelopes ignored
        env.setEASR(12 * 1000, 0, 17 * 1000, 0);
        REQUIRE_FALSE(env.isAR());
        SEND_ENV(t, env);
        PROPERTY_REQUEST(t, "@asr", 11, 100, 16);

        t.sendList(LA("abc"));
        PROPERTY_REQUEST(t, "@asr", 11, 100, 16);

        t.sendList(LF(0, 0, 0));
        PROPERTY_REQUEST(t, "@asr", 0.f, 0.f, 0.f);
        t.sendList(LF(1, 15, 2));
        PROPERTY_REQUEST(t, "@asr", 1, 15, 2);
        t.sendList(LF(1, 10, -1));
        PROPERTY_REQUEST(t, "@asr", 1, 15, 2);
        t.sendList(LF(100, -8, 1));
        PROPERTY_REQUEST(t, "@asr", 1, 15, 2);

        t.sendList(LF(100000, 100, 100000));
        PROPERTY_REQUEST(t, "@asr", 100000, 100, 100000);
    }

    SECTION("reset")
    {
        TestExtEnvAsr t("env.asr~");
        // default
        PROPERTY_REQUEST(t, "@asr", 10, 100, 300);

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
        t.schedTicks(20);
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
        TestExtEnvAsr t("env.asr~", LA(10, 100, 20));
        t.sendMessage(SYM("play"), LF(100));
        t.schedTicks(5);
        REQUIRE(!t.hasOutputAt(1));
        t.schedTicks(10);
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
