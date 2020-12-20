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
#include "canvas_top.h"
#include "datatype_dict.h"
#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(CanvasTop, canvas, top)

TEST_CASE("canvas.top", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
        cnv->createObject("declare", LA("-stdpath", TEST_DIR));

        TExt t("canvas.top");
        REQUIRE(t->canvas() == cnv->pd_canvas());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @name, "patch");
        REQUIRE_PROPERTY(t, @dir, "~");

        REQUIRE_PROPERTY(t, @font, 10);
        REQUIRE_PROPERTY(t, @width, 600);
        REQUIRE_PROPERTY(t, @height, 400);

        REQUIRE_PROPERTY(t, @x, 0.f);
        REQUIRE_PROPERTY(t, @y, 0.f);

        REQUIRE_PROPERTY(t, @size, 600, 400);
        REQUIRE_PROPERTY(t, @paths, TEST_DIR);

        t.bang();
        REQUIRE(dataAt(t) == DictAtom("[x: 0 y: 0 size: 600 400 font: 10 dir: \"~\" "
                                      "name: patch paths: \"" TEST_DIR "\" width: 600 height: 400]"));

        CanvasPtr subpatch = PureData::instance().createSubpatch(cnv->pd_canvas(), "sub1");
        REQUIRE(subpatch->owner() == cnv->pd_canvas());

        auto subpatch_declare = subpatch->createObject("declare",
            LA("-path", "abc", "-stdpath", "...."));
        REQUIRE(subpatch_declare);

        auto subpatch_canvas_top = subpatch->createObject("canvas.top", L());
        REQUIRE(subpatch_canvas_top);

        auto props = subpatch_canvas_top->properties();
        REQUIRE(!props.empty());
        REQUIRE(props.size() == 9);

        auto contains = [](decltype(props)& props, const std::string& name) -> bool {
            auto it = std::find_if(props.begin(), props.end(),
                [&name](const PropertyInfo& pi) { return pi.name()->s_name == name; });

            return it != props.end();
        };

        REQUIRE(contains(props, "@name"));
        REQUIRE(contains(props, "@dir"));
        REQUIRE(contains(props, "@x"));
        REQUIRE(contains(props, "@y"));
        REQUIRE(contains(props, "@size"));
        REQUIRE(contains(props, "@width"));
        REQUIRE(contains(props, "@height"));
        REQUIRE(contains(props, "@font"));
        REQUIRE(contains(props, "@paths"));

        auto proxy = (const PdObject<CanvasTop>*)subpatch_canvas_top->object();
        proxy->impl->onBang();
        proxy->impl->dump();
    }
}
