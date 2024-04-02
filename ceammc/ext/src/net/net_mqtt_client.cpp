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
#ifndef WITH_MQTT
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetMqtt, 1, 1, "compiled without mqtt support");
OBJECT_STUB_SETUP(NetMqtt, net_mqtt_client, "net.mqtt");
#else
#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "datatype_json.h"
#include "net_mqtt_client.h"

#include "fmt/core.h"
#include "json/json.hpp"

CEAMMC_DEFINE_HASH(fudi)
CEAMMC_DEFINE_HASH(data)
CEAMMC_DEFINE_HASH(sym)
CEAMMC_DEFINE_HASH(json)
CEAMMC_DEFINE_HASH(bytes)

using MutexLock = std::lock_guard<std::mutex>;
using namespace ceammc::mqtt;
using namespace ceammc::mqtt::reply;
using namespace ceammc::mqtt::req;

static ceammc_mqtt_qos qos2qos(int v)
{
    switch (v) {
    case 1:
        return ceammc_mqtt_qos::AtLeastOnce;
    case 2:
        return ceammc_mqtt_qos::ExactlyOnce;
    case 0:
    default:
        return ceammc_mqtt_qos::AtMostOnce;
    }
}

/*ClientImpl::~ClientImpl()
{
    if (cli_) // to prevent error message
        disconnect();
}

bool ClientImpl::connect(const req::Connect& m)
{
    MutexLock lock(mtx_);
    if (cli_) {
        ceammc_mqtt_client_free(cli_);
        cli_ = nullptr;
    }

    cli_ = ceammc_mqtt_client_create(
        m.url.c_str(),
        m.id.c_str(),
        {
            {
                this,
                [](void* user, const char* msg) {
                    auto this_ = static_cast<ClientImpl*>(user);
                    if (!this_)
                        return;

//                    this_->error(msg);
//                },
//            },
//            {
//                this,
//                [](void* user) {
//                    auto this_ = static_cast<ClientImpl*>(user);
//                    if (this_ && this_->cb_ping)
//                        this_->cb_ping();
//                },
//            },
//            {
//                this,
//                [](void* user, const char* topic, const std::uint8_t* data, size_t len) {
//                    auto this_ = static_cast<ClientImpl*>(user);
//                    if (this_ && this_->cb_pub)
//                        this_->cb_pub(topic, Data(data, data + len));
//                },
//            },
//        });

//    if (!cli_)
//        error(fmt::format("can't connect to '{}' with ID: '{}'", m.url, m.id));

//    return cli_ != nullptr;
//}
*/

// bool ClientImpl::disconnect()
//{
//     MutexLock lock(mtx_);
//     if (cli_) {
//         ceammc_mqtt_client_free(cli_);
//         cli_ = nullptr;
//         return true;
//     } else {
//         error("not connected");
//         return false;
//     }
// }

// void ClientImpl::process(const req::Subscribe& m)
//{
//     MutexLock lock(mtx_);
//     if (!check_connected())
//         return;

//    if (ceammc_mqtt_client_subscribe(cli_, m.topic->s_name) != ceammc_mqtt_rc::Ok)
//        error(fmt::format("can't subscribe to topic '{}'", m.topic->s_name));
//}

// void ClientImpl::process(const req::Unsubscribe& m)
//{
//     MutexLock lock(mtx_);
//     if (!check_connected())
//         return;

//    if (ceammc_mqtt_client_unsubscribe(cli_, m.topic->s_name) != ceammc_mqtt_rc::Ok)
//        error(fmt::format("can't unsubscribe from topic '{}'", m.topic->s_name));
//}

// void ClientImpl::process(const req::PublishText& m)
//{
//     MutexLock lock(mtx_);
//     if (!check_connected())
//         return;

//    if (ceammc_mqtt_client_publish(cli_, m.topic->s_name, m.msg.c_str(), m.qos, m.retain) != ceammc_mqtt_rc::Ok)
//        error(fmt::format("can't publish to topic '{}'", m.topic->s_name));
//}

// void ClientImpl::process(const req::PublishBytes& m)
//{
//     MutexLock lock(mtx_);
//     if (!check_connected())
//         return;

//    if (ceammc_mqtt_client_publish_data(cli_, m.topic->s_name, m.data.data(), m.data.size(), m.qos, m.retain) != ceammc_mqtt_rc::Ok)
//        error(fmt::format("can't publish to topic '{}'", m.topic->s_name));
//}

// void ClientImpl::process_events()
//{
//     MutexLock lock(mtx_);
//     if (!cli_)
//         return;

//    switch (ceammc_mqtt_process_events(cli_, 100)) {
//    case ceammc_mqtt_rc::Disconnected:
//    case ceammc_mqtt_rc::ConnectionError:
//        ceammc_mqtt_client_free(cli_);
//        cli_ = nullptr;
//        break;
//    default:
//        break;
//    }
//}

// bool ClientImpl::check_connected() const
//{
//     if (!cli_) {
//         error("not connected");
//         return false;
//     } else
//         return true;
// }

// void mqtt::ClientImpl::error(const char* msg) const
//{
//     if (cb_err)
//         cb_err(msg);
// }

int NetMqttClient::id_count_ = 0;

NetMqttClient::NetMqttClient(const PdArgs& args)
    : BaseMqttClient(args)
{
    id_ = new SymbolProperty("@id", gensym(fmt::format("pd_mqtt_client_{}", id_count_++).c_str()));
    addProperty(id_);

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json, str_bytes });
    addProperty(mode_);

    qos_ = new IntEnumProperty("@qos", { 0, 1, 2 });
    addProperty(qos_);

    retain_ = new BoolProperty("@retain", false);
    addProperty(retain_);

    //    cli_.reset(new ClientImpl);
    //    cli_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    //    cli_->cb_ping = [this]() { addReply(ReplyPing {}); };
    //    cli_->cb_pub = [this](const char* topic, const Data& data) {
    //        switch (crc32_hash(mode_->value())) {
    //        case hash_json: {
    //            try {
    //                ReplyText txt;
    //                txt.topic = topic;
    //                std::string msg(reinterpret_cast<const char*>(data.data()), data.size());
    //                addReply(ReplyText { topic, nlohmann::json::parse(msg).dump() });
    //            } catch (std::exception& e) {
    //                workerThreadError("json parse error");
    //            }
    //        } break;
    //        case hash_bytes: {
    //            addReply(ReplyBytes { topic, data });
    //        } break;
    //        case hash_sym:
    //        default: {
    //            addReply(ReplyText { topic, { reinterpret_cast<const char*>(data.data()), data.size() } });
    //        } break;
    //        };
    //    };

    createOutlet();
    createOutlet();
}

void NetMqttClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("URL:s");
    if (!chk.check(lv, this))
        return;

    cli_.reset(new MqttClient(
        ceammc_mqtt_client_init {
            lv.symbolAt(0, &s_)->s_name,
            id_->value()->s_name,
        },
        ceammc_mqtt_client_new,
        ceammc_mqtt_client_free,
        ceammc_mqtt_client_process,
        ceammc_mqtt_client_result_cb {
            this,
            [](void* user) {
                auto this_ = static_cast<NetMqttClient*>(user);
                if (this_)
                    this_->bangTo(1);
            },
            [](void* user, ceammc_mqtt_rc code) {
                auto this_ = static_cast<NetMqttClient*>(user);
                if (this_) {
                    switch (code) {
                    case ceammc_mqtt_rc::Success:
                        this_->floatTo(1, 1);
                        break;
                    case ceammc_mqtt_rc::RefusedProtocolVersion:
                        Error(this_) << "refused protocol version";
                        this_->floatTo(1, 0);
                        break;
                    case ceammc_mqtt_rc::BadClientId:
                        Error(this_) << "base client id";
                        this_->floatTo(1, 0);
                        break;
                    case ceammc_mqtt_rc::ServiceUnavailable:
                        Error(this_) << "service unavailable";
                        this_->floatTo(1, 0);
                        break;
                    case ceammc_mqtt_rc::BadUserNamePassword:
                        Error(this_) << "base username/password";
                        this_->floatTo(1, 0);
                        break;
                    case ceammc_mqtt_rc::NotAuthorized:
                        Error(this_) << "not authorized";
                        this_->floatTo(1, 0);
                        break;
                    default:
                        this_->floatTo(1, 0);
                        break;
                    }
                }
            },
            [](void* user, const char* topic, const uint8_t* data, size_t data_len, ceammc_mqtt_qos qos, bool retain, uint16_t pkid) {
                auto this_ = static_cast<NetMqttClient*>(user);
                if (this_)
                    this_->processReply(topic, data, data_len, qos, retain, pkid);
            } },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } })
        //
    );

    //    addRequest(Connect {
    //        id_->str(),
    //        lv.asSymbol()->s_name,
    //    });
}

void NetMqttClient::m_close(t_symbol* s, const AtomListView& lv)
{
    cli_.reset();
}

void NetMqttClient::m_subscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TOPIC:s ON:b?");
    if (!chk.check(lv, this))
        return;

    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto topic = lv[0].asT<t_symbol*>();
    if (lv.boolAt(1, true))
        ceammc_mqtt_client_subscribe(cli_->handle(), topic->s_name, ceammc_mqtt_qos::AtLeastOnce);
    else
        ceammc_mqtt_client_unsubscribe(cli_->handle(), topic->s_name);
}

void NetMqttClient::m_unsubscribe(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto topic = lv[0].asT<t_symbol*>()->s_name;
    ceammc_mqtt_client_unsubscribe(cli_->handle(), topic);
}

void NetMqttClient::m_publish(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TOPIC:s MSG:a+");
    if (!chk.check(lv, this))
        return;

    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    auto topic = lv[0].asT<t_symbol*>();
    auto payload = lv.subView(1);

    switch (crc32_hash(mode_->value())) {
    case hash_fudi:
    case hash_sym: {
        ceammc_mqtt_client_publish(cli_->handle(), topic->s_name, to_string(payload).c_str(), ceammc_mqtt_qos::AtLeastOnce, false);
    } break;
    case hash_json: {
        std::string msg;

        if (payload.isAtom() && !payload.isData())
            msg = json::to_json_string(payload[0]);
        else if (payload.isData())
            msg = payload[0].asData()->toJsonString();
        else
            msg = json::to_json_string(payload);

        ceammc_mqtt_client_publish(cli_->handle(), topic->s_name, msg.c_str(), ceammc_mqtt_qos::AtLeastOnce, false);
    } break;
    case hash_data: {
        auto res = parseDataList(payload);
        if (res) {
            auto msg = json::to_json_string(res.result());
            ceammc_mqtt_client_publish(cli_->handle(), topic->s_name, msg.c_str(), ceammc_mqtt_qos::AtLeastOnce, false);
        } else {
            METHOD_ERR(s) << "can't parse data: " << lv;
        }
    } break;
    case hash_bytes: {
        Data data;
        data.reserve(payload.size());
        for (auto& a : payload)
            data.push_back(a.asInt(0));

        ceammc_mqtt_client_publish_data(cli_->handle(), topic->s_name, data.data(), data.size(), ceammc_mqtt_qos::AtLeastOnce, false);
    } break;
    }
}

bool NetMqttClient::notify(int code)
{
    if (cli_)
        cli_->processResults();

    return true;
}

void NetMqttClient::processReply(const char* topic, const uint8_t* data, size_t data_len, ceammc_mqtt_qos qos, bool retain, uint16_t pkid)
{
    auto sym_topic = gensym(topic);

    switch (crc32_hash(mode_->value())) {
    case hash_fudi: {
        std::string text(reinterpret_cast<const char*>(data), data_len);
        anyTo(0, sym_topic, AtomList::parseString(text.c_str()));
    } break;
    case hash_json: {
        try {
            std::string text(reinterpret_cast<const char*>(data), data_len);
            auto j = nlohmann::json::parse(text);

            using js = nlohmann::json::value_t;
            switch (j.type()) {
            case js::array: {
                AtomList lst;
                from_json(j, lst);
                anyTo(0, sym_topic, lst);
            } break;
            default: {
                Atom a;
                from_json(j, a);
                anyTo(0, sym_topic, a);
            } break;
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    } break;
    case hash_data: {
        std::string text(reinterpret_cast<const char*>(data), data_len);
        auto res = parseDataString(text.c_str());
        if (res) {
            anyTo(0, sym_topic, res.result());
        } else {
            OBJ_ERR << "parse data error: " << res.err();
        }
    } break;
    case hash_sym: {
        std::string text(reinterpret_cast<const char*>(data), data_len);
        anyTo(0, sym_topic, Atom(gensym(text.c_str())));
    } break;
    case hash_bytes: {
        AtomList lst;
        lst.reserve(data_len);
        for (size_t i = 0; i < data_len; i++)
            lst.push_back(static_cast<int>(data[i]));

        anyTo(0, sym_topic, lst);
    } break;
    default:
        OBJ_ERR << "unknown mode: " << mode_->value()->s_name;
        break;
    }
}

// void NetMqttClient::processRequest(const Request& req, ResultCallback cb)
//{
//     if (!cli_) {
//         workerThreadError("can't create client");
//         return;
//     }

//    auto& type = req.type();

//    if (type == typeid(Connect)) {
//        if (cli_->connect(boost::get<Connect>(req)))
//            workerThreadDebug("mqtt client connected");
//    } else if (type == typeid(Disconnect)) {
//        if (cli_->disconnect())
//            workerThreadDebug("mqtt client disconnected");
//    } else if (process_request<Subscribe>(req)) {
//    } else if (process_request<Unsubscribe>(req)) {
//    } else if (process_request<PublishText>(req)) {
//    } else if (process_request<PublishBytes>(req)) {
//    } else {
//        workerThreadError(fmt::format("unknown request type: {}", type.name()));
//    }
//}

// void NetMqttClient::processResult(const Reply& res)
//{
//     if (process_reply<ReplyPing>(res)) {
//     } else if (process_reply<ReplyText>(res)) {
//     } else if (process_reply<ReplyBytes>(res)) {
//     } else {
//         OBJ_ERR << "unknown reply type: " << res.type().name();
//     }
// }

// void NetMqttClient::processEvents()
//{
//     if (cli_)
//         cli_->process_events();
// }

// void NetMqttClient::processReply(const mqtt::reply::ReplyPing& m)
//{
//     bangTo(1);
// }

// void NetMqttClient::processReply(const mqtt::reply::ReplyText& m)
//{
//     auto sym_topic = gensym(m.topic.c_str());

//    switch (crc32_hash(mode_->value())) {
//    case hash_fudi: {
//        anyTo(0, sym_topic, AtomList::parseString(m.text.c_str()));
//    } break;
//    case hash_json: {
//        try {
//            auto j = nlohmann::json::parse(m.text);

//            using js = nlohmann::json::value_t;
//            switch (j.type()) {
//            case js::array: {
//                AtomList lst;
//                from_json(j, lst);
//                anyTo(0, sym_topic, lst);
//            } break;
//            default: {
//                Atom a;
//                from_json(j, a);
//                anyTo(0, sym_topic, a);
//            } break;
//            }
//        } catch (std::exception& e) {
//            OBJ_ERR << e.what();
//        }
//    } break;
//    case hash_data: {
//        auto res = parseDataString(m.text.c_str());
//        if (res) {
//            anyTo(0, sym_topic, res.result());
//        } else {
//            OBJ_ERR << "parse data error: " << res.err();
//        }
//    } break;
//    case hash_sym:
//    default: {
//        anyTo(0, sym_topic, Atom(gensym(m.text.c_str())));
//    }
//    }
//}

// void NetMqttClient::processReply(const mqtt::reply::ReplyBytes& m)
//{
//     AtomList data;
//     data.reserve(m.data.size());
//     for (auto x : m.data)
//         data.push_back(static_cast<int>(x));

//    anyTo(0, gensym(m.topic.c_str()), data);
//}

void setup_net_mqtt_client()
{
    ObjectFactory<NetMqttClient> obj("net.mqtt.client");
    obj.addAlias("mqtt.client");
    obj.addMethod("connect", &NetMqttClient::m_connect);
    obj.addMethod("close", &NetMqttClient::m_close);
    obj.addMethod("subscribe", &NetMqttClient::m_subscribe);
    obj.addMethod("unsubscribe", &NetMqttClient::m_unsubscribe);
    obj.addMethod("publish", &NetMqttClient::m_publish);

    obj.setXletsInfo({ "input" }, { "publish messages", "bang on ping" });
}

#endif
