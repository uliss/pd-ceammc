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
#include "datatype_midistream.h"
#include "midi_track.h"
#include "test_midi_base.h"

#include <fstream>

PD_COMPLETE_TEST_SETUP(MidiTrack, midi, track)

TEST_CASE("midi.track", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("midi.track");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);

        REQUIRE_PROPERTY(t, @join, 0.f);
        REQUIRE_PROPERTY(t, @track, 0.f);
        REQUIRE_PROPERTY(t, @tempo, 120);
        REQUIRE_PROPERTY(t, @nevents, 0.f);
        REQUIRE_PROPERTY(t, @current, 0.f);
        REQUIRE_PROPERTY(t, @state, 0.f);
        REQUIRE_PROPERTY(t, @speed, 1);

        REQUIRE(t.begin() == t.end());
        REQUIRE(t.size() == 0);
        REQUIRE_FALSE(t.seekAbs(0));
        REQUIRE_FALSE(t.seekAbs(1));

        t.setProperty("@speed", A(-2));
        REQUIRE_PROPERTY(t, @speed, 1);

        t.setProperty("@speed", A(0.1));
        REQUIRE_PROPERTY(t, @speed, A(0.1));
    }

    SECTION("real")
    {
        DataTypeMidiStream m(TEST_SRC_DIR "/test_01.mid");
        REQUIRE(m.is_open());

        TObj t("midi.track");
        WHEN_SEND_TDATA_TO(0, t, m);
        REQUIRE_PROPERTY(t, @join, 0.);
        REQUIRE_PROPERTY(t, @track, 0.);
        REQUIRE_PROPERTY(t, @tempo, 480);
        REQUIRE_PROPERTY(t, @nevents, 19);
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
        REQUIRE_PROPERTY(t, @current, 0.);

        REQUIRE(t.seekAbs(1));
        REQUIRE_PROPERTY(t, @current, 11);

        REQUIRE(t.seekAbs(2));
        REQUIRE_PROPERTY(t, @current, 12);

        REQUIRE(t.seekAbs(3));
        REQUIRE_PROPERTY(t, @current, 13);

        REQUIRE(t.seekAbs(8));
        REQUIRE_PROPERTY(t, @current, 18);

        REQUIRE_FALSE(t.seekAbs(9));
        REQUIRE_PROPERTY(t, @current, 18);

        REQUIRE(t.seekAbs(0));
        REQUIRE_PROPERTY(t, @current, 0.);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 1_out) == Approx(473.958));
        t.cleanAllMessages();

        REQUIRE(t.seekAbs(1));
        REQUIRE_PROPERTY(t, @current, 11);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));

        REQUIRE(t.messageCount(0) == 1);
        REQUIRE(floatAt(t, 1_out) == Approx(26.04167));
        REQUIRE(t.lastMessage(0).anyValue() == LA("MidiEvent", 455, 0., 0., 144, 60, 0.));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 12);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 1_out) == Approx(473.958));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 480, 0., 473.958, 144, 62, 80));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 13);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 1_out) == Approx(26.04167));
        REQUIRE(t.lastMessage(0).anyValue() == LA("MidiEvent", 935, 0., 0., 144, 62, 0.));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 14);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 1_out) == Approx(473.958));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 960, 0., 473.958, 144, 64, 80));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 15);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 1_out) == Approx(26.04167));
        REQUIRE(t.lastMessage(0).anyValue() == LA("MidiEvent", 1415, 0., 0., 144, 64, 0.));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 16);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(floatAt(t, 1_out) == Approx(473.958));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 1440, 0., 473.958, 144, 65, 80));

        WHEN_CALL(t, reset);
        REQUIRE_PROPERTY(t, @current, A(0.));

        WHEN_CALL_N(t, seek, 1);
        REQUIRE_PROPERTY(t, @current, 11);

        WHEN_CALL_N(t, seek, 0.);
        REQUIRE_PROPERTY(t, @current, A(0.));

        WHEN_CALL_N(t, seek, 3);
        REQUIRE_PROPERTY(t, @current, 13);
    }

    SECTION("play/stop")
    {
        DataTypeMidiStream m(TEST_SRC_DIR "/test_01.mid");
        REQUIRE(m.is_open());

        TObj t("midi.track");
        WHEN_SEND_TDATA_TO(0, t, m);

        t.seekAbs(0);
        REQUIRE_PROPERTY(t, @state, 0.);

        WHEN_CALL(t, play);
        REQUIRE_PROPERTY(t, @state, 1);
        REQUIRE_PROPERTY(t, @current, 11);

        WHEN_CALL(t, pause);
        REQUIRE_PROPERTY(t, @state, 2);
        REQUIRE_PROPERTY(t, @current, 11);

        // twice paused
        WHEN_CALL(t, pause);
        REQUIRE_PROPERTY(t, @state, 2);
        REQUIRE_PROPERTY(t, @current, 11);

        WHEN_CALL(t, play);
        REQUIRE_PROPERTY(t, @state, 1);
        REQUIRE_PROPERTY(t, @current, 12);

        // twice playing
        WHEN_CALL(t, play);
        REQUIRE_PROPERTY(t, @state, 1);
        REQUIRE_PROPERTY(t, @current, 12);

        WHEN_CALL(t, stop);
        REQUIRE_PROPERTY(t, @state, 0.);
        REQUIRE_PROPERTY(t, @current, 0.);

        // twice stopped
        WHEN_CALL(t, stop);
        REQUIRE_PROPERTY(t, @state, 0.);
        REQUIRE_PROPERTY(t, @current, 0.);

        // pause stopped
        WHEN_CALL(t, pause);
        REQUIRE_PROPERTY(t, @state, 0.);
        REQUIRE_PROPERTY(t, @current, 0.);
    }

    SECTION("track count")
    {
        DataTypeMidiStream m(TEST_SRC_DIR "/test_01.mid");
        REQUIRE(m.is_open());

        TObj t("midi.track", LA("@track", 2));
        REQUIRE_PROPERTY(t, @track, 2);

        WHEN_SEND_TDATA_TO(0, t, m);
        REQUIRE_PROPERTY(t, @join, 0.);
        REQUIRE_PROPERTY(t, @track, 2);
        REQUIRE_PROPERTY(t, @tempo, 120);
        REQUIRE_PROPERTY(t, @nevents, 0.);
        REQUIRE(t.begin() == t.end());
        REQUIRE(t.size() == 0);
    }

    SECTION("speed")
    {
        DataTypeMidiStream m(TEST_SRC_DIR "/test_01.mid");
        REQUIRE(m.is_open());

        TObj t("midi.track", LA("@speed", 0.5));
        WHEN_SEND_TDATA_TO(0, t, m);
        REQUIRE(t.size() == 19);

        REQUIRE(t.seekAbs(0));
        REQUIRE_PROPERTY(t, @current, A(0.));
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.95834 * 2));
        t.cleanAllMessages();

        REQUIRE(t.seekAbs(1));
        REQUIRE_PROPERTY(t, @current, 11);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));

        REQUIRE(t.messageCount(0) == 1);
        REQUIRE(t.messageCount(1) == 1);
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(26.04167 * 2));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 455, 0., 0., 144, 60, 0.));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 12);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(473.958 * 2));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 480, 0., 473.958 * 2, 144, 62, 80));

        t.m_next(0, L());
        REQUIRE_PROPERTY(t, @current, 13);
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.hasNewMessages(1));
        REQUIRE(t.lastMessage(1).atomValue().asFloat() == Approx(26.04167 * 2));
        REQUIRE(t.lastMessage(0).anyValue() == LAX("MidiEvent", 935, 0., 0., 144, 62, 0.));
    }
}
