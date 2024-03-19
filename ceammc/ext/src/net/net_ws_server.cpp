/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef WITH_WEBSOCKET
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetWsServer, 1, 1, "compiled without WebSocket support");
OBJECT_STUB_SETUP(NetWsServer, net_ws_server, "net.ws.server");
#else

#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "net_rust.h"
#include "net_ws_server.h"

#include "fmt/core.h"

#include <thread>

using MutexLock = std::lock_guard<std::mutex>;
using namespace ceammc::ws;
using namespace ceammc::ws::srv_req;
using namespace ceammc::ws::srv_reply;

struct WsServerImpl {
    std::mutex mtx_;
    ceammc_ws_server* srv_ { nullptr };
    std::function<void(const char*, const ceammc_ws_conn_info&)> cb_err;
    std::function<void(const char*, const ceammc_ws_conn_info&)> cb_text;
    std::function<void(const ws::Bytes&, const ceammc_ws_conn_info&)> cb_bin, cb_ping, cb_pong;
    std::function<void(const ceammc_ws_conn_info&)> cb_conn, cb_close;

    ~WsServerImpl()
    {
        MutexLock lock(mtx_);
        if (srv_)
            ceammc_ws_server_free(srv_);
    }

    void listen(const char* addr)
    {
        MutexLock lock(mtx_);
        if (srv_) {
            ceammc_ws_server_free(srv_);
            srv_ = nullptr;
        }

        srv_ = ceammc_ws_server_create(addr,
            { this, on_error },
            { this, on_text },
            { this, on_bin },
            { this, on_ping },
            { this, on_conn },
            { this, on_disc });
    }

    void close()
    {
        MutexLock lock(mtx_);
        if (!srv_)
            return;

        ceammc_ws_server_close(srv_);
    }

    void send_text(const std::string& msg)
    {
        MutexLock lock(mtx_);
        if (!srv_)
            return;

        ceammc_ws_server_send_text(srv_, msg.c_str(), ceammc_ws_client_target::ALL);
    }

    void runloop_for()
    {
        MutexLock lock(mtx_);
        if (!srv_)
            return;

        ceammc_ws_server_runloop(srv_);
    }

    static void on_error(void* user, const char* msg, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_err)
            this_->cb_err(msg, *info);
    }

    static void on_text(void* user, const char* msg, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_text)
            this_->cb_text(msg, *info);
    }

    static void on_bin(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_bin)
            this_->cb_bin(Bytes(data, data + len), *info);
    }

    static void on_ping(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_ping)
            this_->cb_ping(Bytes(data, data + len), *info);
    }

    static void on_conn(void* user, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_conn)
            this_->cb_conn(*info);
    }

    static void on_disc(void* user, const ceammc_ws_conn_info* info)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_close)
            this_->cb_close(*info);
    }
};

NetWsServer::NetWsServer(const PdArgs& args)
    : BaseWsServer(args)
{
    createOutlet();
    createOutlet();

    srv_.reset(new WsServerImpl);
    srv_->cb_err = [this](const char* msg, const ceammc_ws_conn_info& info) { workerThreadError(msg); };
    srv_->cb_text = [this](const char* msg, const ceammc_ws_conn_info& info) {
        addReply(MessageText { msg, info.addr, info.id });
    };
    srv_->cb_bin = [this](const Bytes& data, const ceammc_ws_conn_info& info) {
        addReply(MessageBinary { data, info.addr, info.id });
    };
    srv_->cb_ping = [this](const Bytes& data, const ceammc_ws_conn_info& info) {
        workerThreadDebug(fmt::format("ping from {} ({})", info.addr, info.id).c_str());
    };
    srv_->cb_conn = [this](const ceammc_ws_conn_info& info) {
        addReply(ClientConnected { info.addr, info.id });
    };
    srv_->cb_close = [this](const ceammc_ws_conn_info& info) {
        addReply(ClientClosed { info.addr, info.id });
    };
}

NetWsServer::~NetWsServer()
{
}

void NetWsServer::processRequest(const ws::Request& req, ResultCallback cb)
{
    if (!srv_)
        return;

    if (req.type() == typeid(Listen)) {
        auto& x = boost::get<Listen>(req);
        srv_->listen(x.addr.c_str());
    } else if (req.type() == typeid(Close)) {
        srv_->close();
    } else if (req.type() == typeid(SendText)) {
        auto& txt = boost::get<SendText>(req);
        srv_->send_text(txt.msg);
    } else {
        OBJ_ERR << "unknown request type: " << req.type().name();
    }
}

void NetWsServer::processResult(const ws::Reply& res)
{
    if (res.type() == typeid(MessageText)) {
        auto& txt = boost::get<MessageText>(res);
        outputInfo(txt.from, txt.id);
        anyTo(0, gensym("text"), AtomList::parseString(txt.msg.c_str()));
    } else if (res.type() == typeid(ClientConnected)) {
        auto& conn = boost::get<ClientConnected>(res);
        outputInfo(conn.from, conn.id);
        anyTo(0, gensym("connect"), AtomListView {});
    } else if (res.type() == typeid(ClientClosed)) {
        auto& conn = boost::get<ClientClosed>(res);
        outputInfo(conn.from, conn.id);
        anyTo(0, gensym("close"), AtomListView {});
    } else if (res.type() == typeid(MessageBinary)) {
        auto& bin = boost::get<MessageBinary>(res);
        outputInfo(bin.from, bin.id);

        AtomList bytes;
        bytes.reserve(bin.data.size());
        for (auto x : bin.data)
            bytes.append(x);

        anyTo(0, gensym("binary"), bytes);
    } else {
        OBJ_ERR << "unknown result type: " << res.type().name();
    }
}

void NetWsServer::runLoopFor(size_t ms)
{
    if (srv_)
        srv_->runloop_for();
}

void NetWsServer::m_close(t_symbol* s, const AtomListView& lv)
{
    addRequest(Close {});
}

void NetWsServer::m_send(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendText { to_string(lv) });
}

void NetWsServer::m_listen(t_symbol* s, const AtomListView& lv)
{
    addRequest(Listen { to_string(lv) });
}

void NetWsServer::outputInfo(const std::string& from, size_t id)
{
    AtomArray<2> lst { gensym(from.c_str()), id };
    listTo(1, lst.view());
}

void setup_net_ws_server()
{
    ObjectFactory<NetWsServer> obj("net.ws.server");
    obj.addMethod("listen", &NetWsServer::m_listen);
    obj.addMethod("send", &NetWsServer::m_send);
    obj.addMethod("close", &NetWsServer::m_close);
}
#endif
