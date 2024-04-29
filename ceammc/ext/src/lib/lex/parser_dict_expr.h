/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_DICT_EXPR_H
#define PARSER_DICT_EXPR_H

#include "m_pd.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace ceammc {
namespace parser {

    enum class DictExprMatchType {
        NONE,
        ANY,
        DICT,
        LIST
    };

    constexpr int64_t ARRAY_SLICE_LENGTH_NONE = -1;
    constexpr int64_t ARRAY_SLICE_LENGTH_END = -2;

    struct DictExprMatcher {
        t_symbol* key_name { &s_ };
        int64_t array_slice_begin { 0 };
        int64_t array_slice_length { ARRAY_SLICE_LENGTH_END };
        DictExprMatchType type { DictExprMatchType::NONE };

        bool matchNone() const { return type == DictExprMatchType::NONE; }
        bool matchAny() const { return type == DictExprMatchType::ANY; }
        bool matchDict() const { return type == DictExprMatchType::DICT; }
        bool matchList() const { return type == DictExprMatchType::LIST; }

        std::pair<size_t, size_t> arraySlice(size_t size) const
        {
            if (array_slice_length == ARRAY_SLICE_LENGTH_NONE)
                return { 0, 0 };

            if (array_slice_begin < 0 && -array_slice_begin > size)
                return { 0, 0 };

            const auto from = (array_slice_begin < 0) //
                ? std::max<std::int64_t>(0, static_cast<std::int64_t>(size) + array_slice_begin)
                : array_slice_begin;

            if (from >= size)
                return { 0, 0 };

            const auto length = (array_slice_length == ARRAY_SLICE_LENGTH_END) //
                ? std::max<std::int64_t>(0, static_cast<std::int64_t>(size) - from)
                : std::max<std::int64_t>(0,
                    std::min<std::int64_t>(static_cast<std::int64_t>(size) - from, array_slice_length));

            return { from, length };
        }
    };

    using DictExprMatchList = std::vector<DictExprMatcher>;

    bool parse_dict_match_expr(const char* expr, DictExprMatchList* matches = nullptr);

}
}

#endif // PARSER_DICT_EXPR_H
