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
#define CATCH_CONFIG_MAIN
#include "../snd_file.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_sound.h"

#include "g_canvas.h"

#include <stdio.h>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

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
};

#define SF(path) sound::SoundFileLoader::open(path)

#define SA(txt) Atom(gensym(#txt))
#define S(txt) gensym(#txt)

#define REQUIRE_ARRAY_SIZE(array, size)                       \
    {                                                         \
        int sz = 0;                                           \
        t_word* vec = 0;                                      \
        REQUIRE(garray_getfloatwords(array, &sz, &vec) == 1); \
        REQUIRE(size == sz);                                  \
        REQUIRE(vec != 0);                                    \
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
        REQUIRE(sf.numOutlets() == 1);
        REQUIRE(sf.findArray_(SA("unknown")) == 0);

        t_atom cnv_args[6];
        SETFLOAT(&cnv_args[0], 10);
        SETFLOAT(&cnv_args[1], 10);
        SETFLOAT(&cnv_args[2], 300);
        SETFLOAT(&cnv_args[3], 200);
        SETSYMBOL(&cnv_args[4], S(test));
        SETFLOAT(&cnv_args[5], 0);
        t_canvas* cnv = canvas_new(0, S(canvas), 6, cnv_args);
        REQUIRE(cnv != 0);

        t_garray* ga = graph_array(canvas_getcurrent(), gensym("array1"), &s_float, 100, 0);
        REQUIRE(ga != 0);

        REQUIRE(sf.findArray_(Atom(100)) == 0);
        REQUIRE(sf.findArray_(S(unknown)) == 0);
        REQUIRE(sf.findArray_(S(array1)) == ga);

        REQUIRE_FALSE(sf.checkArray_(S(unknown)));
        REQUIRE(sf.checkArray_(S(array1)));

        REQUIRE_FALSE(sf.resizeArray_(S(unknown), 100));
        REQUIRE(sf.resizeArray_(S(array1), 100));
        REQUIRE_ARRAY_SIZE(ga, 100);

        // negative array size
        REQUIRE_FALSE(sf.resizeArray_(S(array1), 0));
        REQUIRE_FALSE(sf.resizeArray_(S(array1), -100));
        // check nothing changes
        REQUIRE_ARRAY_SIZE(ga, 100);

        REQUIRE(sf.resizeArray_(S(array1), 111));
        REQUIRE_ARRAY_SIZE(ga, 111);

        sound::SoundFilePtr file = SF(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(file->isOpened());

        REQUIRE(sf.loadArray_(SF("not-exists"), S(array1), 0, 0) == -1);
        REQUIRE(sf.loadArray_(file, S(unknown), 0, 0) == -1);
        REQUIRE(file->sampleCount() == 441);
        // invalid offset
        REQUIRE(sf.loadArray_(file, S(array1), 0, 1024) == -1);
        // invalid channel
        REQUIRE(sf.loadArray_(file, S(array1), 10, 0) == -1);

        // load left channel
        REQUIRE(sf.loadArray_(file, S(array1), 0, 0) == 111);
        int sz = 0;
        t_word* vec = 0;
        garray_getfloatwords(ga, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / 32768.f));
        }

        // load left channel with offset
        const long offset = 11;
        REQUIRE(sf.loadArray_(file, S(array1), 0, offset) == 111);
        garray_getfloatwords(ga, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / 32768.f));
        }

        // load right channel
        REQUIRE(sf.loadArray_(file, S(array1), 1, 0) == 111);
        garray_getfloatwords(ga, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx(i * 10 / -32767.f));
        }

        // load right channel with offset
        REQUIRE(sf.loadArray_(file, S(array1), 1, offset) == 111);
        garray_getfloatwords(ga, &sz, &vec);
        for (int i = 0; i < sz; i++) {
            REQUIRE(vec[i].w_float == Approx((i + offset) * 10 / -32767.f));
        }
    }
}
