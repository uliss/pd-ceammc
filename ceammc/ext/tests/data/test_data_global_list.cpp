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
#include "datatype_mlist.h"
#include "global_list.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(GlobalList, global, list)

TEST_CASE("global.list", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TestGlobalList t("global.list");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
        REQUIRE_PROPERTY_FLOAT(t, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
    }

    SECTION("do")
    {
        TestExtGlobalList t1("global.list", LA("a1"));
        REQUIRE(t1.object());

        TestExtGlobalList t2("global.list", LA("a1"));
        REQUIRE(t2.object());

        TestExtGlobalList t3("global.list", LA("a2"));
        REQUIRE(t2.object());

        REQUIRE_PROPERTY_FLOAT(t1, @.obj_refs, 2);
        REQUIRE_PROPERTY_FLOAT(t1, @.obj_refs, 2);
        REQUIRE_PROPERTY_FLOAT(t3, @.obj_refs, 1);

        t1 << BANG;
        REQUIRE(t1.outputListAt(0) == L());

        t2 << BANG;
        REQUIRE(t1.outputListAt(0) == L());

        t1 << "SYMBOL";
        REQUIRE(t1.outputListAt(0) == LA("SYMBOL"));
        t2 << BANG;
        REQUIRE(t2.outputListAt(0) == LA("SYMBOL"));

        t1 << LF(1, 2, 3);
        REQUIRE(t1.outputListAt(0) == LF(1, 2, 3));
        t2 << BANG;
        REQUIRE(t2.outputListAt(0) == LF(1, 2, 3));
        t3 << BANG;
        REQUIRE(t3.outputListAt(0) == L());

        REQUIRE_PROPERTY_FLOAT(t1, @size, 3);
        REQUIRE_PROPERTY_FLOAT(t2, @size, 3);
        REQUIRE_PROPERTY_FLOAT(t3, @size, 0);
        REQUIRE_PROPERTY_FLOAT(t1, @empty, 0);
        REQUIRE_PROPERTY_FLOAT(t2, @empty, 0);
        REQUIRE_PROPERTY_FLOAT(t3, @empty, 1);

        t2 << 0.f;
        REQUIRE(t2.outputFloatAt(0) == 1);
        t2 << 1.f;
        REQUIRE(t2.outputFloatAt(0) == 2);
        t2 << 2.f;
        REQUIRE(t2.outputFloatAt(0) == 3);
        t2 << 3.f;
        REQUIRE_FALSE(t2.hasOutput());
        t2 << -1.f;
        REQUIRE(t2.outputFloatAt(0) == 3);
        t2 << -2.f;
        REQUIRE(t2.outputFloatAt(0) == 2);

        t3 << 0.f;
        REQUIRE_FALSE(t3.hasOutput());
        t3 << -1.f;
        REQUIRE_FALSE(t3.hasOutput());
        t3 << 1.f;
        REQUIRE_FALSE(t3.hasOutput());
    }

    SECTION("iface")
    {
        TestExtGlobalList t("global.list", LA("a1"));
        REQUIRE(t.object());

        t << LF(5, 4, 3, 2, 1);
        REQUIRE(t.outputListAt(0) == LF(5, 4, 3, 2, 1));

        t.call("sort");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5));

        t.call("reverse");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(5, 4, 3, 2, 1));

        t.call("set", LF(1, 1, 2));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 1, 2));

        t.call("pop");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 1));

        t.call("clear");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        t.call("pop");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        t.call("append");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == L());

        t.call("append", LF(1, 2));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2));

        t.call("append", LF(3));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3));

        t.call("append", LF(4, 5));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(1, 2, 3, 4, 5));

        t.call("prepend", LA("A"));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("A", 1, 2, 3, 4, 5));

        t.call("prepend", LA("B", "C"));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("B", "C", "A", 1, 2, 3, 4, 5));

        t.call("remove");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("B", "C", "A", 1, 2, 3, 4, 5));

        t.call("remove", LF(0));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 3, 4, 5));

        t.call("remove", LF(-1));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 3, 4));

        t.call("remove", LF(-2));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 4));

        t.call("remove", LF(-200));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 4));

        t.call("remove", LF(5));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 4));

        t.call("remove", LF(-6));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 4));

        t.call("insert");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("C", "A", 1, 2, 4));

        t.call("insert", LA(0.f, "ABC"));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("ABC", "C", "A", 1, 2, 4));

        t.call("insert", LA(-1, "END"));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("ABC", "C", "A", 1, 2, "END", 4));

        t.call("fill");
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LA("ABC", "C", "A", 1, 2, "END", 4));

        t.call("fill", LF(11));
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputListAt(0) == LF(11, 11, 11, 11, 11, 11, 11));
    }
}
