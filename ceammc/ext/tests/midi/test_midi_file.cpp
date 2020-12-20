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
#include "ceammc_platform.h"
#include "midi_file.h"
#include "test_midi_base.h"

#include <fstream>

PD_COMPLETE_TEST_SETUP(XMidiFile, midi, file)

TEST_CASE("midi.file", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("midi.file");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @filename, "");
            REQUIRE_PROPERTY(t, @tracks, 1);
            REQUIRE_PROPERTY(t, @tempo, 120);
            REQUIRE_PROPERTY(t, @length_sec, 0.f);
            REQUIRE_PROPERTY(t, @length_tick, 0.f);
            REQUIRE_PROPERTY(t, @length_beat, 0.f);
        }

        SECTION("path")
        {
            TExt t("midi.file", TEST_SRC_DIR "/test_01.mid");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @filename, "test_01.mid");
            REQUIRE_PROPERTY(t, @tracks, 1);
            REQUIRE_PROPERTY(t, @tempo, 480);
            REQUIRE_PROPERTY(t, @length_sec, 1.975);
            REQUIRE_PROPERTY(t, @length_tick, 1896);
            REQUIRE_PROPERTY(t, @length_beat, 3.95);
        }
    }

    SECTION("read")
    {
        TObj t("midi.file");

        // not-exists
        WHEN_CALL_N(t, read, TEST_SRC_DIR "/not-exists.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // not-exists relative (search in standart paths)
        WHEN_CALL_N(t, read, "unknown.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);
        REQUIRE_PROPERTY(t, @length_sec, Atom(0.f));
        REQUIRE_PROPERTY(t, @length_tick, Atom(0.f));
        REQUIRE_PROPERTY(t, @length_beat, Atom(0.f));

        // wrong argument count
        WHEN_CALL(t, read);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // wrong argument type
        WHEN_CALL_N(t, read, 234);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        WHEN_CALL_N(t, read, TEST_SRC_DIR "/test_01.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "test_01.mid");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 480);
        REQUIRE_PROPERTY(t, @length_sec, Atom(1.975));
        REQUIRE_PROPERTY(t, @length_tick, Atom(1896));
        REQUIRE_PROPERTY(t, @length_beat, Atom(3.95));

        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);
        REQUIRE_PROPERTY(t, @length_sec, Atom(0.f));
        REQUIRE_PROPERTY(t, @length_tick, Atom(0.f));
        REQUIRE_PROPERTY(t, @length_beat, Atom(0.f));
    }

    SECTION("bang")
    {
        TObj t("midi.file");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.lastMessage(0).isData());
    }

    SECTION("onDataT")
    {
        DataTypeMidiStream str(TEST_SRC_DIR "/test_01.mid");
        REQUIRE(str.filename() == gensym("test_01.mid"));
        REQUIRE(str.tempo() == 480);
        REQUIRE(str.trackCount() == 1);

        TObj t("midi.file");
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        WHEN_SEND_TDATA_TO(0, t, str);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "test_01.mid");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 480);
    }

    SECTION("write")
    {
        TObj t("midi.file");

        // wrong argument count
        WHEN_CALL(t, write);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // wrong argument type
        WHEN_CALL_N(t, read, 234);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        WHEN_CALL_N(t, write, TEST_MIDI_DIR "/test_midi_output.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        std::ifstream ifs(TEST_MIDI_DIR "/test_midi_output.mid");
        REQUIRE(ifs);

        WHEN_CALL_N(t, write, TEST_MIDI_DIR "/test_midi_output.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        ifs.close();
        REQUIRE(platform::path_exists(TEST_MIDI_DIR "/test_midi_output.mid"));
        REQUIRE(platform::remove(TEST_MIDI_DIR "/test_midi_output.mid"));
    }

    SECTION("clear")
    {
        TObj t("midi.file");
        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL_N(t, read, TEST_SRC_DIR "/test_01.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "test_01.mid");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 480);

        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);
    }
}
