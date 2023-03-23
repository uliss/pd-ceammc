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
#ifndef ARRAY_SAVER_H
#define ARRAY_SAVER_H

#include <string>
#include <vector>

namespace ceammc {

class ArraySaver {
    ArraySaver(const ArraySaver&) = delete;
    ArraySaver& operator=(const ArraySaver&) = delete;

public:
    ArraySaver();

    /**
     * parse input string: arrays to FILE OPTIONS...
     */
    bool parse(const std::string& str);

    /**
     * Removes invalid arrays from list
     */
    bool removeInvalidArrays();

    /** source array samplerate */
    float arraySamplerate() const { return array_samplerate_; }

    /** output file samplerate */
    float fileSamplerate() const { return file_samplerate_; }

    /** return loading gain */
    float gain() const { return gain_; }

private:
    std::vector<std::string> arrays_;

    float gain_ { 1 };
    float array_samplerate_ = { 44100 };
    float file_samplerate_ = { 44100 };
    size_t array_begin_ = { 0 };
    size_t array_end_ = { 0 };
};

}

#endif // ARRAY_SAVER_H
