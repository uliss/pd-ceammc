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
#include "global_set.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(GlobalSet, global, set)

#define REQUIRE_SET(obj, ...)                              \
    {                                                      \
        obj << BANG;                                       \
        REQUIRE(obj.outputAtomAt(0).isData());             \
        REQUIRE(obj.outputAtomAt(0) == SetA(__VA_ARGS__)); \
    }

#define REQUIRE_EMPTY_SET(obj)                  \
    {                                           \
        obj << BANG;                            \
        REQUIRE(obj.outputAtomAt(0).isData());  \
        REQUIRE(obj.outputAtomAt(0) == SetA()); \
    }

TEST_CASE("global.set", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TestGlobalSet t("global.set");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_PROPERTY_LIST(t, @id, LA("default"));
    }

    SECTION("do")
    {
        TExt t1("global.set", LA("s1"));
        REQUIRE(t1.object());

        TExt t2("global.set", LA("s1"));
        REQUIRE(t2.object());

        TExt t3("global.set", LA("s2"));
        REQUIRE(t2.object());

        REQUIRE(t1->refCount() == 2);
        REQUIRE(t2->refCount() == 2);
        REQUIRE(t3->refCount() == 1);

        t1 << BANG;
        REQUIRE_EMPTY_SET(t1);
        REQUIRE_PROPERTY_FLOAT(t1, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t1, @empty, 1);

        t1 << 1.f;
        REQUIRE_FALSE(t1.hasOutput());
        REQUIRE_SET(t1, 1);
        REQUIRE_SET(t2, 1);

        REQUIRE_PROPERTY_FLOAT(t1, @size, 1);
        REQUIRE_PROPERTY_FLOAT(t1, @empty, 0);
        REQUIRE_PROPERTY_FLOAT(t2, @size, 1);
        REQUIRE_PROPERTY_FLOAT(t2, @empty, 0);
        REQUIRE_PROPERTY_FLOAT(t3, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t3, @empty, 1);

        t1 << "ABC";
        REQUIRE_FALSE(t1.hasOutput());
        REQUIRE_SET(t1, 1, "ABC");
        REQUIRE_SET(t2, 1, "ABC");

        t1 << LF(1, 1, 1, 2, 3, 2);
        REQUIRE_FALSE(t1.hasOutput());
        REQUIRE_SET(t1, 1, 2, 3, "ABC");
        REQUIRE_SET(t2, 1, 2, 3, "ABC");
    }

    SECTION("iface")
    {
        TestExtGlobalSet t("global.set", LA("a1"));
        REQUIRE(t.object());

        t << LF(1, 2, 3);
        REQUIRE_SET(t, 1, 2, 3);

        t.call("add");
        REQUIRE_SET(t, 1, 2, 3);

        t.call("add", LF(1, 2, 3, 4));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_SET(t, 1, 2, 3, 4);

        t.call("remove", LF(4, 3));
        REQUIRE_SET(t, 1, 2);

        t.call("remove");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_SET(t, 1, 2);

        t.call("clear");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_EMPTY_SET(t);

        t.call("set");
        REQUIRE_EMPTY_SET(t);

        t.call("set", LF(1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_SET(t, 1, 2, 3);
    }
}
