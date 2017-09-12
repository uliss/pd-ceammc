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
#include "test_external.h"
#include "ceammc_message.h"
#include "m_pd.h"

#include <iostream>
#include <stdlib.h>

using namespace ceammc;

static t_class* test_output_class;
struct t_test_output {
    t_object x_obj;
    ceammc::Message* msg;
};

static void* test_output_new()
{
    t_test_output* x = reinterpret_cast<t_test_output*>(pd_new(test_output_class));
    x->msg = new Message();
    return static_cast<void*>(x);
}

static void test_output_free(t_test_output* x)
{
    delete x->msg;
}

static void test_output_float(t_test_output* x, t_float f)
{
    x->msg->setFloat(f);
}

static void test_output_symbol(t_test_output* x, t_symbol* s)
{
    x->msg->setSymbol(s);
}

static void test_output_list(t_test_output* x, int argc, t_atom* argv)
{
    x->msg->setList(argc, argv);
}

static void test_output_any(t_test_output* x, t_symbol* s, int argc, t_atom* argv)
{
    x->msg->setAny(s, argc, argv);
}

static bool setup_test_output()
{
    test_output_class = class_new(gensym("test_output"),
        reinterpret_cast<t_newmethod>(test_output_new),
        reinterpret_cast<t_method>(test_output_free),
        sizeof(t_test_output), 0, A_NULL);
    class_addfloat(test_output_class, test_output_float);
    class_addlist(test_output_class, test_output_list);
    class_addsymbol(test_output_class, test_output_symbol);
    class_addanything(test_output_class, test_output_any);
    return true;
}

static bool test_output_init = setup_test_output();

ExternalOutput::ExternalOutput()
    : External("test_output")
{
}

Message ExternalOutput::msg()
{
    t_test_output* obj = (t_test_output*)object();
    return obj ? *obj->msg : Message();
}

void ExternalOutput::reset()
{
    t_test_output* obj = (t_test_output*)object();
    if (obj)
        *obj->msg = Message();
}
