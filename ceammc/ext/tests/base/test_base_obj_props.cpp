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
#include "datatype_dict.h"
#include "mod_base.h"
#include "obj_props.h"
#include "test_sound.h"

#include "test_catch2.hpp"

#include <algorithm>
#include <cstdlib>

PD_COMPLETE_TEST_SETUP(ObjProps, obj, props)

TEST_CASE("obj.props", "[externals]")
{
    pd_test_init();
    ceammc_base_setup();

    SECTION("construct")
    {
        TestObjProps t("obj.props");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("[mix~]")
    {
        TestExtObjProps t("obj.props");
        REQUIRE(t.object());

        pd::External mix("mix~");
        REQUIRE(mix.object());

        REQUIRE(t.connectTo(1, mix, 0));

        t.sendBang();
        REQUIRE(t.hasOutputAt(0));
        DictAtom out(dataAt(t));

        REQUIRE(out->contains("name"));
        REQUIRE(out->at("name") == A("mix~"));
        REQUIRE(out->contains("properties"));
        REQUIRE(out->at("properties").size() == 5);

        for (auto& p : out->at("properties"))
            std::cerr << " - " << p << "\n";

        CHECK(out->at("properties").contains(DictAtom("[units: millisecond default: 20 name: @xfade_time type: float view: slider min: 1]")));
        CHECK(out->at("properties").contains(DictAtom("[units: decibel default: -144 -144 name: @db type: list view: entry]")));
        CHECK(out->at("properties").contains(DictAtom("[default: 0 0 name: @value type: list view: entry]")));
        CHECK(out->at("properties").contains(DictAtom("[default: 0 0 name: @mute type: list view: entry]")));
        CHECK(out->at("properties").contains(DictAtom("[default: 0 0 name: @solo type: list view: entry]")));
    }
}
