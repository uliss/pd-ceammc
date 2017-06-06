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
#include "../string/string_format.h"
#include "base_extension_test.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringFormat> StringFormatTest;

class TestData : public AbstractData {
    int v_;

public:
    TestData(int v)
        : v_(v)
    {
    }

    DataType type() const { return 2; }
    TestData* clone() const { return new TestData(v_); }
    std::string toString() const
    {
        char buf[40];
        sprintf(buf, "TEST: %i", v_);
        return buf;
    }
};

#define REQUIRE_STRING_OUTPUT(t, str_)                    \
    {                                                     \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                 \
        DataString* s = t.typedLastDataAt<DataString>(0); \
        REQUIRE(s != 0);                                  \
        REQUIRE(s->str() == str_);                        \
    }

#define NO_DATA(t) REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

TEST_CASE("string.format", "[external]")
{
    SECTION("create")
    {
        SECTION("empty")
        {
            StringFormatTest t("string.format");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @format, AtomList());

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_NO_MSG(t);
        }

        SECTION("no-%")
        {
            StringFormatTest t("string.format", L1("SAMPLE"));
            REQUIRE_PROPERTY_LIST(t, @format, L1("SAMPLE"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, L2(1, 2));
            REQUIRE_NO_MSG(t);
        }
    }

    SECTION("simple format")
    {
        SECTION("format: %d")
        {
            StringFormatTest t("string.format", L1("%d"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_STRING_OUTPUT(t, "123");

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "123");

            WHEN_SEND_FLOAT_TO(0, t, 3.1415f);
            REQUIRE_STRING_OUTPUT(t, "3.1415");

            WHEN_SEND_FLOAT_TO(0, t, -3.1415f);
            REQUIRE_STRING_OUTPUT(t, "-3.1415");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_STRING_OUTPUT(t, "ABC");

            WHEN_SEND_SYMBOL_TO(0, t, "%");
            REQUIRE_STRING_OUTPUT(t, "%");

            WHEN_SEND_SYMBOL_TO(0, t, "%d");
            REQUIRE_STRING_OUTPUT(t, "%d");

            WHEN_SEND_LIST_TO(0, t, L1(100));
            REQUIRE_STRING_OUTPUT(t, "100");

            //            WHEN_SEND_DATA_TO(0, t, DataString("324"));
            //            REQUIRE_STRING_OUTPUT(t, "324");

            WHEN_SEND_LIST_TO(0, t, L2(100, 200));
            REQUIRE_NO_MSG(t);
        }

        SECTION("format: %d++")
        {
            StringFormatTest t("string.format", L1("%+d"));

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_STRING_OUTPUT(t, "+123");

            WHEN_SEND_FLOAT_TO(0, t, -123);
            REQUIRE_STRING_OUTPUT(t, "-123");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_STRING_OUTPUT(t, "ABC");

            t.setProperty("@format", L1("%4d"));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_STRING_OUTPUT(t, "   1");

            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_STRING_OUTPUT(t, "  -1");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_STRING_OUTPUT(t, " ABC");

            t.setProperty("@format", L1("%02d"));
            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_STRING_OUTPUT(t, "01");
            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_STRING_OUTPUT(t, "-1");
            WHEN_SEND_FLOAT_TO(0, t, -22);
            REQUIRE_STRING_OUTPUT(t, "-22");
        }
    }
}
