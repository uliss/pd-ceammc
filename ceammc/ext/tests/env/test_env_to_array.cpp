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
#include "env_to_array.h"
//#include "test_env_base.h"
#include "test_external.h"

extern "C" {
#include "s_stuff.h"
}

#define REQUIRE_ZERO_ARRAY(arr)                    \
    {                                              \
        for (size_t i = 0; i < arr->size(); i++) { \
            REQUIRE(arr->at(i) == 0);              \
        }                                          \
    }

using TObj = TestExternal<Env2Array>;

TEST_CASE("env.to_array", "[externals]")
{
    setup_env_to_array();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("init")
    {
        SECTION("empty")
        {
            TObj t("env.to_array");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @mode, "fit");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@mode", "raw"));
            REQUIRE_PROPERTY(t, @mode, "raw");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@mode", "fit"));
            REQUIRE_PROPERTY(t, @mode, "fit");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@mode", "resize"));
            REQUIRE_PROPERTY(t, @mode, "resize");
        }

        SECTION("@prop invalid")
        {
            TObj t("env.to_array", LA("@mode", "??resize"));
            REQUIRE_PROPERTY(t, @mode, "fit");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@fit"));
            REQUIRE_PROPERTY(t, @mode, "fit");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@raw"));
            REQUIRE_PROPERTY(t, @mode, "raw");
        }

        SECTION("@prop")
        {
            TObj t("env.to_array", LA("@resize"));
            REQUIRE_PROPERTY(t, @mode, "resize");
        }
    }

    SECTION("on data")
    {
        SECTION("fit")
        {
            ArrayPtr aptr = cnv->createArray("array1", 101);
            REQUIRE_ZERO_ARRAY(aptr);

            TObj t("env->array", LA("array1", "@fit"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
            REQUIRE_ZERO_ARRAY(aptr);

            DataTypeEnv env;
            env.setAR(6000, 14000);
            WHEN_SEND_TDATA_TO(0, t, env);
            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(15) == Approx(0.5));
            REQUIRE(aptr->at(30) == Approx(1));
            REQUIRE(aptr->at(65) == Approx(0.5));
            REQUIRE(aptr->at(100) == 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 101);
        }

        SECTION("resize")
        {
            ArrayPtr aptr = cnv->createArray("array2", 10);
            REQUIRE_ZERO_ARRAY(aptr);

            TObj t("env->array", LA("array2", "@resize"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
            REQUIRE_ZERO_ARRAY(aptr);

            DataTypeEnv env;
            env.setAR(2500, 7500);
            sys_setchsr(1, 1, 40000);
            REQUIRE(sys_getsr() == 40000);
            REQUIRE(aptr->size() == 10);
            WHEN_SEND_TDATA_TO(0, t, env);
            aptr->update();
            REQUIRE(aptr->size() == 401);
            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(50) == Approx(0.5));
            REQUIRE(aptr->at(100) == Approx(1));
            REQUIRE(aptr->at(250) == Approx(0.5));
            REQUIRE(aptr->at(400) == 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 401);
        }

        SECTION("resize + shift")
        {
            ArrayPtr aptr = cnv->createArray("array5", 10);
            REQUIRE_ZERO_ARRAY(aptr);

            TObj t("env->array", LA("array5", "@resize"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
            REQUIRE_ZERO_ARRAY(aptr);

            DataTypeEnv env;
            env.setAR(2500, 7500);
            env.shiftTime(2500);
            sys_setchsr(1, 1, 40000);
            REQUIRE(sys_getsr() == 40000);
            REQUIRE(aptr->size() == 10);
            WHEN_SEND_TDATA_TO(0, t, env);
            aptr->update();
            REQUIRE(aptr->size() == 501);
            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(100) == 0);
            REQUIRE(aptr->at(150) == Approx(0.5));
            REQUIRE(aptr->at(200) == Approx(1));
            REQUIRE(aptr->at(350) == Approx(0.5));
            REQUIRE(aptr->at(500) == 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 501);
        }

        SECTION("raw")
        {
            ArrayPtr aptr = cnv->createArray("array3", 100);
            REQUIRE_ZERO_ARRAY(aptr);

            TObj t("env->array", LA("array3", "@raw"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
            REQUIRE_ZERO_ARRAY(aptr);

            DataTypeEnv env;
            env.setAR(250, 750);
            sys_setchsr(1, 1, 40000);
            REQUIRE(sys_getsr() == 40000);
            REQUIRE(aptr->size() == 100);
            WHEN_SEND_TDATA_TO(0, t, env);
            aptr->update();
            REQUIRE(aptr->size() == 100);
            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(5) == Approx(0.5));
            REQUIRE(aptr->at(10) == Approx(1));
            REQUIRE(aptr->at(25) == Approx(0.5));
            REQUIRE(aptr->at(40) == 0);
            REQUIRE(aptr->at(41) == 0);
            REQUIRE(aptr->at(42) == 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        }

        SECTION("raw + shift")
        {
            ArrayPtr aptr = cnv->createArray("array4", 100);
            REQUIRE_ZERO_ARRAY(aptr);

            TObj t("env->array", LA("array4", "@raw"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeEnv());
            REQUIRE_ZERO_ARRAY(aptr);
            REQUIRE_NO_MSG(t);

            DataTypeEnv env;
            env.setAR(250, 750);
            env.shiftTime(250);
            sys_setchsr(1, 1, 40000);
            REQUIRE(sys_getsr() == 40000);
            REQUIRE(aptr->size() == 100);
            WHEN_SEND_TDATA_TO(0, t, env);
            aptr->update();
            REQUIRE(aptr->size() == 100);
            REQUIRE(aptr->at(0) == 0);
            REQUIRE(aptr->at(1) == 0);
            REQUIRE(aptr->at(2) == 0);
            REQUIRE(aptr->at(3) == 0);
            REQUIRE(aptr->at(4) == 0);
            REQUIRE(aptr->at(5) == 0);
            REQUIRE(aptr->at(6) == 0);
            REQUIRE(aptr->at(7) == 0);
            REQUIRE(aptr->at(8) == 0);
            REQUIRE(aptr->at(9) == 0);
            REQUIRE(aptr->at(10) == 0);
            REQUIRE(aptr->at(15) == Approx(0.5));
            REQUIRE(aptr->at(20) == Approx(1));
            REQUIRE(aptr->at(35) == Approx(0.5));
            REQUIRE(aptr->at(50) == 0);
            REQUIRE(aptr->at(51) == 0);
            REQUIRE(aptr->at(52) == 0);
            REQUIRE_FLOAT_AT_OUTLET(0, t, 100);
        }
    }
}
