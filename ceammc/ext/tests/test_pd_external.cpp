/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ceammc_externals.h"
#include "ceammc_pd.h"
#include "test_base.h"
#include "test_external.h"

#include "catch.hpp"

using namespace ceammc;
using namespace ceammc::pd;

TEST_CASE("pd external", "[pd::External]")
{
    SECTION("coords")
    {
        External t("*");
        REQUIRE(t.xPos() == 0);
        REQUIRE(t.yPos() == 0);
    }

    SECTION("connectTo")
    {
        External t("+");
        REQUIRE(t.object());
        REQUIRE_FALSE(t.connectTo(0, NULL, 0));
        REQUIRE_FALSE(is_ceammc(t.object()));
        REQUIRE_FALSE(t.isCeammc());
        REQUIRE_FALSE(t.isCeammcBase());
        REQUIRE_FALSE(t.isCeammcUI());
        REQUIRE_FALSE(t.isCeammcFaust());
        REQUIRE_FALSE(t.isCeammcFlext());
        REQUIRE(t.asCeammcBaseObject() == nullptr);

        External t1("NONE");
        REQUIRE(t1.isNull());
        REQUIRE_FALSE(t1.connectTo(0, NULL, 0));

        REQUIRE_FALSE(t.connectTo(0, t1, 0));
        REQUIRE_FALSE(t.connectTo(0, t, 0));

        External t2("*");
        REQUIRE_FALSE(t.connectTo(1, t2, 0));
        REQUIRE_FALSE(t.connectTo(0, t2, 2));

        REQUIRE(t.connectTo(0, t2, 1));
        REQUIRE(t.connectTo(0, t2, 0));
    }

    SECTION("connectFrom")
    {
        External t1("NONE");
        External t2("+");
        External t3("swap");

        REQUIRE(t1.isNull());
        REQUIRE(!t2.isNull());
        REQUIRE(!t2.isNull());

        REQUIRE_FALSE(t1.connectFrom(0, t1, 0));
        REQUIRE_FALSE(t1.connectFrom(0, t2, 0));
        REQUIRE_FALSE(t1.connectFrom(0, t2, 10));

        REQUIRE_FALSE(t2.connectFrom(0, t1, 1));
        REQUIRE_FALSE(t2.connectFrom(2, t3, 0));
        REQUIRE_FALSE(t2.connectFrom(0, t3, 2));
    }

    SECTION("abstraction")
    {
        External t1(TEST_DATA_DIR "/test_canvas_01");
        REQUIRE_FALSE(t1.isNull());
        REQUIRE(t1.numInlets() == 1);
        REQUIRE(t1.numOutlets() == 2);
    }

    SECTION("object functions")
    {
        REQUIRE(pd::object_class(nullptr) == nullptr);
        REQUIRE(pd::object_name(nullptr) == &s_);
        REQUIRE(pd::object_dir(nullptr) == &s_);

        External t("+");
        REQUIRE(pd::object_class(t.object()));
        REQUIRE(pd::object_name(t.object()) == gensym("+"));
        REQUIRE(pd::object_dir(t.object()) == &s_);
    }

    SECTION("methods")
    {
        External t("delwrite~");
        REQUIRE_FALSE(t.isNull());
        REQUIRE_FALSE(t.methods().empty());
        REQUIRE(t.methods().size() == 2);
        REQUIRE(t.methods()[0]->s_name == std::string("dsp"));
        REQUIRE(t.methods()[1]->s_name == std::string("clear"));
    }

    SECTION("ceammc")
    {
    }

    SECTION("send")
    {
        External t("+");
        t.sendBang();
        t.sendFloat(10);
        t.sendList(LF(10, 20));

        t.sendBangTo(0);
        t.sendBangTo(1);
        t.sendFloatTo(-1, 0);
        t.sendFloatTo(-1, 1);
        t.sendListTo(LF(10, 20), 0);
        t.sendListTo(LF(10, 20), 1);
    }
}
