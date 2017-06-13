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
#include "ceammc_string.h"
#include "utf8rewind/utf8rewind.h"

#include <boost/scoped_array.hpp>
#include <cstdlib>
#include <iostream>

size_t ceammc::string::utf8_strlen(const char* str)
{
    return utf8len(str);
}

std::string ceammc::string::utf8_to_upper(const char* str)
{
    size_t input_size = strlen(str);
    int32_t errors = 0;

    size_t converted_size = utf8toupper(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors);
    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    boost::scoped_array<char> converted(new char[converted_size + 1]);

    converted_size = utf8toupper(str, input_size, converted.get(), converted_size, UTF8_LOCALE_DEFAULT, &errors);
    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    return std::string(converted.get(), converted_size);
}

std::string ceammc::string::utf8_to_lower(const char* str)
{
    size_t input_size = strlen(str);
    int32_t errors = 0;

    size_t converted_size = utf8tolower(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors);
    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    boost::scoped_array<char> converted(new char[converted_size + 1]);

    converted_size = utf8tolower(str, input_size, converted.get(), converted_size, UTF8_LOCALE_DEFAULT, &errors);
    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    return std::string(converted.get(), converted_size);
}

std::string ceammc::string::utf8_substr(const char* str, int from, size_t len)
{
    int32_t errors = 0;
    const size_t N = utf8len(str);

    if (len == 0)
        return std::string();

    // check range
    // positive position
    if (from >= 0 && from >= N)
        return std::string();

    // negative position
    if (from < 0) {
        if (-from <= int(N))
            from += N;
        else
            return std::string();
    }

    // clip
    len = std::min<size_t>(len, N - from);

    assert(from + len <= N);
    assert(from < N);
    assert(len <= N);

    boost::scoped_array<unicode_t> wide(new unicode_t[N]);
    boost::scoped_array<char> narrow(new char[strlen(str)]);

    size_t converted_size = utf8toutf32(str, strlen(str),
        wide.get(), N * sizeof(unicode_t), &errors);

    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    converted_size = utf32toutf8(wide.get() + from, len * sizeof(unicode_t),
        narrow.get(), strlen(str), &errors);
    if (converted_size == 0 || errors != UTF8_ERR_NONE)
        return std::string();

    return std::string(narrow.get(), converted_size);
}

void ceammc::string::utf8_split_by_char(std::vector<std::string>& vec, const char* str)
{
    vec.clear();
    size_t len = utf8len(str);
    if (len == 0)
        return;

    vec.reserve(len);

    const char* c0 = str;
    const char* c1 = str;

    while (len-- > 0) {
        c1 = utf8seek(c0, strlen(str), c0, 1, SEEK_CUR);
        vec.push_back(std::string(c0, c1 - c0));
        c0 = c1;
    }
}
