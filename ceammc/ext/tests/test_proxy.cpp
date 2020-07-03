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
#include "test_base.h"
#include "ceammc_pd.h"
#include "ceammc_proxy.h"

#include "catch.hpp"

using namespace ceammc;

class TestProxy {
public:
    TestProxy()
        : f(0)
    {
    }

    float f;
    AtomList l;

    void onFloat(t_float v)
    {
        f = v;
    }

    void onList(const AtomList& lst)
    {
        l = lst;
    }
};

typedef PdFloatProxy<TestProxy> TestFloatProxy;
typedef PdListProxy<TestProxy> TestListProxy;

TEST_CASE("proxy", "[ceammc::proxy]")
{
    SECTION("float")
    {
        TestProxy t;
        REQUIRE(t.f == 0);

        TestFloatProxy p(&t, &TestProxy::onFloat);

        p.onFloat(10);
        REQUIRE(t.f == 10);

        pd_float(p.pd(), -100);
        REQUIRE(t.f == -100);
    }

    SECTION("list")
    {
        TestProxy t;
        REQUIRE(t.f == 0);

        TestListProxy p(&t, &TestProxy::onList);

        p.onList(LF(1, 2, 3));
        REQUIRE(t.l == LF(1, 2, 3));

        pd_list(p.pd(), &s_list, 2, LA("A", 100).toPdData());

        REQUIRE(t.l == LA("A", 100));
    }

    SECTION("bind")
    {
        TestProxy t;
        REQUIRE(t.f == 0);

        TestFloatProxy p(&t, &TestProxy::onFloat);
        p.bind(gensym("s1"));

        pd_float(gensym("s1")->s_thing, 10);
        REQUIRE(t.f == 10);

        p.bind(gensym("s1"));
        pd_float(gensym("s1")->s_thing, 11);
        REQUIRE(t.f == 11);

        p.bind(gensym("s2"));
        REQUIRE(!gensym("s1")->s_thing);
        REQUIRE(gensym("s2")->s_thing);

        pd_float(gensym("s2")->s_thing, 12);
        REQUIRE(t.f == 12);

        p.unbind();
        REQUIRE_FALSE(p.isBinded());
        p.unbind();
        REQUIRE_FALSE(p.isBinded());
        REQUIRE(!gensym("s2")->s_thing);

        p.bind(gensym("s2"));
        REQUIRE(gensym("s2")->s_thing);
        REQUIRE(p.isBinded());
        p.bind(gensym("s2"));
        REQUIRE(p.isBinded());
    }
}
