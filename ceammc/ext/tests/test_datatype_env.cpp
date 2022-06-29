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
#include "../env/datatype_env.h"
#include "../lib/ceammc_datatypes.h"

#include "test_base.h"

#include "catch.hpp"

#include <vector>

using namespace ceammc;

TEST_CASE("DataTypeEnv", "[ceammc::DataTypeEnv]")
{
    SECTION("init")
    {
        DataTypeEnv env;
        REQUIRE(env.typeName() == "Env");
        REQUIRE(env.empty());
        REQUIRE_FALSE(env.hasPointAtTime(0));
        REQUIRE_FALSE(env.hasPointAtTime(10));
        REQUIRE_FALSE(env.hasPointAtTime(-10));
        REQUIRE(env.pointAtTime(0) == 0);
        REQUIRE(env.pointAtTime(10) == 0);
        REQUIRE(env.pointAtTime(-10) == 0);
        REQUIRE(env.numPoints() == 0);
        REQUIRE(!env.hasStopPoints());
        REQUIRE(env.totalLength() == 0);

        SECTION("copy")
        {
            DataTypeEnv env1;
            env1.setADSR(10, 20, 0.10, 11);
            DataTypeEnv env2(env1);

            REQUIRE(env1.numPoints() == env2.numPoints());
            REQUIRE(env1.toString() == env2.toString());
        }
    }

    SECTION("insert")
    {
        DataTypeEnv env;
        REQUIRE(env.empty());

        REQUIRE(env.insertPoint(EnvelopePoint(20, 1)) == 0);
        REQUIRE(!env.empty());
        REQUIRE(env.pointAt(0).utime == 20);
        REQUIRE(env.hasPointAtTime(20));

        REQUIRE(env.insertPoint(EnvelopePoint(15, 1)) == 0);
        REQUIRE(env.numPoints() == 2);
        REQUIRE(env.hasPointAtTime(15));
        REQUIRE(env.pointAt(0).utime == 15);
        REQUIRE(env.pointAt(1).utime == 20);

        REQUIRE(!env.hasPointAtTime(21));
        REQUIRE(env.nextStopPoint(env.begin()) == env.end());
        REQUIRE(!env.hasStopPoints());

        env.pointAt(1).stop = true;
        REQUIRE(env.hasStopPoints());
        REQUIRE(env.nextStopPoint(env.begin()) != env.end());
        REQUIRE(env.nextStopPoint(env.begin()) == env.begin() + 1);
        REQUIRE(env.totalLength() == 5);
    }

    SECTION("valueAt")
    {
        SECTION("step")
        {
            DataTypeEnv env;
            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(100) == 0);

            env.insertPoint(EnvelopePoint(1, 0.1, false, CURVE_STEP));
            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == 0.1);
            REQUIRE(env.valueAtTime(2) == 0);

            env.insertPoint(EnvelopePoint(20, 1, false, CURVE_STEP));

            REQUIRE(env.valueAtTime(0) == 0);
            for (int i = 1; i < 20; i++)
                REQUIRE(env.valueAtTime(i) == 0.1);

            REQUIRE(env.valueAtTime(20) == 1);
            REQUIRE(env.valueAtTime(21) == 0);
        }

        SECTION("line")
        {
            DataTypeEnv env;
            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(100) == 0);
            REQUIRE(env.totalLength() == 0);

            env.insertPoint(EnvelopePoint(3, 2, false, CURVE_LINE));
            REQUIRE(env.totalLength() == 0);
            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == 0);
            REQUIRE(env.valueAtTime(2) == 0);
            REQUIRE(env.valueAtTime(3) == 2);
            REQUIRE(env.valueAtTime(4) == 0);

            env.insertPoint(EnvelopePoint(12, 8, false, CURVE_LINE));
            REQUIRE(env.totalLength() == 9);

            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == 0);
            REQUIRE(env.valueAtTime(2) == 0);
            REQUIRE(env.valueAtTime(3) == 2);
            REQUIRE(env.valueAtTime(6) == 4);
            REQUIRE(env.valueAtTime(9) == 6);
            REQUIRE(env.valueAtTime(10) == Approx(6.666666));
            REQUIRE(env.valueAtTime(12) == 8);
            REQUIRE(env.valueAtTime(13) == 0);
        }

        SECTION("pow")
        {
            DataTypeEnv env;

            EnvelopePoint p0(0, 0, false, CURVE_EXP);
            p0.data = 0;

            env.insertPoint(p0);
            env.insertPoint(EnvelopePoint(10, 10, false, CURVE_LINE));

            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == 1);
            REQUIRE(env.valueAtTime(2) == 2);
            REQUIRE(env.valueAtTime(3) == 3);
            REQUIRE(env.valueAtTime(4) == 4);
            REQUIRE(env.valueAtTime(5) == 5);
            REQUIRE(env.valueAtTime(6) == 6);
            REQUIRE(env.valueAtTime(7) == 7);
            REQUIRE(env.valueAtTime(8) == 8);
            REQUIRE(env.valueAtTime(9) == 9);
            REQUIRE(env.valueAtTime(10) == 10);

            env.pointAt(0).data = 1;

            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == Approx(0.6120702456));
            REQUIRE(env.valueAtTime(2) == Approx(1.2885124809));
            REQUIRE(env.valueAtTime(3) == Approx(2.036096767));
            REQUIRE(env.valueAtTime(4) == Approx(2.8623051789));
            REQUIRE(env.valueAtTime(5) == Approx(3.775406688));
            REQUIRE(env.valueAtTime(6) == Approx(4.7845399211));
            REQUIRE(env.valueAtTime(7) == Approx(5.8998046227));
            REQUIRE(env.valueAtTime(8) == Approx(7.132362737));
            REQUIRE(env.valueAtTime(9) == Approx(8.4945501197));
            REQUIRE(env.valueAtTime(10) == 10);
        }

        SECTION("sin2")
        {
            DataTypeEnv env;

            EnvelopePoint p0(0, 0, false, CURVE_SIN2);
            p0.data = 0;

            env.insertPoint(p0);
            env.insertPoint(EnvelopePoint(10, 10));

            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == Approx(0.2447174185));
            REQUIRE(env.valueAtTime(2) == Approx(0.9549150281));
            REQUIRE(env.valueAtTime(3) == Approx(2.0610737385));
            REQUIRE(env.valueAtTime(4) == Approx(3.4549150281));
            REQUIRE(env.valueAtTime(5) == Approx(5));
            REQUIRE(env.valueAtTime(6) == Approx(6.5450849719));
            REQUIRE(env.valueAtTime(7) == Approx(7.9389262615));
            REQUIRE(env.valueAtTime(8) == Approx(9.0450849719));
            REQUIRE(env.valueAtTime(9) == Approx(9.7552825815));
            REQUIRE(env.valueAtTime(10) == 10);
        }

        SECTION("sigmoid")
        {
            DataTypeEnv env;

            EnvelopePoint p0(0, 0, false, CURVE_SIGMOID);
            p0.sigmoid_skew = 16;

            env.insertPoint(p0);
            env.insertPoint(EnvelopePoint(10, 10));

            REQUIRE(env.valueAtTime(0) == 0);
            REQUIRE(env.valueAtTime(1) == Approx(0.0165880108));
            REQUIRE(env.valueAtTime(2) == Approx(0.0816257115));
            REQUIRE(env.valueAtTime(3) == Approx(0.391657228));
            REQUIRE(env.valueAtTime(4) == Approx(1.6798161487));
            REQUIRE(env.valueAtTime(5) == Approx(5));
            REQUIRE(env.valueAtTime(6) == Approx(8.3201838513));
            REQUIRE(env.valueAtTime(7) == Approx(9.608342772));
            REQUIRE(env.valueAtTime(8) == Approx(9.9183742885));
            REQUIRE(env.valueAtTime(9) == Approx(9.9834119892));
            REQUIRE(env.valueAtTime(10) == 10);
        }
    }

    SECTION("setAR")
    {
        DataTypeEnv env;
        env.setAR(100, 200);
        REQUIRE(!env.empty());
        REQUIRE(env.isAR());
        REQUIRE(env.isAR(true));
        REQUIRE(env.numPoints() == 3);
        REQUIRE(!env.hasStopPoints());
        REQUIRE(env.totalLength() == 300);
        REQUIRE(env.valueAtTime(0) == 0);
        REQUIRE(env.valueAtTime(25) == Approx(0.25));
        REQUIRE(env.valueAtTime(50) == Approx(0.5));
        REQUIRE(env.valueAtTime(75) == Approx(0.75));
        REQUIRE(env.valueAtTime(100) == 1);
        REQUIRE(env.valueAtTime(150) == 0.75);
        REQUIRE(env.valueAtTime(200) == 0.5);
        REQUIRE(env.valueAtTime(250) == 0.25);
        REQUIRE(env.valueAtTime(300) == 0);

        env.setAR(200, 100);
        REQUIRE(!env.empty());
        REQUIRE(env.numPoints() == 3);
        REQUIRE(!env.hasStopPoints());
        REQUIRE(env.totalLength() == 300);
        REQUIRE(env.valueAtTime(0) == 0);

        REQUIRE_FALSE(env.setAR(LF(1, 10, 20)));
        REQUIRE_FALSE(env.setAR(LF(10, -20)));
        REQUIRE_FALSE(env.setAR(LA(-10, 20)));
        REQUIRE(env.setAR(LF(10, 20)));
        REQUIRE(env.hasPointAtTime(0));
        REQUIRE(env.hasPointAtTime(10 * 1000));
        REQUIRE(env.hasPointAtTime(30 * 1000));
        REQUIRE(!env.hasStopPoints());

        REQUIRE(env.isAR());
        REQUIRE(env.isAR(true));
        REQUIRE_FALSE(env.isADSR());
        REQUIRE_FALSE(env.isADSR(true));
        REQUIRE_FALSE(env.checkASR());
        REQUIRE(env.checkAR());
        REQUIRE_FALSE(env.checkADSR());
    }

    SECTION("setASR")
    {
        DataTypeEnv env;
        env.setASR(100, 200);
        REQUIRE(!env.empty());
        REQUIRE(env.isAR());
        REQUIRE(env.isAR(true));
        REQUIRE(env.numPoints() == 3);
        REQUIRE(env.hasStopPoints());
        REQUIRE(env.totalLength() == 300);
        REQUIRE(env.nextStopIdx(0) == 1);
        REQUIRE(env.nextStopIdx(1) == 2);
        REQUIRE(env.nextStopIdx(2) == -1);

        REQUIRE_FALSE(env.setASR(LF(1, 10, 20)));
        REQUIRE_FALSE(env.setASR(LF(10, -20)));
        REQUIRE_FALSE(env.setASR(LA(-10, 20)));
        REQUIRE(env.setASR(LF(10, 20)));
        REQUIRE(env.hasPointAtTime(0));
        REQUIRE(env.hasPointAtTime(10 * 1000));
        REQUIRE(env.hasPointAtTime(30 * 1000));

        REQUIRE(env.isAR());
        REQUIRE(env.isAR(true));
        REQUIRE_FALSE(env.isADSR());
        REQUIRE_FALSE(env.isADSR(true));
        REQUIRE(env.checkASR());
        REQUIRE_FALSE(env.checkAR());
        REQUIRE_FALSE(env.checkADSR());
    }

    SECTION("setADSR")
    {
        DataTypeEnv env;

        REQUIRE_FALSE(env.setADSR(LF(1, 10, 20)));
        REQUIRE_FALSE(env.setADSR(LF(-10, 20, 10, 100)));
        REQUIRE_FALSE(env.setADSR(LF(10, -20, 10, 100)));
        REQUIRE_FALSE(env.setADSR(LF(10, 20, -1, 100)));
        REQUIRE_FALSE(env.setADSR(LF(10, 20, 101, 100)));
        REQUIRE_FALSE(env.setADSR(LF(10, 20, 10, -100)));
        REQUIRE(env.setADSR(LF(10, 20, 10, 70)));
        REQUIRE(env.hasPointAtTime(0));
        REQUIRE(env.hasPointAtTime(10 * 1000));
        REQUIRE(env.hasPointAtTime(30 * 1000));
        REQUIRE(env.hasPointAtTime(100 * 1000));
        REQUIRE(env.hasStopPoints());

        REQUIRE(env.isADSR());
        REQUIRE(env.isADSR(true));
        REQUIRE_FALSE(env.isAR());
        REQUIRE_FALSE(env.isAR(true));
        REQUIRE(env.checkADSR());
        REQUIRE_FALSE(env.checkASR());
        REQUIRE_FALSE(env.checkAR());
    }

    SECTION("render")
    {
        SECTION("empty")
        {
            DataTypeEnv env;
            std::vector<float> data(12, -1);
            env.render(data.begin(), data.end());
            REQUIRE(data == std::vector<float>(12, -1));
        }

        SECTION("simple")
        {
            DataTypeEnv env;
            env.setAR(20, 10);
            REQUIRE(env.numPoints() == 3);
            REQUIRE(env.hasPointAtTime(0));
            REQUIRE(env.hasPointAtTime(20));
            REQUIRE(env.hasPointAtTime(30));
            std::vector<float> data(13, -1);

            env.render(data.begin(), data.end());
            REQUIRE(data[0] == 0);
            REQUIRE(data[2] == 0.25);
            REQUIRE(data[4] == 0.5);
            REQUIRE(data[6] == 0.75);
            REQUIRE(data[8] == 1);
            REQUIRE(data[9] == 0.75);
            REQUIRE(data[10] == 0.5);
            REQUIRE(data[11] == 0.25);
            REQUIRE(data[12] == 0);

            data.resize(7);
            env.render(data.begin(), data.end());
            REQUIRE(data[0] == 0);
            REQUIRE(data[2] == 0.5);
            REQUIRE(data[4] == 1);
            REQUIRE(data[5] == 0.5);
            REQUIRE(data[6] == 0);
        }

        SECTION("complex")
        {
            SECTION("mixed1")
            {
                DataTypeEnv env;
                env.setAR(20, 10);
                env.pointAt(1).type = CURVE_STEP;

                std::vector<float> data(7, -1);
                env.render(data.begin(), data.end());
                REQUIRE(data[0] == 0);
                REQUIRE(data[1] == 0.25);
                REQUIRE(data[2] == 0.5);
                REQUIRE(data[3] == 0.75);
                REQUIRE(data[4] == 1);
                REQUIRE(data[5] == 1);
                REQUIRE(data[6] == 0);
            }

            SECTION("mixed2")
            {
                DataTypeEnv env;
                env.setAR(20, 10);
                env.pointAt(0).type = CURVE_EXP;
                env.pointAt(0).data = 1;
                env.pointAt(1).type = CURVE_EXP;
                env.pointAt(1).data = 0;

                std::vector<t_float> data(7, -1);
                env.render(data.begin(), data.end());
                REQUIRE(data[0] == 0);
                REQUIRE(data[1] == Approx(0.1653f).epsilon(0.001));
                REQUIRE(data[2] == Approx(0.37754f));
                REQUIRE(data[3] == Approx(0.65007f));
                REQUIRE(data[4] == 1);
                REQUIRE(data[5] == Approx(0.5));
                REQUIRE(data[6] == 0);
            }
        }
    }

    SECTION("nextStopPoint")
    {
        SECTION("iterator")
        {
            DataTypeEnv env;
            REQUIRE(env.nextStopPoint(env.begin()) == env.end());
            REQUIRE(env.nextStopPoint(env.end()) == env.end());

            env.setASR(10, 20);
            REQUIRE(env.nextStopPoint(env.begin()) != env.end());
            REQUIRE(env.nextStopPoint(env.end()) == env.end());

            const DataTypeEnv env2(env);
            REQUIRE(env2.pointAtTime(10) != 0);
            REQUIRE(env2.pointAtTime(1000) == 0);
        }

        SECTION("index")
        {
            DataTypeEnv env;
            REQUIRE(env.nextStopIdx(0) == -1);
            REQUIRE(env.nextStopIdx(2) == -1);

            // 7-point envelope
            env.setLine(LA(1, 20, 2, 20, 3, 20, 4) + LA(20, 5, 20, 6, 20, 7));
            env.pointAt(0).stop = true;

            REQUIRE(env.nextStopIdx(0) == 6);
            REQUIRE(env.nextStopIdx(6) == -1);

            env.pointAt(2).stop = true;
            REQUIRE(env.nextStopIdx(0) == 2);
            REQUIRE(env.nextStopIdx(2) == 6);
            REQUIRE(env.nextStopIdx(6) == -1);

            env.pointAt(3).stop = true;
            REQUIRE(env.nextStopIdx(0) == 2);
            REQUIRE(env.nextStopIdx(2) == 3);
            REQUIRE(env.nextStopIdx(3) == 6);
            REQUIRE(env.nextStopIdx(6) == -1);

            env.pointAt(5).stop = true;
            env.pointAt(6).stop = true;
            REQUIRE(env.nextStopIdx(4) == 5);
            REQUIRE(env.nextStopIdx(5) == 6);
            REQUIRE(env.nextStopIdx(6) == -1);
        }
    }

    SECTION("remove point")
    {
        SECTION("point")
        {
            DataTypeEnv env;
            REQUIRE_FALSE(env.removePointByTime(100));

            env.setAR(60, 40);
            REQUIRE_FALSE(env.removePointByTime(101));
            env.setAR(60, 40);

            REQUIRE(env.removePointByTime(100));
            REQUIRE(env.hasPointAtTime(0));
            REQUIRE(env.hasPointAtTime(60));
            REQUIRE_FALSE(env.hasPointAtTime(100));

            env.setAR(60, 40);
            REQUIRE_FALSE(env.removePointByTime(99));
            REQUIRE(env.hasPointAtTime(0));
            REQUIRE(env.hasPointAtTime(60));
            REQUIRE(env.hasPointAtTime(100));

            REQUIRE(env.removePointByTime(60));
            REQUIRE(env.hasPointAtTime(0));
            REQUIRE(env.hasPointAtTime(100));

            env.setAR(60, 40);
            REQUIRE(env.removePointByTime(0));
            REQUIRE(env.hasPointAtTime(60));
            REQUIRE(env.hasPointAtTime(100));
        }

        SECTION("index")
        {
            DataTypeEnv env;
            REQUIRE_FALSE(env.removePoint(0));
            env.setAR(10, 20);
            REQUIRE_FALSE(env.removePoint(3));
            REQUIRE(env.numPoints() == 3);
            REQUIRE(env.removePoint(0));
            REQUIRE(env.numPoints() == 2);
            REQUIRE(env.removePoint(0));
            REQUIRE(env.numPoints() == 1);
            REQUIRE(env.removePoint(0));
            REQUIRE(env.numPoints() == 0);
            REQUIRE_FALSE(env.removePoint(0));
        }
    }

    SECTION("num stop points")
    {
        DataTypeEnv env;
        REQUIRE(env.numStopPoints() == 0);

        env.setAR(10, 20);
        REQUIRE(env.numStopPoints() == 0);

        env.setASR(10, 40);
        REQUIRE(env.numStopPoints() == 1);

        env.clear();
        env.setADSR(10, 40, 1, 30);
        REQUIRE(env.numStopPoints() == 1);
    }

    SECTION("clone")
    {
        DataTypeEnv env1;
        DataTypeEnv env2;

        REQUIRE(env1.isEqual(&env2));

        env1.setAR(10, 20);
        REQUIRE_FALSE(env1.isEqual(&env2));
        env2.setADSR(10, 20, 30, 1);
        REQUIRE_FALSE(env1.isEqual(&env2));

        AbstractData* env3 = env2.clone();
        REQUIRE(env2.isEqual(env3));
        delete env3;
    }

    SECTION("resize")
    {
        DataTypeEnv env;
        REQUIRE(env.totalLength() == 0);

        env.resizeTime(2);
        env.setAR(10, 20);
        REQUIRE(env.totalLength() == 30);

        env.resizeTime(60);
        REQUIRE(env.totalLength() == 60);

        env.resizeTime(600);
        REQUIRE(env.totalLength() == 600);
        REQUIRE(env.hasPointAtTime(0));
        REQUIRE(env.hasPointAtTime(200));
        REQUIRE(env.hasPointAtTime(600));
    }

    SECTION("scale")
    {
        DataTypeEnv env;
        REQUIRE(env.totalLength() == 0);

        env.scaleTime(2);
        REQUIRE(env.totalLength() == 0);

        env.setAR(10, 20);
        env.scaleTime(2);
        REQUIRE(env.totalLength() == 60);

        env.scaleTime(10);
        REQUIRE(env.totalLength() == 600);
        REQUIRE(env.hasPointAtTime(0));
        REQUIRE(env.hasPointAtTime(200));
        REQUIRE(env.hasPointAtTime(600));

        env.scaleTime(-1);
        REQUIRE(env.totalLength() == 600);
    }

    SECTION("to string")
    {
        DataTypeEnv env;
        env.insertPoint(EnvelopePoint(0, 0, false, CURVE_EXP));
        REQUIRE(env.toString() == "Envelope:\n"
                                  "      point:        0(ms)        0 exp\n");

        std::ostringstream ss;
        ss << env;

        REQUIRE(env.toString() == ss.str());
    }

    SECTION("toList")
    {
        DataTypeEnv env;
        REQUIRE(env.toList() == L());
        env.setASR(10, 20, -10);

        REQUIRE(DataTypeEnv::fromListView(env.toList()) == env);

        AtomList lst;
        lst.append(S("EnvelopePoint"));
        lst.append(0.f);
        lst.append(0.f);
        lst.append(0.f);
        lst.append(16);
        lst.append(1);
        lst.append(0.f);

        lst.append(S("EnvelopePoint"));
        lst.append(10);
        lst.append(-10);
        lst.append(0.f);
        lst.append(16);
        lst.append(1);
        lst.append(1);

        lst.append(S("EnvelopePoint"));
        lst.append(30);
        lst.append(0.f);
        lst.append(0.f);
        lst.append(16);
        lst.append(1);
        lst.append(0.f);

        REQUIRE(env.toList() == lst);
    }

    SECTION("named envelope")
    {
        DataTypeEnv e;
        REQUIRE(e.isNamedEnvelope("ar"));
        REQUIRE(e.isNamedEnvelope("asr"));
        REQUIRE(e.isNamedEnvelope("adsr"));
        REQUIRE(e.isNamedEnvelope("ear"));
        REQUIRE(e.isNamedEnvelope("easr"));
        REQUIRE(e.isNamedEnvelope("eadsr"));
        REQUIRE(e.isNamedEnvelope("step"));
        REQUIRE(e.isNamedEnvelope("line"));
        REQUIRE(e.isNamedEnvelope("sin2"));
        REQUIRE(e.isNamedEnvelope("sigmoid"));
        REQUIRE(e.isNamedEnvelope("exp"));

        REQUIRE_FALSE(e.isNamedEnvelope("??R"));

        SECTION("set")
        {
            e.setNamedEnvelope("ar", LF(10, 20));
            REQUIRE(e.numPoints() == 3);
            REQUIRE(e.totalLength() == 30000);
            REQUIRE(e.numStopPoints() == 0);
            REQUIRE(e.isAR());

            e.setNamedEnvelope("asr", LF(20, 20));
            REQUIRE(e.numPoints() == 3);
            REQUIRE(e.totalLength() == 40000);
            REQUIRE(e.numStopPoints() == 1);
            REQUIRE(e.isAR());

            e.setNamedEnvelope("adsr", LF(20, 30, 10, 100));
            REQUIRE(e.numPoints() == 4);
            REQUIRE(e.totalLength() == 150000);
            REQUIRE(e.numStopPoints() == 1);

            e.setNamedEnvelope("ear", LF(10, -1, 20, -2));
            REQUIRE(e.numPoints() == 3);
            REQUIRE(e.totalLength() == 30000);
            REQUIRE(e.numStopPoints() == 0);
            REQUIRE_FALSE(e.isAR());

            e.setNamedEnvelope("easr", LF(20, -3, 20, -4));
            REQUIRE(e.numPoints() == 3);
            REQUIRE(e.totalLength() == 40000);
            REQUIRE(e.numStopPoints() == 1);
            REQUIRE_FALSE(e.isAR());

            e.setNamedEnvelope("eadsr", LF(20, -1, 30, -2, 10, 100, -5));
            REQUIRE(e.numPoints() == 4);
            REQUIRE(e.totalLength() == 150000);
            REQUIRE(e.numStopPoints() == 1);
            REQUIRE_FALSE(e.isAR());
        }
    }

    SECTION("fromList")
    {
        REQUIRE(DataTypeEnv::fromListView(L()).empty());
        REQUIRE(DataTypeEnv::fromListView(LF(1, 2)).empty());
        REQUIRE(DataTypeEnv::fromListView(LA("Not an EnvelopePoint", 1, 1, 1, 1, 1, 1)).empty());
        REQUIRE(DataTypeEnv::fromListView(LA("EnvelopePoint", 1, 1, 1, 1, 1, 1)).numPoints() == 1);

        DataTypeEnv env = DataTypeEnv::fromListView(LA("EnvelopePoint", 1, 100, 1, 1, 1, 1));
        REQUIRE(env.hasPointAtTime(1));
        REQUIRE(env.valueAtTime(1) == 100);
        REQUIRE(env.pointAt(0).type == CURVE_LINE);
    }

    SECTION("normalize")
    {
        DataTypeEnv e;
        REQUIRE(e.normalize() == e);

        e.setAR(100, 200);
        DataTypeEnv e1(e);

        e.shiftTime(10);
        REQUIRE_FALSE(e == e1);
        REQUIRE(e.hasPointAtTime(10));
        REQUIRE(e.hasPointAtTime(110));
        REQUIRE(e.hasPointAtTime(310));

        REQUIRE(e.normalize() == e1);
        REQUIRE_FALSE(e == e1);

        e.pointAt(1).value = 100;
        e.pointAt(0).value = -100;

        DataTypeEnv e2;
        e2.insertPoint(EnvelopePoint(0, 0));
        e2.insertPoint(EnvelopePoint(100, 1));
        e2.insertPoint(EnvelopePoint(300, 0.5));

        REQUIRE(e.normalize() == e2);
    }

    SECTION("symbol2curve")
    {
        CurveType t;
        REQUIRE(symbol2curve(gensym("step"), t));
        REQUIRE(t == CURVE_STEP);
        REQUIRE(symbol2curve(gensym("line"), t));
        REQUIRE(t == CURVE_LINE);
        REQUIRE(symbol2curve(gensym("exp"), t));
        REQUIRE(t == CURVE_EXP);
        REQUIRE(symbol2curve(gensym("sin2"), t));
        REQUIRE(t == CURVE_SIN2);
        REQUIRE(symbol2curve(gensym("sigmoid"), t));
        REQUIRE(t == CURVE_SIGMOID);
        REQUIRE_FALSE(symbol2curve(gensym("lin"), t));
        REQUIRE(t == CURVE_SIGMOID);
        REQUIRE_FALSE(symbol2curve(gensym("abc"), t));
    }

    SECTION("+=")
    {
        DataTypeEnv e0;
        DataTypeEnv e1;

        e0 += e1;

        REQUIRE(e0 == e1);
        REQUIRE(e0.empty());

        REQUIRE(e0 == e1);
        REQUIRE(e0.empty());

        e0.setAR(LF(20, 30));
        REQUIRE(!e0.empty());
        DataTypeEnv t(e0);

        e0 += e1;
        REQUIRE(e0 == t);
        e1 += e0;
        REQUIRE(e0 == e1);

        // self append (!)
        e0 += e0;
        REQUIRE(e0.numPoints() == 5);
        REQUIRE(e0.totalLength() == 100 * 1000);

        e0 = e0;
        REQUIRE(e0.numPoints() == 5);
    }

    SECTION("setStep")
    {
        DataTypeEnv e;
        REQUIRE(!e.setStep(L()));

        REQUIRE(e.setStep(LF(10)));
        REQUIRE(e.numPoints() == 1);
        REQUIRE(e.pointAt(0).value == 10);

        REQUIRE(!e.setStep(L()));
        REQUIRE(e.numPoints() == 1);

        REQUIRE(e.setStep(LA(0.1, 10, 0.9, 20, 0.7, 30, 0.4)));
        REQUIRE(e.numPoints() == 4);
        REQUIRE(e.pointAt(0).value == 0.1f);
        REQUIRE(e.pointAt(1).value == 0.9f);
        REQUIRE(e.pointAt(2).value == 0.7f);
        REQUIRE(e.pointAt(3).value == 0.4f);
        REQUIRE(e.pointAt(0).utime == 0 * 1000);
        REQUIRE(e.pointAt(1).utime == 10 * 1000);
        REQUIRE(e.pointAt(2).utime == 30 * 1000);
        REQUIRE(e.pointAt(3).utime == 60 * 1000);
        REQUIRE(e.pointAt(0).type == CURVE_STEP);
        REQUIRE(e.pointAt(1).type == CURVE_STEP);
        REQUIRE(e.pointAt(2).type == CURVE_STEP);
        REQUIRE(e.pointAt(3).type == CURVE_STEP);
    }

    SECTION("setLine")
    {
        DataTypeEnv e;
        REQUIRE(!e.setLine(L()));

        REQUIRE(e.setLine(LF(10)));
        REQUIRE(e.numPoints() == 1);
        REQUIRE(e.pointAt(0).value == 10);

        REQUIRE(!e.setLine(L()));
        REQUIRE(e.numPoints() == 1);

        REQUIRE(e.setLine(LA(0.1, 10, 0.9, 20, 0.7, 30, 0.4)));
        REQUIRE(e.numPoints() == 4);
        REQUIRE(e.pointAt(0).value == 0.1f);
        REQUIRE(e.pointAt(1).value == 0.9f);
        REQUIRE(e.pointAt(2).value == 0.7f);
        REQUIRE(e.pointAt(3).value == 0.4f);
        REQUIRE(e.pointAt(0).utime == 0 * 1000);
        REQUIRE(e.pointAt(1).utime == 10 * 1000);
        REQUIRE(e.pointAt(2).utime == 30 * 1000);
        REQUIRE(e.pointAt(3).utime == 60 * 1000);
        REQUIRE(e.pointAt(0).type == CURVE_LINE);
        REQUIRE(e.pointAt(1).type == CURVE_LINE);
        REQUIRE(e.pointAt(2).type == CURVE_LINE);
        REQUIRE(e.pointAt(3).type == CURVE_LINE);
    }

    SECTION("setExponential")
    {
        DataTypeEnv e;
        REQUIRE(!e.setExponential(L()));
        REQUIRE(!e.setExponential(LF(1)));
        REQUIRE(!e.setExponential(LF(1, 2)));
        REQUIRE(!e.setExponential(LF(1, 2, 3)));

        REQUIRE(e.setExponential(LA(0.1f, 10, -2, 1)));
        REQUIRE(e.numPoints() == 2);
        REQUIRE(e.pointAt(0).utime == 0);
        REQUIRE(e.pointAt(0).value == 0.1f);
        REQUIRE(e.pointAt(0).type == CURVE_EXP);
        REQUIRE(e.pointAt(0).data == -2);
        REQUIRE(e.pointAt(1).timeMs() == 10);
        REQUIRE(e.pointAt(1).value == 1);
        REQUIRE(e.pointAt(1).type == CURVE_EXP);
        REQUIRE(e.pointAt(1).data == 0);

        REQUIRE(e.setExponential(LF(0.1f, 10, -2) + LA(0.9, 20, -4, 0.2f)));
        REQUIRE(e.numPoints() == 3);
        REQUIRE(e.pointAt(0).timeMs() == 0);
        REQUIRE(e.pointAt(1).timeMs() == 10);
        REQUIRE(e.pointAt(2).timeMs() == 30);
        REQUIRE(e.pointAt(0).value == 0.1f);
        REQUIRE(e.pointAt(1).value == 0.9f);
        REQUIRE(e.pointAt(2).value == 0.2f);
    }

    SECTION("scaleValue")
    {
        DataTypeEnv e;
        e.scaleValue(0);
        REQUIRE(e.empty());
        e.scaleValue(2);
        REQUIRE(e.empty());

        REQUIRE(e.setADSR(LF(10, 20, 80, 60)));
        e.scaleValue(0.5);

        REQUIRE(e.numPoints() == 4);
        REQUIRE(e.pointAt(0).value == 0);
        REQUIRE(e.pointAt(1).value == Approx(0.5));
        REQUIRE(e.pointAt(2).value == Approx(0.4));
        REQUIRE(e.pointAt(3).value == 0);

        e.scaleValue(-2);

        REQUIRE(e.numPoints() == 4);
        REQUIRE(e.pointAt(0).value == 0);
        REQUIRE(e.pointAt(1).value == Approx(-1));
        REQUIRE(e.pointAt(2).value == Approx(-0.8));
        REQUIRE(e.pointAt(3).value == 0);

        e.scaleValue(0);
        REQUIRE(e.numPoints() == 4);
        REQUIRE(e.pointAt(0).value == 0);
        REQUIRE(e.pointAt(1).value == 0);
        REQUIRE(e.pointAt(2).value == 0);
        REQUIRE(e.pointAt(3).value == 0);
    }

    SECTION("concat")
    {
        DataTypeEnv e0, e1;

        REQUIRE(e0 + e1 == DataTypeEnv());

        DataTypeEnv adsr0;
        adsr0.setADSR(10, 20, 0.6, 100);
        REQUIRE(e0 + adsr0 == adsr0);
        REQUIRE(adsr0 + e0 == adsr0);

        DataTypeEnv easr0;
        easr0.setEASR(60, -2, 40, 16);

        DataTypeEnv sum0 = adsr0 + easr0;
        REQUIRE(sum0.numPoints() == 6);

        REQUIRE(sum0.pointAt(0).value == Approx(0));
        REQUIRE(sum0.pointAt(1).value == Approx(1));
        REQUIRE(sum0.pointAt(2).value == Approx(0.6));
        REQUIRE(sum0.pointAt(3).value == Approx(0));
        REQUIRE(sum0.pointAt(4).value == Approx(1));
        REQUIRE(sum0.pointAt(5).value == Approx(0));

        REQUIRE(sum0.pointAt(0).utime == 0);
        REQUIRE(sum0.pointAt(1).utime == 10);
        REQUIRE(sum0.pointAt(2).utime == 30);
        REQUIRE(sum0.pointAt(3).utime == 130);
        REQUIRE(sum0.pointAt(4).utime == 190);
        REQUIRE(sum0.pointAt(5).utime == 230);

        REQUIRE(sum0.pointAt(0).type == CURVE_LINE);
        REQUIRE(sum0.pointAt(1).type == CURVE_LINE);
        REQUIRE(sum0.pointAt(2).type == CURVE_LINE);
        REQUIRE(sum0.pointAt(3).type == CURVE_EXP);
        REQUIRE(sum0.pointAt(4).type == CURVE_EXP);
        REQUIRE(sum0.pointAt(5).type == CURVE_LINE);

        easr0.shiftTime(100);
        sum0 = adsr0 + easr0;
        REQUIRE(sum0.numPoints() == 6);
        REQUIRE(sum0.pointAt(0).utime == 0);
        REQUIRE(sum0.pointAt(1).utime == 10);
        REQUIRE(sum0.pointAt(2).utime == 30);
        REQUIRE(sum0.pointAt(3).utime == 130);
        REQUIRE(sum0.pointAt(4).utime == 190);
        REQUIRE(sum0.pointAt(5).utime == 230);

        adsr0.shiftTime(100);
        sum0 = adsr0 + easr0;
        REQUIRE(sum0.numPoints() == 6);
        REQUIRE(sum0.pointAt(0).utime == 100);
        REQUIRE(sum0.pointAt(1).utime == 110);
        REQUIRE(sum0.pointAt(2).utime == 130);
        REQUIRE(sum0.pointAt(3).utime == 230);
        REQUIRE(sum0.pointAt(4).utime == 290);
        REQUIRE(sum0.pointAt(5).utime == 330);

        DataTypeEnv step;
        step.insertPoint(EnvelopePoint(0, 0.5));
        step.insertPoint(EnvelopePoint(50, 0));

        sum0 = adsr0 + step;
        REQUIRE(sum0.numPoints() == 6);
        REQUIRE(sum0.pointAt(0).utime == 100);
        REQUIRE(sum0.pointAt(1).utime == 110);
        REQUIRE(sum0.pointAt(2).utime == 130);
        REQUIRE(sum0.pointAt(3).utime == 230);
        REQUIRE(sum0.pointAt(4).utime == 230);
        REQUIRE(sum0.pointAt(5).utime == 280);

        REQUIRE(sum0.pointAt(0).value == Approx(0));
        REQUIRE(sum0.pointAt(1).value == Approx(1));
        REQUIRE(sum0.pointAt(2).value == Approx(0.6));
        REQUIRE(sum0.pointAt(3).value == Approx(0));
        REQUIRE(sum0.pointAt(4).value == Approx(0.5));
        REQUIRE(sum0.pointAt(5).value == Approx(0));
    }
}
