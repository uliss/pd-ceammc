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
#include "random_atom.h"
#include "test_random_base.h"

#include <map>

PD_COMPLETE_TEST_SETUP(RandomAtom, random, atom)

TEST_CASE("random.atom", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("random.atom");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @a, L());
            REQUIRE_PROPERTY(t, @w, L());
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("pos args")
        {
            TObj t("random.int", LA("a", "b", "c"));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, L());
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TObj t("random.int", LA("@a", "a", "b", "c", "@w", 1, 2));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, LF(1, 2, 0));
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TObj t("random.int", LA("@a", "a", "b", "c", "@w"));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, LF(0, 0, 0));
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TObj t("random.int", LA("@a", "a", "b", "c", "@w", "d", "e"));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, LF(0, 0, 0));
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TObj t("random.int", LA("@a", "a", "b", "c", "@w", "d", 2));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, LF(0, 2, 0));
            REQUIRE_PROPERTY(t, @seed, 0);
        }

        SECTION("props")
        {
            TObj t("random.int", LA("@a", "a", "b", "c", "@w", 1, 2, 3, 4));

            REQUIRE_PROPERTY(t, @a, LA("a", "b", "c"));
            REQUIRE_PROPERTY(t, @w, L());
            REQUIRE_PROPERTY(t, @seed, 0);
        }
    }

    SECTION("do empty")
    {
        TExt t("random.a");
        t << BANG;
        REQUIRE(!t.hasOutputAt(0));
    }

    SECTION("do no weights")
    {
        TExt t("random.a", "A", "B", 200, "@seed", 1);
        t << BANG;
        REQUIRE(t.outputAtomAt(0) == A("B"));
        t << BANG;
        REQUIRE(t.outputAtomAt(0) == A("A"));
        t << BANG;
        REQUIRE(t.outputAtomAt(0) == A("A"));
    }

    SECTION("weights")
    {
        TExt t("random.a", "A", "B", 200, "@w", 2, 1, 0., "@seed", 1);

        std::map<Atom, size_t> cnt;
        for (size_t i = 0; i < 1024; i++) {
            t << BANG;
            cnt[t.outputAtomAt(0)]++;
        }

        REQUIRE(cnt.size() == 2);
        REQUIRE(cnt[A("A")] == 701);
        REQUIRE(cnt[A("B")] == 323);
    }
}
