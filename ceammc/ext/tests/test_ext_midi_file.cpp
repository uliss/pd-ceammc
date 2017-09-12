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
#include "../midi/midi_file.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_datatypes.h"

#include <fstream>
#include <stdio.h>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

typedef TestExtension<XMidiFile> MidiFileTest;

TEST_CASE("midi.file", "[externals]")
{
    SECTION("init")
    {
        setup_midi_file();

        MidiFileTest t("midi.file");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);
    }

    SECTION("read")
    {
        MidiFileTest t("midi.file");

        // not-exists
        WHEN_CALL_1(t, read, TEST_DATA_DIR "/not-exists.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // not-exists relative (search in standart paths)
        WHEN_CALL_1(t, read, "unknown.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // wrong argument count
        WHEN_CALL(t, read);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // wrong argument type
        WHEN_CALL_1(t, read, 234);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        WHEN_CALL_1(t, read, TEST_DATA_DIR "/test_01.mid");
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

    SECTION("bang")
    {
        MidiFileTest t("midi.file");
        WHEN_SEND_BANG_TO(0, t);
        REQUIRE(t.lastMessage(0).isData());
    }

    SECTION("onDataT")
    {
        DataTypeMidiStream str(TEST_DATA_DIR "/test_01.mid");
        REQUIRE(str.filename() == gensym("test_01.mid"));
        REQUIRE(str.tempo() == 480);
        REQUIRE(str.trackCount() == 1);
        REQUIRE(str.type() == data::DATA_MIDI_STREAM);

        MidiFileTest t("midi.file");
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
        MidiFileTest t("midi.file");

        // wrong argument count
        WHEN_CALL(t, write);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        // wrong argument type
        WHEN_CALL_1(t, read, 234);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        WHEN_CALL_1(t, write, "./test_midi_output.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        std::ifstream ifs("./test_midi_output.mid");
        REQUIRE(ifs);

        WHEN_CALL_1(t, write, "./test_midi_output.mid");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        REQUIRE_PROPERTY(t, @filename, "");
        REQUIRE_PROPERTY(t, @tracks, 1);
        REQUIRE_PROPERTY(t, @tempo, 120);

        REQUIRE(unlink("./test_midi_output.mid") == 0);
    }

    SECTION("clear")
    {
        MidiFileTest t("midi.file");
        WHEN_CALL(t, clear);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        WHEN_CALL_1(t, read, TEST_DATA_DIR "/test_01.mid");
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
