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
#include "test_external_log_output.h"

static t_class* log_output_single = nullptr;
static t_class* log_output_multi = nullptr;

#define LOG_MULTI 1

#ifdef LOG_MULTI
#define LOG_NAME "log_output_multi"
#else
#define LOG_NAME "log_output_single"
#endif

using namespace ceammc;

void LogExternalOutput::setup()
{
    if (!log_output_single)
        setup_log_output_single();

    if (!log_output_multi)
        setup_log_output_multi();
}

LogExternalOutput::LogExternalOutput()
    : External(LOG_NAME)
{
}

Message LogExternalOutput::msg()
{
#if LOG_MULTI
    auto* obj = (t_log_output_multi*)object();
    if (!obj || !obj->msg_list || obj->msg_list->empty())
        return Message();

    return obj->msg_list->back();
#else
    t_log_output_single* obj = (t_log_output_single*)object();
    return obj ? *obj->msg : Message();
#endif
}

LogExternalOutput::MessageList LogExternalOutput::msgList()
{
#if LOG_MULTI
    t_log_output_multi* obj = (t_log_output_multi*)object();
    if (!obj || !obj->msg_list)
        return {};

    return *obj->msg_list;
#else
    t_log_output_single* obj = (t_log_output_single*)object();
    return obj ? MessageList(*obj->msg, 1) : {};
#endif
}

void LogExternalOutput::reset()
{
#if LOG_MULTI
    t_log_output_multi* obj = (t_log_output_multi*)object();
    if (obj && obj->msg_list)
        obj->msg_list->clear();

#else
    t_log_output_single* obj = (t_log_output_single*)object();
    if (obj)
        *obj->msg = Message();
#endif
}

static void* log_output_single_new()
{
    auto x = reinterpret_cast<t_log_output_single*>(pd_new(log_output_single));
    x->msg = new Message();
    return static_cast<void*>(x);
}

static void log_output_single_free(t_log_output_single* x)
{
    delete x->msg;
}

static void log_output_single_bang(t_log_output_single* x)
{
    x->msg->setBang();
}

static void log_output_single_float(t_log_output_single* x, t_float f)
{
    x->msg->setFloat(f);
}

static void log_output_single_symbol(t_log_output_single* x, t_symbol* s)
{
    x->msg->setSymbol(s);
}

static void log_output_single_list(t_log_output_single* x, t_symbol*, int argc, t_atom* argv)
{
    x->msg->setList(argc, argv);
}

static void log_output_single_any(t_log_output_single* x, t_symbol* s, int argc, t_atom* argv)
{
    x->msg->setAny(s, argc, argv);
}

bool setup_log_output_single()
{
    static std::unique_ptr<t_class, std::function<void(t_class*)>> cls_owner(
        nullptr,
        [](t_class* c) { free(c); });

    log_output_single = class_new(gensym("log_output_single"),
        reinterpret_cast<t_newmethod>(log_output_single_new),
        reinterpret_cast<t_method>(log_output_single_free),
        sizeof(t_log_output_single), 0, A_NULL);
    class_addbang(log_output_single, log_output_single_bang);
    class_addfloat(log_output_single, log_output_single_float);
    class_addlist(log_output_single, log_output_single_list);
    class_addsymbol(log_output_single, log_output_single_symbol);
    class_addanything(log_output_single, log_output_single_any);

    cls_owner.reset(log_output_single);

    return true;
}

//
// multi
//

static void* log_output_multi_new()
{
    auto x = reinterpret_cast<t_log_output_multi*>(pd_new(log_output_multi));
    x->msg_list = new std::vector<Message>();
    return static_cast<void*>(x);
}

static void log_output_multi_free(t_log_output_multi* x)
{
    delete x->msg_list;
}

static void log_output_multi_bang(t_log_output_multi* x)
{
    x->msg_list->push_back(Message::makeBang());
}

static void log_output_multi_float(t_log_output_multi* x, t_float f)
{
    x->msg_list->push_back(Message(f));
}

static void log_output_multi_symbol(t_log_output_multi* x, t_symbol* s)
{
    x->msg_list->push_back(Message(s));
}

static void log_output_multi_list(t_log_output_multi* x, t_symbol*, int argc, t_atom* argv)
{
    x->msg_list->push_back(Message(argc, argv));
}

static void log_output_multi_any(t_log_output_multi* x, t_symbol* s, int argc, t_atom* argv)
{
    x->msg_list->push_back(Message(s, argc, argv));
}

bool setup_log_output_multi()
{
    static std::unique_ptr<t_class, std::function<void(t_class*)>> cls_owner(
        nullptr,
        [](t_class* c) { free(c); });

    log_output_multi = class_new(gensym("log_output_multi"),
        reinterpret_cast<t_newmethod>(log_output_multi_new),
        reinterpret_cast<t_method>(log_output_multi_free),
        sizeof(t_log_output_multi), 0, A_NULL);
    class_addbang(log_output_multi, log_output_multi_bang);
    class_addfloat(log_output_multi, log_output_multi_float);
    class_addlist(log_output_multi, log_output_multi_list);
    class_addsymbol(log_output_multi, log_output_multi_symbol);
    class_addanything(log_output_multi, log_output_multi_any);

    cls_owner.reset(log_output_multi);
    return true;
}
