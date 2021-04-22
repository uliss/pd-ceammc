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
#include "path.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(Path, path, object)

TEST_CASE("path", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TExt t("path");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("search")
    {
        TExt t("path", "\"" TEST_DATA_DIR "/../../\"", "@sync");

        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));
    }

    SECTION("search @depth")
    {
        TExt t("path", "@sync");

        // depth 1
        t->setProperty("@depth", LA(1));
        t << TEST_DATA_DIR "/..";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        t << TEST_DATA_DIR;
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));

        t << TEST_DATA_DIR "/path";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));

        // depth 2
        t->setProperty("@depth", LA(2));
        t << TEST_DATA_DIR "/..";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));

        t << TEST_DATA_DIR;
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));

        t << TEST_DATA_DIR "/path";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));

        // depth 0 (no recursion)
        t->setProperty("@depth", LF(0));
        t << TEST_DATA_DIR "/../..";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        t << TEST_DATA_DIR "/..";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        t << TEST_DATA_DIR;
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        t << TEST_DATA_DIR "/path";
        t.call("search", SYM(__FILE__));
        REQUIRE(t.outputAtomAt(0) == StringAtom(TEST_DATA_DIR "/path/test_path.cpp"));
    }

    SECTION("search @depth")
    {
        TExt t("path", "\"" TEST_DATA_DIR "/path\"", "@sync", "@maxnum", 1);

        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(2));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(4));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(8));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(16));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(32));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));

        //        t->setProperty("@maxnum", LF(1000));
        //        t.call("search", SYM(__FILE__));
        //        REQUIRE(t.outputAtomAt(0) == StringAtom(""));
    }
}
