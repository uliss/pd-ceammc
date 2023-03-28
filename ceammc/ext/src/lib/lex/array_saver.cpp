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
#include "fmt/core.h"
#include "lex/parser_array_saver.h"
#include "m_pd.h"

#include <algorithm>

namespace ceammc {

ArraySaver::ArraySaver()
{
}

bool ArraySaver::parse(const std::string& str, BaseObject* obj)
{
    params_.arrays.clear();

    if (!parser::parse_array_saver_params(str.c_str(), 0, params_)) {
        LogPdObject(nullptr, LOG_ERROR).stream() << "can't parse options: " << str;
        return false;
    }

    if (params_.format == sound::FORMAT_UNKNOWN) {
        LogPdObject(nullptr, LOG_ERROR).stream() << "unknown output format";
        return false;
    }

    return true;
}

bool ArraySaver::checkArrays() const
{
    if (params_.arrays.empty()) {
        LIB_ERR << "empty array list";
        return false;
    }

    bool ok = true;
    for (auto& name : params_.arrays) {
        Array arr(name.c_str());
        if (!arr.isValid()) {
            LIB_ERR << fmt::format("array not found: '{}'", name);
            ok = false;
        }
    }

    return ok;
}

sound::SoundFilePtr ArraySaver::open(const std::string& path)
{
    sound::SoundFileOpenParams params;
    params.samplerate = params_.out_sr;
    params.num_channels = params_.arrays.size();
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

    if (params_.arrays.empty())
        return res;

    auto N = params_.arrays.size();
    res.reserve(N);

    for (auto& name : params_.arrays) {
        Array arr(name.c_str());
        if (arr.isValid()) {
            res.data.push_back(arr.begin().data());
            res.lengths.push_back(arr.size());
        }
    }
    return res;
}

size_t ArrayDataView::minSize() const
{
    auto it = std::min_element(lengths.begin(), lengths.end());
    return it == lengths.end() ? 0 : *it;
}

size_t ArrayDataView::maxSize() const
{
    auto it = std::max_element(lengths.begin(), lengths.end());
    return it == lengths.end() ? 0 : *it;
}

}
