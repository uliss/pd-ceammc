/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_ARRAY_SAVER_H
#define PARSER_ARRAY_SAVER_H

#include "ceammc_sound.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

namespace ceammc {
namespace parser {

    enum ArrayOffsetOrigin : uint8_t {
        ORIGIN_BEGIN,
        ORIGIN_END,
    };

    struct ArraySaverParams {
        std::string filename; // output filename
        std::vector<std::string> arrays; // input arrays
        std::size_t begin { 0 };
        std::size_t end { 0 };
        float gain { 1 }; // apply gain
        float in_sr { 44100 }; // input (source array) samplerate
        float out_sr { 44100 }; // output (target file) samplerate
        sound::SoundFileFormat format { sound::FORMAT_UNKNOWN }; // output file format
        sound::SampleFormat sample_format { sound::SAMPLE_DEFAULT }; // sample format
        ArrayOffsetOrigin origin { ORIGIN_BEGIN }; // array offset origin
        bool normalize { false }; // do output normalization
    };

    bool parse_array_saver_params(const char* str, size_t arraySize, ArraySaverParams& params);
}
}

#endif // PARSER_ARRAY_SAVER_H
