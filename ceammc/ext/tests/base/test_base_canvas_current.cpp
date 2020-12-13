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
#include "canvas_current.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(CanvasCurrent, canvas, current)

TEST_CASE("canvas.current", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("canvas.current");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @name, "test_canvas");
        REQUIRE_PROPERTY(t, @dir, "~");
        REQUIRE_PROPERTY(t, @root, Atom(1));
        REQUIRE_PROPERTY(t, @abstraction, Atom(1));
        REQUIRE_PROPERTY(t, @font, Atom(10));
        REQUIRE_PROPERTY(t, @width, Atom(600));
        REQUIRE_PROPERTY(t, @height, Atom(400));
        REQUIRE_PROPERTY(t, @x, Atom(0.f));
        REQUIRE_PROPERTY(t, @y, Atom(0.f));

        REQUIRE_PROPERTY_LIST(t, @args, L());
        REQUIRE_PROPERTY_LIST(t, @paths, L());
        REQUIRE_PROPERTY_LIST(t, @size, AtomList(600, 400));
    }

    SECTION("external")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch");

        { 
            TExt t("canvas.current");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @name, "patch");
            REQUIRE_PROPERTY(t, @dir, TEST_DATA_DIR);
            REQUIRE_PROPERTY(t, @root, 1);
            REQUIRE_PROPERTY(t, @abstraction, Atom(1));
            REQUIRE_PROPERTY(t, @font, 10);
            REQUIRE_PROPERTY(t, @width, 600);
            REQUIRE_PROPERTY(t, @height, 400);

            REQUIRE_PROPERTY(t, @x, Atom(0.f));
            REQUIRE_PROPERTY(t, @y, 0.f);

            REQUIRE_PROPERTY_LIST(t, @args, L());
            REQUIRE_PROPERTY_LIST(t, @size, LF(600, 400));
            REQUIRE_PROPERTY_LIST(t, @paths, L());
        }
    }
}
