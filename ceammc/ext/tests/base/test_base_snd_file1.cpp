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
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(SndFile, snd, file)

//class TObj : public TestExternal<SndFile> {
//public:
//    TObj(const char* name, const AtomList& args)
//        : TestExternal<SndFile>(name, args)
//    {
//    }

//public:
//    t_garray* findArray_(const Atom& name)
//    {
//        return this->findArray(name);
//    }

//    bool checkArray_(const Atom& name)
//    {
//        return this->checkArray(name);
//    }

//    bool resizeArray_(const Atom& name, long newSize)
//    {
//        return this->resizeArray(name, newSize);
//    }

//    long loadArray_(sound::SoundFilePtr file, const Atom& name, size_t channel, long offset)
//    {
//        return this->loadArray(file, name, channel, offset);
//    }

//    bool arrayNameContainsPattern_(const std::string& name) const
//    {
//        return this->arrayNameContainsPattern(name);
//    }

//    AtomList findPatternArrays_(const std::string& pattern) const
//    {
//        return this->findPatternArrays(pattern);
//    }
//};

TEST_CASE("snd.file", "[externals]")
{
    pd_test_init();

    SECTION("test load")
    {
        using SF = sound::SoundFileLoader;

        AtomList args;
        TObj sf("snd.file", args);
        REQUIRE(sf.numInlets() == 1);
        REQUIRE(sf.numOutlets() == 1);

        sf.storeMessageCount();
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        auto arr = cnv->createArray("snd_file1", 10);

        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        sound::SoundFilePtr file = SF::open(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(file);
        REQUIRE(file->isOpened());

//        REQUIRE(sf.loadArray_(SF("not-exists"), S("array1"), 0, 0) == -1);
//        REQUIRE(sf.loadArray_(file, S("unknown"), 0, 0) == -1);
//        REQUIRE(file->sampleCount() == 441);
//        // invalid offset
//        REQUIRE(sf.loadArray_(file, S("array1"), 0, -1) == -1);
//        REQUIRE(sf.loadArray_(file, S("array1"), 0, 1024) == -1);
//        // invalid channel
//        REQUIRE(sf.loadArray_(file, S("array1"), 10, 0) == -1);

//        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        // load left channel
//        REQUIRE(sf.loadArray_(file, S("array1"), 0, 0) == 111);
//        int sz = 0;
//        t_word* vec = 0;
//        garray_getfloatwords(array1, &sz, &vec);
//        for (int i = 0; i < sz; i++) {
//            REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
//        }

//        // load left channel with offset
//        const long offset = 11;
//        REQUIRE(sf.loadArray_(file, S("array1"), 0, offset) == 111);
//        garray_getfloatwords(array1, &sz, &vec);
//        for (int i = 0; i < sz; i++) {
//            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / 32768.f));
//        }

//        // load right channel
//        REQUIRE(sf.loadArray_(file, S("array1"), 1, 0) == 111);
//        garray_getfloatwords(array1, &sz, &vec);
//        for (int i = 0; i < sz; i++) {
//            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
//        }

//        // load right channel with offset
//        REQUIRE(sf.loadArray_(file, S("array1"), 1, offset) == 111);
//        garray_getfloatwords(array1, &sz, &vec);
//        for (int i = 0; i < sz; i++) {
//            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / -32767.f));
//        }

        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL(sf, load);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        // invalid float argument
        WHEN_CALL_N(sf, load, 1000);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        // do destination argument
        WHEN_CALL_N(sf, load, "unknown");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        // invalid destination
        WHEN_CALL_N(sf, load, "unknown", "@to", "arrayX");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        /// create "array2"
        t_garray* array2 = graph_array(canvas_getcurrent(), gensym("array2"), &s_float, 10, 0);
        REQUIRE(array2 != 0);

        // invalid filename
        WHEN_CALL_N(sf, load, "unknown", "@to", "array2");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        REQUIRE(sf.resizeArray_(S("array1"), 20));
        REQUIRE(sf.resizeArray_(S("array2"), 25));
        array_zero(array1);
        array_zero(array2);
        // first load success

        // valid filename
        WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1");
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

        REQUIRE_ARRAY_SIZE(array1, 20);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        array_zero(array1);
        array_zero(array2);

        // load channel 1
        WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "@channel", 1);
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

        REQUIRE_ARRAY_SIZE(array1, 20);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        array_zero(array1);
        array_zero(array2);

        // load channel 1 with resize
        WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "@channel", 1, "@resize");
        REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

        REQUIRE_ARRAY_SIZE(array1, 441);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        array_zero(array1);
        array_zero(array2);

        // load channel 1 with resize and offset
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1",
                "@channel", 1, "@resize", "@offset", 100);
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            int offset = 100;
            REQUIRE_ARRAY_SIZE(array1, 441 - offset);
            REQUIRE_ARRAY_SIZE(array2, 25);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / -32767.f));
            }

            REQUIRE_ARRAY_ZERO(array2);
        }

        // load @to array1, array2 with @resize with default channels
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2", "@resize");
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 441);
            REQUIRE_ARRAY_SIZE(array2, 441);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }

            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
            }
        }

        // load @to array2, array1 with @resize with default channels
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array2", "array1", "@resize");
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 441);
            REQUIRE_ARRAY_SIZE(array2, 441);
            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }

            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
            }
        }

        // load @to array2, array1 with @resize with empty @channels all 0
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2",
                "@channel", 1, "@resize");
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 441);
            REQUIRE_ARRAY_SIZE(array2, 441);
            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }

            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
            }
        }

        // load @to array2, array1 with @resize with invalid @channels
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2",
                "@resize", "@channel", 100, -12);
            // final @channel = [0, 0]
            // so ch0 loaded to array1 and array2
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 441);
            REQUIRE_ARRAY_SIZE(array2, 441);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }

            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }
        }

        // load @to array1, array2 with @resize with @channels and @offset
        {
            array_zero(array1);
            array_zero(array2);

            args = LA(TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2", "@resize", "@offset", 400);
            args.append(LA("@channel", 0.f, 1));
            sf.storeAllMessageCount();
            sf.m_load(gensym("load"), args);
            args.clear();
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 41);
            REQUIRE_ARRAY_SIZE(array2, 41);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx((i + 400) * 10 / 32768.f));
            }

            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx((i + 400) * 10 / -32767.f));
            }
        }

        // load @to array1, array2 with @resize with negative @offset
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2",
                "@resize", "@offset", -41);
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 41);
            REQUIRE_ARRAY_SIZE(array2, 41);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx((i + 400) * 10 / 32768.f));
            }

            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx((i + 400) * 10 / -32767.f));
            }
        }

        // load @to array1, array2 with @resize with huge @offset
        // that became 0
        {
            array_zero(array1);
            array_zero(array2);

            WHEN_CALL_N(sf, load, TEST_DATA_DIR "/test_data1.wav", "@to", "array1", "array2",
                "@resize", "@offset", 10000);
            REQUIRE_NEW_MESSAGES_AT_OUTLET(0, sf);

            REQUIRE_ARRAY_SIZE(array1, 441);
            REQUIRE_ARRAY_SIZE(array2, 441);
            garray_getfloatwords(array1, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
            }

            garray_getfloatwords(array2, &sz, &vec);
            for (int i = 0; i < sz; i++) {
                REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
            }
        }

        canvas_free(cnv);
    }

    SECTION("test info")
    {
        AtomList args;
        TObj sf("snd.file", args);
        REQUIRE(sf.numInlets() == 1);
        REQUIRE(sf.numOutlets() == 1);

        sf.storeMessageCount();
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL(sf, info);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL_N(sf, info, 123);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL_N(sf, info, "unknown");
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, sf);

        WHEN_CALL_N(sf, info, TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(sf.hasNewMessages());
        AtomList info = sf.lastMessage().anyValue();
        AtomList prop;
        REQUIRE(info.property(SYM("@channels"), &prop));
        REQUIRE(prop.asT<size_t>() == 2);
        REQUIRE(info.property(SYM("@samplerate"), &prop));
        REQUIRE(prop.asT<size_t>() == 44100);
        REQUIRE(info.property(SYM("@samples"), &prop));
        REQUIRE(prop.asT<size_t>() == 441);

        REQUIRE(info.property(SYM("@duration"), &prop));
        REQUIRE(prop.at(0).asFloat() == 0.01f);
    }

//    SECTION("test if pattern")
//    {
//        AtomList args;
//        TObj sf("snd.file", args);

//        REQUIRE_FALSE(sf.arrayNameContainsPattern_(std::string()));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_(""));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test"));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_["));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_]"));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_]["));
//        REQUIRE_FALSE(sf.arrayNameContainsPattern_("]...["));
//        REQUIRE(sf.arrayNameContainsPattern_("test_[]_extra"));
//        REQUIRE(sf.arrayNameContainsPattern_("array_ch[1|2|4]_extra"));
//        REQUIRE(sf.arrayNameContainsPattern_("array_ch[2]_test"));
//        REQUIRE(sf.arrayNameContainsPattern_("array_ch[1-3]"));
//    }

//    SECTION("test pattern find")
//    {
//        AtomList args;
//        TObj sf("snd.file", args);

//        REQUIRE(sf.findPatternArrays_(std::string()).empty());
//        REQUIRE(sf.findPatternArrays_("").empty());
//        REQUIRE(sf.findPatternArrays_("no_pattern").empty());
//        REQUIRE(sf.findPatternArrays_("invalid_pattern[").empty());
//        REQUIRE(sf.findPatternArrays_("invalid_pattern]").empty());
//        REQUIRE(sf.findPatternArrays_("invalid_][pattern").empty());
//        REQUIRE(sf.findPatternArrays_("[]").empty());
//        REQUIRE(sf.findPatternArrays_("[?]").empty());
//        REQUIRE(sf.findPatternArrays_("[12.|123]").empty());

//        t_canvas* cnv = make_canvas();
//        REQUIRE(cnv != 0);

//        t_garray* array1 = graph_array(canvas_getcurrent(), gensym("array1"), &s_float, 100, 0);
//        REQUIRE(array1 != 0);

//        REQUIRE(sf.findPatternArrays_("array[]") == LA("array1"));
//        REQUIRE(sf.findPatternArrays_("array[1]") == LA("array1"));
//        REQUIRE(sf.findPatternArrays_("array[|1]") == L());
//        REQUIRE(sf.findPatternArrays_("array[1|]") == L());
//        REQUIRE(sf.findPatternArrays_("array[|1|]") == L());
//        REQUIRE(sf.findPatternArrays_("array[2]") == L()); // not exists yet
//        REQUIRE(sf.findPatternArrays_("array[1|2]") == LA("array1")); // not exists yet

//        t_garray* array2 = graph_array(canvas_getcurrent(), gensym("array2"), &s_float, 100, 0);
//        REQUIRE(array2 != 0);

//        REQUIRE(sf.findPatternArrays_("array[]") == LA("array1", "array2"));
//        REQUIRE(sf.findPatternArrays_("array[1]") == LA("array1"));
//        REQUIRE(sf.findPatternArrays_("array[2]") == LA("array2"));
//        REQUIRE(sf.findPatternArrays_("array[1|2]") == LA("array1", "array2"));
//        REQUIRE(sf.findPatternArrays_("array[0-10]") == LA("array1", "array2"));
//        REQUIRE(sf.findPatternArrays_("array[10-0]") == LA("array2", "array1"));
//        REQUIRE(sf.findPatternArrays_("array[|1|2]") == L());
//        REQUIRE(sf.findPatternArrays_("array[1|2|]") == L());
//        REQUIRE(sf.findPatternArrays_("array[||||1|2|]") == L());
//        REQUIRE(sf.findPatternArrays_("array[2|1]") == LA("array2", "array1"));

//        t_garray* array3 = graph_array(canvas_getcurrent(), gensym("array3"), &s_float, 100, 0);
//        REQUIRE(array3 != 0);

//        REQUIRE(sf.findPatternArrays_("array[1-3]") == LA("array1", "array2", "array3"));
//        REQUIRE(sf.findPatternArrays_("array[1-5]") == LA("array1", "array2", "array3"));
//    }

#if defined(__APPLE__) && defined(__clang__)
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
