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
    AtomList get() const { return AtomList::values(1, 100.f); }
};

struct prop_rw {
    AtomList lst;
    AtomList get() const { return lst; }
    void set(const AtomList& l) { lst = l; }
};

struct prop_t_rw {
    double sz;
    double getSize() const { return sz; }
    void setSize(const double& s) { sz = s; }
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

    SECTION("atom typed cb property")
    {
        AtomList v1;
        TypedCbProperty<size_t, AtomList> p1("@size", &v1, &AtomList::size);
        REQUIRE(p1.name() == "@size");
        REQUIRE(p1.readonly() == true);

        REQUIRE(p1.get() == AtomList::values(1, 0.f));
        v1.append(1.f);
        v1.append(-1231.f);
        REQUIRE(p1.get() == AtomList::values(1, 2.f));

        prop_t_rw s1;
        s1.sz = 3.1415;
        TypedCbProperty<double, prop_t_rw> p2("@size1", &s1, &prop_t_rw::getSize, &prop_t_rw::setSize);
        REQUIRE(p2.name() == "@size1");
        REQUIRE(p2.readonly() == false);

        REQUIRE(p2.get() == AtomList::values(1, 3.1415f));
        REQUIRE_FALSE(p2.set(AtomList()));
        REQUIRE(p2.set(AtomList::values(4, 34.f)));
        REQUIRE(s1.sz == 34.f);
    }

    SECTION("bool property")
    {
        BoolProperty p("test", true);
        REQUIRE(!p.readonly());
        REQUIRE(p.name() == "test");
        REQUIRE(p.value() == true);
        REQUIRE(p.get() == AtomList::ones(1));
        REQUIRE_FALSE(p.set(AtomList::zeroes(0)));
        REQUIRE(p.set(AtomList::zeroes(10)));
        REQUIRE(p.value() == false);
        REQUIRE(p.get() == AtomList::zeroes(1));

        REQUIRE_FALSE(p.set(listFrom(Atom(gensym("a")))));
        REQUIRE(p.set(listFrom(Atom(gensym("True")))));
        REQUIRE(p.value() == true);
        REQUIRE(p.set(listFrom(gensym("false"))));
        REQUIRE(p.value() == false);

        BoolProperty p2("test2", true, true);
        REQUIRE(p2.readonly());
        REQUIRE(p2.name() == "test2");
        REQUIRE_FALSE(p2.set(AtomList::ones(1)));
    }
}
