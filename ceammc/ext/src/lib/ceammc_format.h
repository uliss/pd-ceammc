/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_FORMATTER_H
#define CEAMMC_FORMATTER_H

#include <string>

namespace ceammc {

class Atom;
class AtomList;
class AtomListView;
class Message;

std::string to_string(const Atom& a);
std::string to_string(const AtomListView v, const std::string& separator = " ");
std::string to_string(const AtomList& l, const std::string& separator = " ");
std::string to_string(const Message& m, const std::string& separator = " ");

std::string to_json_string(const Atom& a);
std::string to_json_string(const AtomList& l);

/**
 * same as to_string(), but symbol's that contains spaces will be in double quotes, and if there's
 * inner quotes, they will be escaped with '`'
 * @example a b c -> "a b c", "abc" -> "`"abc`""
 * @param str - string to quote
 * @return quoted string if needed
 */
std::string to_string_quoted(const std::string& str);
std::string to_string_quoted(const Atom& a);

std::string parse_quoted(const Atom& str);
std::string parse_quoted(const AtomList& l);
std::string parse_quoted(const AtomListView& v);

std::string format(const std::string& fmt, const AtomList& l);

/**
 * Simple string quotation
 * @param str - string to quote
 * @param q - quotation character
 * @return quoted string
 */
std::string quote(const std::string& str, char q = '"');

} // namespace ceammc

#endif // CEAMMC_FORMATTER_H
