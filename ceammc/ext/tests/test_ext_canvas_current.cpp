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
#include "../base/canvas_current.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<CanvasCurrent> CanvasCurrentTest;

TEST_CASE("canvas.current", "[externals]")
{
    SECTION("init")
    {
        setup_canvas_current();

        CanvasCurrentTest t("canvas.current");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @name, "");
        REQUIRE_PROPERTY(t, @dir, "");
        REQUIRE_PROPERTY(t, @root, Atom(0.f));
        REQUIRE_PROPERTY(t, @abstraction, Atom(0.f));
        REQUIRE_PROPERTY(t, @font, Atom(0.f));
        REQUIRE_PROPERTY(t, @width, Atom(0.f));
        REQUIRE_PROPERTY(t, @height, Atom(0.f));
        REQUIRE_PROPERTY(t, @x, Atom(0.f));
        REQUIRE_PROPERTY(t, @y, Atom(0.f));

        REQUIRE_PROPERTY_LIST(t, @args, AtomList());
        REQUIRE_PROPERTY_LIST(t, @paths, AtomList());
        REQUIRE_PROPERTY_LIST(t, @size, AtomList(0.f, 0.f));

        CanvasPtr cnv = PureData::instance().createTopCanvas("patch");

        {
            CanvasCurrentTest t("canvas.current");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @name, "patch");
            REQUIRE_PROPERTY(t, @dir, "");
            REQUIRE_PROPERTY(t, @root, 1);
            REQUIRE_PROPERTY(t, @abstraction, Atom(0.f));
            REQUIRE_PROPERTY(t, @font, 10);
            REQUIRE_PROPERTY(t, @width, 600);
            REQUIRE_PROPERTY(t, @height, 400);

#ifdef __APPLE__
            REQUIRE_PROPERTY(t, @x, Atom(0.f));
            REQUIRE_PROPERTY(t, @y, 22);
#endif

            REQUIRE_PROPERTY_LIST(t, @args, AtomList());
            REQUIRE_PROPERTY_LIST(t, @size, L2(600, 400));
            REQUIRE_PROPERTY_LIST(t, @paths, AtomList());
        }
    }
}
