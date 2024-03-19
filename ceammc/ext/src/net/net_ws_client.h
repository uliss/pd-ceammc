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
#include <boost/variant.hpp>
using namespace ceammc;

namespace ceammc {
namespace ws {
    using Bytes = std::vector<std::uint8_t>;

    namespace cli_req {
        struct Connect {
            std::string url;
        };
        struct Close { };
        struct Ping { };
        struct Pong { };
        struct Flush { };
        struct SendText {
            std::string msg;
            bool flush;
        };
        struct SendBinary {
            Bytes data;
            bool flush;
        };

        using Request = boost::variant<SendText, SendBinary, Connect, Close, Ping, Pong, Flush>;
    }

    namespace cli_reply {
        struct MessageText {
            std::string msg;
        };
        struct MessageBinary {
            Bytes data;
        };
        struct MessagePong { };
        struct MessagePing { };
        struct MessageClose { };

        using Reply = boost::variant<MessageText, MessageBinary, MessagePing, MessagePong, MessageClose>;
    }
}
}

using BaseWsClient = FixedSPSCObject<ws::cli_req::Request, ws::cli_reply::Reply>;

class WsClientImpl;

class NetWsClient : public BaseWsClient {
    std::unique_ptr<WsClientImpl> cli_;

public:
    NetWsClient(const PdArgs& args);
    ~NetWsClient();

    void processRequest(const ws::cli_req::Request& req, ResultCallback cb) override;
    void processResult(const ws::cli_reply::Reply& res) override;

    void runLoopFor(size_t ms) final;

    void m_close(t_symbol* s, const AtomListView& lv);
    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_send_binary(t_symbol* s, const AtomListView& lv);
    void m_send_json(t_symbol* s, const AtomListView& lv);
    void m_send_text(t_symbol* s, const AtomListView& lv);
    void m_write_binary(t_symbol* s, const AtomListView& lv);
    void m_write_json(t_symbol* s, const AtomListView& lv);
    void m_write_text(t_symbol* s, const AtomListView& lv);

private:
    static ws::Bytes makeBinary(const AtomListView& lv);
    static std::string makeJson(const AtomListView& lv);
};

void setup_net_ws_client();

#endif // NET_WS_CLIENT_H
