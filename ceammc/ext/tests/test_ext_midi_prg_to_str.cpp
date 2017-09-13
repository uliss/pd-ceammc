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
#include "../midi/midi_common.h"
#include "../midi/midi_prg2str.h"
#include "../string/datatype_string.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <map>
#include <set>

#define REQUIRE_STRING_OUTPUT(t, str_)                                  \
    {                                                                   \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                               \
        const DataTypeString* s = t.typedLastDataAt<DataTypeString>(0); \
        REQUIRE(s != 0);                                                \
        REQUIRE(s->str() == str_);                                      \
    }

typedef std::set<t_symbol*> NamesSet;
typedef std::map<t_symbol*, int> NamesMap;

typedef TestExtension<Prg2Str> Prg2StrTest;

TEST_CASE("midi.prg->str", "[externals]")
{
    pd_init();

    SECTION("init")
    {
        setup_midi_prg2str();

        Prg2StrTest t("midi.prg->str", L1("@symbol"));

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
        Prg2StrTest t("midi.prg->str", L2("@symbol", "@family"));

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
        Prg2StrTest t("midi.prg->str");
        WHEN_SEND_FLOAT_TO(0, t, 9);
        REQUIRE_STRING_OUTPUT(t, "Celesta");

        Prg2StrTest t2("midi.prg->str", L1("@family"));
        WHEN_SEND_FLOAT_TO(0, t2, 8);
        REQUIRE_STRING_OUTPUT(t2, "Piano");
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
