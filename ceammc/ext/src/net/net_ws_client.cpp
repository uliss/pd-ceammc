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
CONTROL_OBJECT_STUB(NetWsClient, 1, 1, "compiled without WebSocket support");
OBJECT_STUB_SETUP(NetWsClient, net_ws_client, "net.ws.client");
#else

#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "datatype_json.h"
#include "net_ws_client.h"

#include "fmt/core.h"

CEAMMC_DEFINE_HASH(fudi)
CEAMMC_DEFINE_HASH(data)
CEAMMC_DEFINE_HASH(sym)
CEAMMC_DEFINE_HASH(json)

CEAMMC_DEFINE_SYM(binary)
CEAMMC_DEFINE_SYM(closed)
CEAMMC_DEFINE_SYM(connected)
CEAMMC_DEFINE_SYM(latency)
CEAMMC_DEFINE_SYM(ping)
CEAMMC_DEFINE_SYM(pong)
CEAMMC_DEFINE_SYM(text)

using namespace ceammc::ws::cli_reply;
using namespace ceammc::ws::cli_req;
using namespace ceammc::ws;

using MutexLock = std::lock_guard<std::mutex>;

void ClientImpl::on_error(void* user, const char* msg)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_err)
        this_->cb_err(msg);
}

void ClientImpl::on_text(void* user, const char* msg)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_text)
        this_->cb_text(msg);
}

void ClientImpl::on_ping(void* user, const std::uint8_t* data, size_t len)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_ping)
        this_->cb_ping(Bytes(data, data + len));
}

void ClientImpl::on_pong(void* user, const std::uint8_t* data, size_t len)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_pong)
        this_->cb_pong(Bytes(data, data + len));
}

void ClientImpl::on_binary(void* user, const std::uint8_t* data, size_t len)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_bin)
        this_->cb_bin(Bytes(data, data + len));
}

void ClientImpl::on_close(void* user, const std::uint8_t* data, size_t len)
{
    auto this_ = static_cast<ClientImpl*>(user);
    if (this_ && this_->cb_close)
        this_->cb_close(Bytes(data, data + len));
}

ClientImpl::~ClientImpl()
{
    disconnect();
}

bool ClientImpl::connect(const Connect& ct)
{
    MutexLock lock(mtx_);
    if (cli_) {
        ceammc_ws_client_free(cli_);
        cli_ = nullptr;
    }

    cli_ = ceammc_ws_client_create(ct.url.c_str(),
        { this, on_error },
        { this, on_text },
        { this, on_binary },
        { this, on_ping },
        { this, on_pong },
        { this, on_close });

    return cli_ != nullptr;
}

void ClientImpl::disconnect()
{
    MutexLock lock(mtx_);
    if (cli_) {
        ceammc_ws_client_free(cli_);
        cli_ = nullptr;
    }
}

void ClientImpl::process(const SendText& txt)
{
    MutexLock lock(mtx_);
    if (cli_)
        ceammc_ws_client_send_text(cli_, txt.msg.c_str(), txt.flush);
}

void ClientImpl::process(const SendBinary& msg)
{
    MutexLock lock(mtx_);
    if (cli_)
        ceammc_ws_client_send_binary(cli_, msg.data.data(), msg.data.size(), msg.flush);
}

void ClientImpl::process(const SendPing& ping)
{
    MutexLock lock(mtx_);
    if (cli_)
        ceammc_ws_client_send_ping(cli_, ping.data.data(), ping.data.size());
}

void ClientImpl::process(const Close&)
{
    MutexLock lock(mtx_);
    if (cli_)
        ceammc_ws_client_close(cli_);
}

void ClientImpl::process(const Flush&)
{
    MutexLock lock(mtx_);
    if (cli_)
        ceammc_ws_client_flush(cli_);
}

bool ClientImpl::process_events()
{
    MutexLock lock(mtx_);
    if (!cli_)
        return false;

    auto rc = ceammc_ws_client_read(cli_, ceammc_ws_trim::BOTH);
    return rc == ceammc_ws_rc::Ok;
}

NetWsClient::NetWsClient(const PdArgs& args)
    : BaseWsClient(args)
{
    createOutlet();

    cli_.reset(new ClientImpl);
    cli_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    cli_->cb_text = [this](const char* msg) { addReply(MessageText { msg }); };
    cli_->cb_bin = [this](const Bytes& data) { addReply(MessageBinary { data }); };
    cli_->cb_ping = [this](const Bytes& data) { addReply(MessagePing { data }); };
    cli_->cb_pong = [this](const Bytes& data) { addReply(MessagePong { data }); };
    cli_->cb_close = [this](const Bytes& data) { addReply(MessageClose { data }); };

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json });
    addProperty(mode_);
}

void NetWsClient::processRequest(const Request& req, ResultCallback cb)
{
    if (!cli_)
        return;

    if (req.type() == typeid(Connect)) {
        if (cli_->connect(boost::get<Connect>(req)))
            addReply(Connected {});
    } else if (process_request<SendText>(req)) {
    } else if (process_request<SendBinary>(req)) {
    } else if (process_request<SendPing>(req)) {
    } else if (process_request<Close>(req)) {
    } else if (process_request<Flush>(req)) {
    } else {
        workerThreadError(fmt::format("unknown request: {}", req.type().name()));
    }
}

void NetWsClient::processResult(const Reply& res)
{
    if (process_reply<MessageText>(res)) {
    } else if (process_reply<MessageBinary>(res)) {
    } else if (process_reply<MessagePing>(res)) {
    } else if (process_reply<MessagePong>(res)) {
    } else if (process_reply<MessageClose>(res)) {
    } else if (process_reply<Connected>(res)) {
    } else {
        OBJ_ERR << "unknown reply type: " << res.type().name();
    }
}

void NetWsClient::runLoopFor(size_t ms)
{
    if (cli_)
        cli_->process_events();
}

void NetWsClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    addRequest(Connect { to_string(lv) });
}

void NetWsClient::m_close(t_symbol* s, const AtomListView& lv)
{
    addRequest(Close {});
}

void NetWsClient::m_send_text(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendText { to_string(lv), true });
}

void NetWsClient::m_write_text(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendText { to_string(lv), false });
}

void NetWsClient::processReply(const MessageText& m)
{
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

void NetWsClient::processReply(const MessageBinary& m)
{
    anyTo(0, sym_binary(), fromBinary(m.data));
}

void NetWsClient::processReply(const MessagePing& m)
{
    anyTo(0, sym_ping(), fromBinary(m.data));
}

void NetWsClient::processReply(const MessagePong& m)
{
    anyTo(0, sym_pong(), fromBinary(m.data));

    if (latency_state_ == LATENCY_START) {
        outputLatency();
        latency_state_ = LATENCY_NONE;
    }
}

void NetWsClient::processReply(const MessageClose& m)
{
    anyTo(0, sym_closed(), AtomListView {});
}

void NetWsClient::processReply(const Connected& m)
{
    anyTo(0, sym_connected(), AtomListView {});
}

void NetWsClient::outputLatency()
{
    auto now = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::milliseconds>(now - latency_measure_begin_).count();
    anyTo(0, sym_latency(), us);
}

void NetWsClient::m_send_binary(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendBinary { toBinary(lv), true });
}

void NetWsClient::m_send_json(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendText { makeJson(lv), true });
}

void NetWsClient::m_write_binary(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendBinary { toBinary(lv), false });
}

void NetWsClient::m_write_json(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendText { makeJson(lv), false });
}

void NetWsClient::m_ping(t_symbol* s, const AtomListView& lv)
{
    addRequest(SendPing { toBinary(lv) });
}

void NetWsClient::m_flush(t_symbol* s, const AtomListView& lv)
{
    addRequest(Flush {});
}

void NetWsClient::m_latency(t_symbol* s, const AtomListView& lv)
{
    if (latency_state_ != LATENCY_NONE)
        return;

    latency_state_ = LATENCY_START;
    latency_measure_begin_ = std::chrono::steady_clock::now();
    addRequest(SendPing {});
}

Bytes NetWsClient::toBinary(const AtomListView& lv)
{
    Bytes data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    return data;
}

AtomList NetWsClient::fromBinary(const Bytes& data)
{
    AtomList lst;
    lst.reserve(data.size());
    for (auto x : data)
        lst.append(x);

    return lst;
}

std::string NetWsClient::makeJson(const AtomListView& lv)
{
    try {
        return json::to_json_string(lv);
    } catch (std::exception& e) {
        LIB_ERR << __FUNCTION__ << " error: " << e.what();
        return {};
    }
}

void setup_net_ws_client()
{
    ObjectFactory<NetWsClient> obj("net.ws.client");
    obj.addMethod("close", &NetWsClient::m_close);
    obj.addMethod("connect", &NetWsClient::m_connect);
    obj.addMethod("flush", &NetWsClient::m_flush);
    obj.addMethod("latency", &NetWsClient::m_latency);
    obj.addMethod("ping", &NetWsClient::m_ping);
    obj.addMethod("send", &NetWsClient::m_send_text);
    obj.addMethod("send_binary", &NetWsClient::m_send_binary);
    obj.addMethod("send_json", &NetWsClient::m_send_json);
    obj.addMethod("write", &NetWsClient::m_write_text);
    obj.addMethod("write_binary", &NetWsClient::m_write_binary);
    obj.addMethod("write_json", &NetWsClient::m_write_json);
}

#endif
