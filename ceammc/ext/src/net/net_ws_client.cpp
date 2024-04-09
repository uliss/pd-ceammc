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

#include "args/argcheck2.h"
#include "ceammc_array.h"
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

NetWsClient::NetWsClient(const PdArgs& args)
    : BaseWsClient(args)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json });
    addProperty(mode_);
}

bool NetWsClient::notify(int code)
{
    if (cli_)
        cli_->processResults();

    return true;
}

void NetWsClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s");
    if (!chk.check(lv, this))
        return;

    cli_.reset(new WsClientImpl(
        { lv.symbolAt(0, &s_)->s_name },
        ceammc_ws_client_new,
        ceammc_ws_client_free,
        ceammc_ws_client_process_events,
        ceammc_ws_client_result_cb {
            this,
            [](void* user, const char* txt) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processText(txt);
            },
            [](void* user, const std::uint8_t* data, size_t data_len) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processBinary(data, data_len);
            },
            [](void* user, const std::uint8_t* data, size_t data_len) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processPing(data, data_len);
            },
            [](void* user, const std::uint8_t* data, size_t data_len) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processPong(data, data_len);
            },
            [](void* user) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processClose();
            },
            [](void* user) {
                auto this_ = static_cast<NetWsClient*>(user);
                if (this_)
                    this_->processConnect();
            },
        },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
}

void NetWsClient::m_close(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    ceammc_ws_client_close(cli_->handle());
}

void NetWsClient::m_send_text(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    ceammc_ws_client_send_text(cli_->handle(), to_string(lv).c_str(), true);
}

void NetWsClient::m_write_text(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    ceammc_ws_client_send_text(cli_->handle(), to_string(lv).c_str(), false);
}

void NetWsClient::outputLatency()
{
    auto now = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::milliseconds>(now - latency_measure_begin_).count();
    anyTo(0, sym_latency(), us);
}

void NetWsClient::processText(const char* txt)
{
    try {
        switch (crc32_hash(mode_->value())) {
        case hash_fudi: {
            anyTo(0, sym_text(), AtomList::parseString(txt));
        } break;
        case hash_sym: {
            anyTo(0, sym_text(), gensym(txt));
        } break;
        case hash_json: {
            auto j = nlohmann::json::parse(txt);
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
            auto res = parseDataString(txt);
            if (res) {
                anyTo(0, sym_text(), res.result());
            } else {
                OBJ_ERR << "can't parse data: " << txt;
            }
        } break;
        default:
            break;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "text reply error: " << e.what();
    }
}

void NetWsClient::processBinary(const std::uint8_t* data, size_t data_len)
{
    AtomList res;
    res.reserve(data_len);
    for (size_t i = 0; i < data_len; i++)
        res.append(Atom(data[i]));

    anyTo(0, sym_binary(), res);
}

void NetWsClient::processClose()
{
    anyTo(0, sym_closed(), AtomListView {});
}

void NetWsClient::processConnect()
{
    anyTo(0, sym_connected(), AtomListView {});
}

void NetWsClient::processPing(const std::uint8_t* data, size_t data_len)
{
    AtomList res;
    res.reserve(data_len);
    for (size_t i = 0; i < data_len; i++)
        res.append(Atom(data[i]));

    anyTo(0, sym_ping(), res);
}

void NetWsClient::processPong(const std::uint8_t* data, size_t data_len)
{
    AtomList res;
    res.reserve(data_len);
    for (size_t i = 0; i < data_len; i++)
        res.append(Atom(data[i]));

    anyTo(0, sym_pong(), res);

    if (latency_state_ == LATENCY_START) {
        outputLatency();
        latency_state_ = LATENCY_NONE;
    }
}

void NetWsClient::m_send_binary(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    std::vector<std::uint8_t> data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    ceammc_ws_client_send_binary(cli_->handle(), data.data(), data.size(), true);
}

void NetWsClient::m_send_json(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto json = makeJson(lv);
    if (!json.empty())
        ceammc_ws_client_send_text(cli_->handle(), json.c_str(), true);
}

void NetWsClient::m_write_binary(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    std::vector<std::uint8_t> data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    ceammc_ws_client_send_binary(cli_->handle(), data.data(), data.size(), false);
}

void NetWsClient::m_write_json(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto json = makeJson(lv);
    if (!json.empty())
        ceammc_ws_client_send_text(cli_->handle(), json.c_str(), false);
}

void NetWsClient::m_ping(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    std::vector<std::uint8_t> data;
    data.reserve(lv.size());
    for (auto& a : lv)
        data.push_back(a.asInt());

    ceammc_ws_client_send_ping(cli_->handle(), data.data(), data.size());
}

void NetWsClient::m_send_array(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ARRAY:s START:i>=0? LEN:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto sym_array = lv.symbolAt(0, &s_);
    auto array = sym_array->s_name;
    auto start = lv.intAt(1, 0);
    auto len = lv.intAt(2, -1);

    Array arr(sym_array);
    if (!arr.isValid()) {
        METHOD_ERR(s) << fmt::format("array not found: '{}'", array);
        return;
    }

    if (start >= arr.size()) {
        METHOD_ERR(s) << fmt::format("invalid array position: {} (should be <{})", start, arr.size());
        return;
    }

    try {
        auto j = nlohmann::json::array();
        auto N = (len > 0) ? std::min<size_t>(arr.size(), start + len)
                           : arr.size();

        for (int i = start; i < N; i++)
            j += arr[i];

        ceammc_ws_client_send_text(cli_->handle(), j.dump().c_str(), true);
    } catch (std::exception& e) {
        METHOD_ERR(s) << fmt::format("error: {}", e.what());
    }
}

void NetWsClient::m_flush(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    ceammc_ws_client_flush(cli_->handle());
}

void NetWsClient::m_latency(t_symbol* s, const AtomListView& lv)
{
    if (latency_state_ != LATENCY_NONE)
        return;

    latency_state_ = LATENCY_START;
    latency_measure_begin_ = std::chrono::steady_clock::now();
    m_ping(s, lv);
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
    obj.addMethod("send_array", &NetWsClient::m_send_array);
    obj.addMethod("send_binary", &NetWsClient::m_send_binary);
    obj.addMethod("send_json", &NetWsClient::m_send_json);
    obj.addMethod("write", &NetWsClient::m_write_text);
    obj.addMethod("write_binary", &NetWsClient::m_write_binary);
    obj.addMethod("write_json", &NetWsClient::m_write_json);
}

#endif
