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
#include "hoa_process.h"

static t_class* hoa_process_inlet_class = nullptr;

t_class* process_inlet_class()
{
    return hoa_process_inlet_class;
}

static void hoa_process_inlet_bang(ProcessInlet* x)
{
    HoaProcess* obj = x->x_owner;

    if (!obj->targetAll())
        obj->sendBangToInstance(obj->target(), x->x_index);
    else
        obj->sendBangToAll(x->x_index);
}

static void hoa_process_inlet_float(ProcessInlet* x, t_float f)
{
    HoaProcess* obj = x->x_owner;

    if (!obj->targetAll())
        obj->sendFloatToInstance(obj->target(), x->x_index, f);
    else
        obj->sendFloatToAll(x->x_index, f);
}

static void hoa_process_inlet_symbol(ProcessInlet* x, t_symbol* s)
{
    HoaProcess* obj = x->x_owner;

    if (!obj->targetAll())
        obj->sendSymbolToInstance(obj->target(), x->x_index, s);
    else
        obj->sendSymbolToAll(x->x_index, s);
}

static void hoa_process_inlet_list(ProcessInlet* x, t_symbol* s, int argc, t_atom* argv)
{
    HoaProcess* obj = x->x_owner;

    if (!obj->targetAll())
        obj->sendListToInstance(obj->target(), x->x_index, AtomList(argc, argv));
    else
        obj->sendListToAll(x->x_index, AtomList(argc, argv));
}

static void hoa_process_inlet_anything(ProcessInlet* x, t_symbol* s, int argc, t_atom* argv)
{
    HoaProcess* obj = x->x_owner;

    if (!obj->targetAll())
        obj->sendAnyToInstance(obj->target(), x->x_index, s, AtomList(argc, argv));
    else
        obj->sendAnyToAll(x->x_index, s, AtomList(argc, argv));
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
