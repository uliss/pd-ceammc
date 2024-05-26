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
CONTROL_OBJECT_STUB(NetWsServer, 1, 2, "compiled without WebSocket support");
OBJECT_STUB_SETUP(NetWsServer, net_ws_server, "net.ws.server");
#else

#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "datatype_json.h"
#include "net_ws_server.h"

#include "fmt/core.h"

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

void NetWsServer::m_close(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_EQ_0))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_)
        ceammc_ws_server_close_clients(srv_->handle(), target);
}

void NetWsServer::m_ping(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_0))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_) {
        auto data = toBinary(args);
        ceammc_ws_server_send_ping(srv_->handle(), data.data(), data.size(), target);
    }
}

void NetWsServer::m_send(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_) {
        auto txt = to_string(args);
        ceammc_ws_server_send_text(srv_->handle(), txt.c_str(), target);
    }
}

void NetWsServer::m_send_binary(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_) {
        auto data = toBinary(args);
        ceammc_ws_server_send_binary(srv_->handle(), data.data(), data.size(), target);
    }
}

void NetWsServer::m_send_json(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_GE_1))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_) {
        auto txt = toJson(args);
        ceammc_ws_server_send_text(srv_->handle(), txt.c_str(), target);
    }
}

void NetWsServer::m_shutdown(t_symbol* s, const AtomListView& lv)
{
    if (!checkClientSelector(s, lv, REQ_ARGS_EQ_0))
        return;

    AtomListView args;
    auto target = make_target(lv, args);

    if (srv_)
        ceammc_ws_server_shutdown_clients(srv_->handle(), target);
}

bool NetWsServer::notify(int code)
{
    if (srv_)
        srv_->processResults();

    return true;
}

NetWsServer::NetWsServer(const PdArgs& args)
    : BaseWsServer(args)
{
    createOutlet();
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json });
    addProperty(mode_);
}

void NetWsServer::m_listen(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PORT:i ADDR:s?");

    if (!chk.check(lv, this))
        return;

    std::uint16_t port = lv.intInClosedIntervalAt(0, 0, 32000, 8000);
    auto addr = lv.symbolAt(1, gensym("0.0.0.0"))->s_name;

    if (port <= 0)
        srv_.release();
    else
        srv_.reset(new WsServerImpl(
            { addr, port },
            ceammc_ws_server_create,
            ceammc_ws_server_free,
            ceammc_ws_server_process_events,
            ceammc_ws_server_result_cb {
                this,
                // text
                [](void* user, const char* txt, const ceammc_ws_peer_info* peer) {
                    auto this_ = static_cast<NetWsServer*>(user);
                    if (this_)
                        this_->processText(txt, peer);
                },
                // binary
                [](void* user, const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer) {
                    auto this_ = static_cast<NetWsServer*>(user);
                    if (this_)
                        this_->processBinary(data, len, peer);
                },
                // ping
                [](void* user, const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer) {
                    auto this_ = static_cast<NetWsServer*>(user);
                    if (this_)
                        this_->processPing(data, len, peer);
                },
                // close?
                {},
                // connected
                [](void* user, bool connected, const ceammc_ws_peer_info* peer) {
                    auto this_ = static_cast<NetWsServer*>(user);
                    if (this_)
                        this_->processConnected(connected, peer);
                } //
            },
            { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));
}

void NetWsServer::m_stop(t_symbol* s, const AtomListView& lv)
{
    srv_.release();
}

void NetWsServer::m_clients(t_symbol* s, const AtomListView& lv)
{
    if (srv_) {
        //                ceammc_ws_server_connected_clients(srv_->handle(), data.data(), data.size(), target);
    }
}

AtomList NetWsServer::fromBinary(const std::uint8_t* data, size_t len)
{
    AtomList bytes;
    bytes.reserve(len);
    for (size_t i = 0; i < len; i++)
        bytes.append(data[i]);

    return bytes;
}

Bytes NetWsServer::toBinary(const AtomListView& lv)
{
    Bytes data;
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

void NetWsServer::outputInfo(const ceammc_ws_peer_info* peer)
{
    AtomArray<2> lst { peer->id, gensym(peer->addr) };
    listTo(1, lst.view());
}

void NetWsServer::processText(const char* msg, const ceammc_ws_peer_info* peer)
{
    outputInfo(peer);
    try {
        switch (crc32_hash(mode_->value())) {
        case hash_fudi: {
            anyTo(0, sym_text(), AtomList::parseString(msg));
        } break;
        case hash_sym: {
            anyTo(0, sym_text(), gensym(msg));
        } break;
        case hash_json: {
            auto j = nlohmann::json::parse(msg);
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
            auto res = parseDataString(msg);
            if (res) {
                anyTo(0, sym_text(), res.result());
            } else {
                OBJ_ERR << "can't parse data: " << msg;
            }
        } break;
        default:
            break;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "text reply error: " << e.what();
    }
}

void NetWsServer::processBinary(const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer)
{
    outputInfo(peer);
    anyTo(0, sym_binary(), fromBinary(data, len));
}

void NetWsServer::processPing(const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer)
{
    outputInfo(peer);
    anyTo(0, sym_ping(), fromBinary(data, len));
}

void NetWsServer::processConnected(bool connected, const ceammc_ws_peer_info* peer)
{
    outputInfo(peer);
    if (connected)
        anyTo(0, sym_connected(), AtomListView {});
    else
        anyTo(0, sym_closed(), AtomListView {});
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
