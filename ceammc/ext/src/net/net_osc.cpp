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
#include "net_osc.h"
#include "ceammc_factory.h"

#include <future>
#include <thread>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

struct NetOscSendOscTask {
    std::string host;
    std::string path;
    NetOscSend::MsgPipe* out;
    lo::Message msg;
    SubscriberId id;
    uint16_t port;
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

                    lo::Address addr(task.host, task.port);
                    const auto rc = addr.send(task.path, task.msg);
                    if (task.out) {
                        if (rc == -1)
                            task.out->try_enqueue({ NET_OSC_SEND_ERROR, addr.errstr() });
                        else
                            task.out->try_enqueue({ NET_OSC_SEND_OK, {} });
                    }

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
            task.msg.add_string(a.asSymbol()->s_name);
        else if (a.isFloat())
            task.msg.add_float(a.asFloat());
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
    task.msg.add_bool(lv[1].asT<bool>());

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

void setup_net_osc()
{
    LIB_DBG << "liblo version: " << lo::version();

    ObjectFactory<NetOscSend> obj("net.osc_send");
    obj.addMethod("send", &NetOscSend::m_send);
    obj.addMethod("send_bool", &NetOscSend::m_send_bool);
}
