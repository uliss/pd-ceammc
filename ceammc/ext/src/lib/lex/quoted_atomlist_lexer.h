/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef QUOTED_ATOMLIST_LEXER_H
#define QUOTED_ATOMLIST_LEXER_H

#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_format.h"
#include "quoted_string.parser.hpp"

namespace ceammc {

class QuotedAtomListLexer {
    AtomListView v_;
    size_t idx_;
    AtomList result_;
    bool quoted_props_;

public:
    QuotedAtomListLexer(const AtomListView& v, bool quoted_props = false)
        : v_(v)
        , idx_(0)
        , quoted_props_(quoted_props)
    {
    }

    int lex(t_interval* tok)
    {
        if (idx_ >= v_.size())
            return QuotedAtomListParser::token::TOK_STRING_END;

        const auto& a = v_[idx_++];

        auto s = a.asSymbol(nullptr);
        if (s == nullptr) // not a symbol
            return QuotedAtomListParser::token::TOK_SIMPLE_ATOM;
        else if (s->s_name[0] == '"' && s->s_name[1] == '\0') // single double quote
            return QuotedAtomListParser::token::TOK_DOUBLE_QUOTE;
        else {
            if (a.isQuoted()) {
                if (quoted_props_ && a.asSymbol()->s_name[1] == '@')
                    return QuotedAtomListParser::token::TOK_QUOTED_PROPERTY;
                else
                    return QuotedAtomListParser::token::TOK_QUOTED_ATOM;
            } else if (a.beginQuote()) {
                return QuotedAtomListParser::token::TOK_DOUBLE_QUOTE_BEGIN;
            } else if (a.endQuote()) {
                return QuotedAtomListParser::token::TOK_DOUBLE_QUOTE_END;
            } else
                return QuotedAtomListParser::token::TOK_SIMPLE_ATOM;
        }
    }

    size_t idx() const { return idx_ - 1; }

    void pushRange(t_interval r)
    {
        if (r.quoted) {
            if(r.quoted_property) {
                result_.append(v_[r.start]);
                return;
            }
            auto str = parse_quoted(v_.subView(r.start, r.length()));
            auto a = Atom(gensym(str.c_str()));
            result_.append(a);
        } else {
            for (int j = r.start; j <= r.end; j++)
                result_.append(v_[j]);
        }
    }

    const AtomList& result() const { return result_; }
};

}

#endif // QUOTED_ATOMLIST_LEXER_H
