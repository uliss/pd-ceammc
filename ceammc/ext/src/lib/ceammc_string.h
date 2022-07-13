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
#ifndef CEAMMC_STRING_H
#define CEAMMC_STRING_H

#include "ceammc_string_types.h"

#include <cstddef>
#include <string>
#include <vector>

namespace ceammc {

class Atom;
class AtomList;
class AtomListView;

namespace string {

    // utf-8 functions
    size_t utf8_strlen(const char* str) noexcept;
    std::string utf8_to_upper(const char* str);
    std::string utf8_to_lower(const char* str);
    std::string utf8_substr(const char* str, int from, size_t len);
    void utf8_split_by_char(std::vector<std::string>& vec, const char* str);
    void split(std::vector<std::string>& vec, const std::string& str, const char* anysep);

    /**
     * @brief utf8_insert
     * @param str - target string
     * @param pos - negative indexes can be used
     * @param subj - string to insert
     * @return new string
     * @throw std::exception on invalid index
     */
    std::string utf8_insert(const char* str, int pos, const char* subj);

    std::string utf8_pop(const char* str);
    char32_t utf8_choose(const char* str);
    std::string utf8_reverse(const char* str);
    std::string utf8_shuffle(const char* str);
    std::string utf8_sort(const char* str);
    std::string utf8_remove_at(const char* str, int pos);

    bool starts_with(const char* str, const char* prefix);
    bool starts_with(const std::string& str, const std::string& prefix);

    bool ends_with(const char* str, const char* suffix);
    bool ends_with(const std::string& str, const std::string& suffix);

    bool contains(const char* haystack, const char* needle);
    bool contains(const std::string& haystack, const std::string& needle);

    /**
     * remove all the occurrences of the string from the input
     */
    std::string remove_all(const std::string& input, const std::string& search);

    /**
     * remove first occurrence of the string from the input
     */
    std::string remove_first(const std::string& input, const std::string& search);

    /**
     * remove last occurrence of the string from the input
     */
    std::string remove_last(const std::string& input, const std::string& search);

    /**
     * replace all the occurrences of the string from
     */
    std::string replace_all(const std::string& input, const std::string& from, const std::string& to);

    /**
     * replace first occurrence of the string from
     */
    std::string replace_first(const std::string& input, const std::string& from, const std::string& to);

    /**
     * replace last occurrence of the string from
     */
    std::string replace_last(const std::string& input, const std::string& from, const std::string& to);

    // pd-string is single quoted or double quoted string
    //   with ` as escape symbol.
    // - `" -> "
    // - `` -> `
    // - `/ -> \
    // - `. -> ,
    // - `: -> ;

    /**
     * Try to parse pd-string
     * @param str - pdstring
     * @param out - writted parsed and unescaped string
     * @return true if pd-string given, false on error
     */
    bool pd_string_parse(const std::string& str, std::string& out);
    bool pd_string_match(const std::string& str, std::string& matched);
    bool is_pd_string(const char* str);
    std::string pd_string_unescape(const std::string& str);
    std::string pd_string_escape(const char* str);
    std::string pd_string_quote_escape(const char* str);

    std::string escape_for_json(const std::string& str);

    /// prepare atoms list for parsing
    /**
     * Write raw atom (only core Pd atoms: floats, symbols, commas, semicolons etc.)
     * to given static string buffer
     * @return true on success, false on error
     */
    bool raw_atom_to_string(const Atom& a, StaticString& out) noexcept;

    /**
     * Write raw atom (only core Pd atoms: floats, symbols, commas, semicolons etc.)
     * to given small string preallocated buffer
     */
    bool raw_atom_to_string(const Atom& a, SmallString& out) noexcept;

    /**
     * Write raw atom (only core Pd atoms: floats, symbols, commas, semicolons etc.)
     * to given small string preallocated buffer
     */
    bool raw_atom_to_string(const Atom& a, MediumString& out) noexcept;

    /**
     * Write raw atomlist (only core Pd types: floats, symbols, commas, semicolons etc.)
     * to given output string with small preallocated size
     */
    void raw_list_to_string(const AtomListView& lv, SmallString& out);

    /**
     * Write raw atomlist (only core Pd types: floats, symbols, commas, semicolons etc.)
     * to given  output string with medium preallocated size
     */
    void raw_list_to_string(const AtomListView& lv, MediumString& out);

    /**
     * Write raw atomlist (only core Pd types: floats, symbols, commas, semicolons etc.)
     * to given static buffer string
     * @return true on success, false on error
     */
    bool raw_list_to_string(const AtomListView& lv, StaticString& out);

    /// output parsed data
    /**
     * Write parsed atom (can contain only null, float, symbol or data atoms)
     * to given static buffer string
     * @note strings are quoted and escaped
     * @return true on success, false on error
     */
    bool parsed_atom_to_string(const Atom& a, StaticString& out) noexcept;

    /**
     * Write parsed atom (can contain only null, float, symbol or data atoms)
     * to given preallocated small string
     * @note strings are quoted and escaped
     */
    bool parsed_atom_to_string(const Atom& a, SmallString& out) noexcept;

    /**
     * Write parsed atom (can contain only null, float, symbol or data atoms)
     * to given preallocated medium string
     * @note strings are quoted and escaped
     */
    bool parsed_atom_to_string(const Atom& a, MediumString& out) noexcept;

    /**
     * Write parsed atomlist (can contain only null, float, symbol or data atoms)
     * to given static string buffer
     * @note strings are quoted and escaped
     * @return true on success, false on error
     */
    bool parsed_list_to_string(const AtomListView& lv, StaticString& out) noexcept;

    /**
     * Write parsed atomlist (can contain only null, float, symbol or data atoms)
     * to given small preallocated string buffer
     * @note strings are quoted and escaped
     */
    bool parsed_list_to_string(const AtomListView& lv, SmallString& out) noexcept;

    /**
     * Write parsed atomlist (can contain only null, float, symbol or data atoms)
     * to given medium preallocated string buffer
     * @note strings are quoted and escaped
     */
    bool parsed_list_to_string(const AtomListView& lv, MediumString& out) noexcept;

    /**
     * Write parsed atom (can contain only null, float, symbol or data atoms)
     * to given static buffer string
     * @note strings are quoted and escaped
     * @return true on success, false on error
     */
    bool parsed_atom_to_raw_string(const Atom& a, StaticString& out) noexcept;
}
}

#endif // CEAMMC_STRING_H
