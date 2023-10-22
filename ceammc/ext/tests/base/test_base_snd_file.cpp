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

static ArrayPtr testArray1(t_float v = 0.0, size_t sz = 100)
{
    static ArrayPtr arr = PureData::instance().findCanvas("test_canvas")->createArray("snd_file1", 100);
    arr->resize(sz);
    arr->fillWith(v);
    return arr;
}

static ArrayPtr testArray2(t_float v = 0.0, size_t sz = 200)
{
    static ArrayPtr arr = PureData::instance().findCanvas("test_canvas")->createArray("snd_file2", 200);
    arr->resize(sz);
    arr->fillWith(v);
    return arr;
}

template <typename It>
bool all_eq(It b, It e, t_float v)
{
    return std::all_of(b, e, [v](t_float f) { return f == Approx(v).epsilon(0.001); });
}

static AtomList load_str(const char* s)
{
    return AtomList::parseString(s);
}

TEST_CASE("snd.file", "[externals]")
{
    pd_test_init();

    SECTION("test load")
    {
        TExt t("snd.file", "@verbose", "@.debug");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        auto arr1 = testArray1();
        auto arr2 = testArray2();

        SECTION("formats")
        {
#if defined(__APPLE__)
            REQUIRE_PROPERTY(t, @formats, AtomList("AAC", "AIFF", "ALAC", "AU", "AVR", "CAF", "FLAC", "HTK", "IFF", "MACE3:1", "MACE6:1", "MAT4", "MAT5", "MP3", "MP4", "MPC", "OGG", "PAF", "PVF", "RAW", "RF64", "SD2", "SDS", "SF", "VOC", "W64", "WAV", "WAVEX", "WVE", "XI"));
#endif
        }

        SECTION("invalid formats")
        {
            t.call("load");
            REQUIRE(!t.hasNewMessages(0));

            t <<= load_str("load 1.wav");
            REQUIRE(!t.hasNewMessages(0));

            t <<= load_str("load 1.wav to");
            REQUIRE(!t.hasNewMessages(0));

            // file not exists
            t <<= load_str("load not_exists.wav to array1");
            REQUIRE(!t.hasNewMessages(0));

            // array not exists
            t <<= load_str("load " TEST_DATA_DIR
                           "/base/snd0_ch01_44.1k_441samp.wav to array1");
            REQUIRE(!t.hasNewMessages(0));

            t <<= load_str("load " TEST_DATA_DIR ""
                           "/base/snd0_ch01_44.1k_441samp.wav to snd_file1 "
                           "@begin 20 @end 20");
            REQUIRE(!t.hasNewMessages(0));
        }

        SECTION("load no resize")
        {
            arr1->fillWith(0.5);
            // no resize
            t <<= load_str("load " TEST_DATA_DIR
                           "/base/snd0_ch01_44.1k_441samp.wav to snd_file1");
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(floatAt(t) == 100);
            REQUIRE(arr1->update());
            REQUIRE(arr1->size() == 100);
            REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav");
            REQUIRE_PROPERTY(t, @sr, 44100);
            REQUIRE_PROPERTY(t, @samples, 100);
            REQUIRE_PROPERTY(t, @channels, 0.);

            REQUIRE(all_eq(arr1->begin(), arr1->end(), 0));
        }

        SECTION("load with resize")
        {
            arr1->fillWith(0.5);
            // resize
            t <<= load_str("load " TEST_DATA_DIR
                           "/base/snd0_ch01_44.1k_441samp.wav to snd_file1 @r");
            REQUIRE(floatAt(t) == 441);
            REQUIRE(arr1->update());
            REQUIRE(arr1->size() == 441);
            REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch01_44.1k_441samp.wav");
            REQUIRE_PROPERTY(t, @sr, 44100);
            REQUIRE_PROPERTY(t, @samples, 441);

            REQUIRE(all_eq(arr1->begin(), arr1->end(), 0));
        }

        SECTION("channels")
        {
            SECTION("load multichannel with default channel 0")
            {
                // default channel (0)
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1");
                REQUIRE(floatAt(t) == 100);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 100);
                REQUIRE_PROPERTY(t, @channels, 0.);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("load multichannel with channel 0")
            {
                // channel 0
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @ch 0");
                REQUIRE(floatAt(t) == 100);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 100);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("load multichannel with channel 1")
            {
                // channel 1
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @ch 1");
                REQUIRE(floatAt(t) == 100);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 100);
                REQUIRE_PROPERTY(t, @channels, 1);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }

            SECTION("load multichannel with channel 4")
            {
                // channel 1
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch07_44.1k_441samp.wav to snd_file1 @ch 4");
                REQUIRE(floatAt(t) == 100);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch07_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 100);
                REQUIRE_PROPERTY(t, @channels, 4);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 0.33333));
            }
        }

        SECTION("length no resize")
        {
            SECTION("@length 50")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @l 50");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 50, -1));
                REQUIRE(all_eq(arr1->begin() + 50, arr1->end(), 0));
            }

            SECTION("@length 50 @array_offset 25")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff 25");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 25, 0));
                REQUIRE(all_eq(arr1->begin() + 25, arr1->begin() + 75, -1));
                REQUIRE(all_eq(arr1->begin() + 75, arr1->end(), 0));
            }

            SECTION("@length 50 @array_offset 50")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff 50");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 50, 0));
                REQUIRE(all_eq(arr1->begin() + 50, arr1->end(), -1));
            }

            SECTION("@length 50 @array_offset 51")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff 51");
                REQUIRE(floatAt(t) == 49);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 49);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 51, 0));
                REQUIRE(all_eq(arr1->begin() + 51, arr1->end(), -1));
            }

            SECTION("@length 50 @array_offset 99")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff 99");
                REQUIRE(floatAt(t) == 1);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 1);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 99, 0));
                REQUIRE(all_eq(arr1->begin() + 99, arr1->end(), -1));
            }

            SECTION("@length 50 @array_offset 100")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff 100");
                REQUIRE(!t.hasNewMessages(0));
            }

            SECTION("@length 50 @array_offset end")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff end");
                REQUIRE(!t.hasNewMessages(0));
            }

            SECTION("@length 50 @array_offset end+1")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff end+1");
                REQUIRE(!t.hasNewMessages(0));
            }

            SECTION("@length 50 @array_offset $-1")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff $-1");
                REQUIRE(floatAt(t) == 1);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 1);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 99, 0));
                REQUIRE(all_eq(arr1->begin() + 99, arr1->end(), -1));
            }

            SECTION("@length 50 @array_offset $-50")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff $-50");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 50, 0));
                REQUIRE(all_eq(arr1->begin() + 50, arr1->end(), -1));
            }

            SECTION("@length 50 @array_offset $-99")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 "
                               "@l 50 @aoff $-99");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 1, 0));
                REQUIRE(all_eq(arr1->begin() + 1, arr1->begin() + 51, -1));
                REQUIRE(all_eq(arr1->begin() + 51, arr1->end(), 0));
            }

            SECTION("@length 50 @array_offset $-100")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @l 50 @aoff "
                               "$-100");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 50, -1));
                REQUIRE(all_eq(arr1->begin() + 50, arr1->end(), 0));
            }

            SECTION("@length 50 @array_offset $-101")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @l 50 @aoff -101");
                REQUIRE(!t.hasNewMessages(0));
            }
        }

        SECTION("length resize")
        {
            SECTION("@length 50")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @r @ch 0 @l 50");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 50);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("@length 50 @aoffset 25")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoff 25");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 75);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 25, 0));
                REQUIRE(all_eq(arr1->begin() + 25, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 49")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 49");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 99);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 49, 0.5));
                REQUIRE(all_eq(arr1->begin() + 49, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 50")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 50");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 100);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 50, 0.5));
                REQUIRE(all_eq(arr1->begin() + 50, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 100")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 100");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 150);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0.5));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 101")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 101");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 151);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0.5));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->begin() + 101, 0));
                REQUIRE(all_eq(arr1->begin() + 101, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 950")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 950");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 1000);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0.5));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->begin() + 950, 0));
                REQUIRE(all_eq(arr1->begin() + 950, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset 950")
            {
                arr1->fillWith(0.5);

                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset 950");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 1000);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0.5));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->begin() + 950, 0));
                REQUIRE(all_eq(arr1->begin() + 950, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset $-10")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset $-10");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 140);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 90, 0));
                REQUIRE(all_eq(arr1->begin() + 90, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset end")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset $+0");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 150);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->end(), 1));
            }

            SECTION("@length 50 @aoffset end+10")
            {
                arr1->fillWith(0.5);
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 50 @aoffset $+10");
                REQUIRE(floatAt(t) == 50);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 160);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 50);

                REQUIRE(all_eq(arr1->begin(), arr1->begin() + 100, 0.5));
                REQUIRE(all_eq(arr1->begin() + 100, arr1->begin() + 110, 0));
                REQUIRE(all_eq(arr1->begin() + 110, arr1->end(), 1));
            }
        }

        SECTION("length ms")
        {
            SECTION("44.1k: 10ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 10ms");
                REQUIRE(floatAt(t) == 441);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 441);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 441);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }

            SECTION("44.1k: 5ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 5ms");
                REQUIRE(floatAt(t) == 221);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 221);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 221);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }

            SECTION("48k: 5ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 @c 1 "
                               "@resize @length 5ms");
                REQUIRE(floatAt(t) == 240);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 240);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_48k_480samp.wav");
                REQUIRE_PROPERTY(t, @sr, 48000);
                REQUIRE_PROPERTY(t, @samples, 240);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }

            SECTION("96k: 0.001s")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_96k_960samp.wav to snd_file1 @c 1 "
                               "@resize @length 0.001s");
                REQUIRE(floatAt(t) == 96);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 96);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_96k_960samp.wav");
                REQUIRE_PROPERTY(t, @sr, 96000);
                REQUIRE_PROPERTY(t, @samples, 96);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }
        }

        SECTION("length smpte")
        {
            SECTION("44.1k: 00:01.00")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_44.1k_441samp.wav to snd_file1 @c 1 "
                               "@resize @length 00:01.00");
                REQUIRE(floatAt(t) == 441);
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 441);
                REQUIRE_PROPERTY(t, @filename, TEST_DATA_DIR "/base/snd0_ch02_44.1k_441samp.wav");
                REQUIRE_PROPERTY(t, @sr, 44100);
                REQUIRE_PROPERTY(t, @samples, 441);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }
        }

        SECTION("begin")
        {
            SECTION("48k: begin 5ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@begin 5ms @resize");
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 240);
                REQUIRE_PROPERTY(t, @samples, 240);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("48k: begin $-2.5ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@begin $-2.5ms @resize");
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 120);
                REQUIRE_PROPERTY(t, @samples, 120);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("48k: begin $-20ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@begin $-20ms @resize");

                REQUIRE(!t.hasNewMessages(0));
            }
        }

        SECTION("end")
        {
            SECTION("48k: end begin-100 $-20")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@begin $-100 @end $-20 @resize");
                REQUIRE(t.hasNewMessages(0));
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 80);
                REQUIRE_PROPERTY(t, @samples, 80);
            }

            SECTION("48k: end $-20ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@end $-20 @resize");
                REQUIRE(t.hasNewMessages(0));
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 460);
                REQUIRE_PROPERTY(t, @samples, 460);
            }

            SECTION("48k: end 2000ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@end 2000ms @resize");
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 480);
                REQUIRE_PROPERTY(t, @samples, 480);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }

            SECTION("48k: end 5ms")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch02_48k_480samp.wav to snd_file1 "
                               "@end 5ms @resize");
                REQUIRE(arr1->update());
                REQUIRE(arr1->size() == 240);
                REQUIRE_PROPERTY(t, @samples, 240);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
            }
        }

        SECTION("channels")
        {
            SECTION("channel list")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file1 snd_file2 "
                               "@ch 0 1 3");
                REQUIRE(listAt(t) == LF(100, 200));
                REQUIRE_PROPERTY(t, @samples, 100, 200);

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), 0));
            }

            SECTION("channel list")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file1 snd_file2 "
                               "@resize "
                               "@ch 2 0"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), -1));
            }

            SECTION("channel list")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file1 snd_file2 "
                               "@resize "
                               "@ch 0 0"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), -1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), -1));
            }

            SECTION("channel list")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file1 snd_file2 "
                               "@resize "
                               "@ch 1-2"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 0));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), 1));
            }

            SECTION("channel list")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file1 snd_file2 "
                               "@resize "
                               "@ch 2-1"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), 0));
            }
        }

        SECTION("patterns")
        {
            SECTION("range")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file[0-8] "
                               "@resize "
                               "@ch 2-1"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), 0));
            }

            SECTION("alternative")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch03_44.1k_441samp.wav "
                               "@to snd_file[1|2|3] "
                               "@resize "
                               "@ch 2-1"
                               "@len 90");
                REQUIRE_PROPERTY(t, @samples, 90, 90);
                REQUIRE(arr1->update());
                REQUIRE(arr2->update());

                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
                REQUIRE(all_eq(arr2->begin(), arr2->end(), 0));
            }
        }

        SECTION("normalize")
        {
            SECTION("no normalize")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch07_48k_480samp.wav to snd_file1 "
                               "@channel 2");
                REQUIRE(arr1->update());
                REQUIRE(all_eq(arr1->begin(), arr1->end(), -0.3333));
            }

            SECTION("@normalize")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch07_48k_480samp.wav to snd_file1 "
                               "@channel 4 @normalize");
                REQUIRE(arr1->update());
                REQUIRE(all_eq(arr1->begin(), arr1->end(), 1));
            }
        }

        SECTION("gain")
        {
            SECTION("@gain amp")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch07_48k_480samp.wav to snd_file1 "
                               "@channel 2 @gain 0.5");
                REQUIRE(arr1->update());
                REQUIRE(all_eq(arr1->begin(), arr1->end(), -0.16667));
            }

            SECTION("@gain db")
            {
                t <<= load_str("load " TEST_DATA_DIR
                               "/base/snd0_ch07_48k_480samp.wav to snd_file1 "
                               "@channel 2 @gain +6.02db");
                REQUIRE(arr1->update());
                REQUIRE(all_eq(arr1->begin(), arr1->end(), -0.66664));
            }
        }
    }

    SECTION("save")
    {
    }
}
