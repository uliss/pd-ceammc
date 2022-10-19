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
#include "ceammc_poll_dispatcher.h"
#include "ceammc_thread.h"
#include "fmt/format.h"
#include "lex/parser_numeric.h"

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>

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
    OscProto proto;

    lo_message msg() const { return m.get(); }
};

namespace {

class OscSendWorker {
    using Pipe = moodycamel::ReaderWriterQueue<NetOscSendOscTask, 64>;
    ThreadPdLogger logger_;

    OscSendWorker()
        : pipe_(64)
        , quit_(false)
    {
        LIB_LOG << "[osc_send] launch OSC sender worker process";

        future_ = std::async(
            std::launch::async,
            [this]() {
                lo_server lo_udp_srv = lo_server_new_with_proto(nullptr, LO_UDP, nullptr);
                if (!lo_udp_srv) {
                    logger_.error("[osc_send] can't create send UDP socket");
                    return false;
                }

                lo_server lo_tcp_srv = lo_server_new_with_proto(nullptr, LO_TCP, nullptr);
                if (!lo_tcp_srv) {
                    logger_.error("[osc_send] can't create send TCP socket");
                    return false;
                }

                while (!quit_) {
                    try {
                        NetOscSendOscTask task;
                        while (pipe_.try_dequeue(task)) {
                            if (quit_)
                                return true;

                            lo_address addr = nullptr;
                            int rc = -1;

                            switch (task.proto) {
                            case OSC_PROTO_UDP:
                                addr = lo_address_new_with_proto(LO_UDP, task.host.c_str(), fmt::format("{:d}", task.port).c_str());
                                rc = lo_send_message_from(addr, lo_udp_srv, task.path.c_str(), task.msg());
                                break;
                            case OSC_PROTO_TCP:
                                addr = lo_address_new_with_proto(LO_TCP, task.host.c_str(), fmt::format("{:d}", task.port).c_str());
                                rc = lo_send_message(addr, task.path.c_str(), task.msg());
                                break;
                            case OSC_PROTO_UNIX:
                                addr = lo_address_new_with_proto(LO_UNIX, nullptr, task.host.c_str());
                                rc = lo_send_message(addr, task.path.c_str(), task.msg());
                                break;
                            default:
                                logger_.error(fmt::format("[osc_send] unsupported OSC protocol: {}", task.proto));
                                break;
                            }

                            if (!addr || rc == -1) {
                                auto url = lo_address_get_url(addr);
                                logger_.error(fmt::format("[osc_send] {} - `{}`", lo_address_errstr(addr), url));
                                free(url);
                            }

                            lo_address_free(addr);
                        }

                        notify_.waitFor(100);

                    } catch (std::exception& e) {
                        std::cerr << "[osc_send]  exception: " << e.what();
                    }
                }

                if (lo_udp_srv)
                    lo_server_free(lo_udp_srv);

                if (lo_tcp_srv)
                    lo_server_free(lo_tcp_srv);

                return true;
            });
    }

    ~OscSendWorker()
    {
        quit_ = true;
        future_.get();
    }

    Pipe pipe_;
    std::future<bool> future_;
    std::atomic_bool quit_;
    ThreadNotify notify_;

public:
    static OscSendWorker& instance()
    {
        static OscSendWorker w;
        return w;
    }

    bool add(const NetOscSendOscTask& task)
    {
        auto ok = pipe_.enqueue(task);
        if (ok)
            notify_.notifyOne();

        return ok;
    }
};

}

NetOscSend::NetOscSend(const PdArgs& args)
    : BaseObject(args)
    , url_(nullptr)
{
    url_ = new net::OscUrlProperty("@url", &s_);
    url_->setArgIndex(0);
    url_->registerProps(this);
    addProperty(url_);

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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_string(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() >= 2 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "invalid args: OSC_PATH ATOMS expected";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    lo_message_add_string(task.msg(), to_string(lv.subView(1)).c_str());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
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
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_char(t_symbol* s, const AtomListView& lv)
{
    char ch = 0;

    if (lv.size() == 2
        && lv[0].isSymbol()
        && lv[1].isInteger()
        && lv[1].asFloat() >= 0
        && lv[1].asFloat() <= 255) {
        ch = lv.intAt(1, 0);
    } else if (lv.size() == 2
        && lv[0].isSymbol()
        && lv[1].isSymbol()) {
        auto str = lv.symbolAt(1, &s_)->s_name;
        if (strlen(str) != 1) {
            METHOD_ERR(s) << "expected string with length = 1, got: " << lv[1];
            return;
        }

        ch = str[0];
    } else {
        METHOD_ERR(s) << "usage: PATH CHAR_CODE|SYM";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);

    lo_message_add_char(task.msg(), ch);

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_midi(t_symbol* s, const AtomListView& lv)
{
    std::uint8_t midi[4];

    if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isSymbol()) {
        parser::NumericFullMatch p;
        if (!p.parse(lv[1].asSymbol()->s_name) || p.isHex()) {
            METHOD_ERR(s) << "hex value expected, got: " << lv[1];
            return;
        }

        const std::uint32_t v = p.result().i0;
        midi[0] = 0xFF & (v >> 24);
        midi[1] = 0xFF & (v >> 16);
        midi[2] = 0xFF & (v >> 8);
        midi[3] = 0xFF & v;
    } else if (lv.size() == 5
        && checkArg(lv[0], ARG_SYMBOL, 0)
        && checkArg(lv[1], ARG_BYTE, 1)
        && checkArg(lv[2], ARG_BYTE, 2)
        && checkArg(lv[3], ARG_BYTE, 3)
        && checkArg(lv[4], ARG_BYTE, 4)) {

        auto b0 = lv[1].asInt();
        auto b1 = lv[2].asInt();
        auto b2 = lv[3].asInt();
        auto b3 = lv[4].asInt();

        midi[0] = b0;
        midi[1] = b1;
        midi[2] = b2;
        midi[3] = b3;
    } else {
        METHOD_ERR(s) << "usage: PATH HEX|(BYTE1 BYTE2 BYTE3 BYTE4)";
        return;
    }

    NetOscSendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);

    lo_message_add_midi(task.msg(), midi);

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::initTask(NetOscSendOscTask& task, const char* path)
{
    task.port = url_->port();
    task.proto = url_->proto();
    task.host = (task.proto == OSC_PROTO_UNIX)
        ? url_->path()->s_name
        : url_->host()->s_name;
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
    obj.addMethod("send_typed", &NetOscSend::m_send_typed);
    obj.addMethod("send_bool", &NetOscSend::m_send_bool);
    obj.addMethod("send_i32", &NetOscSend::m_send_i32);
    obj.addMethod("send_i64", &NetOscSend::m_send_i64);
    obj.addMethod("send_float", &NetOscSend::m_send_float);
    obj.addMethod("send_double", &NetOscSend::m_send_double);
    obj.addMethod("send_null", &NetOscSend::m_send_null);
    obj.addMethod("send_inf", &NetOscSend::m_send_inf);
    obj.addMethod("send_string", &NetOscSend::m_send_string);
    obj.addMethod("send_midi", &NetOscSend::m_send_midi);
    obj.addMethod("send_char", &NetOscSend::m_send_char);
}
