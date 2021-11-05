/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "datatype_string.h"
#include "path_normalize.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(PathNormalize, path, normalize)

TEST_CASE("path.normalize", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("path.normalize");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("alias")
        {
            TExt t("path.norm");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("symbol")
    {
        TExt t("path.norm");

        t << ".";
        REQUIRE(t.outputSymbolAt(0) == SYM("."));

        t << "..";
        REQUIRE(t.outputSymbolAt(0) == SYM(".."));

        t << "../../";
        REQUIRE(t.outputSymbolAt(0)->s_name == std::string("../.."));

        t << "A/B/../C";
        REQUIRE(t.outputSymbolAt(0)->s_name == std::string("A/C"));
    }

    SECTION("string")
    {
        TExt t("path.norm");

        t << StringAtom(".");
        REQUIRE(t.outputAtomAt(0) == StringAtom("."));

        t << StringAtom("..");
        REQUIRE(t.outputAtomAt(0) == StringAtom(".."));

        t << StringAtom("../../");
        REQUIRE(t.outputAtomAt(0) == StringAtom("../.."));

        t << StringAtom("A/B/.././C");
        REQUIRE(t.outputAtomAt(0) == StringAtom("A/C"));
    }
}
