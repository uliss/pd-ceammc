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
#include "catch.hpp"
#include "lex/parser_dict_expr.h"
#include "test_common.h"

using namespace ceammc;

TEST_CASE("parser_dict_expr", "[parsers]")
{
    test::pdPrintToStdError();

    SECTION("syntax")
    {
        REQUIRE(parser::parse_dict_expr("."));
        REQUIRE(parser::parse_dict_expr(".[]"));
        REQUIRE(parser::parse_dict_expr(".[-1]"));
        REQUIRE(parser::parse_dict_expr(".[+1]"));
        REQUIRE(parser::parse_dict_expr(".[100:]"));
        REQUIRE_FALSE(parser::parse_dict_expr(".[1.5]"));
        REQUIRE(parser::parse_dict_expr(".[100:200]"));
        REQUIRE_FALSE(parser::parse_dict_expr(".[100:+200]"));
        REQUIRE_FALSE(parser::parse_dict_expr(".[100:-200]"));
        REQUIRE_FALSE(parser::parse_dict_expr(".[1:2.5]"));
        REQUIRE(parser::parse_dict_expr(".test_key"));
        REQUIRE(parser::parse_dict_expr(".KEY"));
        REQUIRE(parser::parse_dict_expr(".KEY123"));
        REQUIRE(parser::parse_dict_expr(".123KEY"));
        REQUIRE(parser::parse_dict_expr(".[\"any keys ?!@#$%^&*\"]"));
        REQUIRE_FALSE(parser::parse_dict_expr(".['key']"));
        REQUIRE(parser::parse_dict_expr(".a.b.c"));
        REQUIRE(parser::parse_dict_expr("...c"));
    }

    SECTION("match array")
    {
        std::vector<parser::DictExprMatcher> m;

        REQUIRE(parser::parse_dict_expr(".[0]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 1);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 1);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[0:]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 1);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 2);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[0:0]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 0);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[0:1000]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 1);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 2);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[1]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 1);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 1);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 1);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 1);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[1:]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 1);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 1);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 1);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 2);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[1:3]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 1);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 1);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 1);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 2);
        REQUIRE(m[0].arraySlice(4).first == 1);
        REQUIRE(m[0].arraySlice(4).second == 3);
        REQUIRE(m[0].arraySlice(5).first == 1);
        REQUIRE(m[0].arraySlice(5).second == 3);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-1]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 1);
        REQUIRE(m[0].arraySlice(2).first == 1);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 2);
        REQUIRE(m[0].arraySlice(3).second == 1);
        REQUIRE(m[0].arraySlice(4).first == 3);
        REQUIRE(m[0].arraySlice(4).second == 1);
        REQUIRE(m[0].arraySlice(5).first == 4);
        REQUIRE(m[0].arraySlice(5).second == 1);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 1);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 1);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 1);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2:]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 2);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 2);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 2);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 2);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2:0]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 0);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 0);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 0);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 0);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2:1]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 1);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 1);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 1);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 1);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2:2]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 2);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 2);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 2);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 2);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[-2:3]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchList());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(1).first == 0);
        REQUIRE(m[0].arraySlice(1).second == 0);
        REQUIRE(m[0].arraySlice(2).first == 0);
        REQUIRE(m[0].arraySlice(2).second == 2);
        REQUIRE(m[0].arraySlice(3).first == 1);
        REQUIRE(m[0].arraySlice(3).second == 2);
        REQUIRE(m[0].arraySlice(4).first == 2);
        REQUIRE(m[0].arraySlice(4).second == 2);
        REQUIRE(m[0].arraySlice(5).first == 3);
        REQUIRE(m[0].arraySlice(5).second == 2);
    }

    SECTION("match node")
    {
        std::vector<parser::DictExprMatcher> m;

        REQUIRE(parser::parse_dict_expr(".", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchAny());
        REQUIRE(m[0].key_name.empty());
        REQUIRE(m[0].arraySlice(0).first == 0);
        REQUIRE(m[0].arraySlice(0).second == 0);
        REQUIRE(m[0].arraySlice(100).first == 0);
        REQUIRE(m[0].arraySlice(100).second == 100);

        m.clear();
        REQUIRE(parser::parse_dict_expr(".node", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchDict());
        REQUIRE(m[0].key_name == "node");

        m.clear();
        REQUIRE(parser::parse_dict_expr(".[\"node:\"]", &m));
        REQUIRE(m.size() == 1);
        REQUIRE(m[0].matchDict());
        REQUIRE(m[0].key_name == "node:");

        m.clear();
        REQUIRE(parser::parse_dict_expr(".a..b", &m));
        REQUIRE(m.size() == 3);
        REQUIRE(m[0].matchDict());
        REQUIRE(m[0].key_name == "a");
        REQUIRE(m[1].matchAny());
        REQUIRE(m[1].key_name == "");
        REQUIRE(m[2].matchDict());
        REQUIRE(m[2].key_name == "b");
    }
}
