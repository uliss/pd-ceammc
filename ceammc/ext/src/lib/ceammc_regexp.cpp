/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "ceammc_regexp.h"

std::string ceammc::regexp::escape(const std::string& s)
{
    std::string res;
    res.reserve(s.size());
    const size_t N = s.size();

    for (size_t i = 0; i < N; i++) {
        const int c = s[i];
        const bool last = (i == N - 1);
        const int cnext = last ? -1 : s[i + 1];
        const int cprev = (i == 0) ? -1 : s[i - 1];

        if (c == '`') {
            if (last) { /* last ` in the string -> \\ */
                res.push_back('\\');
                break;
            } else if (cnext == '`') { // double `` -> `
                i++;
                res.push_back('`');
            } else if (cnext == ':') { // `: -> ;
                i++;
                res.push_back(';');
            } else { /* ` -> \\ */
                res.push_back('\\');
            }

            continue;

        } else if (c == ')' && cnext == ')') {
            i++;
            res.push_back('}');
        } else if (c == '(' && cnext == '(') {
            i++;
            res.push_back('{');
        } else if (c == '.' && cnext == '.' && cprev != '`') {
            i++;
            res.push_back(',');
        } else {
            res.push_back(c);
        }
    }

    return res;
}

std::string ceammc::regexp::unescape(const std::string& s)
{
    std::string res;
    res.reserve(s.size());

    for (auto c : s) {
        switch (c) {
        case '\\':
            res.push_back('`');
            break;
        case '`':
            res.push_back('`');
            res.push_back('`');
            break;
        case '{':
            res.push_back('(');
            res.push_back('(');
            break;
        case '}':
            res.push_back(')');
            res.push_back(')');
            break;
        case ',':
            res.push_back('.');
            res.push_back('.');
            break;
        case ';':
            res.push_back('`');
            res.push_back(':');
            break;
        default:
            res.push_back(c);
            break;
        }
    }

    return res;
}
