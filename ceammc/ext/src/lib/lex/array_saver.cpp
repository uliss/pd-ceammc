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
#include "array_saver.h"
#include "ceammc_array.h"
#include "ceammc_log.h"
#include "ceammc_sound.h"
#include "ceammc_string.h"
#include "fmt/core.h"
#include "lex/parser_array_saver.h"
#include "m_pd.h"

#include <algorithm>

#define PD_ERR(obj) Error(obj).stream()

namespace ceammc {

ArraySaver::ArraySaver()
{
}

bool ArraySaver::parse(const AtomListView& lv, BaseObject* obj)
{
    auto sym_to = gensym("@to");
    arrays_.clear();
    int file_arg_pos = -1;

    size_t array_size = 0;
    for (size_t i = 0; i < lv.size(); i++) {
        auto& a = lv[i];

        if (!a.isSymbol()) {
            PD_ERR(obj) << "array name expected, got: " << a;
            return false;
        }

        auto name = a.asT<t_symbol*>();

        if (name == sym_to) {
            file_arg_pos = i + 1;
            break;
        }

        Array arr(name);
        if (!arr.isValid()) {
            PD_ERR(obj) << fmt::format("array not found: '{}'", name->s_name);
            return false;
        } else {
            arrays_.push_back(name);
            auto N = arr.size();
            if (array_size == 0)
                array_size = N;
            else if (N < array_size)
                array_size = N;
        }
    }

    if (arrays_.empty()) {
        PD_ERR(obj) << "no arrays specified";
        return false;
    }

    if (file_arg_pos < 1) {
        PD_ERR(obj) << "destination file not specified";
        return false;
    }

    params_.begin = 0;
    params_.end = array_size;

    string::MediumString str;
    if (string::list_to_string(lv.subView(file_arg_pos), str))
        str.push_back(0);

    if (!parser::parse_array_saver_params(str.data(), array_size, params_)) {
        PD_ERR(obj) << "can't parse options: " << str.data();
        return false;
    }

    if (params_.format == sound::FORMAT_UNKNOWN) {
        PD_ERR(obj) << "unknown output format";
        return false;
    }

    return true;
}

sound::SoundFilePtr ArraySaver::open(const std::string& path)
{
    sound::SoundFileOpenParams params;
    params.samplerate = params_.out_sr;
    params.num_channels = arrays_.size();
    params.file_format = params_.format;
    params.sample_format = params_.sample_format;

    auto res = sound::SoundFileFactory::openWrite(path, params);

    if (res) {
        res->setGain(params_.gain);
        res->setResampleRatio(params_.in_sr / params_.out_sr);
    }

    return res;
}

ArrayDataView ArraySaver::arrayData() const
{
    ArrayDataView res;

    if (arrays_.empty())
        return res;

    auto N = arrays_.size();
    res.reserve(N);
    res.length = params_.length() + 1;

    for (auto& name : arrays_) {
        Array arr(name);
        if (arr.isValid())
            res.data.push_back((arr.begin() + params_.begin).data());
    }
    return res;
}

}
