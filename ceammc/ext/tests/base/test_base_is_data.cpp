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
#include "ceammc_pd.h"
#include "datatype_dict.h"
#include "datatype_string.h"
#include "is_data.h"
#include "mod_base.h"

#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(IsData, is, data)

TEST_CASE("is_data", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TExt t("is_data");
        REQUIRE_PROPERTY(t, @in_list, 0);
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("simple")
    {
        TExt t("is_data");

        t.bang();
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << 125.5;
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << "abc";
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << L();
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << LF(1);
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << LF(1, 2, 3);
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t.call("test", LF(1, 2));
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t.call("@prop");
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t.call("@prop?");
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << StringAtom("test");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == StringAtom("test"));

        t << DictAtom("[a: b]");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == DictAtom("[a: b]"));

        t << LA(StringAtom("a"), StringAtom("b"));
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));
    }

    SECTION("@in_list")
    {
        TExt t("is_data", LA("@in_list"));
        REQUIRE_PROPERTY(t, @in_list, 1);

        t << StringAtom("test");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == StringAtom("test"));

        t << DictAtom("[a: b]");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == DictAtom("[a: b]"));

        t << LA(StringAtom("a"), StringAtom("b"));
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(listAt(t, 1_out) == LA(StringAtom("a"), StringAtom("b")));

        t.call("@in_list?");
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));
    }
}
