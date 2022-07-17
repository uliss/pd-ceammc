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
#include "ceammc_format.h"
#include "ceammc_abstractdata.h"
#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_message.h"
#include "ceammc_string.h"
#include "fmt/format.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace ceammc {

std::string to_string(const Atom& a)
{
    if (a.isFloat())
        return fmt::format("{:g}", a.asT<t_float>());
    else if (a.isSymbol())
        return std::string(a.asSymbol()->s_name);
    else if (a.isData())
        return a.asData()->toString();
    else if (a.isNone())
        return "null";
    else {
        std::ostringstream ss;
        ss << a;
        return ss.str();
    }
}

std::string to_string(const AtomListView v, const std::string& separator)
{
    if (v.empty())
        return "";

    std::string res;
    for (size_t i = 0; i < v.size(); i++) {
        if (i != 0)
            res += separator;

        res += to_string(v[i]);
    }

    return res;
}

std::string to_string(const AtomList& l, const std::string& separator)
{
    std::string res;

    if (l.empty())
        return res;

    for (size_t i = 0; i < l.size(); i++) {
        if (i > 0)
            res += separator;

        res += to_string(l[i]);
    }

    return res;
}

std::string to_string(const Message& m, const std::string& separator)
{
    if (m.isFloat() || m.isSymbol())
        return to_string(m.atomValue());
    else if (m.isData())
        return to_string(m.atomValue());
    else if (m.isList())
        return to_string(m.listValue(), separator);
    else if (m.isAny())
        return to_string(m.atomValue()) + separator + to_string(m.listValue(), separator);
    else
        return {};
}



std::string quote(const std::string& str, char q)
{
    std::string res;
    res.reserve(str.size() + 2);
    res.push_back(q);
    res += str;
    res.push_back(q);
    return res;
}

static std::string quote_json(const std::string& str)
{
    std::string res;
    res.reserve(str.length() + 4);
    res.push_back('"');
    for (auto c : str) {
        if (c == '"') {
            res.push_back('\\');
            res.push_back(c);
        } else
            res.push_back(c);
    }
    res.push_back('"');
    return res;
}

std::string to_json_string(const Atom& a)
{
    if (a.isInteger())
        return fmt::format("{}", a.asInt());
    else if (a.isFloat())
        return fmt::format("{}", a.asFloat());
    else if (a.isSymbol()) {
        return quote_json(a.asSymbol()->s_name);
    } else if (a.isData())
        return quote_json(a.asData()->toString());
    else if (a.isNone())
        return "null";
    else {
        std::cerr << "ATOM: " << a << std::endl;
        return "?????????";
    }
}

std::string to_json_string(const AtomList& l)
{
    std::string res;
    res.push_back('[');

    for (size_t i = 0; i < l.size(); i++) {
        if (i > 0)
            res += ", ";

        res += to_json_string(l[i]);
    }

    res.push_back(']');
    return res;
}

std::string parse_quoted(const Atom& a)
{
    auto res = to_string(a);
    string::pd_string_parse(res, res);
    return res;
}

std::string parse_quoted(const AtomList& l)
{
    auto res = to_string(l, " ");
    string::pd_string_parse(res, res);
    return res;
}

std::string parse_quoted(const AtomListView& v)
{
    auto res = to_string(v, " ");
    string::pd_string_parse(res, res);
    return res;
}

std::string format(const std::string& fmt, const AtomList& l)
{
    std::string format_str = fmt;
    for (auto& c : format_str) {
        if (c == '(')
            c = '{';
        else if (c == ')')
            c = '}';
    }

    using ctx = fmt::format_context;
    std::vector<fmt::basic_format_arg<ctx>> fmt_args;

    for (auto& a : l) {
        if (a.isSymbol())
            fmt_args.push_back(fmt::detail::make_arg<ctx>(a.asSymbol()->s_name));
        else if (a.isInteger())
            fmt_args.push_back(fmt::detail::make_arg<ctx>(a.asInt()));
        else if (a.isFloat())
            fmt_args.push_back(fmt::detail::make_arg<ctx>(a.asFloat()));
    }

    return fmt::vformat(format_str, fmt::basic_format_args<ctx>(fmt_args.data(), fmt_args.size()));
}

} // namespace ceammc
