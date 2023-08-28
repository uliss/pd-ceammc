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
#include "canvas_dir.h"
#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(CanvasDir, base_canvas, dir)

TEST_CASE("canvas.dir", "[externals]")
{
    pd_test_init();
    setup_log_output_single();

    SECTION("init")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch3");
        TExt t("canvas.dir");
        REQUIRE(t->canvas() == cnv->pd_canvas());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        t.bang();
        REQUIRE_FLOAT_AT_OUTLET(1, t, 0);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, TEST_DATA_DIR);
    }

    SECTION("subpatch")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch3");
        auto sub = PureData::instance().createSubpatch(cnv->pd_canvas(), "sub");

        canvas_set_current(sub->pd_canvas());

        TExt t("canvas.dir");
        REQUIRE(t->canvas() == sub->pd_canvas());
        REQUIRE(t->canvas(CanvasDir::CanvasType::PARENT) == sub->pd_canvas());
        REQUIRE(t->canvas(CanvasDir::CanvasType::ROOT) == cnv->pd_canvas());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        t.bang();
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, TEST_DATA_DIR);

        canvas_unset_current(sub->pd_canvas());
    }

    SECTION("abstraction 0")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas("/path0");
        auto obj = cnv->createObject(TEST_DATA_DIR "/base/canvas_dir_test0", L());

        REQUIRE(obj);
        REQUIRE(obj->object());
        REQUIRE(obj->numInlets() == 1);
        REQUIRE(obj->numOutlets() == 2);

        LogExternalOutput log0;
        REQUIRE(cnv->connect(obj->object(), 0, log0.object(), 0));
        LogExternalOutput log1;
        REQUIRE(cnv->connect(obj->object(), 1, log1.object(), 0));

        canvas_send_bang((t_canvas*)obj->pd());
        REQUIRE(log0.msg() == LA(TEST_DATA_DIR "/base"));
        REQUIRE(log1.msg() == LF(0));
    }

    SECTION("abstraction 1")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas("/path1");
        auto obj = cnv->createObject(TEST_DATA_DIR "/base/canvas_dir_test1", L());

        REQUIRE(obj);
        REQUIRE(obj->object());
        REQUIRE(obj->numInlets() == 1);
        REQUIRE(obj->numOutlets() == 2);

        LogExternalOutput log0;
        REQUIRE(cnv->connect(obj->object(), 0, log0.object(), 0));
        LogExternalOutput log1;
        REQUIRE(cnv->connect(obj->object(), 1, log1.object(), 0));

        canvas_send_bang((t_canvas*)obj->pd());
        REQUIRE(log0.msg() == LA(TEST_DATA_DIR "/base"));
        REQUIRE(log1.msg() == LF(1));
    }

    SECTION("abstraction subpatch")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas("/patch2");
        auto obj = cnv->createObject(TEST_DATA_DIR "/base/canvas_dir_test2", L());

        REQUIRE(obj);
        REQUIRE(obj->object());
        REQUIRE(obj->numInlets() == 1);
        REQUIRE(obj->numOutlets() == 2);

        LogExternalOutput log0;
        REQUIRE(cnv->connect(obj->object(), 0, log0.object(), 0));
        LogExternalOutput log1;
        REQUIRE(cnv->connect(obj->object(), 1, log1.object(), 0));

        canvas_send_bang((t_canvas*)obj->pd());
        REQUIRE(log0.msg() == Message(LA(TEST_DATA_DIR)));
        REQUIRE(log1.msg() == LF(3));
    }

    SECTION("abstraction subpatch")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch2");
        auto obj = cnv->createObject(TEST_DATA_DIR "/base/canvas_dir_test3", L());

        REQUIRE(obj);
        REQUIRE(obj->object());
        REQUIRE(obj->numInlets() == 1);
        REQUIRE(obj->numOutlets() == 2);

        LogExternalOutput log0;
        REQUIRE(cnv->connect(obj->object(), 0, log0.object(), 0));
        LogExternalOutput log1;
        REQUIRE(cnv->connect(obj->object(), 1, log1.object(), 0));

        canvas_send_bang((t_canvas*)obj->pd());
        REQUIRE(log0.msg() == Message(LA(TEST_DATA_DIR)));
        REQUIRE(log1.msg() == LF(4));
    }
}
