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
#include "re2/re2.h"
#include "utf8rewind/utf8rewind.h"

#include <boost/algorithm/string.hpp>
#include <boost/scoped_array.hpp>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <random>

// list of escapes
//  `" -> "
//  `` -> `
//  `/ -> \
//  `. -> ,
//  `: -> ;
//  `@ -> @

static re2::RE2 re_double_quoted("\"(([^`\"]|`[`\"./:()@])*)\"");
static re2::RE2 re_double_quoted_end("([^`\"]|`[`\"./:()@])*\"");

size_t ceammc::string::utf8_strlen(const char* str) noexcept
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

bool ceammc::string::starts_with(const std::string& str, const std::string& prefix)
{
    return boost::starts_with(str, prefix);
}

bool ceammc::string::starts_with(const char* str, const char* prefix)
{
    return boost::starts_with(str, prefix);
}

bool ceammc::string::ends_with(const char* str, const char* suffix)
{
    return boost::ends_with(str, suffix);
}

bool ceammc::string::ends_with(const std::string& str, const std::string& suffix)
{
    return boost::ends_with(str, suffix);
}

bool ceammc::string::contains(const char* haystack, const char* needle)
{
    return boost::algorithm::contains(haystack, needle);
}

bool ceammc::string::contains(const std::string& haystack, const std::string& needle)
{
    return boost::algorithm::contains(haystack, needle);
}

std::string ceammc::string::escape_for_json(const std::string& str)
{
    std::string res;
    res.reserve(str.length() + 4);

    for (auto c : str) {
        switch (c) {
        case '\b':
            res.push_back('\\');
            res.push_back('b');
            break;
        case '\t':
            res.push_back('\\');
            res.push_back('t');
            break;
        case '\n':
            res.push_back('\\');
            res.push_back('n');
            break;
        case '\f':
            res.push_back('\\');
            res.push_back('f');
            break;
        case '\r':
            res.push_back('\\');
            res.push_back('r');
            break;
        case '"':
            res.push_back('\\');
            res.push_back('"');
            break;
        case '\\':
            res.push_back('\\');
            res.push_back('\\');
            break;
        default:
            res.push_back(c);
            break;
        }
    }

    return res;
}

std::string ceammc::string::pd_string_unescape(const std::string& str)
{
    if (str.size() < 2)
        return str;

    std::string res;
    res.reserve(str.size());

    for (size_t i = 0; i < str.size(); i++) {
        auto* c = &str[i];
        if (*c != '`')
            res.push_back(*c);
        else {
            switch (*(c + 1)) {
            case '"':
                res.push_back('"');
                i++;
                break;
            case '`':
                res.push_back('`');
                i++;
                break;
            case '(':
                res.push_back('{');
                i++;
                break;
            case ')':
                res.push_back('}');
                i++;
                break;
            case '.':
                res.push_back(',');
                i++;
                break;
            case ':':
                res.push_back(';');
                i++;
                break;
            case '/':
                res.push_back('\\');
                i++;
                break;
            case '@':
                res.push_back('@');
                i++;
                break;
            default:
                break;
            }
        }
    }

    return res;
}

bool ceammc::string::pd_string_match(const std::string& str, std::string& matched)
{
    if (str.empty() || str[0] != '"')
        return false;

    return re2::RE2::FullMatch(str, re_double_quoted, &matched);
}

bool ceammc::string::pd_string_parse(const std::string& str, std::string& out)
{
    if (pd_string_match(str, out)) {
        out = pd_string_unescape(out);
        return true;
    } else
        return false;
}

bool ceammc::string::is_pd_string(const char* str)
{
    if (!str[0])
        return false;

    if (str[0] != '"')
        return false;

    const size_t N = strlen(str);
    if (N < 2)
        return false;

    if (str[N - 1] != '"')
        return false;

    return re2::RE2::FullMatch(str, re_double_quoted);
}

bool ceammc::string::pd_string_end_quote(const char* str)
{
    const size_t N = strlen(str);

    if (N < 1)
        return false;

    if (str[N - 1] != '"')
        return false;

    return re2::RE2::FullMatch(str, re_double_quoted_end);
}

std::string ceammc::string::remove_all(const std::string& input, const std::string& search)
{
    return boost::algorithm::erase_all_copy(input, search);
}

std::string ceammc::string::remove_first(const std::string& input, const std::string& search)
{
    return boost::algorithm::erase_first_copy(input, search);
}

std::string ceammc::string::remove_last(const std::string& input, const std::string& search)
{
    return boost::algorithm::erase_last_copy(input, search);
}

std::string ceammc::string::replace_all(const std::string& input, const std::string& from, const std::string& to)
{
    return boost::algorithm::replace_all_copy(input, from, to);
}

std::string ceammc::string::replace_first(const std::string& input, const std::string& from, const std::string& to)
{
    return boost::algorithm::replace_first_copy(input, from, to);
}

std::string ceammc::string::replace_last(const std::string& input, const std::string& from, const std::string& to)
{
    return boost::algorithm::replace_last_copy(input, from, to);
}

void ceammc::string::split(std::vector<std::string>& vec, const std::string& str, const char* anysep)
{
    vec.clear();
    if (str.empty())
        return;

    boost::algorithm::split(vec, str, boost::is_any_of(anysep), boost::token_compress_on);
    // remove all empty elements
    vec.erase(
        std::remove_if(vec.begin(), vec.end(), [](const std::string& s) { return s.empty(); }),
        vec.end());
}

std::string ceammc::string::utf8_insert(const char* str, int pos, const char* subj)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s0 = converter().from_bytes(str);
    std::u32string s1 = converter().from_bytes(subj);
    const int len0 = s0.length();
    // negative index support
    if (pos < 0)
        pos += len0 + 1;

    if (std::abs(pos) <= len0) {
        s0.insert(pos, s1);
        return converter().to_bytes(s0);
    } else
        throw std::out_of_range("invalid insert index: " + std::to_string(pos));
}

std::string ceammc::string::utf8_pop(const char* str)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);
    if (s32.empty())
        return {};

    s32.pop_back();
    return converter().to_bytes(s32);
}

char32_t ceammc::string::utf8_choose(const char* str)
{
    static std::random_device rd;

    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);

    if (s32.length() == 0)
        return -1;

    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, s32.length() - 1);

    return s32[uni(rng)];
}

std::string ceammc::string::utf8_reverse(const char* str)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);
    std::reverse(s32.begin(), s32.end());
    return converter().to_bytes(s32);
}

std::string ceammc::string::utf8_shuffle(const char* str)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);
    std::random_shuffle(s32.begin(), s32.end());
    return converter().to_bytes(s32);
}

std::string ceammc::string::utf8_sort(const char* str)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);
    std::sort(s32.begin(), s32.end());
    return converter().to_bytes(s32);
}

std::string ceammc::string::utf8_remove_at(const char* str, int pos)
{
    using codec = std::codecvt_utf8<char32_t>;
    using converter = std::wstring_convert<codec, char32_t>;

    std::u32string s32 = converter().from_bytes(str);

    if(pos >= s32.length() || (pos < -s32.length())) {

    }
    s32.erase(pos);
    std::sort(s32.begin(), s32.end());
    return converter().to_bytes(s32);
}
