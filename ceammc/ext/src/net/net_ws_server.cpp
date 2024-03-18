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

#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "net_rust.h"
#include "net_ws_server.h"

using MutexLock = std::lock_guard<std::mutex>;

struct WsServerImpl {
    std::mutex mtx_;
    ceammc_rs_ws_server* srv_ { nullptr };
    std::function<void(const char*)> cb_err, cb_text;

    ~WsServerImpl()
    {
        MutexLock lock(mtx_);
        if (srv_)
            ceammc_rs_ws_server_free(srv_);
    }

    void listen(const char* addr)
    {
        MutexLock lock(mtx_);
        if (srv_) {
            ceammc_rs_ws_server_free(srv_);
            srv_ = nullptr;
        }

        srv_ = ceammc_rs_ws_server_create(addr, { this, on_error });
    }

    void runloop_for()
    {
        MutexLock lock(mtx_);
        if (!srv_)
            return;

        ceammc_rs_ws_server_runloop(srv_);
    }

    static void on_error(void* user, const char* msg)
    {
        if (!user)
            return;

        auto this_ = static_cast<WsServerImpl*>(user);
        if (this_ && this_->cb_err)
            this_->cb_err(msg);
    }
};

NetWsServer::NetWsServer(const PdArgs& args)
    : BaseWsServer(args)
{
    createOutlet();
    srv_.reset(new WsServerImpl);
    srv_->cb_err = [this](const char* msg) { workerThreadError(msg); };
}

NetWsServer::~NetWsServer()
{
}

void NetWsServer::processRequest(const WsSrvRequest& req, ResultCallback cb)
{
    if (!srv_)
        return;

    if (req.type() == typeid(WsSrvListen)) {
        auto& x = boost::get<WsSrvListen>(req);
        srv_->listen(x.addr.c_str());
        workerThreadDebug(x.addr.c_str());
    }
}

void NetWsServer::processResult(const WsSrvReply& res)
{
}

void NetWsServer::runLoopFor(size_t ms)
{
    if (srv_)
        srv_->runloop_for();

    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void NetWsServer::m_listen(t_symbol* s, const AtomListView& lv)
{
    addRequest(WsSrvListen { to_string(lv) });
}

void setup_net_ws_server()
{
    ObjectFactory<NetWsServer> obj("net.ws.server");
    obj.addMethod("listen", &NetWsServer::m_listen);
}
#endif
