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
#include "patch_deps.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PatchDeps, patch, deps)

TEST_CASE("patch.deps", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("patch.deps");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_DATA_AT_OUTLET(0, t, DictAtom());
    }

    SECTION("simple test")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch1");

        {
            TExt t("patch.deps");
            REQUIRE(t->canvas() == cnv->pd_canvas());

            External e0("mtof");
            REQUIRE(e0.object());
            cnv->addExternal(e0);
            REQUIRE(cnv->objectList().size() == 1);

            t.sendBang();
            REQUIRE(t.outputAtomAt(0) == DictAtom("[_core: mtof]"));

            External e1("mtof");
            REQUIRE(e1.object());
            cnv->addExternal(e1);
            REQUIRE(cnv->objectList().size() == 2);

            t.sendBang();
            REQUIRE(t.outputAtomAt(0) == DictAtom("[_core: mtof]"));
        }
    }

    SECTION("abstraction simple test")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch2");

        {
            TExt t("patch.deps");
            REQUIRE(t->canvas() == cnv->pd_canvas());

            External p0(TEST_DATA_DIR "/base/patch_deps0");
            REQUIRE(p0.object());
            cnv->addExternal(p0);
            REQUIRE(cnv->objectList().size() == 1);

            t.sendBang();
            DictAtom res(t.outputAtomAt(0));
            REQUIRE(res->size() == 2);
            REQUIRE(res->contains("_core"));
            REQUIRE(res->at("_core") == LA("mtof"));
            REQUIRE(res->contains(TEST_DATA_DIR "/base/patch_deps0.pd"));
            REQUIRE(res->at(TEST_DATA_DIR "/base/patch_deps0.pd") == LA("patch_deps0.pd"));
        }
    }

    SECTION("abstraction complex test")
    {
        CanvasPtr cnv = PureData::instance().createTopCanvas(TEST_DATA_DIR "/patch3");

        {
            TExt t("patch.deps");
            REQUIRE(t->canvas() == cnv->pd_canvas());

            External p0(TEST_DATA_DIR "/base/patch_deps1");
            REQUIRE(p0.object());
            cnv->addExternal(p0);
            REQUIRE(cnv->objectList().size() == 1);

            t.sendBang();
            DictAtom res(t.outputAtomAt(0));
            REQUIRE(res->size() == 3);
            REQUIRE(res->contains("_core"));
            REQUIRE(res->at("_core") == LA("mtof", "osc~", "noise~"));
            REQUIRE(res->contains(TEST_DATA_DIR "/base/patch_deps0.pd"));
            REQUIRE(res->at(TEST_DATA_DIR "/base/patch_deps0.pd") == LA("patch_deps0.pd"));
            REQUIRE(res->contains(TEST_DATA_DIR "/base/patch_deps1.pd"));
            REQUIRE(res->at(TEST_DATA_DIR "/base/patch_deps1.pd") == LA("patch_deps1.pd"));
        }
    }
}
