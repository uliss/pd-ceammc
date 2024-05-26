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
#ifndef NET_MQTT_CLIENT_H
#define NET_MQTT_CLIENT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "net_rust_struct.hpp"
using namespace ceammc;

using BaseMqttClient = DispatchedObject<BaseObject>;
using MqttClient = net::NetService<ceammc_mqtt_client, ceammc_mqtt_client_init, ceammc_mqtt_client_result_cb>;

class NetMqttClient : public BaseMqttClient {
    SymbolProperty* id_ { nullptr };
    IntEnumProperty* qos_ { nullptr };
    BoolProperty* retain_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };

    std::unique_ptr<MqttClient> cli_;

public:
    NetMqttClient(const PdArgs& args);

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_unsubscribe(t_symbol* s, const AtomListView& lv);
    void m_publish(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;

private:
    void processReply(const char* topic, const uint8_t* data, size_t data_len, ceammc_mqtt_qos qos, bool retain, uint16_t pkid);

private:
    static int id_count_;
};

void setup_net_mqtt_client();

#endif // NET_MQTT_CLIENT_H
