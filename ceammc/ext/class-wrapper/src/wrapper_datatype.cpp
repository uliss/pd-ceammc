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

namespace wrapper {

WrapperIDFactory::WrapperIDFactory()
    : id_(1000)
{
}

WrapperIDFactory& WrapperIDFactory::instance()
{
    static WrapperIDFactory f;
    return f;
}

size_t WrapperIDFactory::generateNewId()
{
    return ++id_;
}

AbstractDataId::AbstractDataId(size_t id)
    : data_type_id_(id)
{
}

size_t AbstractDataId::dataTypeId() const
{
    return data_type_id_;
}

bool AbstractDataId::hasEqualType(const AbstractData* d) const
{
    if (type() != d->type())
        return false;

    auto* dw = static_cast<const AbstractDataId*>(d);
    if (dataTypeId() != dw->dataTypeId())
        return false;

    return true;
}

}

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

bool Result::isOk() const
{
    return rc_ == 0;
}

int Result::code() const
{
    return rc_;
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

Result DataIFace::setFromPd(const ceammc::AtomList& lst, t_symbol* prefix)
{
    if (lst.empty())
        return error("empty list");

    // type prefix is specified
    if (lst[0].isSymbol() && lst[0].asSymbol() == prefix) {
        if (lst.size() == 1)
            return error("no data after type constructor");

        // NOTE: this is code duplication
        // it's sake performance: do not do extra AtomList copy
        if (lst[1].isFloat()) {
            auto st = setFromFloat(lst[1].asFloat());
            // ok or error
            if (st.code() != NOT_SUPPORTED)
                return st;
        }

        // try symbol second
        if (lst[1].isSymbol()) {
            auto st = setFromSymbol(lst[1].asSymbol());
            // ok or error
            if (st.code() != NOT_SUPPORTED)
                return st;
        }

        return setFromList(lst.slice(1));
    }

    // NOTE: see note in upper code
    // try float first
    if (lst.isFloat()) {
        auto st = setFromFloat(lst[0].asFloat());
        // ok or error
        if (st.code() != NOT_SUPPORTED)
            return st;
    }

    // try symbol second
    if (lst.isSymbol()) {
        auto st = setFromSymbol(lst[0].asSymbol());
        // ok or error
        if (st.code() != NOT_SUPPORTED)
            return st;
    }

    // try list
    return setFromList(lst);
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
