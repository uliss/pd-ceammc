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
#ifndef TEST_JSON_CPP
#define TEST_JSON_CPP

#include "catch.hpp"
#include "test_base.h"

#include "ceammc_data.h"
#include "ceammc_json.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"

using namespace ceammc;
using namespace ceammc::json;

using MA = MListAtom;
using SA = StringAtom;
using DA = DictAtom;

JsonWriteOpts iopt(int n)
{
    JsonWriteOpts res;
    res.indent = n;
    return res;
}

TEST_CASE("json", "[core]")
{
    SECTION("to_json_string pd types")
    {
        REQUIRE(to_json_string(Atom()) == "null");
        REQUIRE(to_json_string(Atom(), iopt(4)) == "null");

        REQUIRE(to_json_string(A(10)) == "10");
        REQUIRE(to_json_string(A(10), iopt(4)) == "10");
        REQUIRE(to_json_string(A(-2.5)) == "-2.5");
        REQUIRE(to_json_string(A("abc")) == "\"abc\"");

        REQUIRE(to_json_string(L()) == "[]");
        REQUIRE(to_json_string(LF(100)) == "[100]");
        REQUIRE(to_json_string(LF(1, 2, 3)) == "[1,2,3]");
        REQUIRE(to_json_string(LA("a", 2.5, 3)) == "[\"a\",2.5,3]");
    }

    SECTION("String")
    {
        REQUIRE(to_json_string(SA("")) == "\"\"");
        REQUIRE(to_json_string(SA("a b c")) == "\"a b c\"");
        REQUIRE(to_json_string(SA("\\")) == R"("\\")");
        REQUIRE(to_json_string(SA("\"")) == R"("\"")");
        REQUIRE(to_json_string(LA(SA("a b c"), SA("d e f"))) == "[\"a b c\",\"d e f\"]");
    }

    SECTION("MList")
    {
        REQUIRE(to_json_string(MA()) == "[]");
        REQUIRE(to_json_string(MA(1)) == "[1]");
        REQUIRE(to_json_string(MA(2.5)) == "[2.5]");
        REQUIRE(to_json_string(MA(1, 2, 3)) == "[1,2,3]");
        REQUIRE(to_json_string(MA("abc")) == "[\"abc\"]");
        REQUIRE(to_json_string(MA("abc", SA("abc"))) == "[\"abc\",\"abc\"]");
    }

    SECTION("Dict")
    {
        json::JsonWriteOpts opts;
        opts.compressSingleList = false;

        DataTypeDict dict("[a: b]");
        REQUIRE(dict.size() == 1);
        REQUIRE(dict.keys() == LA("a"));
        REQUIRE(dict.contains("a"));
        REQUIRE(dict.at("a") == A("b"));
        REQUIRE(to_json_string(dict) == R"({"a":"b"})");
        REQUIRE(to_json_string(dict, opts) == R"({"a":["b"]})");

        using DT = DataTypeDict;
        DT dict1;
        dict1.insert("a", A("b"));
        REQUIRE(dict1.toJsonString() == R"({"a":"b"})");
        dict1.at("a") = LF(1, 2);
        REQUIRE(dict1.toJsonString() == R"({"a":[1,2]})");
        dict1.at("a") = DA("[b: c]");
        REQUIRE(dict1.toJsonString() == R"({"a":{"b":"c"}})");
    }
}

#endif // TEST_JSON_CPP
