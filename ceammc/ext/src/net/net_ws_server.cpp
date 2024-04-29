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

#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "datatype_json.h"
#include "net_ws_server.h"

#include "fmt/core.h"

#include <thread>

using MutexLock = std::lock_guard<std::mutex>;
using namespace ceammc::ws;
using namespace ceammc::ws::srv_req;
using namespace ceammc::ws::srv_reply;

CEAMMC_DEFINE_HASH(fudi)
CEAMMC_DEFINE_HASH(data)
CEAMMC_DEFINE_HASH(sym)
CEAMMC_DEFINE_HASH(json)

CEAMMC_DEFINE_SYM(binary)
CEAMMC_DEFINE_SYM(closed)
CEAMMC_DEFINE_SYM(connected)
CEAMMC_DEFINE_SYM(id)
CEAMMC_DEFINE_SYM(latency)
CEAMMC_DEFINE_SYM(ping)
CEAMMC_DEFINE_SYM(pong)
CEAMMC_DEFINE_SYM(text)

static inline ceammc_ws_client_target make_target(const AtomListView& lv, AtomListView& args)
{
    auto sel = lv.symbolAt(0, &s_)->s_name;
    size_t id = lv.intAt(1, 0);

    switch (sel[0]) {
    case 'f':
        args = lv.subView(1);
        return ceammc_ws_client_target { ceammc_ws_client_selector::FIRST, 0 };
    case 'l':
        args = lv.subView(1);
        return ceammc_ws_client_target { ceammc_ws_client_selector::LAST, 0 };
    case 'i':
    case '=':
        args = lv.subView(2);
        return ceammc_ws_client_target { ceammc_ws_client_selector::ID, id };
    case 'e':
    case '!':
        args = lv.subView(2);
        return ceammc_ws_client_target { ceammc_ws_client_selector::EXCEPT, id };
    case 'a':
    case '*':
    default:
        args = lv.subView(1);
        return ceammc_ws_client_target { ceammc_ws_client_selector::ALL, 0 };
    }
}

ServerImpl::~ServerImpl()
{
    MutexLock lock(mtx_);
    if (srv_)
        ceammc_ws_server_free(srv_);
}

bool ServerImpl::listen(const Listen& msg)
{
    MutexLock lock(mtx_);
    if (srv_) {
        ceammc_ws_server_free(srv_);
        srv_ = nullptr;
    }

    auto addr = fmt::format("{}:{}", msg.addr, msg.port);
    srv_ = ceammc_ws_server_create(addr.c_str(),
        { this, on_error },
        { this, on_text },
        { this, on_bin },
        { this, on_ping },
        { this, on_conn },
        { this, on_disc });

    return srv_;
}

bool ServerImpl::stop()
{
    MutexLock lock(mtx_);
    if (check_running()) {
        ceammc_ws_server_free(srv_);
        srv_ = nullptr;
        return true;
    } else
        return false;
}

void ServerImpl::process(const CloseClients& msg)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_close_clients(srv_, msg.to);
}

void ServerImpl::process(const SendText& txt)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_send_text(srv_, txt.msg.c_str(), txt.to);
}

void ServerImpl::process(const SendBinary& bin)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_send_binary(srv_, bin.data.data(), bin.data.size(), bin.to);
}

void ServerImpl::process(const SendPing& ping)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_send_ping(srv_, ping.data.data(), ping.data.size(), ping.to);
}

void ServerImpl::process(const DumpClients&)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_connected_clients(srv_, this, on_client_dump);
}

void ServerImpl::process(const AbortClients& cli)
{
    MutexLock lock(mtx_);

    if (check_running())
        ceammc_ws_server_shutdown_clients(srv_, cli.to);
}

void ServerImpl::process_events()
{
    MutexLock lock(mtx_);
    if (!srv_)
        return;

    ceammc_ws_server_process_events(srv_);
}

void ServerImpl::on_error(void* user, const char* msg, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_err)
        this_->cb_err(msg, *info);
}

void ServerImpl::on_text(void* user, const char* msg, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_text)
        this_->cb_text(msg, *info);
}

void ServerImpl::on_bin(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_bin)
        this_->cb_bin(Bytes(data, data + len), *info);
}

void ServerImpl::on_ping(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_ping)
        this_->cb_ping(Bytes(data, data + len), *info);
}

void ServerImpl::on_conn(void* user, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_conn)
        this_->cb_conn(*info);
}

void ServerImpl::on_disc(void* user, const ceammc_ws_conn_info* info)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_close)
        this_->cb_close(*info);
}

void ServerImpl::on_client_dump(void* user, const ceammc_ws_conn_info* info, size_t len)
{
    if (!user)
        return;

    auto this_ = static_cast<ServerImpl*>(user);
    if (this_ && this_->cb_dump_clients) {
        IdList ids;
        ids.reserve(len);
        for (size_t i = 0; i < len; i++)
            ids.push_back({ info[i].id, info[i].addr });

        this_->cb_dump_clients(ids);
    }
}

bool ws::ServerImpl::check_running()
{
    if (!srv_) {
        if (cb_err)
            cb_err("server is not running", { "", 0 });

        return false;
    } else
        return true;
}

NetWsServer::NetWsServer(const PdArgs& args)
    : BaseWsServer(args)
{
    createOutlet();
    createOutlet();

    srv_.reset(new ServerImpl);
    // called from worker thread
    srv_->cb_err = [this](const char* msg, const ceammc_ws_conn_info& info) { workerThreadError(msg); };
    srv_->cb_text = [this](const char* msg, const ceammc_ws_conn_info& info) {
        addReply(MessageText { msg, info.addr, info.id });
    };
    srv_->cb_bin = [this](const Bytes& data, const ceammc_ws_conn_info& info) {
        addReply(MessageBinary { data, info.addr, info.id });
    };
    srv_->cb_ping = [this](const Bytes& data, const ceammc_ws_conn_info& info) {
        addReply(MessagePing { data, info.addr, info.id });
    };
    srv_->cb_conn = [this](const ceammc_ws_conn_info& info) {
        addReply(ClientConnected { info.addr, info.id });
    };
    srv_->cb_close = [this](const ceammc_ws_conn_info& info) {
        addReply(ClientClosed { info.addr, info.id });
    };
    srv_->cb_dump_clients = [this](const IdList& ids) {
        addReply(ConnectedClients { ids });
    };

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json });
    addProperty(mode_);
}

void NetWsServer::processRequest(const Request& req, ResultCallback cb)
{
    if (!srv_)
        return;

    if (req.type() == typeid(Listen)) {
        auto& x = boost::get<Listen>(req);
        if (srv_->listen(x))
            workerThreadDebug(fmt::format("websocket server started: {}:{}", x.addr, x.port));

    } else if (req.type() == typeid(Stop)) {
        if (srv_->stop())
            workerThreadDebug("websocket server stopped");
    } else if (process_request<SendText>(req)) {
    } else if (process_request<SendBinary>(req)) {
    } else if (process_request<SendPing>(req)) {
    } else if (process_request<DumpClients>(req)) {
    } else if (process_request<AbortClients>(req)) {
    } else if (process_request<CloseClients>(req)) {
    } else {
        workerThreadError(fmt::format("unknown request: {}", req.type().name()));
    }
}

void NetWsServer::processResult(const Reply& res)
{
    if (process_reply<MessageText>(res)) {
    } else if (process_reply<MessageBinary>(res)) {
    } else if (process_reply<MessagePing>(res)) {
    } else if (process_reply<ClientConnected>(res)) {
    } else if (process_reply<ClientClosed>(res)) {
    } else if (process_reply<ConnectedClients>(res)) {
    } else {
        OBJ_ERR << "unknown result type: " << res.type().name();
    }
}

void NetWsServer::processEvents()
{
    if (srv_)
        srv_->process_events();
}

void NetWsServer::m_close(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_EQ_0))
        return;

    AtomListView args;
    auto t = make_target(lv, args);
    addRequest(CloseClients { t });
}

void NetWsServer::m_ping(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_0))
        return;

    AtomListView args;
    auto target = make_target(lv, args);
    addRequest(SendPing { toBinary(args), target });
}

void NetWsServer::m_send(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);
    addRequest(SendText { to_string(args), target });
}

void NetWsServer::m_send_binary(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);
    addRequest(SendBinary { toBinary(args), target });
}

void NetWsServer::m_send_json(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);
    addRequest(SendText { toJson(args), target });
}

void NetWsServer::m_shutdown(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_EQ_0))
        return;

    AtomListView args;
    auto target = make_target(lv, args);
    addRequest(AbortClients { target });
}

void NetWsServer::m_listen(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PORT:i ADDR:s?");

    if (!chk.check(lv, this))
        return;

    auto port = lv.intAt(0, 0);
    auto addr = lv.symbolAt(1, gensym("0.0.0.0"))->s_name;

    if (port <= 0)
        addRequest(Stop {});
    else
        addRequest(Listen { addr, static_cast<std::uint16_t>(port) });
}

void NetWsServer::m_stop(t_symbol* s, const AtomListView& lv)
{
    addRequest(Stop {});
}

void NetWsServer::m_clients(t_symbol* s, const AtomListView& lv)
{
    addRequest(DumpClients {});
}

AtomList NetWsServer::fromBinary(const ws::Bytes& data)
{
    AtomList bytes;
    bytes.reserve(data.size());
    for (auto x : data)
        bytes.append(x);

    return bytes;
}

AtomList NetWsServer::fromBinary(const ws::IdList& data)
{
    AtomList bytes;
    bytes.reserve(data.size() * 2);
    for (auto& x : data) {
        bytes.append(x.first);
        bytes.append(gensym(x.second.c_str()));
    }

    return bytes;
}

ws::Bytes NetWsServer::toBinary(const AtomListView& lv)
{
    ws::Bytes data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    return data;
}

std::string NetWsServer::toJson(const AtomListView& lv)
{
    try {
        return json::to_json_string(lv);
    } catch (std::exception& e) {
        LIB_ERR << __FUNCTION__ << " error: " << e.what();
        return {};
    }
}

void NetWsServer::outputInfo(const std::string& from, size_t id)
{
    AtomArray<2> lst { id, gensym(from.c_str()) };
    listTo(1, lst.view());
}

void NetWsServer::processReply(const ws::srv_reply::MessageText& m)
{
    outputInfo(m.from, m.id);

    try {
        switch (crc32_hash(mode_->value())) {
        case hash_fudi: {
            anyTo(0, sym_text(), AtomList::parseString(m.msg.c_str()));
        } break;
        case hash_sym: {
            anyTo(0, sym_text(), gensym(m.msg.c_str()));
        } break;
        case hash_json: {
            auto j = nlohmann::json::parse(m.msg);
            if (j.is_array()) {
                AtomList lst;
                from_json(j, lst);
                anyTo(0, sym_text(), lst);
            } else {
                Atom obj;
                from_json(j, obj);
                anyTo(0, sym_text(), obj);
            }

        } break;
        case hash_data: {
            auto res = parseDataString(m.msg.c_str());
            if (res) {
                anyTo(0, sym_text(), res.result());
            } else {
                OBJ_ERR << "can't parse data: " << m.msg;
            }
        } break;
        default:
            break;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "text reply error: " << e.what();
    }
}

void NetWsServer::processReply(const ws::srv_reply::MessageBinary& bin)
{
    outputInfo(bin.from, bin.id);
    anyTo(0, sym_binary(), fromBinary(bin.data));
}

void NetWsServer::processReply(const ws::srv_reply::MessagePing& m)
{
    outputInfo(m.from, m.id);
    anyTo(0, sym_ping(), fromBinary(m.data));
}

void NetWsServer::processReply(const ws::srv_reply::ClientConnected& conn)
{
    outputInfo(conn.from, conn.id);
    anyTo(0, sym_connected(), AtomListView {});
}

void NetWsServer::processReply(const ws::srv_reply::ClientClosed& closed)
{
    outputInfo(closed.from, closed.id);
    anyTo(0, sym_closed(), AtomListView {});
}

void NetWsServer::processReply(const ws::srv_reply::ConnectedClients& cli)
{
    anyTo(0, sym_id(), fromBinary(cli.ids));
}

static inline bool streq(const char* s1, const char* s2)
{
    return std::strcmp(s1, s2) == 0;
}

bool NetWsServer::checkClientSelector(t_symbol* s, const AtomListView& lv, RequestArgs req)
{
    if (lv.empty() || !lv[0].isSymbol()) {
        METHOD_ERR(s) << fmt::format("usage: {} SEL(*|all|id|==|first|last|except|!=) ID? ARGS*", s->s_name);
        return false;
    }

    auto check_args = [](const AtomListView& lv, RequestArgs req) {
        switch (req) {
        case REQ_ARGS_EQ_0:
            return lv.size() == 0;
        case REQ_ARGS_GE_0:
            return lv.size() >= 0;
        case REQ_ARGS_GE_1:
            return lv.size() >= 1;
        default:
            return false;
        }
    };

    auto args_str = [](RequestArgs req) {
        switch (req) {
        case REQ_ARGS_GE_0:
            return "ARGS*";
        case REQ_ARGS_GE_1:
            return "ARGS+";
        case REQ_ARGS_EQ_0:
        default:
            return "";
        }
    };

    auto cli_sel = lv[0].asSymbol()->s_name;

    if (streq(cli_sel, "all") || streq(cli_sel, "*")) {
        if (!check_args(lv.subView(1), req)) {
            METHOD_ERR(s) << fmt::format("usage: {} {}", cli_sel, args_str(req));
            return false;
        }
    } else if (streq(cli_sel, "first")) {
        if (!check_args(lv.subView(1), req)) {
            METHOD_ERR(s) << fmt::format("usage: {} {}", cli_sel, args_str(req));
            return false;
        }
    } else if (streq(cli_sel, "last")) {
        if (!check_args(lv.subView(1), req)) {
            METHOD_ERR(s) << fmt::format("usage: {} {}", cli_sel, args_str(req));
            return false;
        }
    } else if (streq(cli_sel, "id") || streq(cli_sel, "==")) {
        if (!check_args(lv.subView(2), req)) {
            METHOD_ERR(s) << fmt::format("usage: {} ID {}", cli_sel, args_str(req));
            return false;
        }
    } else if (streq(cli_sel, "except") || streq(cli_sel, "!=")) {
        if (!check_args(lv.subView(2), req)) {
            METHOD_ERR(s) << fmt::format("usage: {} ID {}", cli_sel, args_str(req));
            return false;
        }
    } else {
        METHOD_ERR(s) << fmt::format("invalid client selector '{}', "
                                     "expected values are: *|all|id|==|first|last|except|!=",
            cli_sel);
        return false;
    }

    return true;
}

void setup_net_ws_server()
{
    ObjectFactory<NetWsServer> obj("net.ws.server");
    obj.addMethod("clients", &NetWsServer::m_clients);
    obj.addMethod("close", &NetWsServer::m_close);
    obj.addMethod("listen", &NetWsServer::m_listen);
    obj.addMethod("ping", &NetWsServer::m_ping);
    obj.addMethod("send", &NetWsServer::m_send);
    obj.addMethod("stop", &NetWsServer::m_stop);
    obj.addMethod("send_binary", &NetWsServer::m_send_binary);
    obj.addMethod("send_json", &NetWsServer::m_send_json);
    obj.addMethod("shutdown", &NetWsServer::m_shutdown);

    obj.setXletsInfo({ "input" }, { "client messages", "client info" });
}
#endif
