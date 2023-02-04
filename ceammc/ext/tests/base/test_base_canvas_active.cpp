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
#include "canvas_active.h"
#include "parser_canvas_id.h"
#include "test_base.h"
#include "test_external.h"

#include <cstdio>

PD_COMPLETE_TEST_SETUP(CanvasActive, base, canvas_active)

TEST_CASE("canvas.active", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("canvas_id")
    {
        CanvasId id = 0;
        REQUIRE_FALSE(try_parse_canvas_id("", id));
        REQUIRE(id == 0);

        REQUIRE(try_parse_canvas_id(".x0.c", id));
        REQUIRE(id == 0);

        REQUIRE(try_parse_canvas_id(".xdeadbeef.c", id));
        REQUIRE(id == 0xDEADBEEF);

        auto uip = (std::uintptr_t)(void*)(&id);
        char buf[40];
        sprintf(buf, ".x%lx.c", uip);
        REQUIRE(try_parse_canvas_id(buf, id));
        REQUIRE(id == (std::uintptr_t)(void*)(&id));
    }
}
