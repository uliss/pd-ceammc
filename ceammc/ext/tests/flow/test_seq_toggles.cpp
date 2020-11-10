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
#include "seq_toggles.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(SeqToggles, seq, toggles)

TEST_CASE("seq.toggles", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("seq.toggles");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 3);
            REQUIRE_PROPERTY(t, @bpm, 60);
            REQUIRE_PROPERTY(t, @div, 4);
            REQUIRE_PROPERTY(t, @pattern, L());
            REQUIRE_PROPERTY(t, @dur, 0.75);
        }

        SECTION("args")
        {
            TObj t("seq.toggles", LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @pattern, LF(1, 2, 3));
        }

        SECTION("args invalid")
        {
            TObj t("seq.toggles", LA("A", 2, 3, "[a: 123]"));
            REQUIRE_PROPERTY(t, @pattern, LF(2, 3));
        }

        SECTION("ext")
        {
            TExt t("seq.toggles");
        }

        SECTION("alias")
        {
            TExt t("seq.t");
        }
    }

    SECTION("process")
    {
        SECTION("empty")
        {
            TExt t("seq.t");

            t.sendBang();
            REQUIRE(t.messagesAt(0).empty());
            REQUIRE(t.messagesAt(1).empty());
            REQUIRE(t.messagesAt(2).empty());
        }

        SECTION("single")
        {
            TExt t("seq.t", LA(1, "@bpm", 600, "@div", 4));

            t.sendBang();
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0)[0].atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1)[0].atomValue().asFloat() == 100);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(75);
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0)[1].atomValue().asFloat() == 0);

            t.schedTicks(25);
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 1);
            REQUIRE(t.messagesAt(2)[0].isBang());
        }

        SECTION("many")
        {
            TExt t("seq.t", LA(1, 2, 0.5, "@bpm", 600, "@div", 40, "@dur", 0.25));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(3); // off
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(7); // on
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 20);

            t.schedTicks(5); // off
            REQUIRE(t.messagesAt(0).size() == 4);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(15); // on
            REQUIRE(t.messagesAt(0).size() == 5);
            REQUIRE(t.messagesAt(1).size() == 3);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 5);

            t.schedTicks(2); // off
            REQUIRE(t.messagesAt(0).size() == 6);
            REQUIRE(t.messagesAt(1).size() == 3);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(3); // done
            REQUIRE(t.messagesAt(0).size() == 6);
            REQUIRE(t.messagesAt(1).size() == 3);
            REQUIRE(t.messagesAt(2).size() == 1);
            REQUIRE(t.messagesAt(2).back().isBang());
        }

        SECTION("@dur=0")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40, "@dur", 0.f));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 20);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(19); // on
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0).size() == 4);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(9); // done
            REQUIRE(t.messagesAt(0).size() == 4);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 1);
            REQUIRE(t.messagesAt(2).back().isBang());
        }

        SECTION("@dur=1")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40, "@dur", 1));

            t.sendBang(); // on
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 20);
            REQUIRE(t.messagesAt(2).empty());

            t.schedTicks(19); // none
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            t.schedTicks(1); // off-on
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0)[0].atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(0)[1].atomValue().asFloat() == 0);
            REQUIRE(t.messagesAt(0)[2].atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(9); // none
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);

            t.schedTicks(1); // off
            REQUIRE(t.messagesAt(0).size() == 4);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 1);
            REQUIRE(t.messagesAt(0)[0].atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(0)[1].atomValue().asFloat() == 0);
            REQUIRE(t.messagesAt(0)[2].atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(0)[3].atomValue().asFloat() == 0);
            REQUIRE(t.messagesAt(2).back().isBang());
        }
    }

    SECTION("stop")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(2, 1, "@bpm", 600, "@div", 40));

            REQUIRE(t.messagesAt(0).size() == 0);
            REQUIRE(t.messagesAt(1).size() == 0);
            REQUIRE(t.messagesAt(2).size() == 0);

            t.sendMessage("stop");
            REQUIRE(t.messagesAt(0).size() == 0);
            REQUIRE(t.messagesAt(1).size() == 0);
            REQUIRE(t.messagesAt(2).size() == 0);

            t.schedTicks(40);
            REQUIRE(t.messagesAt(0).size() == 0);
            REQUIRE(t.messagesAt(1).size() == 0);
            REQUIRE(t.messagesAt(2).size() == 0);
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(5); // active
            t.sendMessage("stop");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            // check no changes
            t.schedTicks(10);
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
        }

        SECTION("after off")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(9); // active
            t.sendMessage("stop");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            // check no changes
            t.schedTicks(10);
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
        }
    }

    SECTION("reset")
    {
        SECTION("inactive")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendMessage("reset");
            REQUIRE(t.messagesAt(0).size() == 0);
            REQUIRE(t.messagesAt(1).size() == 0);
            REQUIRE(t.messagesAt(2).size() == 0);
        }

        SECTION("while on")
        {
            TExt t("seq.t", LA(1, 2, "@bpm", 600, "@div", 40, "@dur", 0.75));

            t.sendBang();
            REQUIRE(t.messagesAt(0).size() == 1);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(5);
            t.sendMessage("reset");
            REQUIRE(t.messagesAt(0).size() == 2);
            REQUIRE(t.messagesAt(1).size() == 1);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0).size() == 3);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);

            t.schedTicks(8);
            t.sendMessage("reset");
            REQUIRE(t.messagesAt(0).size() == 4);
            REQUIRE(t.messagesAt(1).size() == 2);
            REQUIRE(t.messagesAt(2).size() == 0);
            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 0);

            t.schedTicks(1);
            REQUIRE(t.messagesAt(0).size() == 5);
            REQUIRE(t.messagesAt(1).size() == 3);
            REQUIRE(t.messagesAt(2).size() == 0);

            REQUIRE(t.messagesAt(0).back().atomValue().asFloat() == 1);
            REQUIRE(t.messagesAt(1).back().atomValue().asFloat() == 10);
        }
    }
}
