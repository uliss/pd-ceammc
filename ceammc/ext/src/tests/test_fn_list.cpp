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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ceammc_fn_list.h"

using namespace ceammc;

TEST_CASE("list functions", "[ceammc::list]")
{
    SECTION("shortest")
    {
        std::vector<AtomList> lst;
        REQUIRE(list::shortestListSize(lst) == 0);

        lst.push_back(AtomList::ones(10));
        REQUIRE(list::shortestListSize(lst) == 10);
        lst.push_back(AtomList::ones(9));
        REQUIRE(list::shortestListSize(lst) == 9);
        lst.push_back(AtomList::ones(10));
        REQUIRE(list::shortestListSize(lst) == 9);
        lst.push_back(AtomList::ones(3));
        REQUIRE(list::shortestListSize(lst) == 3);
        lst.push_back(AtomList());
        REQUIRE(list::shortestListSize(lst) == 0);
    }

    SECTION("longest")
    {
        std::vector<AtomList> lst;
        REQUIRE(list::longestListSize(lst) == 0);

        lst.push_back(AtomList::ones(1));
        REQUIRE(list::longestListSize(lst) == 1);
        lst.push_back(AtomList::ones(10));
        REQUIRE(list::longestListSize(lst) == 10);
        lst.push_back(AtomList::ones(10));
        REQUIRE(list::longestListSize(lst) == 10);
        lst.push_back(AtomList::ones(3));
        REQUIRE(list::longestListSize(lst) == 10);
        lst.push_back(AtomList());
        REQUIRE(list::longestListSize(lst) == 10);
    }
}
