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
#include "lex/parser_array_saver.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("parser_array_saver", "[ceammc::ceammc_units]")
{
    test::pdPrintToStdError();

    using namespace ceammc::parser;

    ArraySaverParams params {};

    SECTION("base")
    {

        REQUIRE(parse_array_saver_params("array1 @to filename.wav", 100, params));
        REQUIRE(params.arrays == std::vector<std::string> { "array1" });
        REQUIRE(params.filename == "filename.wav");
        REQUIRE(params.overwrite == false);
    }

    SECTION("base2")
    {
        REQUIRE(parse_array_saver_params("array1 array2 @to filename.wav", 100, params));
        REQUIRE(params.arrays == std::vector<std::string> { "array1", "array2" });
        REQUIRE(params.filename == "filename.wav");
    }

    SECTION("base3")
    {
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav", 100, params));
        REQUIRE(params.arrays == std::vector<std::string> { "array1", "array2", "array3" });
        REQUIRE(params.filename == "filename.wav");
    }

    SECTION("@gain")
    {
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav @gain 2", 100, params));
        REQUIRE(params.gain == 2);
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav @gain 2.5", 100, params));
        REQUIRE(params.gain == 2.5);
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav @gain -2.5", 100, params));
        REQUIRE(params.gain == -2.5);
    }

    SECTION("@in_sr")
    {
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav @in_sr 500", 100, params));
        REQUIRE(params.in_sr == 500);
        REQUIRE(parse_array_saver_params("array1 array2 array3 @to filename.wav @in_sr 100.5", 100, params));
        REQUIRE(params.in_sr == 100.5);
    }

    SECTION("@out_sr")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @in_sr 400 @out_sr 500", 100, params));
        REQUIRE(params.in_sr == 400);
        REQUIRE(params.out_sr == 500);
    }

    SECTION("@norm")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @in_sr 400 @out_sr 500", 100, params));
        REQUIRE(params.normalize == false);

        REQUIRE(parse_array_saver_params("array1 @to filename.wav @in_sr 400 @out_sr 500 @norm", 100, params));
        REQUIRE(params.normalize == true);
    }

    SECTION("formats")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @wav", 100, params));
        REQUIRE(params.format == sound::FORMAT_WAV);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @mp3", 100, params));
        REQUIRE(params.format == sound::FORMAT_MP3);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @ogg", 100, params));
        REQUIRE(params.format == sound::FORMAT_OGG);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @opus", 100, params));
        REQUIRE(params.format == sound::FORMAT_OPUS);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @aiff", 100, params));
        REQUIRE(params.format == sound::FORMAT_AIFF);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @raw", 100, params));
        REQUIRE(params.format == sound::FORMAT_RAW);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @txt", 100, params));
        REQUIRE(params.format == sound::FORMAT_TEXT);
    }

    SECTION("samp")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp 0", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_DEFAULT);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp 8", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_8);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp 16", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_16);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp 24", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_24);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp 32", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_32);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @samp f", 100, params));
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_FLOAT);
    }

    SECTION("begin")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 10", 100, params));
        REQUIRE(params.begin == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin -1", 100, params));
        REQUIRE(params.begin == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin $-1", 100, params));
        REQUIRE(params.begin == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin $-10", 100, params));
        REQUIRE(params.begin == 90);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 99", 100, params));
        REQUIRE(params.begin == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 100", 100, params));
        REQUIRE(params.begin == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 15sa", 100, params));
        REQUIRE(params.begin == 15);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 18.9samp", 100, params));
        REQUIRE(params.begin == 19);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 99.9samp", 100, params));
        REQUIRE(params.begin == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 10%", 100, params));
        REQUIRE(params.begin == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 0%", 100, params));
        REQUIRE(params.begin == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin -1.5%", 200, params));
        REQUIRE(params.begin == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 90.9%", 200, params));
        REQUIRE(params.begin == 182);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin +100%", 200, params));
        REQUIRE(params.begin == 199);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 50perc", 200, params));
        REQUIRE(params.begin == 100);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 51percent", 200, params));
        REQUIRE(params.begin == 102);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin $-10%", 200, params));
        REQUIRE(params.begin == 180);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 0.25*", 100, params));
        REQUIRE(params.begin == 25);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 0.35phase", 100, params));
        REQUIRE(params.begin == 35);

        params.in_sr = 100;
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 100ms", 100, params));
        REQUIRE(params.begin == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin $-100ms", 100, params));
        REQUIRE(params.begin == 90);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 150msec", 100, params));
        REQUIRE(params.begin == 15);

        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin 0.25s", 100, params));
        REQUIRE(params.begin == 25);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @begin $-0.25sec", 100, params));
        REQUIRE(params.begin == 75);
    }

    SECTION("end")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 10", 100, params));
        REQUIRE(params.end == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end -1", 100, params));
        REQUIRE(params.end == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end $-1", 100, params));
        REQUIRE(params.end == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end $-10", 100, params));
        REQUIRE(params.end == 90);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 99", 100, params));
        REQUIRE(params.end == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 100", 100, params));
        REQUIRE(params.end == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 15sa", 100, params));
        REQUIRE(params.end == 15);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 18.9samp", 100, params));
        REQUIRE(params.end == 19);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 99.9samp", 100, params));
        REQUIRE(params.end == 99);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 10%", 100, params));
        REQUIRE(params.end == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 0%", 100, params));
        REQUIRE(params.end == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end -1.5%", 200, params));
        REQUIRE(params.end == 0);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 90.9%", 200, params));
        REQUIRE(params.end == 182);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end +100%", 200, params));
        REQUIRE(params.end == 199);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 50perc", 200, params));
        REQUIRE(params.end == 100);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 51percent", 200, params));
        REQUIRE(params.end == 102);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end $-10%", 200, params));
        REQUIRE(params.end == 180);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 0.25*", 100, params));
        REQUIRE(params.end == 25);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 0.35phase", 100, params));
        REQUIRE(params.end == 35);

        params.in_sr = 100;
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 100ms", 100, params));
        REQUIRE(params.end == 10);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end $-100ms", 100, params));
        REQUIRE(params.end == 90);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 150msec", 100, params));
        REQUIRE(params.end == 15);

        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end 0.25s", 100, params));
        REQUIRE(params.end == 25);
        REQUIRE(parse_array_saver_params("array1 @to filename.wav @end $-0.25sec", 100, params));
        REQUIRE(params.end == 75);
    }

    SECTION("all")
    {
        REQUIRE(parse_array_saver_params("array2 array1 @to filename.wav @in_sr 100 @out_sr 200 "
                                         "@begin $-90 @end 0.75s @mp3 @norm @samp 16 @gain 0.25 @overwrite",
            100, params));
        REQUIRE(params.arrays == std::vector<std::string> { "array2", "array1" });
        REQUIRE(params.filename == "filename.wav");
        REQUIRE(params.begin == 10);
        REQUIRE(params.end == 75);
        REQUIRE(params.format == sound::FORMAT_MP3);
        REQUIRE(params.normalize);
        REQUIRE(params.in_sr == 100);
        REQUIRE(params.out_sr == 200);
        REQUIRE(params.gain == 0.25);
        REQUIRE(params.sample_format == sound::SAMPLE_PCM_16);
        REQUIRE(params.overwrite);
    }

    SECTION("auto format")
    {
        REQUIRE(parse_array_saver_params("array1 @to filename.wav", 100, params));
        REQUIRE(params.format == sound::FORMAT_WAV);
        REQUIRE(parse_array_saver_params("array1 @to filename.WAV", 100, params));
        REQUIRE(params.format == sound::FORMAT_WAV);
        REQUIRE(parse_array_saver_params("array1 @to filename.aif", 100, params));
        REQUIRE(params.format == sound::FORMAT_AIFF);
        REQUIRE(parse_array_saver_params("array1 @to filename.AIFF", 100, params));
        REQUIRE(params.format == sound::FORMAT_AIFF);
        REQUIRE(parse_array_saver_params("array1 @to filename.flac", 100, params));
        REQUIRE(params.format == sound::FORMAT_FLAC);
    }
}
