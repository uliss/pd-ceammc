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
#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "fmt/format.h"
#include "lex/parser_strings.h"
#include "re2/re2.h"
#include "utf8rewind/utf8rewind.h"

#include <boost/algorithm/string.hpp>
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

constexpr auto CHAR_ESCAPE = '`';
constexpr auto CHAR_DQUOTE = '"';
constexpr auto CHAR_COMMA = ',';
constexpr auto CHAR_DOT = '.';
constexpr auto CHAR_BRACE_OPEN = '(';
constexpr auto CHAR_BRACE_CLOSE = ')';
constexpr auto CHAR_CURLY_OPEN = '{';
constexpr auto CHAR_CURLY_CLOSE = '}';
constexpr auto CHAR_SLASH = '/';
constexpr auto CHAR_BACKSLASH = '\\';
constexpr auto CHAR_SPACE = ' ';
constexpr auto CHAR_COLON = ':';
constexpr auto CHAR_SEMICOLON = ';';
constexpr auto CHAR_AT = '@';

using SmallWString = boost::container::small_vector<unicode_t, 32>;

namespace ceammc {
namespace string {

    size_t utf8_strlen(const char* str) noexcept
    {
        return utf8len(str);
    }

    std::string utf8_to_upper(const char* str)
    {
        auto input_size = strlen(str);
        int32_t errors = 0;

        auto converted_size = utf8toupper(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors);
        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        SmallString converted(converted_size + 1);

        converted_size = utf8toupper(str, input_size, converted.data(), converted_size, UTF8_LOCALE_DEFAULT, &errors);
        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        return std::string(converted.data(), converted_size);
    }

    std::string utf8_to_lower(const char* str)
    {
        auto input_size = strlen(str);
        int32_t errors = 0;

        auto converted_size = utf8tolower(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors);
        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        SmallString converted(converted_size + 1);

        converted_size = utf8tolower(str, input_size, converted.data(), converted_size, UTF8_LOCALE_DEFAULT, &errors);
        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        return std::string(converted.data(), converted_size);
    }

    std::string utf8_substr(const char* str, int from, size_t len)
    {
        int32_t errors = 0;
        const auto N = utf8len(str);

        if (len == 0)
            return {};

        // check range
        // positive position
        if (from >= 0 && from >= N)
            return {};

        // negative position
        if (from < 0) {
            if (-from <= int(N))
                from += N;
            else
                return {};
        }

        // clip
        len = std::min<size_t>(len, N - from);

        assert(from + len <= N);
        assert(from < N);
        assert(len <= N);

        SmallWString wide(N);
        SmallString narrow(strlen(str));

        auto converted_size = utf8toutf32(str, strlen(str),
            wide.data(), N * sizeof(unicode_t), &errors);

        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        converted_size = utf32toutf8(wide.data() + from, len * sizeof(unicode_t),
            narrow.data(), strlen(str), &errors);
        if (converted_size == 0 || errors != UTF8_ERR_NONE)
            return {};

        return std::string(narrow.data(), converted_size);
    }

    void utf8_split_by_char(std::vector<std::string>& vec, const char* str)
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

    bool starts_with(const std::string& str, const std::string& prefix)
    {
        return boost::starts_with(str, prefix);
    }

    bool starts_with(const char* str, const char* prefix)
    {
        return boost::starts_with(str, prefix);
    }

    bool ends_with(const char* str, const char* suffix)
    {
        return boost::ends_with(str, suffix);
    }

    bool ends_with(const std::string& str, const std::string& suffix)
    {
        return boost::ends_with(str, suffix);
    }

    bool contains(const char* haystack, const char* needle)
    {
        return boost::algorithm::contains(haystack, needle);
    }

    bool contains(const std::string& haystack, const std::string& needle)
    {
        return boost::algorithm::contains(haystack, needle);
    }

    std::string escape_for_json(const std::string& str)
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

    std::string pd_string_escape(const char* str)
    {
        const auto N = strlen(str);
        std::string res;
        res.reserve(N + 4);

        for (size_t i = 0; i < N; i++) {
            auto c = str[i];
            switch (c) {
            case CHAR_ESCAPE:
            case CHAR_DQUOTE:
                res += CHAR_ESCAPE;
                res += c;
                break;
            case CHAR_COMMA:
                res += CHAR_ESCAPE;
                res += CHAR_DOT;
                break;
            case CHAR_CURLY_OPEN:
                res += CHAR_ESCAPE;
                res += CHAR_BRACE_OPEN;
                break;
            case CHAR_CURLY_CLOSE:
                res += CHAR_ESCAPE;
                res += CHAR_BRACE_CLOSE;
                break;
            case CHAR_BACKSLASH:
                res += CHAR_ESCAPE;
                res += CHAR_SLASH;
                break;
            case CHAR_SEMICOLON:
                res += CHAR_ESCAPE;
                res += CHAR_COLON;
                break;
            default:
                res += c;
                break;
            }
        }

        return res;
    }

    std::string pd_string_unescape(const std::string& str)
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

    bool pd_string_match(const std::string& str, std::string& matched)
    {
        if (str.empty() || str[0] != '"')
            return false;

        return re2::RE2::FullMatch(str, re_double_quoted, &matched);
    }

    bool pd_string_parse(const std::string& str, std::string& out)
    {
        if (pd_string_match(str, out)) {
            out = pd_string_unescape(out);
            return true;
        } else
            return false;
    }

    bool is_pd_string(const char* str)
    {
        return is_quoted_string(str);
    }

    std::string remove_all(const std::string& input, const std::string& search)
    {
        return boost::algorithm::erase_all_copy(input, search);
    }

    std::string remove_first(const std::string& input, const std::string& search)
    {
        return boost::algorithm::erase_first_copy(input, search);
    }

    std::string remove_last(const std::string& input, const std::string& search)
    {
        return boost::algorithm::erase_last_copy(input, search);
    }

    std::string replace_all(const std::string& input, const std::string& from, const std::string& to)
    {
        return boost::algorithm::replace_all_copy(input, from, to);
    }

    std::string replace_first(const std::string& input, const std::string& from, const std::string& to)
    {
        return boost::algorithm::replace_first_copy(input, from, to);
    }

    std::string replace_last(const std::string& input, const std::string& from, const std::string& to)
    {
        return boost::algorithm::replace_last_copy(input, from, to);
    }

    void split(std::vector<std::string>& vec, const std::string& str, const char* anysep)
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

    std::string utf8_insert(const char* str, int pos, const char* subj)
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

    std::string utf8_pop(const char* str)
    {
        using codec = std::codecvt_utf8<char32_t>;
        using converter = std::wstring_convert<codec, char32_t>;

        std::u32string s32 = converter().from_bytes(str);
        if (s32.empty())
            return {};

        s32.pop_back();
        return converter().to_bytes(s32);
    }

    char32_t utf8_choose(const char* str)
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

    std::string utf8_reverse(const char* str)
    {
        using codec = std::codecvt_utf8<char32_t>;
        using converter = std::wstring_convert<codec, char32_t>;

        std::u32string s32 = converter().from_bytes(str);
        std::reverse(s32.begin(), s32.end());
        return converter().to_bytes(s32);
    }

    std::string utf8_shuffle(const char* str)
    {
        using codec = std::codecvt_utf8<char32_t>;
        using converter = std::wstring_convert<codec, char32_t>;

        std::u32string s32 = converter().from_bytes(str);
        std::random_shuffle(s32.begin(), s32.end());
        return converter().to_bytes(s32);
    }

    std::string utf8_sort(const char* str)
    {
        using codec = std::codecvt_utf8<char32_t>;
        using converter = std::wstring_convert<codec, char32_t>;

        std::u32string s32 = converter().from_bytes(str);
        std::sort(s32.begin(), s32.end());
        return converter().to_bytes(s32);
    }

    std::string utf8_remove_at(const char* str, int pos)
    {
        using codec = std::codecvt_utf8<char32_t>;
        using converter = std::wstring_convert<codec, char32_t>;

        std::u32string s32 = converter().from_bytes(str);

        if (pos >= s32.length() || (pos < -s32.length())) {
        }
        s32.erase(pos);
        std::sort(s32.begin(), s32.end());
        return converter().to_bytes(s32);
    }

    template <typename T>
    static bool atom_to_string(const Atom& a, T& out) noexcept
    {
        try {
            auto t = a.atom().a_type;
            switch (t) {
            case A_NULL: {
                constexpr const char s[] = "#null";
                std::copy(s, s + sizeof(s) - 1, std::back_inserter(out));
            } break;
            case A_FLOAT:
                fmt::format_to(std::back_inserter(out), "{:g}", a.asT<t_float>());
                break;
            case A_SYMBOL: {
                auto s = a.asT<t_symbol*>()->s_name;
                std::copy(s, s + strlen(s), std::back_inserter(out));
            } break;
            case A_COMMA:
                if (!out.empty() && out.back() == ' ')
                    out.back() = ',';
                else
                    out.push_back(',');
                break;
            case A_SEMI:
                if (!out.empty() && out.back() == ' ')
                    out.back() = ';';
                else
                    out.push_back(';');
                break;
            default:
                LIB_ERR << fmt::format("[{}] unknown atom type: '{}'", __FUNCTION__, t);
                return false;
            }

            return true;
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    template <typename T>
    static bool to_stringT(const AtomListView& lv, T& out) noexcept
    {
        for (auto& a : lv) {
            if (atom_to_string(a, out))
                out.push_back(' ');
            else
                break;
        }

        if (!out.empty() && out.back() == ' ')
            out.pop_back();

        return true;
    }

    void raw_atom_to_string(const Atom& a, std::string& out)
    {
        MediumString str;
        atom_to_string(a, str);
        out.assign(str.data(), str.size());
    }

    bool raw_atom_to_string(const Atom& a, StaticString& out)
    {
        return atom_to_string(a, out);
    }

    void raw_atom_to_string(const Atom& a, SmallString& out)
    {
        atom_to_string(a, out);
    }

    void raw_atom_to_string(const Atom& a, MediumString& out)
    {
        atom_to_string(a, out);
    }

    void raw_list_to_string(const AtomListView& lv, std::string& out)
    {
        MediumString str;
        to_stringT(lv, str);
        out.assign(str.data(), str.size());
    }

    bool raw_list_to_string(const AtomListView& lv, StaticString& out)
    {
        return to_stringT(lv, out);
    }

    void raw_list_to_string(const AtomListView& lv, SmallString& out)
    {
        to_stringT(lv, out);
    }

    void raw_list_to_string(const AtomListView& lv, MediumString& out)
    {
        to_stringT(lv, out);
    }

    template <typename T>
    static bool parsed_atom_to_string_t(const Atom& a, T& out) noexcept
    {
        try {
            if (a.isFloat()) {
                fmt::format_to(std::back_inserter(out), "{:g}", a.asT<t_float>());
            } else if (a.isSymbol()) {
                auto s = a.asT<t_symbol*>()->s_name;
                escape_and_quote(s, out);
            } else if (a.isNone()) {
                constexpr const char s[] = "#null";
                std::copy(s, s + sizeof(s) - 1, std::back_inserter(out));
            } else if (a.isData()) {
                auto str = a.asData()->toString();
                std::copy(str.begin(), str.end(), std::back_inserter(out));
            } else {
                return false;
            }

            return true;
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    template <typename T>
    static bool parsed_list_to_string_t(const AtomListView& lv, T& out) noexcept
    {
        for (auto& a : lv) {
            if (parsed_atom_to_string_t<T>(a, out))
                out.push_back(' ');
            else
                break;
        }

        if (!out.empty() && out.back() == ' ')
            out.pop_back();

        return true;
    }

    void parsed_atom_to_string(const Atom& a, std::string& out)
    {
        SmallString str;
        parsed_atom_to_string_t(a, str);
        out.assign(str.data(), str.size());
    }

    bool parsed_atom_to_string(const Atom& a, StaticString& out)
    {
        return parsed_atom_to_string_t(a, out);
    }

    void parsed_atom_to_string(const Atom& a, SmallString& out)
    {
        parsed_atom_to_string_t(a, out);
    }

    void parsed_atom_to_string(const Atom& a, MediumString& out)
    {
        parsed_atom_to_string_t(a, out);
    }

    void parsed_list_to_string(const AtomListView& lv, std::string& out)
    {
        MediumString str;
        parsed_list_to_string_t(lv, str);
        out.assign(str.data(), str.size());
    }

    bool parsed_list_to_string(const AtomListView& lv, StaticString& out)
    {
        return parsed_list_to_string_t(lv, out);
    }

    void parsed_list_to_string(const AtomListView& lv, SmallString& out)
    {
        parsed_list_to_string_t(lv, out);
    }

    void parsed_list_to_string(const AtomListView& lv, MediumString& out)
    {
        parsed_list_to_string_t(lv, out);
    }

    void parsed_atom_to_raw_list(const Atom& a, AtomList& out)
    {
        SmallString str;
        parsed_atom_to_string(a, str);

        t_binbuf* b = binbuf_new();
        binbuf_text(b, str.data(), str.size());
        out = AtomList(binbuf_getnatom(b), binbuf_getvec(b));
        binbuf_free(b);
    }
}
}
