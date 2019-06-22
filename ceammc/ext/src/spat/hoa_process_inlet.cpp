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

static Maybe<uint8_t> parseTarget(t_symbol* s)
{
    if (strlen(s->s_name) < 2)
        return Maybe<uint8_t>();

    if (s->s_name[0] != '#')
        return Maybe<uint8_t>();

    char* end = nullptr;
    long v = strtol(&s->s_name[1], &end, 10);
    if ((&s->s_name[1] == end) || (errno == ERANGE))
        return Maybe<uint8_t>();

    if (v < 0 || v > std::numeric_limits<uint8_t>::max())
        return Maybe<uint8_t>();

    return Maybe<uint8_t>(v);
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

static void hoa_process_inlet_anything(ProcessInlet* x, t_symbol* s, int argc, t_atom* argv)
{
    HoaProcess* obj = x->x_owner;

    auto maybe_target = parseTarget(s);

    if (!maybe_target) {
        obj->sendAnyToAll(x->x_index, s, AtomList(argc, argv));
    } else {
        assert(argc >= 0);

        // argc == 0: #T
        if (argc == 0)
            obj->sendBangToInstance(*maybe_target, x->x_index);
        // argc == 1: #T 100
        else if (argc == 1 && argv[0].a_type == A_FLOAT)
            obj->sendFloatToInstance(*maybe_target, x->x_index, argv[0].a_w.w_float);
        // argc == 1: ->T bang
        else if (argc == 1 && argv[0].a_type == A_SYMBOL && argv[0].a_w.w_symbol == &s_bang)
            obj->sendBangToInstance(*maybe_target, x->x_index);
        // argc == 1: ->T nobang
        else if (argc == 1 && argv[0].a_type == A_SYMBOL)
            obj->sendAnyToInstance(*maybe_target, x->x_index, argv[0].a_w.w_symbol, AtomList());
        // argc == 2: ->T symbol name
        else if (argc == 2 && argv[0].a_type == A_SYMBOL
            && argv[1].a_type == A_SYMBOL && argv[0].a_w.w_symbol == &s_symbol)
            obj->sendSymbolToInstance(*maybe_target, x->x_index, argv[1].a_w.w_symbol);
        // argc == 2: ->T float 100
        else if (argc == 2 && argv[0].a_type == A_SYMBOL
            && argv[1].a_type == A_FLOAT && argv[0].a_w.w_symbol == &s_symbol)
            obj->sendFloatToInstance(*maybe_target, x->x_index, argv[1].a_w.w_float);
        // argc > 1: ->T list A B C etc...
        else if (argc > 1 && argv[0].a_type == A_SYMBOL && argv[0].a_w.w_symbol == &s_list)
            obj->sendListToInstance(*maybe_target, x->x_index, AtomList(argc - 1, &argv[1]));
        // argc > 1: ->T 100 A B C etc...
        else if (argc > 1 && argv[0].a_type == A_FLOAT)
            obj->sendListToInstance(*maybe_target, x->x_index, AtomList(argc, argv));
        // all others
        else if (argc > 0 && argv[0].a_type == A_SYMBOL)
            obj->sendAnyToInstance(*maybe_target, x->x_index, argv[0].a_w.w_symbol, AtomList(argc - 1, &argv[1]));
    }
}

void setup_spat_hoa_process_inlet()
{
    t_class* c = class_new(gensym("hoa.2d.process.inlet"),
        0, 0, sizeof(ProcessInlet), CLASS_PD, A_CANT, A_NULL);

    if (c) {
        class_addbang(c, (t_method)hoa_process_inlet_bang);
        class_addfloat(c, (t_method)hoa_process_inlet_float);
        class_addsymbol(c, (t_method)hoa_process_inlet_symbol);
        class_addlist(c, (t_method)hoa_process_inlet_list);
        class_addanything(c, (t_method)hoa_process_inlet_anything);
    }

    hoa_process_inlet_class = c;
}
