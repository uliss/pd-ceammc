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

#include "ceammc_sound.h"
#include "lex/parser_array_saver.h"

#include <string>
#include <vector>

namespace ceammc {

struct ArrayDataView {
    std::vector<const t_word*> data;
    std::vector<size_t> lengths;

    bool empty() const
    {
        return data.empty() || lengths.empty();
    }

    void reserve(size_t n)
    {
        data.reserve(n);
        lengths.reserve(n);
    }

    size_t minSize() const;
    size_t maxSize() const;
};

class BaseObject;

class ArraySaver {
    ArraySaver(const ArraySaver&) = delete;
    ArraySaver& operator=(const ArraySaver&) = delete;

public:
    ArraySaver();

    /**
     * parse input string: arrays... @to FILE OPTIONS...
     */
    bool parse(const std::string& str, BaseObject* obj = nullptr);

    /**
     * Check arrays in list
     */
    bool checkArrays() const;

    /** source array samplerate */
    float arraySamplerate() const { return params_.in_sr; }

    /** set source array samplerate */
    void setArraySamplerate(float sr) { params_.in_sr = sr; }

    /** output file samplerate */
    float fileSamplerate() const { return params_.out_sr; }

    /** set output file samplerate */
    void setFileSamplerate(float sr) { params_.out_sr = sr; }

    /** return loading gain */
    float gain() const { return params_.gain; }

    float resampleRatio() const { return params_.in_sr / params_.out_sr; }

    /** open file for writing */
    sound::SoundFilePtr open(const std::string& path);

    const std::string& filename() const { return params_.filename; }

    /** should overwrite existing files */
    bool overwrite() const { return params_.overwrite; }

    ArrayDataView arrayData() const;

private:
    parser::ArraySaverParams params_;
};

}

#endif // ARRAY_SAVER_H
