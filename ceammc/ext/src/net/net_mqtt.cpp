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
#include "ceammc_json.h"
#ifndef WITH_MQTT
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetMqtt, 1, 1, "compiled without mqtt support");
OBJECT_STUB_SETUP(NetMqtt, net_mqtt_client, "net.mqtt");
#else
#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_json.h"
#include "net_mqtt.h"
#include "net_rust.h"

#include "fmt/core.h"
#include "json/json.hpp"

CEAMMC_DEFINE_HASH(fudi)
CEAMMC_DEFINE_HASH(data)
CEAMMC_DEFINE_HASH(sym)
CEAMMC_DEFINE_HASH(json)
CEAMMC_DEFINE_HASH(bytes)
CEAMMC_DEFINE_SYM(publish)
CEAMMC_DEFINE_SYM(ping)

using MutexLock = std::lock_guard<std::mutex>;

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

MqttRequest MqttRequest::connect()
{
    MqttRequest req;
    req.type = CONNECT;
    return req;
}

MqttRequest MqttRequest::disconnect()
{
    MqttRequest req;
    req.type = DISCONNECT;
    return req;
}

MqttRequest MqttRequest::subscribe(t_symbol* topic)
{
    MqttRequest req;
    req.type = SUBSCRIBE;
    req.topic = topic;
    return req;
}

MqttRequest MqttRequest::unsubscribe(t_symbol* topic)
{
    MqttRequest req;
    req.type = UNSUBSCRIBE;
    req.topic = topic;
    return req;
}

MqttRequest MqttRequest::publish(t_symbol* topic, const std::string& msg)
{
    MqttRequest req;
    req.type = PUBLISH;
    req.topic = topic;
    req.msg = msg;
    return req;
}

MqttRequest MqttRequest::publish(t_symbol* topic, std::string&& msg)
{
    MqttRequest req;
    req.type = PUBLISH;
    req.topic = topic;
    req.msg = std::move(msg);
    return req;
}

MqttRequest MqttRequest::publish(t_symbol* topic, MqttData&& data)
{
    MqttRequest req;
    req.type = PUBLISH;
    req.topic = topic;
    req.msg = std::move(data);
    return req;
}

MqttRequest::MqttRequest()
    : topic(&s_)
    , msg("")
{
}

class MqttClient {
    ceammc_mqtt_client* cli_ { nullptr };
    std::mutex mtx_;

public:
    MqttClient() = default;

    ~MqttClient()
    {
        disconnect();
    }

    void disconnect()
    {
        MutexLock lock(mtx_);

        if (cli_) {
            ceammc_mqtt_client_free(cli_);
            cli_ = nullptr;
        }
    }

    bool connect(const char* id,
        const char* host,
        const char* user = nullptr,
        const char* password = nullptr,
        int port = 1883)
    {
        MutexLock lock(mtx_);

        if (cli_) {
            ceammc_mqtt_client_free(cli_);
            cli_ = nullptr;
        }

        cli_ = ceammc_mqtt_client_create(host, port, id, user, password);
        return cli_ != nullptr;
    }

    ceammc_mqtt_rc subscribe(const char* topic)
    {
        MutexLock lock(mtx_);

        if (!cli_)
            return ceammc_mqtt_rc::InvalidClient;

        return ceammc_mqtt_client_subscribe(cli_, topic);
    }

    ceammc_mqtt_rc unsubscribe(const char* topic)
    {
        MutexLock lock(mtx_);

        if (!cli_)
            return ceammc_mqtt_rc::InvalidClient;

        return ceammc_mqtt_client_unsubscribe(cli_, topic);
    }

    ceammc_mqtt_rc publish(const char* topic, const char* message, ceammc_mqtt_qos qos, bool retain)
    {
        MutexLock lock(mtx_);

        if (!cli_)
            return ceammc_mqtt_rc::InvalidClient;

        return ceammc_mqtt_client_publish(cli_, topic, message, qos, retain);
    }

    ceammc_mqtt_rc publish(const char* topic, const MqttData& data, ceammc_mqtt_qos qos, bool retain)
    {
        MutexLock lock(mtx_);

        if (!cli_)
            return ceammc_mqtt_rc::InvalidClient;

        return ceammc_mqtt_client_publish_data(cli_, topic, data.data(), data.size(), qos, retain);
    }

    ceammc_mqtt_rc process_events(void* user)
    {
        MutexLock lock(mtx_);

        if (!cli_)
            return ceammc_mqtt_rc::InvalidClient;

        return ceammc_mqtt_process_events(
            cli_, 100, user,
            [](void* user) {
                auto obj = static_cast<NetMqttClient*>(user);
                obj->addReply(MqttReplyPing {});
            },
            [](void* user, const char* topic, const uint8_t* data, size_t len) {
                auto obj = static_cast<NetMqttClient*>(user);
                obj->onWorkerPub(topic, data, len);
            },
            [](void* user, ceammc_mqtt_rc rc) {
                auto obj = static_cast<NetMqttClient*>(user);
                obj->onWorkerConn(static_cast<int>(rc));
            });
    }
};

int NetMqttClient::id_count_ = 0;

NetMqttClient::NetMqttClient(const PdArgs& args)
    : BaseMqtt(args)
{
    id_ = new SymbolProperty("@id", &s_);
    addProperty(id_);

    id_count_++;

    host_ = new SymbolProperty("@host", gensym("localhost"));
    host_->setArgIndex(0);
    addProperty(host_);

    user_ = new SymbolProperty("@user", &s_);
    addProperty(user_);

    pass_ = new SymbolProperty("@pass", &s_);
    addProperty(pass_);

    port_ = new IntProperty("@port", 1883);
    port_->checkClosedRange(1, std::numeric_limits<std::uint16_t>::max());
    port_->setArgIndex(1);
    addProperty(port_);

    mode_ = new SymbolEnumProperty("@mode", { str_fudi, str_data, str_sym, str_json, str_bytes });
    addProperty(mode_);

    qos_ = new IntEnumProperty("@qos", { 0, 1, 2 });
    addProperty(qos_);

    retain_ = new BoolProperty("@retain", false);
    addProperty(retain_);

    cli_.reset(new MqttClient);

    createOutlet();
}

NetMqttClient::~NetMqttClient()
{
}

void NetMqttClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    addRequest(MqttRequest::connect());
}

void NetMqttClient::m_disconnect(t_symbol* s, const AtomListView& lv)
{
    addRequest(MqttRequest::disconnect());
}

void NetMqttClient::m_subscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TOPIC:s ON:b?");

    if (!chk.check(lv, this))
        return;

    auto topic = lv[0].asT<t_symbol*>();
    if (lv.boolAt(1, true))
        addRequest(MqttRequest::subscribe(topic));
    else
        addRequest(MqttRequest::unsubscribe(topic));
}

void NetMqttClient::m_unsubscribe(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    auto topic = lv[0].asT<t_symbol*>();
    addRequest(MqttRequest::unsubscribe(topic));
}

void NetMqttClient::m_publish(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TOPIC:s MSG:a+");

    if (!chk.check(lv, this))
        return;

    auto topic = lv[0].asT<t_symbol*>();
    auto payload = lv.subView(1);

    switch (crc32_hash(mode_->value())) {
    case hash_fudi:
    case hash_sym: {
        auto msg = to_string(payload);
        addRequest(MqttRequest::publish(topic, std::move(msg)));
    } break;
    case hash_json: {
        std::string msg;

        if (payload.isAtom() && !payload.isData())
            msg = json::to_json_string(payload[0]);
        else if (payload.isData())
            msg = payload[0].asData()->toJsonString();
        else
            msg = json::to_json_string(payload);

        addRequest(MqttRequest::publish(topic, std::move(msg)));
    } break;
    case hash_data: {
        auto res = parseDataList(payload);
        if (res) {
            auto msg = json::to_json_string(res.result());
            addRequest(MqttRequest::publish(topic, std::move(msg)));
        } else {
            METHOD_ERR(s) << "can't parse data: " << lv;
        }
    } break;
    case hash_bytes: {
        std::vector<std::uint8_t> data;
        data.reserve(payload.size());
        for (auto& a : payload)
            data.push_back(a.asInt(0));

        addRequest(MqttRequest::publish(topic, std::move(data)));
    } break;
    }
}

void NetMqttClient::processRequest(const MqttRequest& req, ResultCallback cb)
{
    if (!cli_) {
        workerThreadError("can't create client");
        return;
    }

    switch (req.type) {
    case MqttRequest::CONNECT: {
        auto id = clientId();
        auto host = hostname();
        auto port = port_->value();
        if (!cli_->connect(id, host, user_->value()->s_name, pass_->value()->s_name, port))
            workerThreadError(fmt::format("can't connect to '{}:{}' with ID: '{}'",
                host, port, id));

    } break;
    case MqttRequest::DISCONNECT: {
        cli_->disconnect();
    } break;
    case MqttRequest::SUBSCRIBE: {
        if (cli_->subscribe(req.topic->s_name) != ceammc_mqtt_rc::Ok)
            workerThreadError(fmt::format("can't subscribe to topic '{}'", req.topic->s_name));
    } break;
    case MqttRequest::UNSUBSCRIBE: {
        if (cli_->unsubscribe(req.topic->s_name) != ceammc_mqtt_rc::Ok)
            workerThreadError(fmt::format("can't unsubscribe from topic '{}'", req.topic->s_name));
    } break;
    case MqttRequest::PUBLISH: {
        auto qos = qos2qos(qos_->value());

        if (req.msg.type() == typeid(std::string)) {
            auto& str = boost::get<std::string>(req.msg);
            if (cli_->publish(req.topic->s_name, str.c_str(), qos, retain_->value()) != ceammc_mqtt_rc::Ok)
                workerThreadError(fmt::format("can't publish '{}' to topic '{}'", str, req.topic->s_name));
            else
                workerThreadDebug(fmt::format("publish '{}' to topic '{}'", str, req.topic->s_name));
        } else if (req.msg.type() == typeid(MqttData)) {
            auto& data = boost::get<MqttData>(req.msg);
            if (cli_->publish(req.topic->s_name, data, qos, retain_->value()) != ceammc_mqtt_rc::Ok)
                workerThreadError(fmt::format("can't publish data to topic '{}'", req.topic->s_name));
            else
                workerThreadDebug(fmt::format("publish data to topic '{}'", req.topic->s_name));
        }

    } break;
    default:
        break;
    }
}

void NetMqttClient::processResult(const MqttReply& res)
{
    if (res.type() == typeid(MqttReplyPing)) {
        anyTo(0, sym_ping(), AtomListView {});
    } else if (res.type() == typeid(MqttReplyTxt)) {
        auto& txt = boost::get<MqttReplyTxt>(res);
        auto sym_topic = gensym(txt.topic.c_str());
        switch (crc32_hash(mode_->value())) {
        case hash_fudi: {
            AtomList res(sym_topic);
            res.append(AtomList::parseString(txt.text.c_str()));
            anyTo(0, sym_publish(), res);
        } break;
        case hash_json: {
            try {
                auto j = nlohmann::json::parse(txt.text);

                using js = nlohmann::json::value_t;
                switch (j.type()) {
                case js::array: {
                    AtomList lst;
                    from_json(j, lst);

                    AtomList data;
                    data.append(sym_topic);
                    data.append(lst);
                    anyTo(0, sym_publish(), data.view());
                } break;
                default: {
                    Atom a;
                    from_json(j, a);

                    AtomArray<2> data { sym_topic, a };
                    anyTo(0, sym_publish(), data.view());
                } break;
                }
            } catch (std::exception& e) {
                OBJ_ERR << e.what();
            }
        } break;
        case hash_data: {
            auto res = parseDataString(txt.text.c_str());
            if (res) {
                AtomList data(sym_topic);
                data.append(res.result());
                anyTo(0, sym_publish(), data);
            } else {
                OBJ_ERR << "parse data error: " << res.err();
            }
        } break;
        case hash_sym:
        default: {
            AtomArray<2> data { sym_topic, gensym(txt.text.c_str()) };
            anyTo(0, sym_publish(), data.view());
        }
        }
    } else if (res.type() == typeid(MqttReplyBytes)) {
        auto& bytes = boost::get<MqttReplyBytes>(res);
        AtomList data;
        data.reserve(bytes.data.size());
        for (auto x : bytes.data)
            data.push_back(static_cast<int>(x));

        anyTo(0, sym_publish(), data);
    } else {
        OBJ_ERR << "unknown reply type";
    }
}

void NetMqttClient::processEvents()
{
    if (cli_) {
        auto rc = cli_->process_events(this);
        switch (rc) {
        case ceammc_mqtt_rc::RefusedProtocolVersion:
            break;
        case ceammc_mqtt_rc::BadClientId:
            break;
        case ceammc_mqtt_rc::ServiceUnavailable:
            break;
        case ceammc_mqtt_rc::BadUserNamePassword:
            break;
        case ceammc_mqtt_rc::NotAuthorized:
            break;
        case ceammc_mqtt_rc::InvalidString:
            break;
        case ceammc_mqtt_rc::InvalidClient:
            break;
        case ceammc_mqtt_rc::ClientError:
            break;
        case ceammc_mqtt_rc::Disconnected:
            workerThreadError("disconnected");
            break;
        case ceammc_mqtt_rc::NetworkTimeout:
            workerThreadError("connection timeout");
            break;
        case ceammc_mqtt_rc::FlushTimeout:
            break;
        case ceammc_mqtt_rc::ConnectionRefused:
            workerThreadError("connection refused");
            break;
        case ceammc_mqtt_rc::ConnectionReset:
            workerThreadError("connection reset");
            break;
        case ceammc_mqtt_rc::ConnectionError:
            workerThreadError("connection error");
            break;
        default:
            break;
        }
    }
}

const char* NetMqttClient::clientId() const
{
    return ((id_->value() == &s_) ? gensym(fmt::format("pd_mqtt_{}", id_count_).c_str()) : id_->value())->s_name;
}

const char* NetMqttClient::hostname() const
{
    return (host_->value() == &s_) ? "localhost" : host_->value()->s_name;
}

void NetMqttClient::onWorkerPub(const char* topic, const std::uint8_t* data, size_t len)
{
    switch (crc32_hash(mode_->value())) {
    case hash_json: {
        try {
            MqttReplyTxt txt;
            txt.topic = topic;
            std::string msg(reinterpret_cast<const char*>(data), len);
            txt.text = nlohmann::json::parse(msg).dump();
            addReply(txt);
        } catch (std::exception& e) {
            workerThreadError("json parse error");
        }
    } break;
    case hash_bytes: {
        MqttReplyBytes bytes;
        bytes.topic = topic;
        bytes.data.assign(data, data + len);
        addReply(bytes);
    } break;
    case hash_sym:
    default: {
        MqttReplyTxt txt;
        txt.topic = topic;
        txt.text.assign(reinterpret_cast<const char*>(data), len);
        addReply(txt);
    } break;
    }
}

void NetMqttClient::onWorkerConn(int rc)
{
#define CASE(v, msg)                                 \
    case ceammc_mqtt_rc::v:                          \
        workerThreadError("connection error: " msg); \
        break;

    switch (static_cast<ceammc_mqtt_rc>(rc)) {
    case ceammc_mqtt_rc::Ok:
        workerThreadDebug("connected");
        break;
        CASE(BadClientId, "bad client id");
        CASE(RefusedProtocolVersion, "refuse protocol version");
        CASE(ServiceUnavailable, "service unavailable");
        CASE(BadUserNamePassword, "bad user/password");
        CASE(NotAuthorized, "bad user/password");
        CASE(InvalidClient, "invalid client");
        CASE(InvalidString, "invalid string");
    default:
        workerThreadError("connection error");
        break;
    }

#undef CASE
}

void setup_net_mqtt_client()
{
    ObjectFactory<NetMqttClient> obj("net.mqtt.client");
    obj.addMethod("connect", &NetMqttClient::m_connect);
    obj.addMethod("disconnect", &NetMqttClient::m_disconnect);
    obj.addMethod("subscribe", &NetMqttClient::m_subscribe);
    obj.addMethod("unsubscribe", &NetMqttClient::m_unsubscribe);
    obj.addMethod("publish", &NetMqttClient::m_publish);
}
#endif
