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
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "ceammc_log.h"
#include "lex/data_string.lexer.h"
#include "lex/data_string.parser.hpp"

namespace ceammc {

AtomList parseDataList(const AtomList& lst) noexcept
{
    if (lst.empty())
        return lst;

    auto str = to_string(lst, " ");
    if (str.empty())
        return lst;

    try {
        AtomList res;
        DataStringLexer lex(str);
        DataStringParser p(lex, res);

        return (p.parse() == 0) ? res : lst;

    } catch (std::exception& e) {
        return lst;
    }
}

AtomList parseDataString(const std::string& str)
{
    if (str.empty())
        return AtomList();

    try {
        LogPdObject logger(nullptr, LOG_ERROR);
        logger.setLogEmpty(false);

        AtomList res;
        DataStringLexer lex(str);
        lex.output_indent = logger.prefix().length() + 1;
        lex.out(logger);
        DataStringParser p(lex, res);

        if (p.parse() != 0)
            return AtomList();

        return res;

    } catch (std::exception& e) {
        LIB_ERR << "parse error: " << e.what();
        return AtomList();
    }
}

AtomList parseDataList(const AtomListView& view) noexcept
{
    if (view.empty())
        return view;

    auto str = to_string(view, " ");
    if (str.empty())
        return view;

    try {
        AtomList res;
        DataStringLexer lex(str);
        DataStringParser p(lex, res);

        return (p.parse() == 0) ? res : AtomList(view);

    } catch (std::exception& e) {
        return view;
    }
}

}
