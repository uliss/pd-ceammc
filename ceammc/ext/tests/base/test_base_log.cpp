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
#include "base_log.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(BaseLog, base, log)

static std::string log_output;
static void log_output_fn(const char* msg)
{
    log_output.clear();
    log_output = msg;
}

TEST_CASE("log", "[externals]")
{
    pd_test_init();
    test::pdSetPrintFunction(log_output_fn);
    test::pdPrintToStdError();

    SECTION("error")
    {
        TExt t("log.error");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 0);

        t.bang();
        t.bang();
        REQUIRE(log_output == "error: [logger] bang\n");
        t << -100;
        REQUIRE(log_output == "error: [logger] -100\n");
        t << "ABC";
        REQUIRE(log_output == "error: [logger] symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "error: [logger] list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "error: [logger] a message\n");

        log_output.clear();
        t->setProperty("@prefix", LA("a", "prefix"));
        t.bang();
        REQUIRE(log_output == "error: [logger] a prefix bang\n");
        t << -100;
        REQUIRE(log_output == "error: [logger] a prefix -100\n");
        t << "ABC";
        REQUIRE(log_output == "error: [logger] a prefix symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "error: [logger] a prefix list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "error: [logger] a prefix a message\n");

        log_output.clear();
        t->setProperty("@active", LF(0));
        t.bang();
        t << -100;
        t << "ABC";
        t << LA(1, 2, "?");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output.empty());
    }

    SECTION("post")
    {
        TExt t("log.post");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 0);

        t.bang();
        t.bang();
        REQUIRE(log_output == "[logger] bang\n");
        t << -100;
        REQUIRE(log_output == "[logger] -100\n");
        t << "ABC";
        REQUIRE(log_output == "[logger] symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "[logger] list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "[logger] a message\n");

        log_output.clear();
        t->setProperty("@prefix", LA("a", "prefix"));
        t.bang();
        REQUIRE(log_output == "[logger] a prefix bang\n");
        t << -100;
        REQUIRE(log_output == "[logger] a prefix -100\n");
        t << "ABC";
        REQUIRE(log_output == "[logger] a prefix symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "[logger] a prefix list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "[logger] a prefix a message\n");

        log_output.clear();
        t->setProperty("@active", LF(0));
        t.bang();
        t << -100;
        t << "ABC";
        t << LA(1, 2, "?");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output.empty());
    }

    SECTION("debug")
    {
        TExt t("log.debug");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 0);

        t.bang();
        t.bang();
        REQUIRE(log_output == "verbose(3): [logger] bang\n");
        t << -100;
        REQUIRE(log_output == "verbose(3): [logger] -100\n");
        t << "ABC";
        REQUIRE(log_output == "verbose(3): [logger] symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "verbose(3): [logger] list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "verbose(3): [logger] a message\n");

        log_output.clear();
        t->setProperty("@prefix", LA("a", "prefix"));
        t.bang();
        REQUIRE(log_output == "verbose(3): [logger] a prefix bang\n");
        t << -100;
        REQUIRE(log_output == "verbose(3): [logger] a prefix -100\n");
        t << "ABC";
        REQUIRE(log_output == "verbose(3): [logger] a prefix symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "verbose(3): [logger] a prefix list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "verbose(3): [logger] a prefix a message\n");

        log_output.clear();
        t->setProperty("@active", LF(0));
        t.bang();
        t << -100;
        t << "ABC";
        t << LA(1, 2, "?");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output.empty());
    }

    SECTION("verbose")
    {
        TExt t("log.verbose");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 0);

        t.bang();
        t.bang();
        REQUIRE(log_output == "verbose(4): [logger] bang\n");
        t << -100;
        REQUIRE(log_output == "verbose(4): [logger] -100\n");
        t << "ABC";
        REQUIRE(log_output == "verbose(4): [logger] symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "verbose(4): [logger] list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "verbose(4): [logger] a message\n");

        log_output.clear();
        t->setProperty("@prefix", LA("a", "prefix"));
        t.bang();
        REQUIRE(log_output == "verbose(4): [logger] a prefix bang\n");
        t << -100;
        REQUIRE(log_output == "verbose(4): [logger] a prefix -100\n");
        t << "ABC";
        REQUIRE(log_output == "verbose(4): [logger] a prefix symbol ABC\n");
        t << LA(1, 2, "?");
        REQUIRE(log_output == "verbose(4): [logger] a prefix list 1 2 ?\n");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output == "verbose(4): [logger] a prefix a message\n");

        log_output.clear();
        t->setProperty("@active", LF(0));
        t.bang();
        t << -100;
        t << "ABC";
        t << LA(1, 2, "?");
        t.sendMessage("a", LA("message"));
        REQUIRE(log_output.empty());
    }
}
