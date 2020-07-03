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
#include "patch_args.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PatchArgs, patch, args)

TEST_CASE("patch.args", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("patch.args");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_LIST_AT_OUTLET(0, t, L());

        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch");

        {
            TestExtPatchArgs t("patch.args");
            t.sendBang();
            REQUIRE(t.hasOutput());
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == L());
        }
    }

    SECTION("init")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch1", LX(1, 2, 3));

        TExt t("patch.args");
        t.sendBang();
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LX(1, 2, 3));

        // pd hardcore
        cnv->createPdObject(10, 20, SYM("pd"), LA("some", "args"));
        auto x0 = cnv->findObjectByClassName(gensym("canvas"));
        Canvas subpatch((t_glist*)x0.front());
        REQUIRE(subpatch.name()->s_name == std::string("some"));
        REQUIRE(subpatch.owner() == cnv->pd_canvas());
        REQUIRE(canvas_info_args(subpatch.pd_canvas()) == LA("some", "args"));

        subpatch.createPdObject(10, 20, SYM("patch.args"));
        subpatch.createPdObject(10, 20, SYM("log_output_multi"));
        auto x1 = subpatch.findIf([](t_gobj* y) { return y->g_pd->c_name == gensym("patch.args"); });
        REQUIRE(x1);
        auto x2 = subpatch.findIf([](t_gobj* y) { return y->g_pd->c_name == gensym("log_output_multi"); });
        REQUIRE(x2);
        auto log = (t_log_output_multi*)(x2);
        REQUIRE(log->msg_list->empty());

        REQUIRE(subpatch.connect((t_object*)x1, 0, (t_object*)x2, 0));
        pd_bang(&x1->g_pd);
        REQUIRE(log->msg_list->size() == 1);
        REQUIRE(log->msg_list->back().listValue() == LA("some", "args"));
    }
}
