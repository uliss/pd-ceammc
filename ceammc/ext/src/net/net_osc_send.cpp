/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "net_osc_send.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>

#include <future>

#include <lo/lo.h>

class OscMessage {
    lo_message message;

public:
    OscMessage()
        : message(lo_message_new())
    {
        lo_message_incref(message);
    }

    OscMessage(const OscMessage& m) noexcept
        : message(m.message)
    {
        if (m.message)
            lo_message_incref(m.message);
    }

    ~OscMessage() noexcept
    {
        if (message)
            lo_message_free(message);
    }

    OscMessage& operator=(const OscMessage& m) noexcept
    {
        message = m.message;
        if (message)
            lo_message_incref(message);

        return *this;
    }

    lo_message get() const noexcept { return message; }
};

struct NetOscSendOscTask {
    std::string host;
    std::string path;
    NetOscSend::MsgPipe* out;
    OscMessage m;
    SubscriberId id;
    uint16_t port;

    lo_message msg() const { return m.get(); }
};

namespace {

class OscSendWorker {
    using Pipe = moodycamel::ReaderWriterQueue<NetOscSendOscTask>;

    static bool launchSender(OscSendWorker* w, bool& quit)
    {
        while (!quit) {
            NetOscSendOscTask task;
            try {
                while (w->pipe_.try_dequeue(task)) {
                    if (quit)
                        break;

                    lo_address addr = lo_address_new(task.host.c_str(), fmt::format("{:d}", task.port).c_str());
                    const auto rc = lo_send_message(addr, task.path.c_str(), task.msg());
                    if (task.out) {
                        if (rc == -1)
                            task.out->try_enqueue({ NET_OSC_SEND_ERROR, lo_address_errstr(addr) });
                        else
                            task.out->try_enqueue({ NET_OSC_SEND_OK, {} });
                    }
                    lo_address_free(addr);

                    Dispatcher::instance().send({ task.id, NOTIFY_UPDATE });
                }
            } catch (std::exception& e) {
                std::cerr << "exception: " << e.what();
            }
        }

        return true;
    }

    OscSendWorker()
        : quit_(false)
    {
        LIB_LOG << "launch OSC sender worker process";
        future_ = std::async(std::launch::async, launchSender, this, std::ref(quit_));
    }

    ~OscSendWorker()
    {
        quit_ = false;
        future_.wait();
    }

    Pipe pipe_;
    std::future<bool> future_;
    bool quit_;

public:
    static OscSendWorker& instance()
    {
        static OscSendWorker w;
        return w;
    }

    bool add(const NetOscSendOscTask& task)
    {
        return pipe_.try_enqueue(task);
    }
};

}

NetOscSend::NetOscSend(const PdArgs& args)
    : NetOscSendBase(args)
    , host_(nullptr)
    , port_(nullptr)
{
    Dispatcher::instance().subscribe(this, reinterpret_cast<SubscriberId>(this));

    host_ = new SymbolProperty("@host", &s_);
    host_->setArgIndex(0);
    addProperty(host_);

    port_ = new IntProperty("@port", 0);
    port_->setArgIndex(1);
    addProperty(port_);

    createOutlet();
}

NetOscSend::~NetOscSend()
{
    Dispatcher::instance().unsubscribe(this);
}

void NetOscSend::m_send(t_symbol* s, const AtomListView& lv)
{
    // set path
    auto path = lv.symbolAt(0, &s_);
    if (path == &s_) {
        METHOD_ERR(s) << "empty path. OSC_PATH ARGS?... expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, path->s_name);

    for (auto& a : lv.subView(1)) {
        if (a.isSymbol())
            lo_message_add_string(task.msg(), a.asSymbol()->s_name);
        else if (a.isFloat())
            lo_message_add_float(task.msg(), a.asFloat());
    }

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_bool(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_BOOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH BOOL expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    if (lv[1].asT<bool>())
        lo_message_add_true(task.msg());
    else
        lo_message_add_false(task.msg());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_i32(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_INT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH INT expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_int32(task.msg(), lv[1].asT<int>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_i64(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_INT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH INT expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_int64(task.msg(), lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_float(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH FLOAT expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_float(task.msg(), lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_double(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH DOUBLE expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_double(task.msg(), lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_null(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_nil(task.msg());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_inf(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_infinitum(task.msg());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::m_send_string(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() == 2 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "invalid args: OSC_PATH ATOM expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_string(task.msg(), to_string(lv[1]).c_str());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::processMessage(const NetOscSendMsg& msg)
{
    switch (msg.status) {
    case NET_OSC_SEND_OK:
        break;
    case NET_OSC_SEND_ERROR:
    default:
        OBJ_ERR << msg.error_msg;
        break;
    }
}

bool NetOscSend::notify(NotifyEventType /*code*/)
{
    NetOscSendMsg msg;
    while (msg_pipe_.try_dequeue(msg))
        processMessage(msg);

    return true;
}

void NetOscSend::initTask(NetOscSendOscTask& task, const char* path)
{
    task.host = host_->value()->s_name;
    task.port = port_->value();
    task.path = path;
    task.id = reinterpret_cast<SubscriberId>(this);
    task.out = &msg_pipe_;
}

void setup_net_osc_send()
{
    char str[16];
    lo_version(str, sizeof(str), 0, 0, 0, 0, 0, 0, 0);
    LIB_DBG << "liblo version: " << str;

    ObjectFactory<NetOscSend> obj("net.osc.send");
    obj.addAlias("net.osc.s");
    obj.addMethod("send", &NetOscSend::m_send);
    obj.addMethod("send_bool", &NetOscSend::m_send_bool);
    obj.addMethod("send_i32", &NetOscSend::m_send_i32);
    obj.addMethod("send_i64", &NetOscSend::m_send_i64);
    obj.addMethod("send_float", &NetOscSend::m_send_float);
    obj.addMethod("send_double", &NetOscSend::m_send_double);
    obj.addMethod("send_null", &NetOscSend::m_send_null);
    obj.addMethod("send_inf", &NetOscSend::m_send_inf);
    obj.addMethod("send_string", &NetOscSend::m_send_string);
}
