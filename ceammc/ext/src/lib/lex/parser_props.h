#ifndef PARSER_PROP_RAGEL_H_
#define PARSER_PROP_RAGEL_H_

#include "ceammc_atomlist_view.h"
#include "ceammc_property_info.h"

#include <algorithm>
#include <cmath>

namespace ceammc {
namespace parser {

    enum class BoolPropOp {
        UNKNOWN,
        TRUE,
        FALSE,
        RANDOM,
        INVERT,
        DEFAULT
    };

    enum class NumericPropOp {
        UNKNOWN,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        DEFAULT,
        RANDOM
    };

    BoolPropOp parse_bool_prop(const char*);
    NumericPropOp parse_numeric_prop_op(const char*);

    const char* bool_prop_expected();
    const char* int_prop_expected();
    const char* float_prop_expected();

    enum class PropParseRes {
        OK,
        UNKNOWN,
        NORANGE,
        INVALID_RANDOM_ARGS,
        DIVBYZERO,
        UNKNOWN_OP
    };

    PropParseRes bool_prop_calc(bool prev, bool def, const AtomListView& lv, bool& res);

    bool numeric_prop_random_range(const PropertyInfo& info, int& new_min, int& new_max);
    bool numeric_prop_random_range(const PropertyInfo& info, t_float& new_min, t_float& new_max);

    t_float numeric_prop_calc_random(t_float min, t_float max);
    int numeric_prop_calc_random(int min, int max);

    template <typename T>
    static PropParseRes numeric_prop_calc_math(NumericPropOp op, T arg, T prev, T& res)
    {
        switch (op) {
        case NumericPropOp::ADD:
            res = prev + arg;
            return PropParseRes::OK;
        case NumericPropOp::SUB:
            res = prev - arg;
            return PropParseRes::OK;
        case NumericPropOp::MUL:
            res = prev * arg;
            return PropParseRes::OK;
        case NumericPropOp::DIV: {
            if (arg == 0)
                return PropParseRes::DIVBYZERO;
            res = prev / arg;
            return PropParseRes::OK;
        }
        case NumericPropOp::MOD: {
            if (arg == 0)
                return PropParseRes::DIVBYZERO;
            res = std::fmod(prev, arg);
            return PropParseRes::OK;
        }
        default:
            return PropParseRes::UNKNOWN_OP;
        }
    }

    template <typename T>
    static PropParseRes numeric_prop_calc(T prev, const PropertyInfo& info, const AtomListView& lv, T& res)
    {
        if (lv.empty())
            return PropParseRes::UNKNOWN;

        if (lv.size() == 1 && lv[0].isFloat()) {
            res = lv[0].asT<T>();
            return PropParseRes::OK;
        } else if (lv[0].isSymbol()) {

            auto s = lv[0].asT<t_symbol*>();
            auto op = parse_numeric_prop_op(s->s_name);

            switch (op) {
            case NumericPropOp::DEFAULT: {
                T def = 0;
                if (!info.getDefault(def))
                    return PropParseRes::UNKNOWN;

                res = def;
                return PropParseRes::OK;
            }
            case NumericPropOp::RANDOM: {
                T new_min = 0;
                T new_max = 0;

                if (!numeric_prop_random_range(info, new_min, new_max))
                    return PropParseRes::NORANGE;

                const bool void_random_args = (lv.size() == 1);
                const bool random_args = (lv.size() == 3 && lv[1].isA<T>() && lv[2].isA<T>());

                if (!void_random_args && !random_args)
                    return PropParseRes::INVALID_RANDOM_ARGS;

                if (random_args) {
                    new_min = std::max(new_min, lv[1].asT<T>());
                    new_max = std::min(new_max, lv[2].asT<T>());
                }

                res = numeric_prop_calc_random(new_min, new_max);
                return PropParseRes::OK;
            } break;
            case NumericPropOp::UNKNOWN:
                return PropParseRes::UNKNOWN;
            default: // math
                if (lv.size() == 2 && lv[1].isFloat()) {
                    const T arg = lv[1].asT<T>();
                    return numeric_prop_calc_math(op, arg, prev, res);
                } else
                    return PropParseRes::UNKNOWN;

            } // end switch

        } else {
            return PropParseRes::UNKNOWN;
        }
    }

}
}

#endif // PARSER_PROP_RAGEL_H_
