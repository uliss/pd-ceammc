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
#ifndef NET_WS_SERVER_H
#define NET_WS_SERVER_H

#include "ceammc_pollthread_spsc.h"
#include "ceammc_property_enum.h"
#include "net_rust.h"

#include <boost/variant.hpp>
#include <cstdint>
#include <vector>

using namespace ceammc;

namespace ceammc {
namespace ws {
    using Bytes = std::vector<std::uint8_t>;
    using IdList = std::vector<std::pair<size_t, std::string>>;
    namespace srv_req {
        struct Listen {
            std::string addr;
            std::uint16_t port;
        };
        struct Stop { };
        struct CloseClients {
            ceammc_ws_client_target to;
        };
        struct AbortClients {
            ceammc_ws_client_target to;
        };
        struct DumpClients { };
        struct SendText {
            std::string msg;
            ceammc_ws_client_target to;
        };
        struct SendBinary {
            Bytes data;
            ceammc_ws_client_target to;
        };
        struct SendPing {
            Bytes data;
            ceammc_ws_client_target to;
        };

        using Request = boost::variant<
            Listen,
            Stop,
            SendText,
            SendBinary,
            SendPing,
            CloseClients,
            DumpClients,
            AbortClients>;
    }

    namespace srv_reply {
        struct MessageText {
            std::string msg;
            std::string from;
            std::size_t id;
        };
        struct MessageBinary {
            Bytes data;
            std::string from;
            std::size_t id;
        };
        struct MessagePong {
            Bytes data;
            std::string from;
            std::size_t id;
        };
        struct ClientConnected {
            std::string from;
            std::size_t id;
        };
        struct ClientClosed {
            std::string from;
            std::size_t id;
        };
        struct ConnectedClients {
            IdList ids;
        };

        using Reply = boost::variant<
            MessageText,
            MessageBinary,
            MessagePong,
            ClientConnected,
            ClientClosed,
            ConnectedClients>;
    }

    struct ServerImpl {
        std::mutex mtx_;
        ceammc_ws_server* srv_ { nullptr };
        std::function<void(const char*, const ceammc_ws_conn_info&)> cb_err;
        std::function<void(const char*, const ceammc_ws_conn_info&)> cb_text;
        std::function<void(const Bytes&, const ceammc_ws_conn_info&)> cb_bin, cb_ping, cb_pong;
        std::function<void(const ceammc_ws_conn_info&)> cb_conn, cb_close;
        std::function<void(const IdList&)> cb_dump_clients;

        ~ServerImpl();

        void listen(const srv_req::Listen& msg);
        void process(const srv_req::Stop&);
        void process(const srv_req::CloseClients& msg);
        void process(const srv_req::SendText& txt);
        void process(const srv_req::SendBinary& bin);
        void process(const srv_req::SendPing& ping);
        void process(const srv_req::DumpClients&);
        void process(const srv_req::AbortClients& cli);

        void process_events();

        static void on_error(void* user, const char* msg, const ceammc_ws_conn_info* info);
        static void on_text(void* user, const char* msg, const ceammc_ws_conn_info* info);
        static void on_bin(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info);
        static void on_ping(void* user, const std::uint8_t* data, size_t len, const ceammc_ws_conn_info* info);
        static void on_conn(void* user, const ceammc_ws_conn_info* info);
        static void on_disc(void* user, const ceammc_ws_conn_info* info);
        static void on_client_dump(void* user, const ceammc_ws_conn_info* info, size_t len);

    private:
        bool check_running();
    };
}
}

using BaseWsServer = FixedSPSCObject<ws::srv_req::Request, ws::srv_reply::Reply, BaseObject, 16, 20>;

class NetWsServer : public BaseWsServer {
    std::unique_ptr<ws::ServerImpl> srv_;
    SymbolEnumProperty* mode_ { nullptr };

    enum RequestArgs {
        REQ_ARGS_EQ_0, // equal to zero
        REQ_ARGS_GE_0, // >= 0
        REQ_ARGS_GE_1, // >= 1
    };

public:
    NetWsServer(const PdArgs& args);

    void processRequest(const ws::srv_req::Request& req, ResultCallback cb) override;
    void processResult(const ws::srv_reply::Reply& res) override;
    void processEvents() final;

    void m_listen(t_symbol* s, const AtomListView& lv);

    void m_clients(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_send_binary(t_symbol* s, const AtomListView& lv);
    void m_send_json(t_symbol* s, const AtomListView& lv);
    void m_shutdown(t_symbol* s, const AtomListView& lv);

private:
    static AtomList fromBinary(const ws::Bytes& data);
    static AtomList fromBinary(const ws::IdList& data);
    static ws::Bytes toBinary(const AtomListView& lv);
    static std::string toJson(const AtomListView& lv);

private:
    void outputInfo(const std::string& from, size_t id);
    void processReply(const ws::srv_reply::MessageText& m);
    void processReply(const ws::srv_reply::MessageBinary& bin);
    void processReply(const ws::srv_reply::MessagePong& pong);
    void processReply(const ws::srv_reply::ClientConnected& conn);
    void processReply(const ws::srv_reply::ClientClosed& closed);
    void processReply(const ws::srv_reply::ConnectedClients& cli);
    bool checkClientSelector(t_symbol* s, const AtomListView& lv, RequestArgs req);

    template <class T>
    bool process_reply(const ws::srv_reply::Reply& res)
    {
        if (res.type() == typeid(T)) {
            this->processReply(boost::get<T>(res));
            return true;
        } else
            return false;
    }

    template <class T>
    bool process_request(const ws::srv_req::Request& req)
    {
        if (req.type() == typeid(T)) {
            this->srv_->process(boost::get<T>(req));
            return true;
        } else
            return false;
    }
};

void setup_net_ws_server();

#endif // NET_WS_SERVER_H
