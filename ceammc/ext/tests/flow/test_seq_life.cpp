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
#include "seq_life.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqLife, seq, life)

TEST_CASE("seq.life", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.life");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @rows, LF(16));
            REQUIRE_PROPERTY(t, @cols, LF(16));
            REQUIRE_PROPERTY(t, @size, LF(256));
        }

        SECTION("arg")
        {
            TExt t("seq.life", LF(2, 3));
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @rows, LF(2));
            REQUIRE_PROPERTY(t, @cols, LF(3));
            REQUIRE_PROPERTY(t, @size, LF(6));
        }
    }

    SECTION("class")
    {
        ConwayLife l;
        REQUIRE(l.rows() == 16);
        REQUIRE(l.cols() == 16);
        REQUIRE(l.numCells() == 256);

        REQUIRE(l.set(3, 4));
        REQUIRE(l.rows() == 3);
        REQUIRE(l.cols() == 4);
        REQUIRE(l.numCells() == 12);

        l.clear();
        REQUIRE(l.numAlive() == 0);

        REQUIRE(l.at(0, 0) == 0);
        REQUIRE(l.set({ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 }));
        REQUIRE(l.rows() == 3);
        REQUIRE(l.cols() == 4);
        REQUIRE(l.numCells() == 12);
        REQUIRE(l.numAlive() == 8);

        REQUIRE(l.at(0, 0) == 1);
        REQUIRE(l.at(0, 1) == 1);
        REQUIRE(l.at(0, 2) == 1);
        REQUIRE(l.at(0, 3) == 1);

        REQUIRE(l.at(1, 0) == 0);
        REQUIRE(l.at(1, 1) == 0);
        REQUIRE(l.at(1, 2) == 0);
        REQUIRE(l.at(1, 3) == 0);

        REQUIRE(l.at(2, 0) == 1);
        REQUIRE(l.at(2, 1) == 1);
        REQUIRE(l.at(2, 2) == 1);
        REQUIRE(l.at(2, 3) == 1);

        l.next();
        REQUIRE(l.countAt(0, 0) == 1);
        REQUIRE(l.countAt(0, 1) == 2);
        REQUIRE(l.countAt(0, 2) == 2);
        REQUIRE(l.countAt(0, 3) == 1);

        REQUIRE(l.countAt(1, 0) == 4);
        REQUIRE(l.countAt(1, 1) == 6);
        REQUIRE(l.countAt(1, 2) == 6);
        REQUIRE(l.countAt(1, 3) == 4);

        REQUIRE(l.countAt(2, 0) == 1);
        REQUIRE(l.countAt(2, 1) == 2);
        REQUIRE(l.countAt(2, 2) == 2);
        REQUIRE(l.countAt(2, 3) == 1);

        REQUIRE(l.at(0, 0) == 0);
        REQUIRE(l.at(0, 1) == 1);
        REQUIRE(l.at(0, 2) == 1);
        REQUIRE(l.at(0, 3) == 0);

        REQUIRE(l.at(1, 0) == 0);
        REQUIRE(l.at(1, 1) == 0);
        REQUIRE(l.at(1, 2) == 0);
        REQUIRE(l.at(1, 3) == 0);

        REQUIRE(l.at(2, 0) == 0);
        REQUIRE(l.at(2, 1) == 1);
        REQUIRE(l.at(2, 2) == 1);
        REQUIRE(l.at(2, 3) == 0);

        REQUIRE(l.numAlive() == 4);
        l.setAt(0, 0, true);
        REQUIRE(l.numAlive() == 5);
        l.setAt(0, 0, true);
        REQUIRE(l.numAlive() == 5);
        l.setAt(0, 0, false);
        REQUIRE(l.numAlive() == 4);
        l.setAt(0, 1, false);
        REQUIRE(l.numAlive() == 3);
        l.setAt(0, 2, false);
        REQUIRE(l.numAlive() == 2);

        l.flipAt(2, 3);
        REQUIRE(l.at(2, 3) == 1);
        REQUIRE(l.numAlive() == 3);
        l.flipAt(2, 3);
        REQUIRE(l.at(2, 3) == 0);
        REQUIRE(l.numAlive() == 2);

        l.clear();
        REQUIRE(l.numAlive() == 0);
        l.addBlock(1, 2);
        REQUIRE(l.numAlive() == 4);
        REQUIRE(l.at(1, 2));
        REQUIRE(l.at(1, 3));
        REQUIRE(l.at(2, 2));
        REQUIRE(l.at(2, 3));
    }
}
