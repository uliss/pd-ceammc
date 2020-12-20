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
#include "midi_prg2str.h"
#include "test_midi_base.h"

#include <map>
#include <set>

typedef std::set<t_symbol*> NamesSet;
typedef std::map<t_symbol*, int> NamesMap;

PD_COMPLETE_TEST_SETUP(Prg2Str, midi, prg2str)

typedef TestExternal<Prg2Str> TObj;

TEST_CASE("midi.prg->str", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("midi.prg->str", LA("@symbol"));

        WHEN_SEND_FLOAT_TO(0, t, -2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 129.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 1000.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        NamesSet names;

        for (int i = 1; i < 128; i++) {
            WHEN_SEND_FLOAT_TO(0, t, i);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isSymbol());
            names.insert(t.lastMessage(0).atomValue().asSymbol());
        }

        REQUIRE(names.size() == 127);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Acoustic Grand Piano");

        WHEN_SEND_FLOAT_TO(0, t, 9);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Celesta");
    }

    SECTION("family")
    {
        TObj t("midi.prg->str", LA("@symbol", "@family"));

        WHEN_SEND_FLOAT_TO(0, t, -2);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 129.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_SEND_FLOAT_TO(0, t, 1000.f);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        NamesMap names;

        for (int i = 1; i <= 128; i++) {
            WHEN_SEND_FLOAT_TO(0, t, i);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isSymbol());
            t_symbol* s = t.lastMessage(0).atomValue().asSymbol();
            names[s]++;
        }

        REQUIRE(names.size() == 16);
        for (NamesMap::iterator it = names.begin(); it != names.end(); ++it) {
            REQUIRE(it->second == 8);
        }

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Piano");
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Piano");
        WHEN_SEND_FLOAT_TO(0, t, 8);
        REQUIRE_SYMBOL_AT_OUTLET(0, t, "Piano");
    }

    SECTION("str")
    {
        TExt t("midi.prg->str");
        t << 9;
        REQUIRE(dataAt(t) == StrA("Celesta"));

        TExt t2("midi.prg->str", LA("@family"));
        t2 << 8;
        REQUIRE(dataAt(t2) == StrA("Piano"));
    }

    SECTION("common")
    {
        for (size_t i = 1; i <= 128; i++) {
            REQUIRE(midi::instrument_name(i) != &s_);
        }

        REQUIRE(midi::instrument_index(midi::instrument_name(10)) == 10);

        for (size_t i = 1; i <= 128; i++) {
            REQUIRE(midi::instrument_index(midi::instrument_name(i)) == i);
        }
    }
}
