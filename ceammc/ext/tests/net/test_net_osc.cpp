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
#include "net_osc_server.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc::osc;
using namespace ceammc::net;

extern int sys_verbose;

static void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

TEST_CASE("osc", "[net]")
{
    sys_verbose = 1;
    test::pdPrintToStdError();

    SECTION("OscServer")
    {
        osc::OscServer s("test_osc", 19234);
        REQUIRE(s.name() == "test_osc");
        REQUIRE(s.port() == 19234);
        REQUIRE(s.hostname().substr(0, 10) == "osc.udp://");
        REQUIRE(s.isValid());
        REQUIRE(!s.isRunning());
        REQUIRE(s.start(true));

        sleep_ms(200);
        REQUIRE(s.isRunning());
    }
}
