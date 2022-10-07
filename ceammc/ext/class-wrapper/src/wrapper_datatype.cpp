/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2018-2019 Alex Nadzharov, Serge Poltavsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "wrapper_datatype.h"
#include "data_iface.h"

#include "ceammc_log.h"
#include "ceammc_platform.h"

#include <string>

Result::Result(const std::string& errMsg, int code)
    : boost::optional<std::string>(errMsg)
    , rc_(code)
{
}

Result::Result()
    : boost::optional<std::string>(boost::none)
    , rc_(0)
{
}

bool Result::error(std::string* msg, int* code) const
{
    if (rc_ == 0)
        return false;

    if (msg)
        *msg = boost::get(*this);

    if (code)
        *code = rc_;

    return true;
}

std::string DataIFace::makeNewFileName(const std::string& path)
{
    std::string path2 = ceammc::platform::expandenv(ceammc::platform::expand_tilde_path(path).c_str());

    // return absolute path untouched
    if (!ceammc::platform::is_path_relative(path2.c_str()))
        return path2;

    t_canvas* cnv = canvas_getcurrent();
    // try make path relative to current canvas
    if (cnv) {
        char res[MAXPDSTRING];
        canvas_makefilename(cnv, (char*)path2.c_str(), res, MAXPDSTRING);
        return res;
    } else {
        // return user directory related
        return ceammc::platform::pd_user_directory() + "/" + path2;
    }
}

std::string DataIFace::findFileName(const std::string& path)
{
    std::string path2 = ceammc::platform::expandenv(ceammc::platform::expand_tilde_path(path).c_str());

    // return absolute path untouched
    if (!ceammc::platform::is_path_relative(path2.c_str()))
        return path2;

    t_canvas* cnv = canvas_getcurrent();
    if (cnv)
        return ceammc::platform::find_in_std_path(cnv, path2.c_str());
    else
        return ceammc::platform::pd_user_directory() + "/" + path2;
}

Result DataIFace::setFromPd(const ceammc::AtomListView& lv)
{
    if (lv.empty())
        return error("empty list");

    // try float first
    if (lv.isFloat()) {
        auto st = setFromFloat(lv[0].asFloat());
        // ok or error
        if (st.code() != NOT_SUPPORTED)
            return st;
    }

    // try symbol second
    if (lv.isSymbol()) {
        auto st = setFromSymbol(lv[0].asSymbol());
        // ok or error
        if (st.code() != NOT_SUPPORTED)
            return st;
    }

    // try list
    return setFromList(lv);
}

DataIFace::DataIFace()
{
}

DataIFace::~DataIFace()
{
}

Result DataIFace::ok()
{
    return Result();
}

Result DataIFace::error(const std::string& msg)
{
    return Result(msg, ERROR);
}
