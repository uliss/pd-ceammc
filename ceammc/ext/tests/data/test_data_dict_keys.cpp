/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"
#include "dict_keys.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictKeys, dict, keys)

TEST_CASE("dict.keys", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TExt t("dict.keys");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TExt t("dict.keys");

        t << DictA();
        REQUIRE(listAt(t) == L());

        t << DictA("[a: b 2: 300 @p: ABC]");
        REQUIRE(listAt(t).size() == 3);
        REQUIRE(list::containsAllOff(listAt(t), LA("a", "2", "@p")));
    }
}
