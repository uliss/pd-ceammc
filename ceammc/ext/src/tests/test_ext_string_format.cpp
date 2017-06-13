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
#include "ceammc_pd.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringFormat> StringFormatTest;

#define REQUIRE_STRING_OUTPUT(t, str_)                            \
    {                                                             \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                         \
        DataTypeString* s = t.typedLastDataAt<DataTypeString>(0); \
        REQUIRE(s != 0);                                          \
        REQUIRE(s->str() == str_);                                \
    }

#define NO_DATA(t) REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("string.format", "[external]")
{
    setup_string0x2eformat();

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

            t.dump();
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

        SECTION("format: %o")
        {
            SECTION("float mode")
            {
                StringFormatTest t("string.format", L1("%o"));

                WHEN_SEND_FLOAT_TO(0, t, 64);
                REQUIRE_STRING_OUTPUT(t, "64");

                WHEN_SEND_FLOAT_TO(0, t, 8);
                REQUIRE_STRING_OUTPUT(t, "8");

                WHEN_SEND_LIST_TO(0, t, L1(15));
                REQUIRE_STRING_OUTPUT(t, "15");
            }

            SECTION("int mode")
            {
                StringFormatTest t("string.format", L2("%o", "@int"));

                WHEN_SEND_FLOAT_TO(0, t, 64);
                REQUIRE_STRING_OUTPUT(t, "100");

                WHEN_SEND_FLOAT_TO(0, t, 8);
                REQUIRE_STRING_OUTPUT(t, "10");

                WHEN_SEND_LIST_TO(0, t, L1(15));
                REQUIRE_STRING_OUTPUT(t, "17");
            }
        }

        SECTION("format: %x")
        {
            StringFormatTest t("string.format", L2("%x", "@int"));

            WHEN_SEND_FLOAT_TO(0, t, 15);
            REQUIRE_STRING_OUTPUT(t, "f");

            WHEN_SEND_FLOAT_TO(0, t, 255);
            REQUIRE_STRING_OUTPUT(t, "ff");

            WHEN_SEND_LIST_TO(0, t, L1(32));
            REQUIRE_STRING_OUTPUT(t, "20");
        }

        SECTION("format: %x")
        {
            SECTION("float mode")
            {
                StringFormatTest t("string.format", L1("0x%X"));

                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "0x15");

                WHEN_SEND_FLOAT_TO(0, t, 255);
                REQUIRE_STRING_OUTPUT(t, "0x255");

                WHEN_SEND_LIST_TO(0, t, L1(32));
                REQUIRE_STRING_OUTPUT(t, "0x32");
            }

            SECTION("int mode ")
            {
                StringFormatTest t("string.format", L2("0x%X", "@int"));

                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "0xF");

                WHEN_SEND_FLOAT_TO(0, t, 255);
                REQUIRE_STRING_OUTPUT(t, "0xFF");

                WHEN_SEND_LIST_TO(0, t, L1(32));
                REQUIRE_STRING_OUTPUT(t, "0x20");
            }

            SECTION("#")
            {
                SECTION("int")
                {
                    StringFormatTest t("string.format", L2("%#X", "@int"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "0XF");

                    t.propSetFormat(L1("%#x"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "0xf");
                }

                SECTION("float")
                {
                    StringFormatTest t("string.format", L1("%#X"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "15.0000");

                    t.propSetFormat(L1("%#x"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "15.0000");
                }
            }

            SECTION("-")
            {
                StringFormatTest t("string.format", L2("%-d", "@int"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15");

                t.propSetFormat(L1("%-8d"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15      ");
            }

            SECTION(" ")
            {
                StringFormatTest t("string.format", L2("% d", "@int"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, " 15");

                t.propSetFormat(L1("% 8d"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "      15");
            }

            SECTION("*")
            {
                StringFormatTest t("string.format", L2("%2*d", "@int"));
                WHEN_SEND_LIST_TO(0, t, L2(12, 10));
                REQUIRE_STRING_OUTPUT(t, "          10");
            }

            SECTION(".")
            {
                StringFormatTest t("string.format", L1("%.*d"));
                WHEN_SEND_LIST_TO(0, t, L2(12, 10));
                REQUIRE_STRING_OUTPUT(t, "000000000010");

                t.propSetFormat(L1("%.3s"));
                WHEN_SEND_SYMBOL_TO(0, t, "ABCDEFG");
                REQUIRE_STRING_OUTPUT(t, "ABC");
            }

            SECTION("*")
            {
                StringFormatTest t("string.format", L1("%E"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "1.500000E+01");

                t.propSetFormat(L1("%e"));
                WHEN_SEND_FLOAT_TO(0, t, 0.15f);
                REQUIRE_STRING_OUTPUT(t, "1.500000e-01");
            }

            SECTION("f")
            {
                StringFormatTest t("string.format", L1("%f"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15.000000");

                t.propSetFormat(L1("%F"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15.000000");
            }

            SECTION("n")
            {
                StringFormatTest t("string.format", L1("%n"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }

            SECTION("%")
            {
                StringFormatTest t("string.format", L1("%s%%"));
                WHEN_SEND_SYMBOL_TO(0, t, "100");
                REQUIRE_STRING_OUTPUT(t, "100%");
            }
        }
    }

    SECTION("invalid args")
    {
        StringFormatTest t("string.format", L1("%d-%d-%d"));
        WHEN_SEND_FLOAT_TO(0, t, 15);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L2(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, L3(1, 2, 3));
        REQUIRE_STRING_OUTPUT(t, "1-2-3");

        WHEN_SEND_LIST_TO(0, t, L4(1, 2, 3, 4));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("list")
    {
        StringFormatTest t("string.format", L1("A:%s D:%d"));
        WHEN_SEND_LIST_TO(0, t, L2("string", 100));
        REQUIRE_STRING_OUTPUT(t, "A:string D:100");
    }

    SECTION("data")
    {
        Data d(new IntData(158));

        StringFormatTest t("string.format", L1("DATA-%s"));

        WHEN_SEND_DATA_TO<StringFormatTest, AbstractData>(0, t, d.data());
        REQUIRE_STRING_OUTPUT(t, "DATA-158");
    }
}
