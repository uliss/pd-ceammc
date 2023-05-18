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
#include "ceammc_canvas.h"
#include "ceammc_pd.h"
#include "test_base.h"

using namespace ceammc;

extern int sys_verbose;

TEST_CASE("Canvas", "[ceammc::Canvas]")
{
    const bool i = []() { PureData::instance(); return true; }();
    test::pdPrintToStdError();

    SECTION("utils")
    {
        SECTION("name")
        {
            REQUIRE(canvas_info_name(0)->s_name == std::string());
            REQUIRE(canvas_info_name(0) == gensym(""));

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_name(cnv->pd_canvas()) == gensym("test"));
            REQUIRE(cnv->name() == gensym("test"));

            Canvas null(nullptr);
            REQUIRE(null.name() == gensym(""));
        }

        SECTION("dir")
        {
            REQUIRE(canvas_info_dir(0)->s_name == std::string());
            REQUIRE(canvas_info_dir(0) == gensym(""));

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_dir(cnv->pd_canvas()) == gensym("~"));

#ifdef __WIN32
            const char* FNAME = "C:/dir/test";
            const char* FDIR = "C:/dir";
#else
            const char* FNAME = "/dir/test";
            const char* FDIR = "/dir";
#endif

            cnv = PureData::instance().createTopCanvas(FNAME);
            REQUIRE(canvas_info_dir(cnv->pd_canvas())->s_name == std::string(FDIR));
            REQUIRE(canvas_info_name(cnv->pd_canvas())->s_name == std::string("test"));
        }

        SECTION("font")
        {
            REQUIRE(canvas_info_font(0) == 0);

            CanvasPtr cnv = PureData::instance().createTopCanvas("test");
            REQUIRE(canvas_info_font(cnv->pd_canvas()) == 10);
        }

        SECTION("paths")
        {
            REQUIRE(canvas_info_paths(nullptr) == L());

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_paths(cnv->pd_canvas()) == L());

            pd::External declare("declare", LA("-path", "dir2"));
            REQUIRE(!declare.isNull());

            REQUIRE(canvas_info_paths(cnv->pd_canvas()) == L());
        }

        SECTION("is root")
        {
            REQUIRE_FALSE(canvas_info_is_root(0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_is_root(cnv->pd_canvas()));
            REQUIRE(cnv->owner() == nullptr);

            Canvas null(nullptr);
            REQUIRE(null.owner() == nullptr);
        }

        SECTION("is abstraction")
        {
            REQUIRE_FALSE(canvas_info_is_abstraction(0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(canvas_info_is_abstraction(cnv->pd_canvas()));
        }

        SECTION("rect")
        {
            REQUIRE(canvas_info_rect(0) == t_rect(0, 0, 0, 0));

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");

#ifdef __APPLE__
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).x == 0);
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).y == 0);
#endif

            REQUIRE(canvas_info_rect(cnv->pd_canvas()).w == 600);
            REQUIRE(canvas_info_rect(cnv->pd_canvas()).h == 400);
        }

        SECTION("args")
        {
            REQUIRE(canvas_info_args(0) == L());

            CanvasPtr cnv = PureData::instance().createTopCanvas("patch", LF(1, 2, 3));
            REQUIRE(canvas_info_args(cnv->pd_canvas()) == LF(1, 2, 3));

            // load abstraction
            CanvasPtr cnv2 = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch2", LX(1, 2, 3));
            REQUIRE(canvas_info_args(cnv2->pd_canvas()) == LF(1, 2, 3));
        }

        SECTION("createObject")
        {
            CanvasPtr cnv = PureData::instance().createTopCanvas("patch");
            REQUIRE(cnv->objectList().empty());

            auto pplus = cnv->createObject("+", LF(10));
            REQUIRE(pplus);
            REQUIRE(pplus->numInlets() == 2);
            REQUIRE(pplus->numOutlets() == 1);
            // not creates patchable object
            REQUIRE(cnv->objectList().size() == 0);

            cnv->createPdObject(20, 30, gensym("osc~"), LF(1000));

            REQUIRE(cnv->objectList().size() == 1);
            REQUIRE(cnv->objectList().front()->te_g.g_pd->c_name == gensym("osc~"));
            REQUIRE(cnv->findIf([](t_gobj* x) { return x->g_pd->c_name == gensym("osc~"); }));
            REQUIRE_FALSE(cnv->findIf([](t_gobj* x) { return x->g_pd->c_name == gensym("osc12~"); }));
            REQUIRE(cnv->findIf([](t_object* x) { return x->te_g.g_pd->c_name == gensym("osc~"); }));
            REQUIRE_FALSE(cnv->findIf([](t_object* x) { return x->te_g.g_pd->c_name == gensym("osc12~"); }));
            REQUIRE(cnv->findObjectByClassName(gensym("osc~")).size() == 1);

            // NULL
            CanvasPtr null = std::make_shared<Canvas>(nullptr);
            REQUIRE_FALSE(null->createObject("+", LF(10)));
            REQUIRE(null->objectList().empty());
            null->createPdObject(20, 30, gensym("osc~"), LF(1000));
            REQUIRE(null->objectList().empty());
            REQUIRE_FALSE(null->findIf([](t_gobj* x) { return x->g_pd->c_name == gensym("osc~"); }));
            REQUIRE(null->findObjectByClassName(gensym("osc~")).empty());
            REQUIRE_FALSE(null->findIf([](t_object* x) { return x->te_g.g_pd->c_name == gensym("osc~"); }));
            REQUIRE_FALSE(null->findIf([](t_object* x) { return x->te_g.g_pd->c_name == gensym("osc12~"); }));
        }

        SECTION("connect")
        {
            CanvasPtr cnv = PureData::instance().createTopCanvas("patch2");
            REQUIRE(cnv->objectList().empty());

            cnv->createPdObject(20, 30, gensym("sig~"), LF(1000));
            cnv->createPdObject(20, 60, gensym("osc~"), LF(2000));

            auto ol = cnv->objectList();
            REQUIRE(ol.size() == 2);
            REQUIRE(cnv->connect((t_object*)ol[0], 0, (t_object*)ol[1], 0));
        }

        SECTION("createArray")
        {
            CanvasPtr cnv = PureData::instance().createTopCanvas("patch3");
            REQUIRE(cnv->objectList().empty());

            auto a1 = cnv->createArray("array1", 20);
            REQUIRE(a1);
            REQUIRE(a1->name() == SYM("array1"));
            REQUIRE(a1->size() == 20);
            REQUIRE(cnv->objectList().empty());
        }

        SECTION("createAbstraction")
        {
            sys_verbose = 10;

            CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch3");
            REQUIRE(cnv);
            cnv->createPdObject(10, 10, gensym("declare"), LA("-stdpath", TEST_DATA_DIR));
            REQUIRE(cnv->objectList().size() == 1);
            REQUIRE(canvas_info_paths(cnv->pd_canvas()) == LA(TEST_DATA_DIR));

            auto pabs = cnv->createAbstraction(10, 20, gensym("test_canvas_01"), LA(10, 20, 30));
            REQUIRE(pabs);
            REQUIRE(cnv->objectList().size() == 2);
            REQUIRE(canvas_info_rect(pabs) == t_rect({ 0, 0, 0, 0 }));
            REQUIRE(Canvas(pabs).owner());
            REQUIRE(Canvas(pabs).owner() == cnv->pd_canvas());
            REQUIRE(canvas_info_is_abstraction(pabs));
            REQUIRE_FALSE(canvas_info_is_root(pabs));
            REQUIRE(canvas_info_args(pabs) == LF(10, 20, 30));

            auto pabs2 = cnv->createAbstraction(10, 20, gensym("test_canvas_???"), LA(10, 20, 30));
            REQUIRE_FALSE(pabs2);
            REQUIRE(cnv->objectList().size() == 3);
            REQUIRE(canvas_info_args(pabs2) == L());

            auto pabs3 = cnv->createAbstraction(10, 20, gensym("test_canvas_02"), LA(10, 20, 30));
            REQUIRE(pabs3);
            REQUIRE(cnv->objectList().size() == 4);
            auto bbox3 = canvas_info_rect(pabs3);
            REQUIRE(bbox3.x == 100);
            REQUIRE(bbox3.y == 100);
            REQUIRE(bbox3.w == 85);
            REQUIRE(bbox3.h == 60);

            Canvas null(nullptr);
            REQUIRE_FALSE(null.createAbstraction(10, 20, gensym("test_canvas_02")));
        }
    }
}
