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
#include "flow_split.h"
#include "test_flow_base.h"

/// test object
static t_class* is_any;
struct t_is_any {
    t_object x_obj;
    t_outlet* o;
};

void* x_is_any_new()
{
    t_is_any* x = (t_is_any*)pd_new(is_any);
    outlet_new(&x->x_obj, &s_float);
    return x;
}
void x_is_any_not_any(t_is_any* x) { outlet_float(x->x_obj.te_outlet, 0); }
void x_is_any_any(t_is_any* x) { outlet_float(x->x_obj.te_outlet, 1); }
void x_is_any_setup()
{
    is_any = class_new(gensym("isany"), (t_newmethod)x_is_any_new, 0, sizeof(t_is_any), 0, A_NULL);
    class_addbang(is_any, (t_method)x_is_any_not_any);
    class_addfloat(is_any, (t_method)x_is_any_not_any);
    class_addsymbol(is_any, (t_method)x_is_any_not_any);
    class_addlist(is_any, (t_method)x_is_any_not_any);
    class_addanything(is_any, (t_method)x_is_any_any);
}

PD_COMPLETE_TEST_SETUP(FlowSplit, flow, split)

TEST_CASE("flow.split", "[externals]")
{
    pd_test_init();
    x_is_any_setup();

    SECTION("init")
    {
        TObj t("flow.split");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 3);
    }

    SECTION("bang")
    {
        TObj t("flow.split");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_BANG_AT_OUTLET(2, t);
        REQUIRE_BANG_AT_OUTLET(1, t);
    }

    SECTION("float")
    {
        TObj t("flow.split");
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(2, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 2);
    }

    SECTION("symbol")
    {
        TObj t("flow.split");
        WHEN_SEND_SYMBOL_TO(0, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(2, t, "a");
        REQUIRE_SYMBOL_AT_OUTLET(1, t, "a");
    }

    SECTION("list")
    {
        TObj t("flow.split");
        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(2, t, LF(1, 2, 3));
        REQUIRE_LIST_AT_OUTLET(1, t, LF(1, 2, 3));
    }

    SECTION("any")
    {
        TObj t("flow.split");
        WHEN_SEND_ANY_TO(t, "go", LF(1, 2));
        REQUIRE_ANY_AT_OUTLET(2, t, LA("go", 1, 2));
        REQUIRE_ANY_AT_OUTLET(1, t, LA("go", 1, 2));
    }

    SECTION("@props")
    {
        TExt t("flow.split");

        pd::External isa("isany");
        REQUIRE(isa.object());
        t.connectTo(2, isa, 0);
        t.connectFrom(0, isa, 1);

        t <<= LA("@abc", 1);
        REQUIRE(t.messagesAt(0) == MessageList { Message(SYM("@abc"), LF(1)) });
        REQUIRE(t.messagesAt(1).empty());

        t.clearAll();

        t << 123;
        REQUIRE(t.messagesAt(0).empty());
        REQUIRE(t.messagesAt(1) == MessageList { Message(123) });
    }
}
