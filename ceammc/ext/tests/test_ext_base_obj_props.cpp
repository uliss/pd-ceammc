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
#include "../base/mod_base.h"
#include "../base/obj_props.h"
#include "ceammc_pd.h"
#include "datatype_dict.h"
#include "test_external.h"
#include "test_sound.h"

#include <algorithm>
#include <cstdlib>
#include <stdio.h>

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
        REQUIRE(t.isOutputDataAt(0));
        auto props = t.outputDataAt(0);
        const DataTypeDict* dict = props->as<DataTypeDict>();
        REQUIRE(dict->size() == 5);
        REQUIRE(dict->contains(A("@value")));
        REQUIRE(dict->contains(A("@db")));
        REQUIRE(dict->contains(A("@mute")));
        REQUIRE(dict->contains(A("@solo")));
        REQUIRE(dict->contains(A("@xfade_time")));
    }
}
