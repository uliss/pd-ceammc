/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "ceammc_string.h"
#include "conv_char2morse.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvChar2Morse, conv, char2morse)

using namespace ceammc;

TEST_CASE("conv.char2morse", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TExt t("conv.char2morse");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_FLOAT(t, @sym, 0);
    }

    SECTION("numeric")
    {
        TObj t("conv.char2morse");

        t << "a";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1));
        t << "A";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1));
        t << "а";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1));
        t << "А";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1));
        std::map<std::string, std::string> map_en;

        // english case
        for (int c = 'A'; c <= 'Z'; c++) {
            char buf[] = { (char)c, 0 };
            t << buf;
            REQUIRE(t.hasNewMessages());
            auto msg = t.lastMessage();
            map_en[to_string(msg)] = buf;

            buf[0] = std::tolower(buf[0]);
            t << buf;
            REQUIRE(t.hasNewMessages());
            REQUIRE(msg == t.lastMessage());
        }

        REQUIRE(map_en.size() == 26);

        // russian case test
        const char* CYR_UPPER = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФЦЧШЩЪЫЬЭЮЯ";
        const char* CYR_LOWER = "абвгдеёжзийклмнопрстуфцчшщъыьэюя";

        std::vector<std::string> vec_up;
        string::utf8_split_by_char(vec_up, CYR_UPPER);
        std::vector<std::string> vec_low;
        string::utf8_split_by_char(vec_low, CYR_LOWER);

        REQUIRE(vec_up.size() == vec_low.size());

        for (size_t i = 0; i < vec_up.size(); i++) {
            t << vec_up[i].c_str();
            REQUIRE(t.hasNewMessages());
            auto msg = t.lastMessage();
            t << vec_low[i].c_str();
            REQUIRE(t.hasNewMessages());
            REQUIRE(msg == t.lastMessage());
        }

        std::map<std::string, std::string> map_up;
        for (auto& s : vec_up) {
            t << s.c_str();
            map_up[to_string(t.lastMessage())] = s;
        }

        // reverse key->values to values->keys
        std::map<std::string, std::string> map_up_vk;
        for (auto& kv : map_up) {
            map_up_vk[kv.second] = kv.first;
        }

        // ЁЕ and ЬЪ has same codes in russian
        REQUIRE(map_up.size() == (vec_up.size() - 2));
    }

    SECTION("alias")
    {
        TExt obj("conv.char2morse");
    }
}
