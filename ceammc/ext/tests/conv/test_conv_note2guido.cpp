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
#include "conv_note2guido.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvNote2Guido, conv, note2guido)

using namespace ceammc;

#define N2G(obj, in, out)                      \
    {                                          \
        WHEN_SEND_FLOAT_TO(0, obj, float(in)); \
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, out); \
    }

TEST_CASE("conv.note2guido", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        TObj t("conv.amp2dbfs");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("convert float")
    {
        TObj t("conv.note2guido");

        N2G(t, 60, "c1");
        N2G(t, 61, "c#1");
        N2G(t, 59, "b0");
        N2G(t, -1, "_");
        N2G(t, 60.5, "c1");
        N2G(t, 60.25, "c1");
        N2G(t, 60.125, "c1");
    }

    SECTION("convert list")
    {
        TObj t("conv.note2guido");

        t.sendList(LA("msg", "_", 60, -1, 37));
        REQUIRE_LIST_AT_OUTLET(0, t, LA("msg", "_", "c1", "_", "c#-1"));
    }

    SECTION("note")
    {
        TExt t("conv.note2guido");

        t.call("note", LF(63, 4.5));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "d#1/4.");
        t.call("note", LF(63, 4.25));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "d#1/4..");
        t.call("note", LF(63, 4.125));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "d#1/4...");
        t.call("note", LF(-1, 4.125));
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "_/4...");
    }
}
