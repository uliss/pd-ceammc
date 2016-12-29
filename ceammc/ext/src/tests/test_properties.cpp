/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "ceammc_property.h"

using namespace ceammc;

struct prop_ro {
    AtomList get() { return AtomList::values(1, 100.f); }
};

struct prop_rw {
    AtomList lst;
    AtomList get() { return lst; }
    void set(const AtomList& l) { lst = l; }
};

TEST_CASE("Properties", "[ceammc::properties]")
{
    SECTION("float property")
    {
        FloatProperty p("test", 0.1f);
        REQUIRE(!p.readonly());
        REQUIRE(p.name() == "test");
        p.setName("a");
        REQUIRE(p.name() == "a");
        AtomList v = p.get();

        REQUIRE(v.size() == 1);
        REQUIRE(v[0].isFloat());
        REQUIRE(v[0].asFloat(0.0f) == 0.1f);

        REQUIRE_FALSE(p.set(AtomList()));
        REQUIRE(p.set(AtomList::ones(2)));

        AtomList al;
        al.append(gensym("a"));
        REQUIRE_FALSE(p.set(al));

        FloatProperty p2("test2", 123123, true);
        REQUIRE_FALSE(p2.set(AtomList::ones(1)));
    }

    SECTION("list property")
    {
        ListProperty p("test", AtomList::values(3, -1.f, -2.f, -3.f), false);
        REQUIRE(!p.readonly());
        REQUIRE(p.name() == "test");
        REQUIRE(p.get() == AtomList::values(3, -1.f, -2.f, -3.f));
        REQUIRE(p.set(AtomList()));
        REQUIRE(p.get() == AtomList());

        ListProperty p2("test2", AtomList::values(3, -1.f, -2.f, -3.f), true);
        REQUIRE(p2.readonly());
        REQUIRE(p2.name() == "test2");
        REQUIRE(p2.get() == AtomList::values(3, -1.f, -2.f, -3.f));
        REQUIRE_FALSE(p2.set(AtomList()));
        REQUIRE_FALSE(p2.get() == AtomList());
    }

    SECTION("atom property")
    {
        AtomProperty p("test", Atom(1.5f));
        REQUIRE(!p.readonly());
        REQUIRE(p.name() == "test");
        REQUIRE(p.get() == AtomList::values(1, 1.5f));
        REQUIRE_FALSE(p.set(AtomList::zeroes(0)));
        REQUIRE(p.set(AtomList::zeroes(10)));
        REQUIRE(p.get() == AtomList::zeroes(1));

        AtomProperty p2("test2", Atom(1.5f), true);
        REQUIRE(p2.readonly());
        REQUIRE(p2.name() == "test2");
        REQUIRE_FALSE(p2.set(AtomList::ones(10)));
    }

    SECTION("atom cb property")
    {
        prop_ro r1;
        CallbackProperty<prop_ro> p1("test", &r1, &prop_ro::get);
        REQUIRE(p1.name() == "test");
        REQUIRE(p1.readonly() == true);
        REQUIRE(p1.get() == AtomList::values(1, 100.f));
        REQUIRE_FALSE(p1.set(AtomList()));

        prop_rw r2;
        CallbackProperty<prop_rw> p2("test2", &r2, &prop_rw::get, &prop_rw::set);
        REQUIRE(p2.name() == "test2");
        REQUIRE(p2.readonly() == false);
        REQUIRE(p2.get() == AtomList());
        REQUIRE(p2.set(AtomList::ones(5)));
        REQUIRE_FALSE(p2.set(AtomList()));
        REQUIRE(p2.get() == AtomList::ones(5));
    }
}
