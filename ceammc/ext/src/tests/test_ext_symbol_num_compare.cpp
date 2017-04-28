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
#include "../symbol/symbol_num_compare.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<SymbolNumCompare> SymbolNumCmpTest;

TEST_CASE("symbol.num_compare", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            SymbolNumCmpTest t("symbol.num_compare", AtomList());
            REQUIRE_PROPERTY_NONE(t, @with);

            WHEN_SEND_SYMBOL_TO(0, t, "123");
            REQUIRE_NO_MSG(t);

            t.setProperty("@with", A("11.txt"));
            REQUIRE_PROPERTY(t, @with, "11.txt");

            WHEN_SEND_SYMBOL_TO(0, t, "1.txt");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "12.txt");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "124.txt");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "10.txt");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "9abc.txt");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }

        SECTION("properties")
        {
            SymbolNumCmpTest t("symbol.num_compare", L2("@with", "199.test.txt"));
            REQUIRE_PROPERTY(t, @with, A("199.test.txt"));

            WHEN_SEND_SYMBOL_TO(0, t, "abc");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "-199");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "c2000");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "2000_a");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("positional args")
        {
            SymbolNumCmpTest t("symbol.num_compare", A("20_abc"));
            REQUIRE_PROPERTY(t, @with, "20_abc");

            WHEN_SEND_SYMBOL_TO(0, t, "19_def");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }

        SECTION("invalid args")
        {
            SymbolNumCmpTest t("symbol.num_compare", A(25));
            REQUIRE_PROPERTY_NONE(t, @with);
        }
    }

    SECTION("list propagation")
    {
        SymbolNumCmpTest t("symbol.num_compare", AtomList());
        REQUIRE_PROPERTY_NONE(t, @with);

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L1("a"));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L2("9", "10"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_PROPERTY(t, @with, "10");
    }
}
