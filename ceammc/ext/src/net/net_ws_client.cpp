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
#include "datatype_json.h"
#ifndef WITH_WEBSOCKET
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetWsClient, 1, 1, "compiled without WebSocket support");
OBJECT_STUB_SETUP(NetWsClient, net_ws_client, "net.ws.client");
#else

#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "net_rust.h"
#include "net_ws_client.h"

#include "fmt/core.h"

CEAMMC_DEFINE_HASH(fudi)
CEAMMC_DEFINE_HASH(data)
CEAMMC_DEFINE_HASH(sym)
CEAMMC_DEFINE_HASH(json)

CEAMMC_DEFINE_SYM(binary)
CEAMMC_DEFINE_SYM(connected)
CEAMMC_DEFINE_SYM(ping)
CEAMMC_DEFINE_SYM(pong)
CEAMMC_DEFINE_SYM(text)

using namespace ceammc::ws::cli_reply;
using namespace ceammc::ws::cli_req;

using MutexLock = std::lock_guard<std::mutex>;

struct WsClientImpl {
    std::mutex mtx_;
    ceammc_ws_client* cli_ { nullptr };
    std::function<void(const char*)> cb_err, cb_text;
    std::function<void(const ws::Bytes&)> cb_ping, cb_pong, cb_bin;

    static void on_error(void* user, const char* msg)
    {
        auto this_ = static_cast<WsClientImpl*>(user);
        if (this_ && this_->cb_err)
            this_->cb_err(msg);
    }

    static void on_text(void* user, const char* msg)
    {
        auto this_ = static_cast<WsClientImpl*>(user);
        if (this_ && this_->cb_text)
            this_->cb_text(msg);
    }

    static void on_ping(void* user, const std::uint8_t* data, size_t len)
    {
        auto this_ = static_cast<WsClientImpl*>(user);
        if (this_ && this_->cb_ping)
            this_->cb_ping(ws::Bytes(data, data + len));
    }

    static void on_pong(void* user, const std::uint8_t* data, size_t len)
    {
        auto this_ = static_cast<WsClientImpl*>(user);
        if (this_ && this_->cb_pong)
            this_->cb_pong(ws::Bytes(data, data + len));
    }

    static void on_binary(void* user, const std::uint8_t* data, size_t len)
    {
        auto this_ = static_cast<WsClientImpl*>(user);
        if (this_ && this_->cb_bin)
            this_->cb_bin(ws::Bytes(data, data + len));
    }

    WsClientImpl()
    {
    }

    ~WsClientImpl()
    {
        disconnect();
    }

    // this is blocking function at this moment
    bool connect(const char* url)
    {
        MutexLock lock(mtx_);
        if (cli_) {
            ceammc_ws_client_free(cli_);
            cli_ = nullptr;
        }

        cli_ = ceammc_ws_client_create(url,
            { this, on_error },
            { this, on_text },
            { this, on_binary },
            { this, on_ping },
            { this, on_pong });

        return cli_ != nullptr;
    }

    void disconnect()
    {
        MutexLock lock(mtx_);
        if (cli_) {
            ceammc_ws_client_free(cli_);
            cli_ = nullptr;
        }
    }

    void send_text(const SendText& txt)
    {
        MutexLock lock(mtx_);
        if (cli_)
            ceammc_ws_client_send_text(cli_, txt.msg.c_str(), txt.flush);
    }

    void send_binary(const SendBinary& msg)
    {
        MutexLock lock(mtx_);
        if (cli_)
            ceammc_ws_client_send_binary(cli_, msg.data.data(), msg.data.size(), msg.flush);
    }

    void send_ping(const Ping& ping)
    {
        MutexLock lock(mtx_);
        if (cli_)
            ceammc_ws_client_send_ping(cli_, ping.data.data(), ping.data.size());
    }

    void close()
    {
        MutexLock lock(mtx_);
        if (cli_)
            ceammc_ws_client_close(cli_);
    }

    // non-blocking
    bool read()
    {
        MutexLock lock(mtx_);
        if (!cli_)
            return false;

        auto rc = ceammc_ws_client_read(cli_, ceammc_ws_trim::BOTH);
        return rc == ceammc_ws_rc::Ok;
    }

    void flush()
    {
        MutexLock lock(mtx_);
        if (cli_)
            ceammc_ws_client_flush(cli_);
    }
};

NetWsClient::NetWsClient(const PdArgs& args)
    : BaseWsClient(args)
{
    createOutlet();

    cli_.reset(new WsClientImpl);
    cli_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    cli_->cb_text = [this](const char* msg) { addReply(MessageText { msg }); };
    cli_->cb_bin = [this](const ws::Bytes& data) { addReply(MessageBinary { data }); };
    cli_->cb_ping = [this](const ws::Bytes& data) { addReply(MessagePing { data }); };
    cli_->cb_pong = [this](const ws::Bytes& data) { addReply(MessagePong { data }); };

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json });
    addProperty(mode_);
}

NetWsClient::~NetWsClient() = default; // for std::unique_ptr

void NetWsClient::processRequest(const Request& req, ResultCallback cb)
{
    if (!cli_)
        return;

    if (req.type() == typeid(Connect)) {
        if (cli_->connect(boost::get<Connect>(req).url.c_str()))
            addReply(Connected {});
    } else if (req.type() == typeid(SendText)) {
        cli_->send_text(boost::get<SendText>(req));
    } else if (req.type() == typeid(SendBinary)) {
        cli_->send_binary(boost::get<SendBinary>(req));
    } else if (req.type() == typeid(Close)) {
        cli_->close();
    } else if (req.type() == typeid(Flush)) {
        cli_->flush();
    } else if (req.type() == typeid(Ping)) {
        cli_->send_ping(boost::get<Ping>(req));
    } else {
        workerThreadError(fmt::format("unknown request: {}", req.type().name()));
    }
}

void NetWsClient::processResult(const Reply& res)
{
    if (res.type() == typeid(MessageText)) {
        processTextReply(boost::get<MessageText>(res));
    } else if (res.type() == typeid(MessagePong)) {
        auto& p = boost::get<MessagePong>(res);
        anyTo(0, sym_pong(), fromBinary(p.data));
    } else if (res.type() == typeid(MessagePing)) {
        auto& p = boost::get<MessagePing>(res);
        anyTo(0, sym_ping(), fromBinary(p.data));
    } else if (res.type() == typeid(MessageBinary)) {
        auto& bin = boost::get<MessageBinary>(res);
        anyTo(0, sym_binary(), fromBinary(bin.data));
    } else if (res.type() == typeid(Connected)) {
        anyTo(0, sym_connected(), AtomListView {});
    } else {
        OBJ_ERR << "unknown reply type: " << res.type().name();
    }
}

void NetWsClient::runLoopFor(size_t ms)
{
    if (cli_)
        cli_->read();

    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
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

void NetWsClient::processTextReply(const ws::cli_reply::MessageText& txt)
{
    try {
        switch (crc32_hash(mode_->value())) {
        case hash_fudi: {
            anyTo(0, sym_text(), AtomList::parseString(txt.msg.c_str()));
        } break;
        case hash_sym: {
            anyTo(0, sym_text(), gensym(txt.msg.c_str()));
        } break;
        case hash_json: {
            auto j = nlohmann::json::parse(txt.msg);
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
            auto res = parseDataString(txt.msg.c_str());
            if (res) {
                anyTo(0, sym_text(), res.result());
            } else {
                OBJ_ERR << "can't parse data: " << txt.msg;
            }
        } break;
        default:
            break;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "text reply error: " << e.what();
    }
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
    addRequest(Ping { toBinary(lv) });
}

void NetWsClient::m_flush(t_symbol* s, const AtomListView& lv)
{
    addRequest(Flush {});
}

ws::Bytes NetWsClient::toBinary(const AtomListView& lv)
{
    ws::Bytes data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    return data;
}

AtomList NetWsClient::fromBinary(const ws::Bytes& data)
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
    obj.addMethod("ping", &NetWsClient::m_ping);
    obj.addMethod("send", &NetWsClient::m_send_text);
    obj.addMethod("send_binary", &NetWsClient::m_send_binary);
    obj.addMethod("send_json", &NetWsClient::m_send_json);
    obj.addMethod("write", &NetWsClient::m_write_text);
    obj.addMethod("write_binary", &NetWsClient::m_write_binary);
    obj.addMethod("write_json", &NetWsClient::m_write_json);
}

#endif
