/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "grain.h"
#include "grain_expr_parser.h"
#include "grain_random.h"
#include "test_array_base.h"

#include <cmath>

TEST_CASE("array_grain", "[externals]")
{
    setTestSampleRate(48000);
    test::pdPrintToStdError();

    SECTION("empty")
    {
        Grain g;
        GrainExprParser p(&g);
        REQUIRE(p.result() == 0);

        REQUIRE(p.parse(AtomList::parseString("@tb 100")));
        REQUIRE(p.result() == 100);

        REQUIRE(p.parse(AtomList::parseString("@tb 100.25")));
        REQUIRE(p.result() == 100.25);

        REQUIRE(p.parse(AtomList::parseString("@tb =101")));
        REQUIRE(p.result() == 101);

        REQUIRE(g.pan() == 0.5);
        REQUIRE(p.parse(AtomList::parseString("@p +=100")));
        REQUIRE(p.result() == 100.5);
        REQUIRE(g.pan() == 1);

        REQUIRE(p.parse(AtomList::parseString("@p *=5")));
        REQUIRE(p.result() == 5);
        REQUIRE(g.pan() == 1);

        REQUIRE(p.parse(AtomList::parseString("@l -25.25")));

        REQUIRE(p.parse(AtomList::parseString("@pan (12)")));
        REQUIRE(p.result() == 12);

        REQUIRE(p.parse(AtomList::parseString("@amp 15")));
        REQUIRE(p.result() == 15);

        REQUIRE(p.parse(AtomList::parseString("@at 1+2")));
        REQUIRE(p.result() == 3);

        REQUIRE(p.parse(AtomList::parseString("@s 10-15")));
        REQUIRE(p.result() == -5);

        REQUIRE(p.parse(AtomList::parseString("@speed 10-4")));
        REQUIRE(p.result() == 6);

        REQUIRE(p.parse(AtomList::parseString("@len 2+3*4")));
        REQUIRE(p.result() == 14);

        REQUIRE(p.parse(AtomList::parseString("@length (2+3)*4")));
        REQUIRE(p.result() == 20);

        REQUIRE(p.parse(AtomList::parseString("@tb 8/4/2")));
        REQUIRE(p.result() == 1);

        REQUIRE(p.parse(AtomList::parseString("@p 1-2*5")));
        REQUIRE(p.result() == -5);

        REQUIRE(p.parse(AtomList::parseString("@tb 1-(2*5)")));
        REQUIRE(p.result() == -9);

        REQUIRE(p.parse(AtomList::parseString("@l 10+-3")));
        REQUIRE(p.result() == 7);

        REQUIRE(p.parse(AtomList::parseString("@tb -10+-4")));
        REQUIRE(p.result() == -14);

        p.parse("@tb -10-6");
        REQUIRE(p.result() == -16);

        p.parse("@tb (3+-2)");
        REQUIRE(p.result() == 1);

        p.parse("@tb 2.5*2");
        REQUIRE(p.result() == 5);

        p.parse("@tb 5/2");
        REQUIRE(p.result() == 2.5);

        p.parse("@tb (2*3)+(4*3)");
        REQUIRE(p.result() == 18);

        p.parse("@tb 2^8");
        REQUIRE(p.result() == 256);

        p.parse("@tb 11%3");
        REQUIRE(p.result() == 2);

        p.parse("@tb $pi");
        REQUIRE(p.result() == Approx(std::acos(-1)));

        p.parse("@tb 100ms");
        REQUIRE(p.result() == 4800);

        p.parse("@tb 5msec");
        REQUIRE(p.result() == 240);

        p.parse("@tb 0.5sec");
        REQUIRE(p.result() == 24000);

        p.parse("@tb 0.25s");
        REQUIRE(p.result() == 12000);

        p.parse("@tb 5samp");
        REQUIRE(p.result() == 5);

        REQUIRE(p.parse("@tag tmp @l 2 @amp 0.25 @tb 12samp"));
        REQUIRE(g.tag() == SYM("tmp"));
        REQUIRE(g.lengthInSamples() == 2);
        REQUIRE(g.amplitude() == 0.25);
        REQUIRE(g.timeBefore() == 12);
    }

    SECTION("fn")
    {
        GrainExprParser p;

        REQUIRE(p.parse("@tb abs(-10*100)"));
        REQUIRE(p.result() == 1000);

        REQUIRE(p.parse("@tb abs(0.25*10.0)"));
        REQUIRE(p.result() == 2.5);

        p.parse("@tb sqrt(15)");
        REQUIRE(p.result() == Approx(std::sqrt(15)));

        p.parse("@tb sqrt(15)");
        REQUIRE(p.result() == Approx(std::sqrt(15)));

        p.parse("@tb sin($pi/2)");
        REQUIRE(p.result() == Approx(1));

        p.parse("@tb cos(2*$pi)");
        REQUIRE(p.result() == Approx(1));

        p.parse("@tb tan($pi/4)");
        REQUIRE(p.result() == Approx(1));

        p.parse("@tb acos(-1)");
        REQUIRE(p.result() == Approx(std::acos(-1)));

        p.parse("@tb asin(1)");
        REQUIRE(p.result() == Approx(std::acos(-1) * 0.5));

        p.parse("@tb atan(1)");
        REQUIRE(p.result() == Approx(std::acos(-1) * 0.25));

        p.parse("@tb ceil(10.1)");
        REQUIRE(p.result() == 11);

        p.parse("@tb floor(10.1)");
        REQUIRE(p.result() == 10);

        p.parse("@tb round(10.1)");
        REQUIRE(p.result() == 10);

        p.parse("@tb round(10.5)");
        REQUIRE(p.result() == 11);

        p.parse("@tb log10(1000)");
        REQUIRE(p.result() == 3);

        p.parse("@tb log2(512)");
        REQUIRE(p.result() == 9);

        p.parse("@tb frac(0)");
        REQUIRE(p.result() == 0);

        p.parse("@tb frac(0.125)");
        REQUIRE(p.result() == 0.125);

        p.parse("@tb frac(1)");
        REQUIRE(p.result() == 0);

        p.parse("@tb frac(-0.25)");
        REQUIRE(p.result() == -0.25);

        p.parse("@tb frac(1.125)");
        REQUIRE(p.result() == 0.125);

        REQUIRE(p.parse("@tb wrap(3.75)"));
        REQUIRE(p.result() == 0.75);

        REQUIRE(p.parse("@tb wrap(3.5,2.25)"));
        REQUIRE(p.result() == 1.25);

        REQUIRE(p.parse("@tb wrap(3.5,4,5)"));
        REQUIRE(p.result() == 4.5);

        REQUIRE(p.parse("@tb fold(0)"));
        REQUIRE(p.result() == 0);

        REQUIRE(p.parse("@tb fold(0.5)"));
        REQUIRE(p.result() == 0.5);

        REQUIRE(p.parse("@tb fold(1)"));
        REQUIRE(p.result() == 1);

        REQUIRE(p.parse("@tb fold(1.25)"));
        REQUIRE(p.result() == 0.75);

        REQUIRE(p.parse("@tb fold(1.75)"));
        REQUIRE(p.result() == 0.25);

        REQUIRE(p.parse("@tb fold(2)"));
        REQUIRE(p.result() == 0);

        REQUIRE(p.parse("@tb fold(-1)"));
        REQUIRE(p.result() == 1);

        REQUIRE(p.parse("@tb fold(1,3)"));
        REQUIRE(p.result() == 1);

        REQUIRE(p.parse("@tb fold(3.5,3)"));
        REQUIRE(p.result() == 2.5);

        REQUIRE(p.parse("@tb fold(5,3)"));
        REQUIRE(p.result() == 1);

        REQUIRE(p.parse("@tb rand(0.74)"));
        auto res = p.result() >= 0 && p.result() < 1;
        REQUIRE(res);

        REQUIRE(p.parse("@tb rand(100)"));
        res = p.result() >= 0 && p.result() < 100;
        REQUIRE(res);

        REQUIRE(p.parse("@tb rand(100,105)"));
        res = p.result() >= 100 && p.result() < 105;
        REQUIRE(res);
    }

    SECTION("byte code")
    {
        GrainExprParser p;
        REQUIRE(p.parse("@tb +=1+2+3+4+5+6"));
        REQUIRE(p.result() == 21);

        ByteCode bc = p.byteCode();
        double res = 0;
        REQUIRE(bc.eval(res));
        REQUIRE(res == 21);
        REQUIRE(bc.eval(res));
        REQUIRE(res == 42);
    }

    SECTION("vars")
    {
        Grain g;
        GrainExprParser p(&g);

        REQUIRE(p.parse("@tb $sr"));
        REQUIRE(p.result() == 48000);

        REQUIRE(p.parse("@tb $sr/2"));
        REQUIRE(p.result() == 24000);

        REQUIRE(p.parse("@tb $amp"));
        REQUIRE(p.result() == 1);

        g.setAmplitude(0.5);
        REQUIRE(p.parse("@tb $amp*100"));
        REQUIRE(p.result() == 50);

        REQUIRE(p.parse("@p $pan"));
        REQUIRE(p.result() == 0.5);

        g.setPan(0.25);
        REQUIRE(p.parse("@tb $pan/2"));
        REQUIRE(p.result() == 0.125);

        g.setLengthInSamples(14);
        REQUIRE(p.parse("@l $l*2"));
        REQUIRE(p.result() == 28);

        g.setSpeed(-5);
        REQUIRE(p.parse("@s $speed"));
        REQUIRE(p.result() == -5);

        g.setSpeed(-2);
        REQUIRE(p.parse("@speed $speed*-4"));
        REQUIRE(p.result() == 8);

        REQUIRE(p.parse("@tb $n"));
        REQUIRE(p.result() == 0);

        g.done();
        REQUIRE(p.parse("@tb $n*3"));
        REQUIRE(p.result() == 3);
    }

    SECTION("stack overflow")
    {
        GrainExprParser p;
        REQUIRE(p.parse("@tb 1+1+1+1+1+1+1+1+1+1"));
        REQUIRE(p.result() == 10);

        REQUIRE(p.parse("@tb 1+1+1+1+1+1+1+1+1+1+1+1"));
        REQUIRE(p.result() == 12);

        REQUIRE_FALSE(p.parse("@tb 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1"));
    }

    SECTION("prop parser")
    {
        Grain g;
        GrainExprParser p(&g);

        SECTION("time")
        {
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb 10")));
            REQUIRE(g.timeBefore() == 10);

            REQUIRE(p.parse(AtomList::parseString("@tb 12samp")));
            REQUIRE(g.timeBefore() == 12);

            REQUIRE(p.parse(AtomList::parseString("@tb 1s")));
            REQUIRE(g.timeBefore() == 48000);

            REQUIRE(p.parse(AtomList::parseString("@tb 0.5s")));
            REQUIRE(g.timeBefore() == 24000);

            REQUIRE(p.parse(AtomList::parseString("@tb 10ms")));
            REQUIRE(g.timeBefore() == 480);

            REQUIRE(p.parse(AtomList::parseString("@tb 10.5ms")));
            REQUIRE(g.timeBefore() == 504);
        }

        SECTION("time set")
        {
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb =10")));
            REQUIRE(g.timeBefore() == 10);

            REQUIRE(p.parse(AtomList::parseString("@tb =12samp")));
            REQUIRE(g.timeBefore() == 12);

            REQUIRE(p.parse(AtomList::parseString("@tb =1s")));
            REQUIRE(g.timeBefore() == 48000);

            REQUIRE(p.parse(AtomList::parseString("@tb =0.5s")));
            REQUIRE(g.timeBefore() == 24000);

            REQUIRE(p.parse(AtomList::parseString("@tb =10ms")));
            REQUIRE(g.timeBefore() == 480);

            REQUIRE(p.parse(AtomList::parseString("@tb =10.5ms")));
            REQUIRE(g.timeBefore() == 504);
        }

        SECTION("time add")
        {
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb +=10")));
            REQUIRE(g.timeBefore() == 10);

            REQUIRE(p.parse(AtomList::parseString("@tb +=12samp")));
            REQUIRE(g.timeBefore() == 22);

            REQUIRE(p.parse(AtomList::parseString("@tb +=1s")));
            REQUIRE(g.timeBefore() == 48022);

            REQUIRE(p.parse(AtomList::parseString("@tb -=0.5s")));
            REQUIRE(g.timeBefore() == 24022);

            REQUIRE(p.parse(AtomList::parseString("@tb +=10ms")));
            REQUIRE(g.timeBefore() == 24502);

            REQUIRE(p.parse(AtomList::parseString("@tb -=10.0msec")));
            REQUIRE(g.timeBefore() == 24022);

            REQUIRE(p.parse(AtomList::parseString("@tb -=5sec")));
            REQUIRE(g.timeBefore() == 0);
        }

        SECTION("time random")
        {
            REQUIRE(g.timeBefore() == 0);

            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@tb rand(0\\,100)")));
            REQUIRE(g.timeBefore() == 54);

            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@tb rand(0s\\,1sec)")));
            REQUIRE(g.timeBefore() == 26343);
        }

        SECTION("time choice")
        {
            REQUIRE(g.timeBefore() == 0);

            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@tb choice(1\\,2\\,3)")));
            REQUIRE(g.timeBefore() == 1);

            REQUIRE(p.parse(AtomList::parseString("@tb choice(1\\,2\\,3)")));
            REQUIRE(g.timeBefore() == 2);

            REQUIRE(p.parse(AtomList::parseString("@tb choice(1\\,2\\,3)")));
            REQUIRE(g.timeBefore() == 1);

            REQUIRE(p.parse(AtomList::parseString("@tb choice(1\\,2\\,3)")));
            REQUIRE(g.timeBefore() == 2);

            REQUIRE(p.parse(AtomList::parseString("@tb choice(1\\,2\\,3)")));
            REQUIRE(g.timeBefore() == 2);
        }

        SECTION("time ondone")
        {
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb ondone 600")));
            REQUIRE(g.timeBefore() == 0);

            g.done();
            REQUIRE(g.timeBefore() == 600);
            g.setTimeBefore(100);
            g.done();
            REQUIRE(g.timeBefore() == 600);

            REQUIRE(p.parse(AtomList::parseString("@tb ondone -=250")));
            REQUIRE(g.timeBefore() == 600);

            g.done();
            REQUIRE(g.timeBefore() == 350);
            g.done();
            REQUIRE(g.timeBefore() == 100);
            g.done();
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb ondone =480")));
            REQUIRE(g.timeBefore() == 0);

            g.done();
            REQUIRE(g.timeBefore() == 480);

            g.setTimeBefore(0);
            g.done();
            REQUIRE(g.timeBefore() == 480);

            REQUIRE(p.parse(AtomList::parseString("@tb ondone =100msec")));
            REQUIRE(g.timeBefore() == 480);
            g.done();
            REQUIRE(g.timeBefore() == 4800);
        }

        SECTION("time expr")
        {
            REQUIRE(g.timeBefore() == 0);

            REQUIRE(p.parse(AtomList::parseString("@tb 10*$pi")));
            REQUIRE(g.timeBefore() == 31);

            REQUIRE(p.parse(AtomList::parseString("@tb +=10*$pi")));
            REQUIRE(g.timeBefore() == 62);

            REQUIRE(g.durationInSamples() == 0);
            REQUIRE(p.parse(AtomList::parseString("@l 500")));
            REQUIRE(g.durationInSamples() == 500);

            REQUIRE(p.parse(AtomList::parseString("@l =$l/10")));
            REQUIRE(g.durationInSamples() == 50);

            REQUIRE(p.parse(AtomList::parseString("@tb =$l*10")));
            REQUIRE(g.timeBefore() == 500);

            REQUIRE(p.parse(AtomList::parseString("@tb 20")));
            REQUIRE(g.timeBefore() == 20);

            REQUIRE(p.parse(AtomList::parseString("@l 30")));
            REQUIRE(g.durationInSamples() == 30);

            REQUIRE(p.parse(AtomList::parseString("@tb 200 @l 300")));
            REQUIRE(g.timeBefore() == 200);
            REQUIRE(g.durationInSamples() == 300);

            REQUIRE(p.parse(AtomList::parseString("@l 1000 @tb $l/10")));
            REQUIRE(g.durationInSamples() == 1000);
            REQUIRE(g.timeBefore() == 100);

            REQUIRE(p.parse(AtomList::parseString("@tb 0 @l 10 @tb ondone =$l*12")));
            REQUIRE(g.timeBefore() == 0);
            REQUIRE(g.durationInSamples() == 10);
            g.done();
            REQUIRE(g.timeBefore() == 120);
            g.setLengthInSamples(15);
            g.done();
            REQUIRE(g.timeBefore() == 12 * 15);
        }

        SECTION("pan")
        {
            REQUIRE(g.pan() == 0.5);
            REQUIRE(p.parse(AtomList::parseString("@pan 0.25")));
            REQUIRE(g.pan() == 0.25);

            REQUIRE(p.parse(AtomList::parseString("@pan 0.125")));
            REQUIRE(g.pan() == 0.125);

            REQUIRE(p.parse(AtomList::parseString("@pan -0.125")));
            REQUIRE(g.pan() == 0);

            REQUIRE(p.parse(AtomList::parseString("@pan 1.125")));
            REQUIRE(g.pan() == 1);

            REQUIRE(p.parse(AtomList::parseString("@pan +=0.25")));
            REQUIRE(g.pan() == 1);

            REQUIRE(p.parse(AtomList::parseString("@pan -=0.25")));
            REQUIRE(g.pan() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("@pan -=1.25")));
            REQUIRE(g.pan() == 0);

            REQUIRE(p.parse(AtomList::parseString("@pan =0.75")));
            REQUIRE(g.pan() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan 0.875")));
            REQUIRE(g.pan() == 0.875);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan 0.99999")));
            REQUIRE(g.pan() == Approx(0.99999));

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan 1")));
            REQUIRE(g.pan() == 0);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan 1.25")));
            REQUIRE(g.pan() == 0.25);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan 100.25")));
            REQUIRE(g.pan() == 0.25);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow wrap @pan -0.1")));
            REQUIRE(g.pan() == Approx(0.9));

            // fold
            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 0.875")));
            REQUIRE(g.pan() == 0.875);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 0.99999")));
            REQUIRE(g.pan() == Approx(0.99999));

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 1")));
            REQUIRE(g.pan() == 1);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 1.25")));
            REQUIRE(g.pan() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 2")));
            REQUIRE(g.pan() == Approx(0));

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan 2.25")));
            REQUIRE(g.pan() == Approx(0.25));

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan -1.25")));
            REQUIRE(g.pan() == Approx(0.75));

            REQUIRE(p.parse(AtomList::parseString("@pan overflow fold @pan -2.25")));
            REQUIRE(g.pan() == Approx(0.25));

            REQUIRE(p.parse(AtomList::parseString("@pan mode none")));
            REQUIRE(g.panMode() == GRAIN_PAN_NONE);
            REQUIRE(p.parse(AtomList::parseString("@pan mode lin")));
            REQUIRE(g.panMode() == GRAIN_PAN_LINEAR);
            REQUIRE(p.parse(AtomList::parseString("@pan mode sqrt")));
            REQUIRE(g.panMode() == GRAIN_PAN_SQRT);
        }

        SECTION("pan random")
        {
            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@pan rand(1)")));
            REQUIRE(g.pan() == Approx(0.54881));

            REQUIRE(p.parse(AtomList::parseString("@pan =rand()")));
            REQUIRE(g.pan() == Approx(0.59284));

            REQUIRE(p.parse(AtomList::parseString("@pan =rand(-1\\,1)")));
            REQUIRE(g.pan() == Approx(0.43038));
        }

        SECTION("pan ondone")
        {
            REQUIRE(p.parse(AtomList::parseString("@pan ondone 0.75")));
            REQUIRE(g.pan() == Approx(0.5));
            g.done();
            REQUIRE(g.pan() == 0.75);
            g.done();
            REQUIRE(g.pan() == 0.75);

            REQUIRE(p.parse(AtomList::parseString("@pan 0.25 @pan ondone 0.875")));
            REQUIRE(g.pan() == Approx(0.25));
            g.done();
            REQUIRE(g.pan() == 0.875);
            g.done();
            REQUIRE(g.pan() == 0.875);

            REQUIRE(p.parse(AtomList::parseString("@pan 0.5 @pan ondone +=0.125")));
            REQUIRE(g.pan() == Approx(0.5));
            g.done();
            REQUIRE(g.pan() == 0.625);
            g.done();
            REQUIRE(g.pan() == 0.75);

            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@pan 0 @pan ondone rand()")));
            REQUIRE(g.pan() == 0);
            g.done();
            REQUIRE(g.pan() == Approx(0.54881));
        }

        SECTION("interp")
        {
            REQUIRE(g.playInterpolation() == GRAIN_INTERP_NONE);
            REQUIRE(p.parse(AtomList::parseString("@interp lin")));
            REQUIRE(g.playInterpolation() == GRAIN_INTERP_LINEAR);
            REQUIRE(p.parse(AtomList::parseString("@interp cubic")));
            REQUIRE(g.playInterpolation() == GRAIN_INTERP_CUBIC);
            REQUIRE(p.parse(AtomList::parseString("@interp none")));
            REQUIRE(g.playInterpolation() == GRAIN_INTERP_NONE);
        }

        SECTION("amp")
        {
            GrainRandom::seed(0);
            REQUIRE(p.parse(AtomList::parseString("@amp =0.25")));
            REQUIRE(g.amplitude() == Approx(0.25));

            REQUIRE(p.parse(AtomList::parseString("@amp =clip($amp+0.25\\,2\\,5)")));
            REQUIRE(g.amplitude() == Approx(2));

            REQUIRE(p.parse(AtomList::parseString("@amp =rand(2\\,10)")));
            REQUIRE(g.amplitude() == Approx(6.39051));

            REQUIRE(p.parse(AtomList::parseString("@amp =clip(1\\,2\\,3)")));
            REQUIRE(g.amplitude() == 2);

            REQUIRE(p.parse(AtomList::parseString("@amp =clip(4\\,2\\,3)")));
            REQUIRE(g.amplitude() == 3);

            REQUIRE(p.parse(AtomList::parseString("@amp =clip(2.25\\,2\\,3)")));
            REQUIRE(g.amplitude() == 2.25);
        }

        SECTION("amp ondone")
        {
            REQUIRE(p.parse(AtomList::parseString("@amp $n @amp ondone =$n")));
            REQUIRE(g.amplitude() == 0);
            g.done();
            REQUIRE(g.amplitude() == 1);
            g.done();
            REQUIRE(g.amplitude() == 2);
        }

        SECTION("win")
        {
            REQUIRE(p.parse(AtomList::parseString("@w tri")));
            REQUIRE(g.winType() == GRAIN_WIN_TRI);

            REQUIRE(p.parse(AtomList::parseString("@w rect")));
            REQUIRE(g.winType() == GRAIN_WIN_RECT);

            REQUIRE(p.parse(AtomList::parseString("@w hann")));
            REQUIRE(g.winType() == GRAIN_WIN_HANN);

            REQUIRE(p.parse(AtomList::parseString("@w trpz")));
            REQUIRE(g.winType() == GRAIN_WIN_TRPZ);

            REQUIRE(p.parse(AtomList::parseString("@w trpz(2)")));
            REQUIRE(g.winType() == GRAIN_WIN_TRPZ);
            REQUIRE(g.winParam() == 2);

            REQUIRE(p.parse(AtomList::parseString("@w trpz(2000.25)")));
            REQUIRE(g.winType() == GRAIN_WIN_TRPZ);
            REQUIRE(g.winParam() == 2000.25);
        }

        SECTION("tag")
        {
            REQUIRE(p.parse(AtomList::parseString("@tag abc")));
            REQUIRE(g.tag() == SYM("abc"));
        }
    }

    SECTION("play")
    {
        constexpr size_t SZ = 64;
        auto cnv = PureData::instance().findCanvas("test_canvas");
        ArrayPtr aptr = cnv->createArray("array_gexp1", SZ);
        aptr->fillWith([](size_t i) -> t_float { return i; });

        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(1);
        g.setLengthInSamples(9);
        g.setTimeBefore(0);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 1);
        REQUIRE(bl[2] == 2);
        REQUIRE(bl[3] == 3);
        REQUIRE(g.currentLogicPlayPos() == 4);
        REQUIRE(g.currentAbsPlayPos() == 4);
        REQUIRE(g.currentArrayPlayPos() == 4);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(br[0] == 4);
        REQUIRE(br[1] == 5);
        REQUIRE(br[2] == 6);
        REQUIRE(br[3] == 7);
        REQUIRE(g.currentLogicPlayPos() == 8);
        REQUIRE(g.currentAbsPlayPos() == 8);
        REQUIRE(g.currentArrayPlayPos() == 8);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 8);
        REQUIRE(br[1] == 0);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);
        REQUIRE(g.currentLogicPlayPos() == 0);
        REQUIRE(g.currentAbsPlayPos() == 0);
        REQUIRE(g.currentArrayPlayPos() == 0);
    }

    SECTION("play 2")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(1);
        g.setLengthInSamples(9);
        g.setArrayPosInSamples(3);
        g.setTimeBefore(0);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 3);
        REQUIRE(bl[1] == 4);
        REQUIRE(bl[2] == 5);
        REQUIRE(bl[3] == 6);
        REQUIRE(g.currentLogicPlayPos() == 4);
        REQUIRE(g.currentAbsPlayPos() == 4);
        REQUIRE(g.currentArrayPlayPos() == 7);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(br[0] == 7);
        REQUIRE(br[1] == 8);
        REQUIRE(br[2] == 9);
        REQUIRE(br[3] == 10);
        REQUIRE(g.currentLogicPlayPos() == 8);
        REQUIRE(g.currentAbsPlayPos() == 8);
        REQUIRE(g.currentArrayPlayPos() == 11);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 11);
        REQUIRE(br[1] == 0);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);
        REQUIRE(g.currentLogicPlayPos() == 0);
        REQUIRE(g.currentAbsPlayPos() == 0);
        REQUIRE(g.currentArrayPlayPos() == 3);
    }

    SECTION("play 3")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(2);
        g.setLengthInSamples(10);
        g.setArrayPosInSamples(3);
        g.setTimeBefore(0);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 3);
        REQUIRE(bl[1] == 5);
        REQUIRE(bl[2] == 7);
        REQUIRE(bl[3] == 9);
        REQUIRE(g.currentLogicPlayPos() == 8);
        REQUIRE(g.currentAbsPlayPos() == 8);
        REQUIRE(g.currentArrayPlayPos() == 11);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 11);
        REQUIRE(br[1] == 0);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);
        REQUIRE(g.currentLogicPlayPos() == 0);
        REQUIRE(g.currentAbsPlayPos() == 0);
        REQUIRE(g.currentArrayPlayPos() == 3);
    }

    SECTION("play 4")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(0.5);
        g.setLengthInSamples(5);
        g.setArrayPosInSamples(3);
        g.setTimeBefore(0);
        g.setPanMode(GRAIN_PAN_NONE);
        g.setPlayInterpolation(GRAIN_INTERP_LINEAR);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 3);
        REQUIRE(bl[1] == 3.5);
        REQUIRE(bl[2] == 4);
        REQUIRE(bl[3] == 4.5);
        REQUIRE(g.currentLogicPlayPos() == 2);
        REQUIRE(g.currentAbsPlayPos() == 2);
        REQUIRE(g.currentArrayPlayPos() == 5);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(br[0] == 5);
        REQUIRE(br[1] == 5.5);
        REQUIRE(br[2] == 6);
        REQUIRE(br[3] == 6.5);
        REQUIRE(g.currentLogicPlayPos() == 4);
        REQUIRE(g.currentAbsPlayPos() == 4);
        REQUIRE(g.currentArrayPlayPos() == 7);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 7);
        REQUIRE(br[1] == 7.5);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);
        REQUIRE(g.currentLogicPlayPos() == 0);
        REQUIRE(g.currentAbsPlayPos() == 0);
        REQUIRE(g.currentArrayPlayPos() == 3);
    }

    SECTION("play 5")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(2);
        g.setLengthInSamples(10);
        g.setArrayPosInSamples(3);
        g.setTimeBefore(3);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 0);
        REQUIRE(bl[2] == 0);
        REQUIRE(bl[3] == 3);
        REQUIRE(g.currentLogicPlayPos() == 2);
        REQUIRE(g.currentAbsPlayPos() == 2);
        REQUIRE(g.currentArrayPlayPos() == 5);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 5);
        REQUIRE(br[1] == 7);
        REQUIRE(br[2] == 9);
        REQUIRE(br[3] == 11);
        REQUIRE(g.currentLogicPlayPos() == -3);
        REQUIRE(g.currentAbsPlayPos() == -3);
        REQUIRE(g.currentArrayPlayPos() == 0);
    }

    SECTION("play 6")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 8;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(1);
        g.setLengthInSamples(8);
        g.setArrayPosInSamples(3);
        g.setTimeBefore(2);
        g.setTimeAfter(8);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 0);
        REQUIRE(bl[2] == 3);
        REQUIRE(bl[3] == 4);
        REQUIRE(bl[4] == 5);
        REQUIRE(bl[5] == 6);
        REQUIRE(bl[6] == 7);
        REQUIRE(bl[7] == 8);
        REQUIRE(g.currentLogicPlayPos() == 6);
        REQUIRE(g.currentAbsPlayPos() == 6);
        REQUIRE(g.currentArrayPlayPos() == 9);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 9);
        REQUIRE(bl[1] == 10);
        REQUIRE(bl[2] == 0);
        REQUIRE(bl[3] == 0);
        REQUIRE(bl[4] == 0);
        REQUIRE(bl[5] == 0);
        REQUIRE(bl[6] == 0);
        REQUIRE(bl[7] == 0);
        REQUIRE(g.currentLogicPlayPos() == 14);
        REQUIRE(g.currentAbsPlayPos() == 14);
        REQUIRE(g.currentArrayPlayPos() == 17);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 0);
        REQUIRE(bl[2] == 0);
        REQUIRE(bl[3] == 0);
        REQUIRE(bl[4] == 0);
        REQUIRE(bl[5] == 0);
        REQUIRE(bl[6] == 0);
        REQUIRE(bl[7] == 0);
        REQUIRE(g.currentLogicPlayPos() == -2);
        REQUIRE(g.currentAbsPlayPos() == -2);
        REQUIRE(g.currentArrayPlayPos() == 1);
    }

    SECTION("play 6")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        t_sample br[BS] = { 0 };
        t_sample bl[BS] = { 0 };
        t_sample* buf[] = { br, bl };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setSpeed(-1);
        g.setLengthInSamples(6);
        g.setArrayPosInSamples(0);
        g.setTimeBefore(0);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_PLAYING);
        REQUIRE(bl[0] == 5);
        REQUIRE(bl[1] == 4);
        REQUIRE(bl[2] == 3);
        REQUIRE(bl[3] == 2);
        REQUIRE(g.currentLogicPlayPos() == 4);
        REQUIRE(g.currentAbsPlayPos() == 1);
        REQUIRE(g.currentArrayPlayPos() == 1);

        std::fill(br, br + BS, 0);
        std::fill(bl, bl + BS, 0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(br[0] == 1);
        REQUIRE(br[1] == 0);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);
        REQUIRE(g.currentLogicPlayPos() == 0);
        REQUIRE(g.currentAbsPlayPos() == 5);
        REQUIRE(g.currentArrayPlayPos() == 5);

        REQUIRE(g.doneCounter() == 1);
    }

    SECTION("play 6")
    {
        constexpr size_t SZ = 64;
        auto cnv = PureData::instance().findCanvas("test_canvas");
        ArrayPtr aptr = cnv->createArray("array_gexp2", SZ);
        aptr->fillWith(1);
        Grain g;

        constexpr size_t BS = 8;
        constexpr size_t SR = 16;
        std::array<t_sample, BS> br, bl;
        br.fill(0);
        bl.fill(0);
        t_sample* buf[] = { br.data(), bl.data() };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setLengthInSamples(8);
        g.setPanMode(GRAIN_PAN_NONE);

        REQUIRE_FALSE(g.hasModulation(GRAIN_PROP_AMP));

        g.setModulation(GRAIN_PROP_AMP, GrainPropModulator(GRAIN_MOD_NONE, 1, 0, 1));

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 1);
        REQUIRE(bl[1] == 1);
        REQUIRE(bl[2] == 1);
        REQUIRE(bl[3] == 1);
        REQUIRE(bl[4] == 1);
        REQUIRE(bl[5] == 1);
        REQUIRE(bl[6] == 1);
        REQUIRE(bl[7] == 1);

        br.fill(0);
        bl.fill(0);

        g.start(0);
        g.setModulation(GRAIN_PROP_AMP, GrainPropModulator(GRAIN_MOD_SAW, 2, -1, 1));
        REQUIRE(g.hasModulation(GRAIN_PROP_AMP));

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == -1);
        REQUIRE(bl[1] == -0.75);
        REQUIRE(bl[2] == -0.5);
        REQUIRE(bl[3] == -0.25);
        REQUIRE(bl[4] == 0);
        REQUIRE(bl[5] == 0.25);
        REQUIRE(bl[6] == 0.5);
        REQUIRE(bl[7] == 0.75);

        br.fill(0);
        bl.fill(0);

        g.start(0);
        g.setModulation(GRAIN_PROP_AMP, GrainPropModulator(GRAIN_MOD_SIN, 2, -1, 1));
        REQUIRE(g.hasModulation(GRAIN_PROP_AMP));

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == Approx(0));
        REQUIRE(bl[1] == Approx(0.707107));
        REQUIRE(bl[2] == Approx(1));
        REQUIRE(bl[3] == Approx(0.707107));
        REQUIRE(bl[4] == Approx(0).margin(0.0001));
        REQUIRE(bl[5] == Approx(-0.707107));
        REQUIRE(bl[6] == Approx(-1));
        REQUIRE(bl[7] == Approx(-0.707107));

        br.fill(0);
        bl.fill(0);

        g.start(0);
        g.setModulation(GRAIN_PROP_AMP, GrainPropModulator(GRAIN_MOD_SQR, 4, -1, 0.5));
        REQUIRE(g.hasModulation(GRAIN_PROP_AMP));

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == -1);
        REQUIRE(bl[1] == -1);
        REQUIRE(bl[2] == 0.5);
        REQUIRE(bl[3] == 0.5);
        REQUIRE(bl[4] == -1);
        REQUIRE(bl[5] == -1);
        REQUIRE(bl[6] == 0.5);
        REQUIRE(bl[7] == 0.5);

        br.fill(0);
        bl.fill(0);

        g.start(0);
        g.setModulation(GRAIN_PROP_AMP, GrainPropModulator(GRAIN_MOD_TRI, 2, -1, 1));
        REQUIRE(g.hasModulation(GRAIN_PROP_AMP));

        REQUIRE(g.process(aptr->begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 1);
        REQUIRE(bl[1] == 0.5);
        REQUIRE(bl[2] == 0);
        REQUIRE(bl[3] == -0.5);
        REQUIRE(bl[4] == -1);
        REQUIRE(bl[5] == -0.5);
        REQUIRE(bl[6] == 0);
        REQUIRE(bl[7] == 0.5);
    }

    SECTION("play 7")
    {
        constexpr size_t SZ = 64;
        Array arr("array_gexp1");
        Grain g;

        constexpr size_t BS = 4;
        constexpr size_t SR = 48000;
        std::array<t_sample, BS> br, bl;
        br.fill(0);
        bl.fill(0);
        t_sample* buf[] = { bl.data(), br.data() };

        g.setArraySizeInSamples(SZ);
        g.setPlayStatus(GRAIN_PLAYING);
        g.setLengthInSamples(4);
        g.setPanMode(GRAIN_PAN_NONE);
        g.setPan(0);

        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 1);
        REQUIRE(bl[2] == 2);
        REQUIRE(bl[3] == 3);
        REQUIRE(br[0] == 0);
        REQUIRE(br[1] == 1);
        REQUIRE(br[2] == 2);
        REQUIRE(br[3] == 3);

        br.fill(0);
        bl.fill(0);

        g.setPanMode(GRAIN_PAN_LINEAR);
        g.setPan(0);
        REQUIRE(g.pan() == 0);

        g.start(0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 1);
        REQUIRE(bl[2] == 2);
        REQUIRE(bl[3] == 3);
        REQUIRE(br[0] == 0);
        REQUIRE(br[1] == 0);
        REQUIRE(br[2] == 0);
        REQUIRE(br[3] == 0);

        br.fill(0);
        bl.fill(0);

        g.setPan(1);

        g.start(0);
        REQUIRE(g.process(arr.begin(), SZ, buf, BS, SR) == GRAIN_FINISHED);
        REQUIRE(bl[0] == 0);
        REQUIRE(bl[1] == 0);
        REQUIRE(bl[2] == 0);
        REQUIRE(bl[3] == 0);
        REQUIRE(br[0] == 0);
        REQUIRE(br[1] == 1);
        REQUIRE(br[2] == 2);
        REQUIRE(br[3] == 3);

        br.fill(0);
        bl.fill(0);
    }
}
