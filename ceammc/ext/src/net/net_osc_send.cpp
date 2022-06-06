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

#include <condition_variable>
#include <future>
#include <mutex>

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
    OscMessage m;
    SubscriberId id;
    uint16_t port;

    lo_message msg() const { return m.get(); }
};

namespace {

class ThreadLogger : public NotifiedObject {
    std::mutex mtx_;
    std::list<std::string> msg_;

public:
    ThreadLogger()
    {
        Dispatcher::instance().subscribe(this, id());
    }

    ~ThreadLogger()
    {
        Dispatcher::instance().unsubscribe(this);
    }

    SubscriberId id() const { return reinterpret_cast<SubscriberId>(this); }

    bool notify(NotifyEventType /*code*/) final
    {
        std::lock_guard<std::mutex> g(mtx_);
        while (!msg_.empty()) {
            LIB_ERR << msg_.front();
            msg_.pop_front();
        }

        return true;
    }

    void error(const std::string& msg)
    {
        {
            std::lock_guard<std::mutex> g(mtx_);
            msg_.push_back(fmt::format("[osc] [error] {}", msg));
        }

        Dispatcher::instance().send({ id(), NOTIFY_UPDATE });
    }
};

class OscSendWorker {
    using UniqueLock = std::unique_lock<std::mutex>;
    using Pipe = moodycamel::ReaderWriterQueue<NetOscSendOscTask, 64>;

    static bool launchSender(OscSendWorker* w, const std::atomic_bool& quit, std::condition_variable& notified, std::mutex& m)
    {
        ThreadLogger logger;

        while (!quit) {
            try {
                NetOscSendOscTask task;
                while (w->pipe_.try_dequeue(task)) {
                    if (quit)
                        return true;

                    lo_address addr = lo_address_new(task.host.c_str(), fmt::format("{:d}", task.port).c_str());
                    const auto rc = lo_send_message(addr, task.path.c_str(), task.msg());
                    if (rc == -1) {
                        auto url = lo_address_get_url(addr);
                        logger.error(fmt::format("{} - `{}`", lo_address_errstr(addr), url));
                        free(url);
                    }

                    lo_address_free(addr);
                }

                UniqueLock lock(m);
                // if a signal from the main thread occures here
                // we can miss this signal and have to wait 100ms or until next send
                // so we have to lock sending of signal notification to prevent
                // spourious delays in OSC sending
                notified.wait_for(lock, std::chrono::milliseconds(100));

            } catch (std::exception& e) {
                std::cerr << "exception: " << e.what();
            }
        }

        return true;
    }

    OscSendWorker()
        : pipe_(64)
        , quit_(false)
    {
        LIB_LOG << "launch OSC sender worker process";
        future_ = std::async(std::launch::async, launchSender, this, std::ref(quit_), std::ref(notify_), std::ref(mtx_));
    }

    ~OscSendWorker()
    {
        quit_ = true;
        future_.get();
    }

    Pipe pipe_;
    std::future<bool> future_;
    std::atomic_bool quit_;
    std::mutex mtx_;
    std::condition_variable notify_;

public:
    static OscSendWorker& instance()
    {
        static OscSendWorker w;
        return w;
    }

    bool add(const NetOscSendOscTask& task)
    {
        auto ok = pipe_.enqueue(task);
        if (ok) {
            // @see comments in worker function
            UniqueLock lock(mtx_);
            notify_.notify_one();
        }

        return ok;
    }
};

}

NetOscSend::NetOscSend(const PdArgs& args)
    : BaseObject(args)
    , host_(nullptr)
    , port_(nullptr)
{
    host_ = new SymbolProperty("@host", &s_);
    host_->setArgIndex(0);
    addProperty(host_);

    port_ = new IntProperty("@port", 0);
    port_->setArgIndex(1);
    addProperty(port_);

    createOutlet();
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

static inline char atom_arg_type(const Atom& a)
{
    switch (a.type()) {
    case Atom::FLOAT:
        return 'f';
    case Atom::SYMBOL:
        return 's';
    default:
        return '?';
    }
}

void NetOscSend::m_send_typed(t_symbol* s, const AtomListView& lv)
{
    const bool ok = (lv.size() == 1 && lv[0].isSymbol())
        || (lv.size() >= 3 && lv[0].isSymbol() && lv[1].isSymbol());

    if (!ok) {
        METHOD_ERR(s) << "usage: PATH [TYPES ARGS...]?";
        return;
    }

    NetOscSendOscTask task;
    auto path = lv[0].asT<t_symbol*>()->s_name;
    initTask(task, path);

    if (lv.size() >= 3) {
        const auto types = lv[1].asT<t_symbol*>()->s_name;
        const auto num_types = strlen(types);
        const auto num_args = lv.size() - 2;

        if (num_types != num_args) {
            METHOD_ERR(s) << fmt::format("number of types mismatch number of arguments: {}!={}", num_types, num_args);
            return;
        }

        for (size_t i = 0; i < num_types; i++) {
            const auto t = types[i];
            const auto& a = lv[i + 2];
            switch (t) {
            case LO_FLOAT:
                if (!a.isFloat()) {
                    METHOD_ERR(s) << fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                    break;
                } else
                    lo_message_add_float(task.msg(), a.asT<t_float>());
                break;
            case LO_DOUBLE:
                if (!a.isFloat()) {
                    METHOD_ERR(s) << fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                    break;
                } else
                    lo_message_add_double(task.msg(), a.asT<t_float>());
                break;
            case LO_INT32:
                if (!a.isFloat()) {
                    METHOD_ERR(s) << fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                    break;
                } else
                    lo_message_add_int32(task.msg(), a.asT<t_float>());
                break;
            case LO_INT64:
                if (!a.isFloat()) {
                    METHOD_ERR(s) << fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                    break;
                } else
                    lo_message_add_int64(task.msg(), a.asT<t_float>());
                break;
            case LO_STRING:
                if (!a.isSymbol())
                    lo_message_add_string(task.msg(), to_string(a).c_str());
                else
                    lo_message_add_string(task.msg(), a.asT<t_symbol*>()->s_name);
                break;
            case LO_SYMBOL:
                if (!a.isSymbol())
                    lo_message_add_symbol(task.msg(), to_string(a).c_str());
                else
                    lo_message_add_symbol(task.msg(), a.asT<t_symbol*>()->s_name);
                break;
            default:
                METHOD_ERR(s) << fmt::format("unknown argument type: '{}'", t);
                break;
            }
        }
    }

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "can't add task";
}

void NetOscSend::initTask(NetOscSendOscTask& task, const char* path)
{
    task.host = host_->value()->s_name;
    task.port = port_->value();
    task.path = path;
    task.id = reinterpret_cast<SubscriberId>(this);
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
    obj.addMethod("send_typed", &NetOscSend::m_send_typed);
}
