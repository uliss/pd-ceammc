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

//static void hoa_process_inlet_symbol(t_hoa_process_inlet* x, t_symbol* s)
//{
//    const size_t target = x->x_owner->f_target;
//    if (target != (size_t)-1) {
//        hoa_process_instance_send_symbol(x->x_owner->f_instances + target, x->x_index, s);
//    } else {
//        for (size_t i = 0; i < x->x_owner->f_ninstances; ++i) {
//            hoa_process_instance_send_symbol(x->x_owner->f_instances + i, x->x_index, s);
//        }
//    }
//}

//static void hoa_process_inlet_list(t_hoa_process_inlet* x, t_symbol* s, int argc, t_atom* argv)
//{
//    size_t i;
//    const size_t target = x->x_owner->f_target;
//    if (target != (size_t)-1) {
//        hoa_process_instance_send_list(x->x_owner->f_instances + target, x->x_index, s, argc, argv);
//    } else {
//        for (i = 0; i < x->x_owner->f_ninstances; ++i) {
//            hoa_process_instance_send_list(x->x_owner->f_instances + i, x->x_index, s, argc, argv);
//        }
//    }
//}

//static void hoa_process_inlet_anything(t_hoa_process_inlet* x, t_symbol* s, int argc, t_atom* argv)
//{
//    size_t i;
//    const size_t target = x->x_owner->f_target;
//    if (target != (size_t)-1) {
//        hoa_process_instance_send_anything(x->x_owner->f_instances + target, x->x_index, s, argc, argv);
//    } else {
//        for (i = 0; i < x->x_owner->f_ninstances; ++i) {
//            hoa_process_instance_send_anything(x->x_owner->f_instances + i, x->x_index, s, argc, argv);
//        }
//    }
//}

void setup_spat_hoa_process_inlet()
{
    t_class* c = class_new(gensym("hoa.2d.process.inlet"),
        0, 0, sizeof(ProcessInlet), CLASS_PD, A_CANT, A_NULL);

    if (c) {
        class_addbang(c, (t_method)hoa_process_inlet_bang);
        class_addfloat(c, (t_method)hoa_process_inlet_float);
        //        class_addsymbol(c, (t_method)hoa_process_inlet_symbol);
        //        class_addlist(c, (t_method)hoa_process_inlet_list);
        //        class_addanything(c, (t_method)hoa_process_inlet_anything);
    }

    hoa_process_inlet_class = c;
}
