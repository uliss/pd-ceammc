/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "parser_props.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

namespace ceammc {
namespace parser {

    const char* bool_prop_expected()
    {
        return "bool value (1|0|true|false) or operation (random|defaut|!|~)";
    }

    const char* float_prop_expected()
    {
        return "float or operation (+|-|*|/|%|random|defaut)";
    }

    const char* int_prop_expected()
    {
        return "int or operation (+|-|*|/|%|random|defaut)";
    }

    PropParseRes bool_prop_calc(bool prev, bool def, const AtomListView& lv, bool& res)
    {
        if (lv.empty())
            return PropParseRes::UNKNOWN;

        if (lv.isBool()) {
            res = lv[0].asBool();
            return PropParseRes::OK;
        } else if (lv.isSymbol()) {

            auto s = lv[0].asT<t_symbol*>();
            auto op = parse_bool_prop(s->s_name);

            switch (op) {
            case BoolPropOp::TRUE:
                res = true;
                break;
            case BoolPropOp::FALSE:
                res = false;
                break;
            case BoolPropOp::INVERT:
                res = !prev;
                break;
            case BoolPropOp::DEFAULT:
                res = def;
                break;
            case BoolPropOp::RANDOM:
                res = std::rand() & 0x1;
                break;
            default:
                return PropParseRes::UNKNOWN;
            }

            return PropParseRes::OK;
        } else
            return PropParseRes::UNKNOWN;
    }

    bool numeric_prop_random_range(const PropertyInfo& info, t_float& new_min, t_float& new_max)
    {
        constexpr auto max_float = std::numeric_limits<t_float>::max();
        constexpr auto min_float = -std::numeric_limits<t_float>::max();

        auto c = info.constraints();
        switch (c) {
        case PropValueConstraints::CLOSED_OPEN_RANGE:
            new_min = info.minFloat();
            new_max = info.maxFloat();
            return true;
        case PropValueConstraints::CLOSED_RANGE:
            new_min = info.minFloat();
            new_max = std::nextafter(info.maxFloat(), max_float);
            return true;
        case PropValueConstraints::OPEN_RANGE:
            new_min = std::nextafter(info.minFloat(), min_float);
            new_max = info.maxFloat();
            return true;
        case PropValueConstraints::OPEN_CLOSED_RANGE:
            new_min = std::nextafter(info.minFloat(), min_float);
            new_max = std::nextafter(info.maxFloat(), max_float);
            return true;
        default:
            return false;
        }
    }

    bool numeric_prop_random_range(const PropertyInfo& info, int& new_min, int& new_max)
    {
        switch (info.constraints()) {
        case PropValueConstraints::CLOSED_RANGE:
            new_min = info.minInt();
            new_max = info.maxInt();
            return true;
        case PropValueConstraints::OPEN_RANGE:
            new_min = info.minInt() + 1;
            new_max = info.maxInt() - 1;
            return true;
        case PropValueConstraints::OPEN_CLOSED_RANGE:
            new_min = info.minInt() + 1;
            new_max = info.maxInt();
            return true;
        case PropValueConstraints::CLOSED_OPEN_RANGE:
            new_min = info.minInt();
            new_max = info.maxInt() - 1;
            return true;
        default:
            return false;
        }
    }

    t_float numeric_prop_calc_random(t_float min, t_float max)
    {
        std::random_device rnd;
        std::uniform_real_distribution<t_float> dist(min, max);
        return dist(rnd);
    }

    int numeric_prop_calc_random(int min, int max)
    {
        std::random_device rnd;
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rnd);
    }

}
}
