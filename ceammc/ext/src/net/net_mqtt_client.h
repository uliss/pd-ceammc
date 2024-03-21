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

#include "ceammc_pollthread_spsc.h"
#include "ceammc_property_enum.h"
#include "net_rust.h"
using namespace ceammc;

#include <boost/variant.hpp>

namespace ceammc {
namespace mqtt {
    using Data = std::vector<std::uint8_t>;

    namespace req {
        struct Connect {
            std::string id, url;
        };
        struct Disconnect { };
        struct Subscribe {
            t_symbol* topic;
        };
        struct Unsubscribe {
            t_symbol* topic;
        };
        struct PublishText {
            t_symbol* topic;
            std::string msg;
            ceammc_mqtt_qos qos;
            bool retain;
        };
        struct PublishBytes {
            t_symbol* topic;
            Data data;
            ceammc_mqtt_qos qos;
            bool retain;
        };
    }

    namespace reply {
        struct ReplyPing { };
        struct ReplyBytes {
            std::string topic;
            Data data;
        };
        struct ReplyText {
            std::string topic;
            std::string text;
        };

    }

    using Reply = boost::variant<
        reply::ReplyPing,
        reply::ReplyText,
        reply::ReplyBytes>;

    using Request = boost::variant<
        req::Connect,
        req::Disconnect,
        req::Subscribe,
        req::Unsubscribe,
        req::PublishText,
        req::PublishBytes>;

    class ClientImpl {
        ceammc_mqtt_client* cli_ { nullptr };
        std::mutex mtx_;

    public:
        std::function<void(const char* msg)> cb_err;
        std::function<void()> cb_ping;
        std::function<void(const char* topic, const Data& data)> cb_pub;

    public:
        ~ClientImpl();

        bool connect(const req::Connect& m);
        bool disconnect();

        void process(const req::Subscribe& m);
        void process(const req::Unsubscribe& m);
        void process(const req::PublishText& m);
        void process(const req::PublishBytes& m);

        void process_events();

        bool check_connected() const;
        void error(const char* msg) const;
        inline void error(const std::string& msg) const { error(msg.c_str()); };
    };
}
}

using BaseMqttClient = FixedSPSCObject<mqtt::Request, mqtt::Reply>;

class NetMqttClient : public BaseMqttClient {
    SymbolProperty* id_ { nullptr };
    IntEnumProperty* qos_ { nullptr };
    BoolProperty* retain_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };

    std::unique_ptr<mqtt::ClientImpl> cli_;

public:
    NetMqttClient(const PdArgs& args);

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_unsubscribe(t_symbol* s, const AtomListView& lv);
    void m_publish(t_symbol* s, const AtomListView& lv);

    void processRequest(const mqtt::Request& req, ResultCallback cb) override;
    void processResult(const mqtt::Reply& res) override;
    void processEvents() final;

private:
    void processReply(const mqtt::reply::ReplyPing& m);
    void processReply(const mqtt::reply::ReplyText& m);
    void processReply(const mqtt::reply::ReplyBytes& m);

    template <class T>
    bool process_reply(const mqtt::Reply& res)
    {
        if (res.type() == typeid(T)) {
            this->processReply(boost::get<T>(res));
            return true;
        } else
            return false;
    }

    template <class T>
    bool process_request(const mqtt::Request& req)
    {
        if (req.type() == typeid(T)) {
            this->cli_->process(boost::get<T>(req));
            return true;
        } else
            return false;
    }

private:
    static int id_count_;
};

void setup_net_mqtt_client();

#endif // NET_MQTT_CLIENT_H
