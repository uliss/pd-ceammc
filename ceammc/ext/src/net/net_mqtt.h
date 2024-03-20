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
#ifndef NET_MQTT_H
#define NET_MQTT_H

#include "ceammc_pollthread_spsc.h"
#include "ceammc_property_enum.h"
#include <boost/variant.hpp>
using namespace ceammc;

using MqttData = std::vector<std::uint8_t>;

struct MqttRequest {
    enum Type : std::uint8_t {
        NONE,
        CONNECT,
        DISCONNECT,
        SUBSCRIBE,
        UNSUBSCRIBE,
        PUBLISH,
    };

    t_symbol* topic;
    boost::variant<std::string, MqttData> msg;

    Type type { NONE };

    static MqttRequest connect();
    static MqttRequest disconnect();
    static MqttRequest subscribe(t_symbol* topic);
    static MqttRequest unsubscribe(t_symbol* topic);
    static MqttRequest publish(t_symbol* topic, const std::string& msg);
    static MqttRequest publish(t_symbol* topic, std::string&& msg);
    static MqttRequest publish(t_symbol* topic, MqttData&& msg);

private:
    MqttRequest();
};

struct MqttReplyPing { };
struct MqttReplyTxt {
    std::string topic;
    std::string text;
};
struct MqttReplyBytes {
    std::string topic;
    std::vector<std::uint8_t> data;
};

using MqttReply = boost::variant<MqttReplyPing, MqttReplyTxt, MqttReplyBytes>;

class MqttClient;

using BaseMqtt = FixedSPSCObject<MqttRequest, MqttReply>;

class NetMqtt : public BaseMqtt {
    SymbolProperty* id_ { nullptr };
    SymbolProperty* host_ { nullptr };
    SymbolProperty* user_ { nullptr };
    SymbolProperty* pass_ { nullptr };
    IntProperty* port_ { nullptr };
    IntEnumProperty* qos_ { nullptr };
    BoolProperty* retain_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };
    std::unique_ptr<MqttClient> cli_;

public:
    NetMqtt(const PdArgs& args);
    ~NetMqtt();

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_disconnect(t_symbol* s, const AtomListView& lv);
    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_unsubscribe(t_symbol* s, const AtomListView& lv);
    void m_publish(t_symbol* s, const AtomListView& lv);

    void processRequest(const MqttRequest& req, ResultCallback cb) override;
    void processResult(const MqttReply& res) override;
    void processEvents() final;

private:
    const char* clientId() const;
    const char* hostname() const;

    void onWorkerPub(const char* topic, const std::uint8_t* data, size_t len);
    void onWorkerConn(int rc);
    friend class MqttClient;

private:
    static int id_count_;
};

void setup_net_mqtt_client();

#endif // NET_MQTT_H
