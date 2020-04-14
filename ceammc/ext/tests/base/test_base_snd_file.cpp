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
#include "ceammc_array.h"
#include "snd_file.h"
#include "test_external.h"

#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(SndFile, snd, file)

TEST_CASE("snd.file", "[externals]")
{
    pd_test_init();

    SECTION("test load")
    {
        TExt t("snd.file", "@verbose");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

#if defined(__APPLE__)
        REQUIRE_PROPERTY(t, @formats, AtomList("AAC", "AIFF", "ALAC", "AU", "AVR", "CAF", "FLAC", "HTK", "IFF", "MACE3:1", "MACE6:1", "MAT4", "MAT5", "MP3", "MP4", "MPC", "OGG", "PAF", "PVF", "RAW", "RF64", "SD2", "SDS", "SF", "VOC", "W64", "WAV", "WAVEX", "WVE", "XI"));
#endif

        auto arr = cnv->createArray("snd_file1", 100);

        t.call("load");
        REQUIRE(!t.hasNewMessages(0));

        t <<= AtomList::parseString("load 1.wav");
        REQUIRE(!t.hasNewMessages(0));

        t <<= AtomList::parseString("load 1.wav to");
        REQUIRE(!t.hasNewMessages(0));

        t <<= AtomList::parseString("load not_exists.wav to array1");
        REQUIRE(!t.hasNewMessages(0));

        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/test_ch1_44100_8820.wav to array1");
        REQUIRE(!t.hasNewMessages(0));

        // no resize
        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav to snd_file1");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t) == 100);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 100);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 100);

        // array resize
        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav to snd_file1 @r");
        REQUIRE(floatAt(t) == 441);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 441);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 441);

        for (auto& a : *arr)
            REQUIRE(a == Approx(0));

        // default channel (0)
        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @r");
        REQUIRE(floatAt(t) == 441);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 441);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 441);

        for (auto& a : *arr)
            REQUIRE(a == Approx(-1).epsilon(0.01));

        // channel 1
        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @r @ch 1");
        REQUIRE(floatAt(t) == 441);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 441);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 441);

        for (auto& a : *arr)
            REQUIRE(a == Approx(1).epsilon(0.01));

        // channel 0 @length 100 samples
        t <<= AtomList::parseString("load " TEST_DATA_DIR
                                    "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @r @ch 0 @l 100");
        REQUIRE(floatAt(t) == 100);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 100);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 100);

        for (auto& a : *arr)
            REQUIRE(a == Approx(-1).epsilon(0.01));

        // channel 1 @length 100 samples
        t <<= AtomList::parseString("load " TEST_DATA_DIR
                                    "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @r @ch 0 @l 100");
        REQUIRE(floatAt(t) == 100);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 100);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 100);

        for (auto& a : *arr)
            REQUIRE(a == Approx(-1).epsilon(0.01));

        // array pattern
        t <<= AtomList::parseString("load " TEST_DATA_DIR "/base/snd0_ch03_44.1k_441samp.wav to snd_file[0-4] @r @ch 1");
        REQUIRE(floatAt(t) == 441);
        REQUIRE(arr->update());
        REQUIRE(arr->size() == 441);
        REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch03_44.1k_441samp.wav");
        REQUIRE_PROPERTY(t, @sr, 44100);
        REQUIRE_PROPERTY(t, @samples, 441);

        for (auto& a : *arr)
            REQUIRE(a == Approx(0));
    }

#if !defined(__APPLE__) && defined(__clang__)
    SECTION("coreaudio tests")
    {
        AtomList args;
        TObj sf("snd.file", args);

        sf.storeMessageCount();
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL(sf, info);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL_N(sf, info, 123);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL_N(sf, info, "unknown");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        sf.storeMessageCount();
        WHEN_CALL_N(sf, info, TEST_DATA_DIR "/test_data0.mp3");
        REQUIRE(sf.hasNewMessages());
        AtomList info = sf.lastMessage().anyValue();
        AtomList prop;
        REQUIRE(info.property(SYM("@channels"), &prop));
        REQUIRE(prop.asT<size_t>() == 1);
        REQUIRE(info.property(SYM("@samplerate"), &prop));
        REQUIRE(prop.asT<size_t>() == 44100);
        REQUIRE(info.property(SYM("@samples"), &prop));
        REQUIRE(prop.asT<size_t>() == 441);

        REQUIRE(info.property(SYM("@duration"), &prop));
        REQUIRE(prop.at(0).asFloat() == 0.01f);

        sf.cleanMessages();
        sf.storeMessageCount(0);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);
        WHEN_CALL_N(sf, info, TEST_DATA_DIR "/test_data0.m4a");
        REQUIRE(sf.hasNewMessages(0));
        info = sf.lastMessage().anyValue();
        prop.clear();
        REQUIRE(info.property(SYM("@channels"), &prop));
        REQUIRE(prop.asT<size_t>() == 1);
        REQUIRE(info.property(SYM("@samplerate"), &prop));
        REQUIRE(prop.asT<size_t>() == 44100);
        REQUIRE(info.property(SYM("@samples"), &prop));
        REQUIRE(prop.asT<size_t>() == 441);
    }
#endif
}
