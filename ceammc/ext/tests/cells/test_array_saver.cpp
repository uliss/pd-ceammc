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
#include "catch.hpp"
#include "ceammc_array.h"
#include "ceammc_canvas.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "fmt/core.h"
#include "lex/array_saver.h"
#include "test_base.h"

#include "config.h"
#ifdef HAVE_UNISTD_H
 #ifdef SOX_EXEC
  #define TEST_SOX 1
  #ifdef __WIN32__
   #define WIFEXITED(x) ((x) != 3)
   #define WEXITSTATUS(x) (x)
  #else
   #include <sys/wait.h>
  #endif
 #endif
#endif

#include <fstream>

extern "C" {
#include "s_stuff.h"
}

void set_sr(int sr = 48000)
{
    sys_setchsr(2, 2, sr);
}

std::vector<float> read_f32(const char* file)
{
    std::ifstream ifs(file, std::ios::in | std::ios::ate | std::ios::binary);
    auto nul_floats = ifs.tellg() / sizeof(float);
    ifs.seekg(0, std::ios::beg);

    std::vector<float> res;
    for (int i = 0; i < nul_floats; i++) {
        float f = 0;
        ifs.read((char*)&f, sizeof(float));
        res.push_back(f);
    }

    return res;
}

using namespace ceammc;

TEST_CASE("ArraySaver", "[ceammc::core]")
{
    test::pdPrintToStdError();
    set_sr();

    using namespace ceammc::parser;

    ArraySaverParams params {};

    SECTION("base")
    {
        ArraySaver x;
        REQUIRE(x.filename().empty());
        REQUIRE(x.arraySamplerate() == 44100);
        REQUIRE(x.fileSamplerate() == 44100);
        REQUIRE(x.resampleRatio() == 1);
        REQUIRE(x.gain());
        REQUIRE_FALSE(x.overwrite());
        REQUIRE(x.arrayData().empty());
        REQUIRE(x.arrayData().length == 0);
    }

    SECTION("1ch")
    {
        const char* PATH = TEST_BIN_DIR "/array_saver_1ch.wav";
        auto cnv = PureData::instance().createTopCanvas("ArraySaver");
        auto as1 = cnv->createArray("as0", 10);
        as1->fillWith(0.75);

        ArraySaver x;
        x.setArraySamplerate(48000);
        x.setFileSamplerate(48000);
        REQUIRE(x.parse(AtomList::parseString("as0 @to array_saver_1ch.wav @overwrite")));
        REQUIRE(x.filename() == "array_saver_1ch.wav");
        REQUIRE(x.overwrite());
        auto f = x.open(PATH);
        REQUIRE(f);
        REQUIRE(f->write(x.arrayData().rawData(), x.arrayData().length, 0) == 10);

#ifdef TEST_SOX
        const char* PATH_DAT = TEST_BIN_DIR "/array_saver_1ch.f32";
        int rc = std::system(fmt::format("{} {} {}", SOX_EXEC, PATH, PATH_DAT).c_str());
        REQUIRE(WEXITSTATUS(rc) == 0);
        auto data = read_f32(PATH_DAT);
        REQUIRE(data.size() == 10);
        REQUIRE(data[0] == Approx(0.75));
        REQUIRE(data[1] == Approx(0.75));
        REQUIRE(data[2] == Approx(0.75));
        REQUIRE(data[3] == Approx(0.75));
        REQUIRE(data[4] == Approx(0.75));
        REQUIRE(data[5] == Approx(0.75));
        REQUIRE(data[6] == Approx(0.75));
        REQUIRE(data[7] == Approx(0.75));
        REQUIRE(data[8] == Approx(0.75));
        REQUIRE(data[9] == Approx(0.75));
#endif

        REQUIRE(platform::remove(PATH));
    }

    SECTION("2ch")
    {
        const char* PATH = TEST_BIN_DIR "/array_saver_2ch.wav";
        auto cnv = PureData::instance().createTopCanvas("ArraySaver");
        auto as1 = cnv->createArray("as2.0", 10);
        as1->fillWith(0.75);
        auto as2 = cnv->createArray("as2.1", 10);
        as2->fillWith(0.25);

        ArraySaver x;
        x.setArraySamplerate(48000);
        x.setFileSamplerate(48000);
        REQUIRE(x.parse(AtomList::parseString("as2.0 as2.1 @to array_saver_2ch.wav @overwrite")));
        REQUIRE(x.filename() == "array_saver_2ch.wav");
        REQUIRE(x.overwrite());
        REQUIRE(x.gain() == 1);
        auto f = x.open(PATH);
        REQUIRE(f);
        REQUIRE(f->write(x.arrayData().rawData(), x.arrayData().length, 0) == 20);

#ifdef TEST_SOX
        const char* PATH_DAT = TEST_BIN_DIR "/array_saver_2ch.f32";
        int rc = std::system(fmt::format("{} -c 2 {} {}", SOX_EXEC, PATH, PATH_DAT).c_str());
        REQUIRE(WEXITSTATUS(rc) == 0);
        auto data = read_f32(PATH_DAT);
        REQUIRE(data.size() == 20);
        REQUIRE(data[0] == Approx(0.75));
        REQUIRE(data[1] == Approx(0.25));
        REQUIRE(data[2] == Approx(0.75));
        REQUIRE(data[3] == Approx(0.25));
        REQUIRE(data[4] == Approx(0.75));
        REQUIRE(data[5] == Approx(0.25));
        REQUIRE(data[6] == Approx(0.75));
        REQUIRE(data[7] == Approx(0.25));
        REQUIRE(data[8] == Approx(0.75));
        REQUIRE(data[9] == Approx(0.25));
        REQUIRE(data[10] == Approx(0.75));
        REQUIRE(data[11] == Approx(0.25));
        REQUIRE(data[12] == Approx(0.75));
        REQUIRE(data[13] == Approx(0.25));
        REQUIRE(data[14] == Approx(0.75));
        REQUIRE(data[15] == Approx(0.25));
        REQUIRE(data[16] == Approx(0.75));
        REQUIRE(data[17] == Approx(0.25));
        REQUIRE(data[18] == Approx(0.75));
        REQUIRE(data[19] == Approx(0.25));
#endif

        REQUIRE(platform::remove(PATH));
    }

    SECTION("3ch")
    {
        const char* PATH = TEST_BIN_DIR "/filename.wav";
        auto cnv = PureData::instance().createTopCanvas("ArraySaver");
        auto as1 = cnv->createArray("as1", 10);
        auto as2 = cnv->createArray("as2", 20);
        as1->fillWith(0.5);
        as2->fillWith(0.25);

        platform::remove(PATH);

        ArraySaver x;
        REQUIRE(x.parse(AtomList::parseString("as1 as2 as1 @to filename.wav @overwrite")));
        REQUIRE(x.filename() == "filename.wav");
        REQUIRE(x.overwrite());
        auto f = x.open(PATH);
        REQUIRE(f);
        REQUIRE(f->write(x.arrayData().rawData(), x.arrayData().length, 0) == 30);

        REQUIRE(platform::path_exists(PATH));

#ifdef TEST_SOX
        const char* PATH_DAT = TEST_BIN_DIR "/array_saver_3ch.f32";
        int rc = std::system(fmt::format("{} -c 3 {} {}", SOX_EXEC, PATH, PATH_DAT).c_str());
        REQUIRE(WEXITSTATUS(rc) == 0);
        auto data = read_f32(PATH_DAT);
        REQUIRE(data.size() == 30);
        REQUIRE(data[0] == Approx(0.5));
        REQUIRE(data[1] == Approx(0.25));
        REQUIRE(data[2] == Approx(0.5)); //
        REQUIRE(data[3] == Approx(0.5));
        REQUIRE(data[4] == Approx(0.25));
        REQUIRE(data[5] == Approx(0.5)); //
        REQUIRE(data[6] == Approx(0.5));
        REQUIRE(data[7] == Approx(0.25));
        REQUIRE(data[8] == Approx(0.5)); //
        REQUIRE(data[9] == Approx(0.5));
        REQUIRE(data[10] == Approx(0.25));
        REQUIRE(data[11] == Approx(0.5)); //
        REQUIRE(data[12] == Approx(0.5));
        REQUIRE(data[13] == Approx(0.25));
        REQUIRE(data[14] == Approx(0.5)); //
        REQUIRE(data[15] == Approx(0.5));
        REQUIRE(data[16] == Approx(0.25));
        REQUIRE(data[17] == Approx(0.5)); //
        REQUIRE(data[18] == Approx(0.5));
        REQUIRE(data[19] == Approx(0.25));
        REQUIRE(data[20] == Approx(0.5)); //
        REQUIRE(data[21] == Approx(0.5));
        REQUIRE(data[22] == Approx(0.25));
        REQUIRE(data[23] == Approx(0.5)); //
        REQUIRE(data[24] == Approx(0.5));
        REQUIRE(data[25] == Approx(0.25));
        REQUIRE(data[26] == Approx(0.5)); //
        REQUIRE(data[27] == Approx(0.5));
        REQUIRE(data[28] == Approx(0.25));
        REQUIRE(data[29] == Approx(0.5)); //
#endif
    }
}
