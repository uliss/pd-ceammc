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
#include "../midi/datatype_midistream.h"
#include "../midi/midi_track.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_datatypes.h"

#include <fstream>
#include <stdio.h>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

static const float Z = 0.f;

typedef TestExtension<MidiTrack> MidiTrackTest;

static bool approxCompare(const AtomList& l1, const AtomList& l2)
{
    if (l1.size() != l2.size()) {
        std::cerr << "\t" << l1 << "\n\t != \n\t" << l2 << "\n";
        return false;
    }

    for (size_t i = 0; i < l1.size(); i++) {
        if (l1[i].isFloat() && l2[i].isFloat()) {
            if (l1[i].asFloat() != Approx(l2[i].asFloat())) {
                std::cerr << "\t" << l1 << "\n\t != \n\t" << l2 << "\n";
                return false;
            } else
                continue;
        }

        if (l1[i] != l2[i]) {
            std::cerr << "\t" << l1 << "\n\t != \n\t" << l2 << "\n";
            return false;
        }
    }

    return true;
}

TEST_CASE("midi.track", "[externals]")
{
    SECTION("init")
    {
        setup_midi_track();

        MidiTrackTest t("midi.track");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        REQUIRE_PROPERTY(t, @join, A(0.f));
        REQUIRE_PROPERTY(t, @index, A(0.f));
        REQUIRE_PROPERTY(t, @tempo, A(120));
        REQUIRE_PROPERTY(t, @events, A(0.f));
        REQUIRE_PROPERTY(t, @current, A(0.f));

        REQUIRE(t.begin() == t.end());
        REQUIRE(t.size() == 0);
        REQUIRE_FALSE(t.seekAbs(0));
        REQUIRE_FALSE(t.seekAbs(1));
    }

    SECTION("real")
    {
        DataTypeMidiStream m(TEST_DATA_DIR "/test_01.mid");
        REQUIRE(m.is_open());

        MidiTrackTest t("midi.track");
        WHEN_SEND_TDATA_TO(0, t, m);
        REQUIRE_PROPERTY(t, @join, A(0.f));
        REQUIRE_PROPERTY(t, @index, A(0.f));
        REQUIRE_PROPERTY(t, @tempo, A(480));
        REQUIRE_PROPERTY(t, @events, A(19));
        REQUIRE(t.begin() != t.end());
        REQUIRE(t.size() == 19);

        REQUIRE(t.findNextTickEventIndex(0) == 11);
        REQUIRE(t.findNextTickEventIndex(11) == 12);
        REQUIRE(t.findNextTickEventIndex(12) == 13);
        REQUIRE(t.findNextTickEventIndex(13) == 14);
        REQUIRE(t.findNextTickEventIndex(14) == 15);
        REQUIRE(t.findNextTickEventIndex(15) == 16);
        REQUIRE(t.findNextTickEventIndex(16) == 17);
        REQUIRE(t.findNextTickEventIndex(17) == 18);
        REQUIRE(t.findNextTickEventIndex(18) == 19);
        REQUIRE(t.findNextTickEventIndex(19) == 0);

        REQUIRE(t.seekAbs(0));
        REQUIRE_PROPERTY(t, @current, A(0.f));

        REQUIRE(t.seekAbs(1));
        REQUIRE_PROPERTY(t, @current, A(11));

        REQUIRE(t.seekAbs(2));
        REQUIRE_PROPERTY(t, @current, A(12));

        REQUIRE(t.seekAbs(3));
        REQUIRE_PROPERTY(t, @current, A(13));

        REQUIRE(t.seekAbs(8));
        REQUIRE_PROPERTY(t, @current, A(18));

        REQUIRE_FALSE(t.seekAbs(9));
        REQUIRE_PROPERTY(t, @current, A(18));

        REQUIRE(t.seekAbs(0));
        REQUIRE_PROPERTY(t, @current, A(Z));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.95834));
        t.cleanAllMessages();

        REQUIRE(t.seekAbs(1));
        REQUIRE_PROPERTY(t, @current, A(11));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));

        REQUIRE(t.messageCount(0) == 1);
        REQUIRE(t.messageCount(1) == 1);
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(26.04167));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 455, Z, Z, 144, 60, Z)));

        t.m_next(0, AtomList());
        REQUIRE_PROPERTY(t, @current, A(12));

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.958));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 480, Z, 473.958, 144, 62, 80)));

        t.m_next(0, AtomList());
        REQUIRE_PROPERTY(t, @current, A(13));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(26.04167));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 935, Z, Z, 144, 62, Z)));

        t.m_next(0, AtomList());
        REQUIRE_PROPERTY(t, @current, A(14));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.958));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 960, Z, 473.958, 144, 64, 80)));

        t.m_next(0, AtomList());
        REQUIRE_PROPERTY(t, @current, A(15));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(26.04167));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 1415, Z, Z, 144, 64, Z)));

        t.m_next(0, AtomList());
        REQUIRE_PROPERTY(t, @current, A(16));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.958));
        REQUIRE(approxCompare(t.lastMessage(0).anyValue(), L7("MidiEvent", 1440, Z, 473.958, 144, 65, 80)));
    }
}
