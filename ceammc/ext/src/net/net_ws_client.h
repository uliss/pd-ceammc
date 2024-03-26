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
#ifndef NET_WS_CLIENT_H
#define NET_WS_CLIENT_H

#include "ceammc_pollthread_spsc.h"
#include "ceammc_property_enum.h"
#include "net_rust.hpp"

#include <boost/variant.hpp>
#include <chrono>
using namespace ceammc;

namespace ceammc {
namespace ws {
    using Bytes = std::vector<std::uint8_t>;

    namespace cli_req {
        struct SendText {
            std::string msg;
            bool flush;
        };
        struct SendBinary {
            Bytes data;
            bool flush;
        };
        struct SendPing {
            Bytes data;
        };

        struct Connect {
            std::string url;
        };
        struct Close { };
        struct Flush { };

        using Request = boost::variant<SendText,
            SendBinary,
            SendPing,
            Connect,
            Close,
            Flush>;
    }

    struct ClientImpl {
        std::mutex mtx_;
        ceammc_ws_client* cli_ { nullptr };
        std::function<void(const char*)> cb_err, cb_text;
        std::function<void(const ws::Bytes&)> cb_ping, cb_pong, cb_bin, cb_close;

        static void on_error(void* user, const char* msg);
        static void on_text(void* user, const char* msg);
        static void on_ping(void* user, const std::uint8_t* data, size_t len);
        static void on_pong(void* user, const std::uint8_t* data, size_t len);
        static void on_binary(void* user, const std::uint8_t* data, size_t len);
        static void on_close(void* user, const std::uint8_t* data, size_t len);

        ~ClientImpl();

        // this is blocking function at this moment
        bool connect(const cli_req::Connect& c);
        void disconnect();

        void process(const cli_req::SendText& txt);
        void process(const cli_req::SendBinary& msg);
        void process(const cli_req::SendPing& ping);
        void process(const cli_req::Close&);
        void process(const cli_req::Flush&);

        // non-blocking
        void process_events();
    };

    namespace cli_reply {
        struct MessageText {
            std::string msg;
        };
        struct MessageBinary {
            Bytes data;
        };
        struct MessagePong {
            Bytes data;
        };
        struct MessagePing {
            Bytes data;
        };
        struct MessageClose {
            Bytes data;
        };
        struct Connected { };

        using Reply = boost::variant<MessageText,
            MessageBinary,
            MessagePing,
            MessagePong,
            MessageClose,
            Connected>;
    }
}
}

using BaseWsClient = FixedSPSCObject<ws::cli_req::Request, ws::cli_reply::Reply, BaseObject, 32, 20>;

class NetWsClient : public BaseWsClient {
    std::unique_ptr<ws::ClientImpl> cli_; // should be accessed only in worker thread
    SymbolEnumProperty* mode_ { nullptr };

    enum LatencyState : uint8_t {
        LATENCY_NONE,
        LATENCY_START
    };

    LatencyState latency_state_ { LATENCY_NONE };
    std::chrono::steady_clock::time_point latency_measure_begin_;

public:
    NetWsClient(const PdArgs& args);

    void processRequest(const ws::cli_req::Request& req, ResultCallback cb) override;
    void processResult(const ws::cli_reply::Reply& res) override;
    void processEvents() final;

    void m_close(t_symbol* s, const AtomListView& lv);
    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_latency(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_send_array(t_symbol* s, const AtomListView& lv);
    void m_send_binary(t_symbol* s, const AtomListView& lv);
    void m_send_json(t_symbol* s, const AtomListView& lv);
    void m_send_text(t_symbol* s, const AtomListView& lv);
    void m_write_binary(t_symbol* s, const AtomListView& lv);
    void m_write_json(t_symbol* s, const AtomListView& lv);
    void m_write_text(t_symbol* s, const AtomListView& lv);

private:
    void processReply(const ws::cli_reply::MessageText& m);
    void processReply(const ws::cli_reply::MessageBinary& m);
    void processReply(const ws::cli_reply::MessagePing& m);
    void processReply(const ws::cli_reply::MessagePong& m);
    void processReply(const ws::cli_reply::MessageClose& m);
    void processReply(const ws::cli_reply::Connected& m);

    void outputLatency();

    template <class T>
    bool process_request(const ws::cli_req::Request& req)
    {
        if (req.type() == typeid(T)) {
            this->cli_->process(boost::get<T>(req));
            return true;
        } else
            return false;
    }

    template <class T>
    bool process_reply(const ws::cli_reply::Reply& res)
    {
        if (res.type() == typeid(T)) {
            this->processReply(boost::get<T>(res));
            return true;
        } else
            return false;
    }

private:
    static ws::Bytes toBinary(const AtomListView& lv);
    static AtomList fromBinary(const ws::Bytes& data);
    static std::string makeJson(const AtomListView& lv);
};

void setup_net_ws_client();

#endif // NET_WS_CLIENT_H
