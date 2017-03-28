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
#include "../snd_file.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_sound.h"

#include "g_canvas.h"

#include <stdio.h>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

void array_zero(t_garray* a)
{
    int sz = 0;
    t_word* vec = 0;
    if (!garray_getfloatwords(a, &sz, &vec))
        return;

    for (int i = 0; i < sz; i++)
        vec[i].w_float = 0.f;
}

class SndFileTest : public TestExtension<SndFile> {
public:
    SndFileTest(const char* name, const AtomList& args)
        : TestExtension<SndFile>(name, args)
    {
    }

public:
    t_garray* findArray_(const Atom& name)
    {
        return this->findArray(name);
    }

    bool checkArray_(const Atom& name)
    {
        return this->checkArray(name);
    }

    bool resizeArray_(const Atom& name, long newSize)
    {
        return this->resizeArray(name, newSize);
    }

    long loadArray_(sound::SoundFilePtr file, const Atom& name, size_t channel, long offset)
    {
        return this->loadArray(file, name, channel, offset);
    }

    bool arrayNameContainsPattern_(const std::string& name) const
    {
        return this->arrayNameContainsPattern(name);
    }

    AtomList findPatternArrays_(const std::string& pattern) const
    {
        return this->findPatternArrays(pattern);
    }
};

#define SF(path) sound::SoundFileLoader::open(path)

#define SA(txt) Atom(gensym(#txt))
#define S(txt) gensym(#txt)
#define F(n) Atom(float(n))

#define ARG_F1(name, value)    \
    {                          \
        args.append(S(name));  \
        args.append(F(value)); \
    }

#define ARG_F2(name, f1, f2)  \
    {                         \
        args.append(S(name)); \
        args.append(F(f1));   \
        args.append(F(f2));   \
    }

#define ARG_S1(name, value)    \
    {                          \
        args.append(S(name));  \
        args.append(S(value)); \
    }

#define ARG_S2(name, s1, s2)  \
    {                         \
        args.append(S(name)); \
        args.append(S(s1));   \
        args.append(S(s2));   \
    }

#define ARG(name)             \
    {                         \
        args.append(S(name)); \
    }

#define REQUIRE_ARRAY_SIZE(array, size)                       \
    {                                                         \
        int sz = 0;                                           \
        t_word* vec = 0;                                      \
        REQUIRE(garray_getfloatwords(array, &sz, &vec) == 1); \
        REQUIRE(size == sz);                                  \
        REQUIRE(vec != 0);                                    \
    }

#define REQUIRE_ARRAY_ZERO(array)                             \
    {                                                         \
        int sz = 0;                                           \
        t_word* vec = 0;                                      \
        REQUIRE(garray_getfloatwords(array, &sz, &vec) == 1); \
        for (int i = 0; i < sz; i++) {                        \
            REQUIRE(vec[i].w_float == 0.f);                   \
        }                                                     \
    }

#define CALL(obj, method)                            \
    {                                                \
        obj.storeMessageCount();                     \
        obj.m_##method(gensym(#method), AtomList()); \
    }

#define CALL1(obj, method, arg1)                         \
    {                                                    \
        obj.storeMessageCount();                         \
        obj.m_##method(gensym(#method), AtomList(arg1)); \
    }

#define REQUIRE_LIST_MSG(obj, lst)                     \
    {                                                  \
        REQUIRE(obj.hasNewMessages());                 \
        REQUIRE(obj.lastMessage().isList());           \
        REQUIRE(obj.lastMessage().listValue() == lst); \
    }

#define REQUIRE_PROP(obj, name, val)                   \
    {                                                  \
        Property* p = obj.property(gensym("@" #name)); \
        REQUIRE(p != 0);                               \
        REQUIRE(p->get() == val);                      \
    }

#define REQUIRE_INDEX(obj, idx)                         \
    {                                                   \
        REQUIRE(obj.p_index() == AtomList(float(idx))); \
    }

#define REQUIRE_NO_MSG(obj) REQUIRE_FALSE(obj.hasNewMessages())

extern "C" void pd_init();

t_canvas* make_canvas(int w = 300, int h = 200, int x = 0, int y = 0)
{
    t_atom cnv_args[6];
    SETFLOAT(&cnv_args[0], x);
    SETFLOAT(&cnv_args[1], y);
    SETFLOAT(&cnv_args[2], w);
    SETFLOAT(&cnv_args[3], h);
    SETSYMBOL(&cnv_args[4], S(test));
    SETFLOAT(&cnv_args[5], 0);
    return canvas_new(0, S(canvas), 6, cnv_args);
}

class PdInit {
public:
    PdInit()
    {
        pd_init();
    }
};

static PdInit pd_;

TEST_CASE("snd.file", "[PureData]")
{
    SECTION("test load")
    {
        AtomList args;
        SndFileTest sf("snd.file", args);
        REQUIRE(sf.numInlets() == 1);
        REQUIRE(sf.numOutlets() == 2);
        REQUIRE(sf.findArray_(SA("unknown")) == 0);

        sf.storeMessageCount();
        REQUIRE_NO_MSG(sf);

        t_canvas* cnv = make_canvas();
        REQUIRE(cnv != 0);

        t_garray* array1 = graph_array(canvas_getcurrent(), gensym("array1"), &s_float, 100, 0);
        REQUIRE(array1 != 0);

        REQUIRE(sf.findArray_(Atom(100)) == 0);
        REQUIRE(sf.findArray_(S(unknown)) == 0);
        REQUIRE(sf.findArray_(S(array1)) == array1);

        REQUIRE_FALSE(sf.checkArray_(S(unknown)));
        REQUIRE(sf.checkArray_(S(array1)));

        REQUIRE_FALSE(sf.resizeArray_(S(unknown), 100));
        REQUIRE(sf.resizeArray_(S(array1), 100));
        REQUIRE_ARRAY_SIZE(array1, 100);

        // negative array size
        REQUIRE_FALSE(sf.resizeArray_(S(array1), 0));
        REQUIRE_FALSE(sf.resizeArray_(S(array1), -100));
        // check nothing changes
        REQUIRE_ARRAY_SIZE(array1, 100);

        REQUIRE(sf.resizeArray_(S(array1), 111));
        REQUIRE_ARRAY_SIZE(array1, 111);

        REQUIRE_NO_MSG(sf);

        sound::SoundFilePtr file = SF(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(file);
        REQUIRE(file->isOpened());

        REQUIRE(sf.loadArray_(SF("not-exists"), S(array1), 0, 0) == -1);
        REQUIRE(sf.loadArray_(file, S(unknown), 0, 0) == -1);
        REQUIRE(file->sampleCount() == 441);
        // invalid offset
        REQUIRE(sf.loadArray_(file, S(array1), 0, -1) == -1);
        REQUIRE(sf.loadArray_(file, S(array1), 0, 1024) == -1);
        // invalid channel
        REQUIRE(sf.loadArray_(file, S(array1), 10, 0) == -1);

        REQUIRE_NO_MSG(sf);

        // load left channel
        REQUIRE(sf.loadArray_(file, S(array1), 0, 0) == 111);
        int sz = 0;
        t_word* vec = 0;
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
        }

        // load left channel with offset
        const long offset = 11;
        REQUIRE(sf.loadArray_(file, S(array1), 0, offset) == 111);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / 32768.f));
        }

        // load right channel
        REQUIRE(sf.loadArray_(file, S(array1), 1, 0) == 111);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        // load right channel with offset
        REQUIRE(sf.loadArray_(file, S(array1), 1, offset) == 111);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / -32767.f));
        }

        REQUIRE_NO_MSG(sf);

        args.clear();
        sf.m_load(S(load), args);

        // invalid float argument
        args.append(1000);
        sf.m_load(S(load), args);
        args.clear();

        // do destination argument
        args.append(S(unknown)); //filename
        sf.m_load(S(load), args);
        args.clear();

        // invalid destination
        args.append(S(unknown)); //filename
        args.append(S(@to));
        args.append(S(arrayX));
        sf.m_load(S(load), args);
        args.clear();

        /// create "array2"
        t_garray* array2 = graph_array(canvas_getcurrent(), gensym("array2"), &s_float, 10, 0);
        REQUIRE(array2 != 0);

        // invalid filename
        args.append(S(unknown)); //filename
        args.append(S(@to));
        args.append(S(array2));
        sf.m_load(S(load), args);
        args.clear();
        REQUIRE_NO_MSG(sf);

        REQUIRE(sf.resizeArray_(S(array1), 20));
        REQUIRE(sf.resizeArray_(S(array2), 25));
        array_zero(array1);
        array_zero(array2);
        // first load success

        // valid filename
        args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
        args.append(S(@to));
        args.append(S(array1));
        sf.m_load(S(load), args);
        args.clear();

        REQUIRE_ARRAY_SIZE(array1, 20);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        // load channel 1
        args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
        args.append(S(@to));
        args.append(S(array1));
        args.append(S(@channel));
        args.append(F(1));
        sf.m_load(S(load), args);
        args.clear();

        REQUIRE_ARRAY_SIZE(array1, 20);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        // load channel 1 with resize
        array_zero(array1);
        args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
        args.append(S(@to));
        args.append(S(array1));
        args.append(S(@channel));
        args.append(F(1));
        args.append(S(@resize));
        sf.m_load(S(load), args);
        args.clear();

        REQUIRE_ARRAY_SIZE(array1, 441);
        REQUIRE_ARRAY_SIZE(array2, 25);
        garray_getfloatwords(array1, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        REQUIRE_ARRAY_ZERO(array2);

        // load channel 1 with resize and offset
        {
            array_zero(array1);
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            args.append(S(@to));
            args.append(S(array1));
            args.append(S(@channel));
            args.append(F(1));
            args.append(S(@resize));
            args.append(S(@offset));
            args.append(F(100));
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            args.append(S(@to));
            args.append(S(array1));
            args.append(S(array2));
            args.append(S(@resize));
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            args.append(S(@to));
            args.append(S(array2));
            args.append(S(array1));
            args.append(S(@resize));
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            ARG_S2(@to, array1, array2);
            ARG_F1(@channel, 1);
            ARG(@resize);
            // final @channel = [1, 0]
            // so ch1 loaded to array1, ch0 - to array2
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            ARG_S2(@to, array1, array2);
            ARG(@resize);
            ARG_F2(@channel, 100, -12);
            // final @channel = [0, 0]
            // so ch0 loaded to array1 and array2
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            ARG_S2(@to, array1, array2);
            ARG(@resize);
            ARG_F1(@offset, 400);
            ARG_F2(@channel, 0, 1);
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            ARG_S2(@to, array1, array2);
            ARG_F1(@offset, -41);
            ARG(@resize);
            sf.m_load(S(load), args);
            args.clear();

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
            args.append(gensym(TEST_DATA_DIR "/test_data1.wav")); //filename
            ARG_S2(@to, array1, array2);
            ARG_F1(@offset, 10000); // became 0
            ARG(@resize);
            sf.m_load(S(load), args);
            args.clear();

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
        SndFileTest sf("snd.file", args);
        REQUIRE(sf.numInlets() == 1);
        REQUIRE(sf.numOutlets() == 2);

        sf.storeMessageCount(1);
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL(sf, info);
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, F(123));
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, gensym("unknown"));
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, gensym(TEST_DATA_DIR "/test_data1.wav"));
        REQUIRE(sf.hasNewMessages(1));
        AtomList info = sf.lastMessage(1).listValue();
        AtomList prop;
        REQUIRE(info.property("@channels", &prop));
        REQUIRE(prop.asSizeT() == 2);
        REQUIRE(info.property("@samplerate", &prop));
        REQUIRE(prop.asSizeT() == 44100);
        REQUIRE(info.property("@samples", &prop));
        REQUIRE(prop.asSizeT() == 441);

        REQUIRE(info.property("@duration", &prop));
        REQUIRE(prop.at(0).asFloat() == 0.01f);
    }

    SECTION("test if pattern")
    {
        AtomList args;
        SndFileTest sf("snd.file", args);

        REQUIRE_FALSE(sf.arrayNameContainsPattern_(std::string()));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_(""));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test"));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_["));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_]"));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_("test_]["));
        REQUIRE_FALSE(sf.arrayNameContainsPattern_("]...["));
        REQUIRE(sf.arrayNameContainsPattern_("test_[]_extra"));
        REQUIRE(sf.arrayNameContainsPattern_("array_ch[1|2|4]_extra"));
        REQUIRE(sf.arrayNameContainsPattern_("array_ch[2]_test"));
        REQUIRE(sf.arrayNameContainsPattern_("array_ch[1-3]"));
    }

    SECTION("test pattern find")
    {
        AtomList args;
        SndFileTest sf("snd.file", args);

        REQUIRE(sf.findPatternArrays_(std::string()).empty());
        REQUIRE(sf.findPatternArrays_("").empty());
        REQUIRE(sf.findPatternArrays_("no_pattern").empty());
        REQUIRE(sf.findPatternArrays_("invalid_pattern[").empty());
        REQUIRE(sf.findPatternArrays_("invalid_pattern]").empty());
        REQUIRE(sf.findPatternArrays_("invalid_][pattern").empty());
        REQUIRE(sf.findPatternArrays_("[]").empty());
        REQUIRE(sf.findPatternArrays_("[?]").empty());
        REQUIRE(sf.findPatternArrays_("[12.|123]").empty());

        t_canvas* cnv = make_canvas();
        REQUIRE(cnv != 0);

        t_garray* array1 = graph_array(canvas_getcurrent(), gensym("array1"), &s_float, 100, 0);
        REQUIRE(array1 != 0);

        REQUIRE(sf.findPatternArrays_("array[]") == AtomList(S(array1)));
        REQUIRE(sf.findPatternArrays_("array[1]") == AtomList(S(array1)));
        REQUIRE(sf.findPatternArrays_("array[|1]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[1|]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[|1|]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[2]") == AtomList()); // not exists yet
        REQUIRE(sf.findPatternArrays_("array[1|2]") == AtomList(S(array1))); // not exists yet

        t_garray* array2 = graph_array(canvas_getcurrent(), gensym("array2"), &s_float, 100, 0);
        REQUIRE(array2 != 0);

        REQUIRE(sf.findPatternArrays_("array[]") == AtomList(S(array1), S(array2)));
        REQUIRE(sf.findPatternArrays_("array[1]") == AtomList(S(array1)));
        REQUIRE(sf.findPatternArrays_("array[2]") == AtomList(S(array2)));
        REQUIRE(sf.findPatternArrays_("array[1|2]") == AtomList(S(array1), S(array2)));
        REQUIRE(sf.findPatternArrays_("array[0-10]") == AtomList(S(array1), S(array2)));
        REQUIRE(sf.findPatternArrays_("array[10-0]") == AtomList(S(array2), S(array1)));
        REQUIRE(sf.findPatternArrays_("array[|1|2]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[1|2|]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[||||1|2|]") == AtomList());
        REQUIRE(sf.findPatternArrays_("array[2|1]") == AtomList(S(array2), S(array1)));

        t_garray* array3 = graph_array(canvas_getcurrent(), gensym("array3"), &s_float, 100, 0);
        REQUIRE(array3 != 0);

        AtomList lst3;
        lst3.append(S(array1));
        lst3.append(S(array2));
        lst3.append(S(array3));
        REQUIRE(sf.findPatternArrays_("array[1-3]") == lst3);
        REQUIRE(sf.findPatternArrays_("array[1-5]") == lst3);
    }

#if defined(__APPLE__) && defined(__clang__)
    SECTION("coreaudio tests")
    {
        AtomList args;
        SndFileTest sf("snd.file", args);

        sf.storeMessageCount(1);
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL(sf, info);
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, F(123));
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, gensym("unknown"));
        REQUIRE_FALSE(sf.hasNewMessages(1));

        CALL1(sf, info, gensym(TEST_DATA_DIR "/test_data0.mp3"));
        REQUIRE(sf.hasNewMessages(1));
        AtomList info = sf.lastMessage(1).listValue();
        AtomList prop;
        REQUIRE(info.property("@channels", &prop));
        REQUIRE(prop.asSizeT() == 1);
        REQUIRE(info.property("@samplerate", &prop));
        REQUIRE(prop.asSizeT() == 44100);
        REQUIRE(info.property("@samples", &prop));
        REQUIRE(prop.asSizeT() == 441);

        REQUIRE(info.property("@duration", &prop));
        REQUIRE(prop.at(0).asFloat() == 0.01f);

        sf.cleanMessages(1);
        sf.storeMessageCount(1);
        REQUIRE_FALSE(sf.hasNewMessages(1));
        CALL1(sf, info, gensym(TEST_DATA_DIR "/test_data0.m4a"));
        REQUIRE(sf.hasNewMessages(1));
        info = sf.lastMessage(1).listValue();
        prop.clear();
        REQUIRE(info.property("@channels", &prop));
        REQUIRE(prop.asSizeT() == 1);
        REQUIRE(info.property("@samplerate", &prop));
        REQUIRE(prop.asSizeT() == 44100);
        REQUIRE(info.property("@samples", &prop));
        REQUIRE(prop.asSizeT() == 441);
    }
#endif
}
