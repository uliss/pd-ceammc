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
#include "hoa_process_inlet.h"
#include "ceammc_maybe.h"
#include "hoa_process.h"

#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <errno.h>
#include <limits>

enum class TargetType {
    NONE = 0,
    ERROR,
    ALL,
    EQ,
    GT,
    LT,
    RANGE,
    SPREAD
};

struct t_target {
    uint8_t from;
    uint8_t to;
    TargetType type;
};

enum {
    PARSE_ERROR_NAN = -1,
    PARSE_ERROR_TOOBIG = -2,
    PARSE_ERROR_EMPTY = -3,
    PARSE_ERROR_NEGATIVE = -4
};

static long parseByte(const char* str)
{
    if (strlen(str) < 1)
        return PARSE_ERROR_EMPTY;

    char* end = nullptr;
    long v = strtol(str, &end, 10);

    if (str == end)
        return PARSE_ERROR_NAN;

    if ((errno == ERANGE) || v > std::numeric_limits<uint8_t>::max())
        return PARSE_ERROR_TOOBIG;

    if (v < 0)
        return PARSE_ERROR_NEGATIVE;

    return v;
}

/**
 * @brief parseTarget parse target string
 *
 * @example #* - send to all instances
 * @example #*2 - send to all instances with id >= 1
 * @example #: send list values spreaded amoung all instances
 * @example #:1 send list values spreaded amoung instances with id >= 1
 * @example #1 - send to second instance
 * @example #<2 - send to first and second instances
 * @example #>2 - send to instances with id > 2
 * @example #2-4 send to instances with id in [2, 4] range
 */
static Maybe<t_target> parseTarget(t_symbol* s)
{
    const char* str = s->s_name;

    if (str[0] != '#')
        return Maybe<t_target>({ 0, 0, TargetType::NONE });

    Maybe<t_target> err({ 0, 0, TargetType::ERROR });

    if (strlen(str) < 2)
        return err;

    switch (str[1]) {
    case '*': {

        auto val = parseByte(str + 2);
        switch (val) {
        case PARSE_ERROR_NEGATIVE:
        case PARSE_ERROR_NAN:
            return err;
        case PARSE_ERROR_EMPTY:
        case PARSE_ERROR_TOOBIG:
            return Maybe<t_target>({ 0, 0, TargetType::ALL });
        default:
            return Maybe<t_target>({ static_cast<uint8_t>(val), 0, TargetType::ALL });
        }

    } break;
    case '<': { // #<INT

        auto val = parseByte(str + 2);
        if (val < 0)
            return err;
        else
            return Maybe<t_target>({ static_cast<uint8_t>(val), 0, TargetType::LT });

    } break;
    case '>': { // #>INT

        auto val = parseByte(str + 2);
        if (val < 0)
            return err;
        else
            return Maybe<t_target>({ static_cast<uint8_t>(val), 0, TargetType::GT });

    } break;
    case ':': { // #:INT

        auto val = parseByte(str + 2);
        switch (val) {
        case PARSE_ERROR_NEGATIVE:
        case PARSE_ERROR_NAN:
            return err;
        case PARSE_ERROR_EMPTY:
        case PARSE_ERROR_TOOBIG:
            return Maybe<t_target>({ 0, 0, TargetType::SPREAD });
        default:
            return Maybe<t_target>({ static_cast<uint8_t>(val), 0, TargetType::SPREAD });
        }

    } break;
    default: {
        if (isdigit(str[1])) {
            auto range_start = str + 1;
            char* end = nullptr;
            long from = strtol(range_start, &end, 10);
            if ((range_start == end) || (errno == ERANGE))
                return err;

            if (from < 0 || from > std::numeric_limits<uint8_t>::max())
                return err;

            if (end[0] == '\0') // #INT
                return Maybe<t_target>({ static_cast<uint8_t>(from), 0, TargetType::EQ });
            else if (end[0] == '-') { // #FROM-TO
                const long to = parseByte(end + 1);

                if (to < 0)
                    return err;
                else
                    return Maybe<t_target>({ static_cast<uint8_t>(from), static_cast<uint8_t>(to), TargetType::RANGE });

            } else // #INT-???
                return err;
        }

        return err;
    } break;
    }
}

static t_class* hoa_process_inlet_class = nullptr;

t_class* process_inlet_class()
{
    return hoa_process_inlet_class;
}

static void hoa_process_inlet_bang(ProcessInlet* x)
{
    HoaProcess* obj = x->x_owner;
    obj->sendBangToAll(x->x_index);
}

static void hoa_process_inlet_float(ProcessInlet* x, t_float f)
{
    HoaProcess* obj = x->x_owner;
    obj->sendFloatToAll(x->x_index, f);
}

static void hoa_process_inlet_symbol(ProcessInlet* x, t_symbol* s)
{
    HoaProcess* obj = x->x_owner;
    obj->sendSymbolToAll(x->x_index, s);
}

static void hoa_process_inlet_list(ProcessInlet* x, t_symbol* s, int argc, t_atom* argv)
{
    HoaProcess* obj = x->x_owner;
    obj->sendListToAll(x->x_index, AtomList(argc, argv));
}

auto is_float = [](const t_atom& a) { return a.a_type == A_FLOAT; };
auto is_symbol = [](const t_atom& a) { return a.a_type == A_SYMBOL; };
auto is_sym_bang = [](const t_atom& a) { return is_symbol(a) && a.a_w.w_symbol == &s_bang; };
auto is_sym_float = [](const t_atom& a) { return is_symbol(a) && a.a_w.w_symbol == &s_float; };
auto is_sym_symbol = [](const t_atom& a) { return is_symbol(a) && a.a_w.w_symbol == &s_symbol; };
auto is_sym_list = [](const t_atom& a) { return is_symbol(a) && a.a_w.w_symbol == &s_list; };

struct MsgCallbacks {
    std::function<void(size_t, size_t)> fn_bang;
    std::function<void(size_t, size_t, t_float)> fn_float;
    std::function<void(size_t, size_t, t_symbol*)> fn_symbol;
    std::function<void(size_t, size_t, const AtomList&)> fn_list;
    std::function<void(size_t, size_t, t_symbol*, const AtomListView&)> fn_any;
};

static void message_handler(size_t idx, size_t inlet, int argc, t_atom* argv, const MsgCallbacks& cb)
{
    /** BANG */
    if ((argc == 0) || (argc == 1 && is_sym_bang(argv[0])))
        cb.fn_bang(idx, inlet);
    /** FLOAT */
    else if (argc == 1 && is_float(argv[0]))
        cb.fn_float(idx, inlet, argv[0].a_w.w_float);
    else if (argc == 2 && is_sym_float(argv[0]) && is_float(argv[1]))
        cb.fn_float(idx, inlet, argv[1].a_w.w_float);
    /** SYMBOL */
    else if (argc == 2 && is_sym_symbol(argv[0]) && is_symbol(argv[1]))
        cb.fn_symbol(idx, inlet, argv[1].a_w.w_symbol);
    /** LIST */
    else if (argc > 0 && is_sym_list(argv[0]))
        cb.fn_list(idx, inlet, AtomList(argc - 1, &argv[1]));
    else if (argc > 1 && is_float(argv[0]))
        cb.fn_list(idx, inlet, AtomList(argc, argv));
    /** ANY */
    else if (argc > 0 && is_symbol(argv[0]))
        cb.fn_any(idx, inlet, argv[0].a_w.w_symbol, AtomList(argc - 1, &argv[1]));
    else {
        pd_error(nullptr, "unknown message format:");
        postatom(argc, argv);
    }
}

static void hoa_process_inlet_anything(ProcessInlet* x, t_symbol* s, int argc, t_atom* argv)
{
    HoaProcess* obj = x->x_owner;

    auto maybe_target = parseTarget(s);

    if (!maybe_target) {
        obj->sendAnyToAll(x->x_index, s, AtomListView(argv, argc));
    } else {
        assert(argc >= 0);

        switch (maybe_target.value().type) {
        case TargetType::EQ: {

            MsgCallbacks cb {
                [obj](size_t idx, size_t inlet) { obj->sendBangToInstance(idx, inlet); },
                [obj](size_t idx, size_t inlet, t_float f) { obj->sendFloatToInstance(idx, inlet, f); },
                [obj](size_t idx, size_t inlet, t_symbol* s) { obj->sendSymbolToInstance(idx, inlet, s); },
                [obj](size_t idx, size_t inlet, const AtomList& l) { obj->sendListToInstance(idx, inlet, l); },
                [obj](size_t idx, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToInstance(idx, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);
        } break;
        case TargetType::GT: {

            MsgCallbacks cb {
                [obj](size_t idx, size_t inlet) { obj->sendBangToGreaterEq(idx + 1, inlet); },
                [obj](size_t idx, size_t inlet, t_float f) { obj->sendFloatToGreaterEq(idx + 1, inlet, f); },
                [obj](size_t idx, size_t inlet, t_symbol* s) { obj->sendSymbolToGreaterEq(idx + 1, inlet, s); },
                [obj](size_t idx, size_t inlet, const AtomList& l) { obj->sendListToGreaterEq(idx + 1, inlet, l); },
                [obj](size_t idx, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToGreaterEq(idx + 1, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);

        } break;
        case TargetType::LT: {

            MsgCallbacks cb {
                [obj](size_t idx, size_t inlet) { obj->sendBangToLessThen(idx, inlet); },
                [obj](size_t idx, size_t inlet, t_float f) { obj->sendFloatToLessThen(idx, inlet, f); },
                [obj](size_t idx, size_t inlet, t_symbol* s) { obj->sendSymbolToLessThen(idx, inlet, s); },
                [obj](size_t idx, size_t inlet, const AtomList& l) { obj->sendListToLessThen(idx, inlet, l); },
                [obj](size_t idx, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToLessThen(idx, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);

        } break;
        case TargetType::RANGE: {

            size_t to = maybe_target.value().to;

            MsgCallbacks cb {
                [obj, to](size_t from, size_t inlet) { obj->sendBangToRange(from, to, inlet); },
                [obj, to](size_t from, size_t inlet, t_float f) { obj->sendFloatToRange(from, to, inlet, f); },
                [obj, to](size_t from, size_t inlet, t_symbol* s) { obj->sendSymbolToRange(from, to, inlet, s); },
                [obj, to](size_t from, size_t inlet, const AtomList& l) { obj->sendListToRange(from, to, inlet, l); },
                [obj, to](size_t from, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToRange(from, to, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);

        } break;
        case TargetType::ALL: {

            MsgCallbacks cb {
                [obj](size_t idx, size_t inlet) { obj->sendBangToGreaterEq(idx, inlet); },
                [obj](size_t idx, size_t inlet, t_float f) { obj->sendFloatToGreaterEq(idx, inlet, f); },
                [obj](size_t idx, size_t inlet, t_symbol* s) { obj->sendSymbolToGreaterEq(idx, inlet, s); },
                [obj](size_t idx, size_t inlet, const AtomList& l) { obj->sendListToGreaterEq(idx, inlet, l); },
                [obj](size_t idx, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToGreaterEq(idx, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);

        } break;
        case TargetType::SPREAD: {

            MsgCallbacks cb {
                [obj](size_t idx, size_t inlet) { obj->sendBangToGreaterEq(idx, inlet); },
                [obj](size_t idx, size_t inlet, t_float f) { obj->sendFloatToGreaterEq(idx, inlet, f); },
                [obj](size_t idx, size_t inlet, t_symbol* s) { obj->sendSymbolToGreaterEq(idx, inlet, s); },
                [obj](size_t idx, size_t inlet, const AtomList& l) { obj->sendListToSpread(idx, inlet, l); },
                [obj](size_t idx, size_t inlet, t_symbol* s, const AtomListView& l) { obj->sendAnyToSpread(idx, inlet, s, l); }
            };

            message_handler(maybe_target.value().from, x->x_index, argc, argv, cb);

        } break;
        case TargetType::NONE: {
            obj->sendAnyToAll(x->x_index, s, AtomListView(argv, argc));
        } break;

        default:
        case TargetType::ERROR: {
            pd_error(x, "[hoa.process~] invalid instance target: %s", s->s_name);
        } break;
        }
    }
}

void setup_spat_hoa_process_inlet()
{
    t_class* c = class_new(gensym("hoa.2d.process.inlet"),
        0, 0, sizeof(ProcessInlet), CLASS_PD, A_CANT, A_NULL);

    if (c) {
        class_addbang(c, reinterpret_cast<t_method>(hoa_process_inlet_bang));
        class_doaddfloat(c, reinterpret_cast<t_method>(hoa_process_inlet_float));
        class_addsymbol(c, reinterpret_cast<t_method>(hoa_process_inlet_symbol));
        class_addlist(c, reinterpret_cast<t_method>(hoa_process_inlet_list));
        class_addanything(c, reinterpret_cast<t_method>(hoa_process_inlet_anything));
    }

    hoa_process_inlet_class = c;
}
