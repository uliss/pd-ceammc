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
#include "base_extension_test.h"
#include "ceammc_canvas.h"
#include "ceammc_pd.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("Canvas", "[ceammc::Canvas]")
{
    SECTION("utils")
    {
        SECTION("name")
        {
            REQUIRE(canvas_info_name(0)->s_name == std::string());
            REQUIRE(canvas_info_name(0) == &s_);

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_name(cnv->pd_canvas()) == gensym("test"));
        }

        SECTION("dir")
        {
            REQUIRE(canvas_info_dir(0)->s_name == std::string());
            REQUIRE(canvas_info_dir(0) == &s_);

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_dir(cnv->pd_canvas()) == &s_);

            cnv = PureData::instance().createTopCanvas("/dir/test");
            REQUIRE(canvas_info_dir(cnv->pd_canvas()) == gensym("/dir"));
            REQUIRE(canvas_info_name(cnv->pd_canvas()) == gensym("test"));
        }

        SECTION("font")
        {
            REQUIRE(canvas_info_font(0) == 0);

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_font(cnv->pd_canvas()) == 10);
        }

        SECTION("paths")
        {
            REQUIRE(canvas_info_paths(0) == AtomList());

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_paths(cnv->pd_canvas()) == AtomList());

            pd::External declare("declare", L2("-path", "dir2"));
            REQUIRE(!declare.isNull());

            REQUIRE(canvas_info_paths(cnv->pd_canvas()) == AtomList());
        }

        SECTION("is root")
        {
            REQUIRE_FALSE(canvas_info_is_root(0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_is_root(cnv->pd_canvas()));
        }

        SECTION("is abstraction")
        {
            REQUIRE_FALSE(canvas_info_is_abstraction(0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE_FALSE(canvas_info_is_abstraction(cnv->pd_canvas()));
        }

        SECTION("rect")
        {
            REQUIRE(canvas_info_rect(0) == t_rect(0, 0, 0, 0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");

#ifdef __APPLE__
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).x == 0);
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).y == 22);
#endif

            REQUIRE(canvas_info_rect(cnv->pd_canvas()).w == 600);
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).h == 400);
        }

        SECTION("args")
        {
            REQUIRE(canvas_info_args(0) == AtomList());

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_args(cnv->pd_canvas()) == AtomList());
        }
    }
}
