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
#include "string_format.h"
#include "test_string_base.h"

PD_COMPLETE_TEST_SETUP(StringFormat, string, format)

#define REQUIRE_STRING_OUTPUT(t, str_)                                               \
    {                                                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                                            \
        const DataTypeString* s = t.lastMessage().atomValue().asD<DataTypeString>(); \
        REQUIRE(s != 0);                                                             \
        REQUIRE(s->str() == str_);                                                   \
    }

#define NO_DATA(t) REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

TEST_CASE("string.format", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("string.format");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @format, L());

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_NO_MSG(t);

            t.dump();
        }

        SECTION("no-%")
        {
            TObj t("string.format", LA("SAMPLE"));
            REQUIRE_PROPERTY_LIST(t, @format, LA("SAMPLE"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_STRING_OUTPUT(t, "");

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_NO_MSG(t);

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_NO_MSG(t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 2));
            REQUIRE_NO_MSG(t);
        }
    }

    SECTION("simple format")
    {
        SECTION("format: %d")
        {
            TObj t("string.format", LA("%d"));

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

            WHEN_SEND_LIST_TO(0, t, LF(100));
            REQUIRE_STRING_OUTPUT(t, "100");

            WHEN_SEND_LIST_TO(0, t, LF(100, 200));
            REQUIRE_NO_MSG(t);
        }

        SECTION("format: %d++")
        {
            TObj t("string.format", LA("%+d"));

            WHEN_SEND_FLOAT_TO(0, t, 123);
            REQUIRE_STRING_OUTPUT(t, "+123");

            WHEN_SEND_FLOAT_TO(0, t, -123);
            REQUIRE_STRING_OUTPUT(t, "-123");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_STRING_OUTPUT(t, "ABC");

            t.setProperty("@format", LA("%4d"));

            WHEN_SEND_FLOAT_TO(0, t, 1);
            REQUIRE_STRING_OUTPUT(t, "   1");

            WHEN_SEND_FLOAT_TO(0, t, -1);
            REQUIRE_STRING_OUTPUT(t, "  -1");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_STRING_OUTPUT(t, " ABC");

            t.setProperty("@format", LA("%02d"));
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
                TObj t("string.format", LA("%o"));

                WHEN_SEND_FLOAT_TO(0, t, 64);
                REQUIRE_STRING_OUTPUT(t, "64");

                WHEN_SEND_FLOAT_TO(0, t, 8);
                REQUIRE_STRING_OUTPUT(t, "8");

                WHEN_SEND_LIST_TO(0, t, LF(15));
                REQUIRE_STRING_OUTPUT(t, "15");
            }

            SECTION("int mode")
            {
                TObj t("string.format", LA("%o", "@int"));

                WHEN_SEND_FLOAT_TO(0, t, 64);
                REQUIRE_STRING_OUTPUT(t, "100");

                WHEN_SEND_FLOAT_TO(0, t, 8);
                REQUIRE_STRING_OUTPUT(t, "10");

                WHEN_SEND_LIST_TO(0, t, LF(15));
                REQUIRE_STRING_OUTPUT(t, "17");
            }
        }

        SECTION("format: %x")
        {
            TObj t("string.format", LA("%x", "@int"));

            WHEN_SEND_FLOAT_TO(0, t, 15);
            REQUIRE_STRING_OUTPUT(t, "f");

            WHEN_SEND_FLOAT_TO(0, t, 255);
            REQUIRE_STRING_OUTPUT(t, "ff");

            WHEN_SEND_LIST_TO(0, t, LF(32));
            REQUIRE_STRING_OUTPUT(t, "20");
        }

        SECTION("format: %x")
        {
            SECTION("float mode")
            {
                TObj t("string.format", LA("0x%X"));

                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "0x15");

                WHEN_SEND_FLOAT_TO(0, t, 255);
                REQUIRE_STRING_OUTPUT(t, "0x255");

                WHEN_SEND_LIST_TO(0, t, LF(32));
                REQUIRE_STRING_OUTPUT(t, "0x32");
            }

            SECTION("int mode ")
            {
                TObj t("string.format", LA("0x%X", "@int"));
                REQUIRE_PROPERTY(t, @format, S("0x%X"));

                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "0xF");

                WHEN_SEND_FLOAT_TO(0, t, 255);
                REQUIRE_STRING_OUTPUT(t, "0xFF");

                WHEN_SEND_LIST_TO(0, t, LF(32));
                REQUIRE_STRING_OUTPUT(t, "0x20");
            }

            SECTION("#")
            {
                SECTION("int")
                {
                    TObj t("string.format", LA("%#X", "@int"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "0XF");

                    t.propSetFormat(LA("%#x"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "0xf");
                }

                SECTION("float")
                {
                    TObj t("string.format", LA("%#X"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "15.0000");

                    t.propSetFormat(LA("%#x"));
                    WHEN_SEND_FLOAT_TO(0, t, 15);
                    REQUIRE_STRING_OUTPUT(t, "15.0000");
                }
            }

            SECTION("-")
            {
                TObj t("string.format", LA("%-d", "@int"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15");

                t.propSetFormat(LA("%-8d"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15      ");
            }

            SECTION(" ")
            {
                TObj t("string.format", LP("% d @int"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, " 15");

                t.propSetFormat(LP("% 8d"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "      15");
            }

            SECTION("*")
            {
                TObj t("string.format", LA("%2*d", "@int"));
                WHEN_SEND_LIST_TO(0, t, LF(12, 10));
                REQUIRE_STRING_OUTPUT(t, "          10");
            }

            SECTION(".")
            {
                TObj t("string.format", LA("%.*d"));
                WHEN_SEND_LIST_TO(0, t, LF(12, 10));
                REQUIRE_STRING_OUTPUT(t, "000000000010");

                t.propSetFormat(LA("%.3s"));
                WHEN_SEND_SYMBOL_TO(0, t, "ABCDEFG");
                REQUIRE_STRING_OUTPUT(t, "ABC");
            }

            SECTION("*")
            {
                TObj t("string.format", LA("%E"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "1.500000E+01");

                t.propSetFormat(LA("%e"));
                WHEN_SEND_FLOAT_TO(0, t, 0.15f);
                REQUIRE_STRING_OUTPUT(t, "1.500000e-01");
            }

            SECTION("f")
            {
                TObj t("string.format", LA("%f"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15.000000");

                t.propSetFormat(LA("%F"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_STRING_OUTPUT(t, "15.000000");
            }

            SECTION("n")
            {
                TObj t("string.format", LA("%n"));
                WHEN_SEND_FLOAT_TO(0, t, 15);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
            }

            SECTION("%")
            {
                TObj t("string.format", LA("%s%%"));
                WHEN_SEND_SYMBOL_TO(0, t, "100");
                REQUIRE_STRING_OUTPUT(t, "100%");
            }
        }
    }

    SECTION("invalid args")
    {
        TObj t("string.format", LA("%d-%d-%d"));
        WHEN_SEND_FLOAT_TO(0, t, 15);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3));
        REQUIRE_STRING_OUTPUT(t, "1-2-3");

        WHEN_SEND_LIST_TO(0, t, LF(1, 2, 3, 4));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("list")
    {
        TObj t("string.format", LP("\"A:%s\" \"D:%d\""));
        WHEN_SEND_LIST_TO(0, t, LA("string", 100));
        REQUIRE_STRING_OUTPUT(t, "A:string D:100");
    }

    SECTION("data")
    {
        //        DataPtr d(new IntData(158));

        //        TObj t("string.format", LA("DATA-%s"));

        //        WHEN_SEND_DATA_TO(0, t, d);
        //        REQUIRE_STRING_OUTPUT(t, "DATA-158");
    }
}
