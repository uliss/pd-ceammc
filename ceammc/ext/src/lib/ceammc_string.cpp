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
#include "ceammc_datastorage.h"
#include "ceammc_log.h"
#include "fmt/core.h"
#include "lex/parser_strings.h"
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
//  `( -> {
//  `) -> }

using SmallWString = boost::container::small_vector<unicode_t, 32>;

namespace ceammc {
namespace string {

    size_t utf8_strlen(const char* str) noexcept
    {
        return utf8len(str);
    }

    const char* utf8_next(const char* text, size_t len)
    {
        return utf8seek(text, len, text, 1, SEEK_CUR);
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
    static bool raw_atom_to_string_t(const Atom& a, T& out)
    {
        auto t = a.atom().a_type;
        switch (t) {
        case A_NULL:
            break;
        case A_FLOAT:
            // this expected to be faster then using atom_string (sprintf by nonius tests)
            fmt::format_to(std::back_inserter(out), "{:g}", a.asT<t_float>());
            break;
        default: {
            char buf[MAXPDSTRING];
            atom_string(&a.atom(), buf, sizeof(buf) - 1);
            out.insert(out.end(), buf, buf + strlen(buf));
        } break;
        }

        return true;
    }

    bool raw_atom_to_string(const Atom& a, StaticString& out) noexcept
    {
        try {
            return raw_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool raw_atom_to_string(const Atom& a, SmallString& out) noexcept
    {
        try {
            return raw_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool raw_atom_to_string(const Atom& a, MediumString& out) noexcept
    {
        try {
            return raw_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    template <typename T>
    static bool raw_list_to_string_t(const AtomListView& lv, T& out) noexcept
    {
        try {
            for (auto& a : lv) {
                // remove space before ',' or ';'
                if ((a.isComma() || a.isSemicolon()) && !out.empty() && out.back() == ' ')
                    out.pop_back();

                if (raw_atom_to_string_t(a, out))
                    out.push_back(' ');
            }

            if (!out.empty() && out.back() == ' ')
                out.pop_back();

            return true;
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool raw_list_to_string(const AtomListView& lv, StaticString& out)
    {
        return raw_list_to_string_t(lv, out);
    }

    void raw_list_to_string(const AtomListView& lv, SmallString& out)
    {
        raw_list_to_string_t(lv, out);
    }

    void raw_list_to_string(const AtomListView& lv, MediumString& out)
    {
        raw_list_to_string_t(lv, out);
    }

    template <typename T>
    static bool parsed_atom_to_string_t(const Atom& a, T& out)
    {
        if (a.isFloat()) {
            fmt::format_to(std::back_inserter(out), "{:g}", a.asT<t_float>());
        } else if (a.isSymbol()) {
            auto s = a.asT<t_symbol*>()->s_name;
            SmallString str;
            // always output quoted string
            // num_esc - number of escaped chars
            auto num_esc = escape_and_quote(s, str);

            if (s[0] != '\0' && num_esc == 0 && str.size() >= 2) {
                // do not use quotes for non empty string without escaped chars
                out.insert(out.end(), str.begin() + 1, str.end() - 1);
            } else if (s[0] == '\0' || num_esc > 0) { // have escaped chars or empty string
                out.insert(out.end(), str.begin(), str.end());
            } else
                LIB_ERR << fmt::format("[{}] quoted and escape error: {}", __FUNCTION__, s);
        } else if (a.isNone()) {
            constexpr const char s[] = "#null";
            out.insert(out.end(), s, s + sizeof(s) - 1);
        } else if (a.isData()) {
            auto str = a.asData()->toString();
            out.insert(out.end(), str.begin(), str.end());
        } else if (a.isComma()) {
            out.push_back(',');
        } else if (a.isSemicolon()) {
            out.push_back(';');
        } else if (a.atom().a_type == A_DOLLSYM) {
            fmt::format_to(std::back_inserter(out), "\"{}\"", a.asT<t_symbol*>()->s_name);
        } else {
            return false;
        }

        return true;
    }

    template <typename T>
    static bool parsed_list_to_string_t(const AtomListView& lv, T& out) noexcept
    {
        try {
            for (auto& a : lv) {
                // remove space before ',' or ';'
                if ((a.isComma() || a.isSemicolon()) && !out.empty() && out.back() == ' ')
                    out.pop_back();

                if (parsed_atom_to_string_t<T>(a, out))
                    out.push_back(' ');
            }

            if (!out.empty() && out.back() == ' ')
                out.pop_back();

            return true;

        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool parsed_atom_to_string(const Atom& a, StaticString& out) noexcept
    {
        try {
            return parsed_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool parsed_atom_to_string(const Atom& a, SmallString& out) noexcept
    {
        try {
            return parsed_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool parsed_atom_to_string(const Atom& a, MediumString& out) noexcept
    {
        try {
            return parsed_atom_to_string_t(a, out);
        } catch (std::exception& e) {
            LIB_ERR << fmt::format("[{}] error: '{}'", __FUNCTION__, e.what());
            return false;
        }
    }

    bool parsed_list_to_string(const AtomListView& lv, StaticString& out) noexcept
    {
        return parsed_list_to_string_t(lv, out);
    }

    bool parsed_list_to_string(const AtomListView& lv, SmallString& out) noexcept
    {
        return parsed_list_to_string_t(lv, out);
    }

    bool parsed_list_to_string(const AtomListView& lv, MediumString& out) noexcept
    {
        return parsed_list_to_string_t(lv, out);
    }

    template <typename T>
    bool atom_to_string_t(const Atom& a, T& out) noexcept
    try {

        switch (a.atom().a_type) {
        case A_FLOAT:
            fmt::format_to(std::back_inserter(out), "{:g}", a.asT<t_float>());
            break;
        case A_SYMBOL: {
            auto s = a.asT<t_symbol*>()->s_name;
            out.insert(out.end(), s, s + strlen(s));
        } break;
        case A_COMMA:
            out.push_back(',');
            break;
        case A_SEMI:
            out.push_back(';');
            break;
        case A_DOLLAR:
            out.push_back('$');
            out.push_back(a.atom().a_w.w_index + '0');
            break;
        case A_DOLLSYM:
            fmt::format_to(std::back_inserter(out), "{}", a.asT<t_symbol*>()->s_name);
            break;
        case A_NULL: {
            const char snull[] = "#null";
            out.insert(out.end(), snull, snull + sizeof(snull) - 1);
        } break;
        default:
            if (a.isData())
                parsed_atom_to_string(a, out);
            break;
        }

        return true;
    } catch (std::exception& e) {
        std::cerr << fmt::format("[{}] error: '{}'\n", __FUNCTION__, e.what());
        return false;
    }

    bool atom_to_string(const Atom& a, StaticString& out) noexcept
    {
        return atom_to_string_t(a, out);
    }

    bool atom_to_string(const Atom& a, SmallString& out) noexcept
    {
        return atom_to_string_t(a, out);
    }

    bool atom_to_string(const Atom& a, MediumString& out) noexcept
    {
        return atom_to_string_t(a, out);
    }

    template <typename T>
    static bool list_to_string_t(const AtomListView& lv, T& out) noexcept
    try {
        for (auto& a : lv) {
            const bool no_space = a.isComma() || a.isSemicolon();

            // remove space before ',' or ';'
            if (no_space && !out.empty() && out.back() == ' ')
                out.pop_back();

            if (atom_to_string_t<T>(a, out) && !no_space)
                out.push_back(' ');
        }

        if (out.size() > 0 && out.back() == ' ')
            out.pop_back();

        return true;

    } catch (std::exception& e) {
        std::cerr << fmt::format("[{}] exception: '{}'\n", __FUNCTION__, e.what());
        return false;
    }

    bool list_to_string(const AtomListView& lv, SmallString& out) noexcept
    {
        return list_to_string_t(lv, out);
    }

    bool list_to_string(const AtomListView& lv, MediumString& out) noexcept
    {
        return list_to_string_t(lv, out);
    }

    bool list_to_string(const AtomListView& lv, StaticString& out) noexcept
    {
        return list_to_string_t(lv, out);
    }

    std::ostream& list_to_stream_typed(const AtomListView& lv, std::ostream& os)
    {
        os << "AtomList: [\n";

        for (auto& a : lv) {
            switch (a.atom().a_type) {
            case A_NULL:
                os << "\t#N\n";
                break;
            case A_FLOAT:
                os << "\t#F (" << a.asFloat() << ")\n";
                break;
            case A_SYMBOL:
                os << "\t#S (" << a.asSymbol()->s_name << ")\n";
                break;
            case A_COMMA:
                os << "\t#COMMA\n";
                break;
            case A_SEMI:
                os << "\t#SEMI\n";
                break;
            case A_DOLLAR:
                os << "\t#DOLLAR (" << a.atom().a_w.w_index << ")\n";
                break;
            case A_DOLLSYM:
                os << "\t#DOLLSYM (" << a.asT<t_symbol*>() << ")\n";
                break;
            case A_POINTER:
                os << "\t#PTR\n";
                break;
            default:
                os << "\t???\n";
                break;
            }
        }

        os << "]\n";
        return os;
    }

    DataTypeName atom_type_name(const Atom& a)
    {
        switch (a.type()) {
        case Atom::NONE:
            return "none";
        case Atom::FLOAT:
            return "float";
        case Atom::SYMBOL:
            return "symbol";
        case Atom::PROPERTY:
            return "property";
        case Atom::DATA:
            return DataStorage::instance().nameByType(a.dataType());
        case Atom::POINTER:
            return "pointer";
        case Atom::SEMICOLON:
            return "semicolon";
        case Atom::COMMA:
            return "comma";
        default:
            return "?";
        }
    }
}
}
