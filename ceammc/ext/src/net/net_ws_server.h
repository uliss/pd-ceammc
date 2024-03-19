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
#include <boost/variant.hpp>
#include <cstdint>
#include <vector>

using namespace ceammc;

namespace ceammc {
namespace ws {
    using Bytes = std::vector<std::uint8_t>;
    namespace srv_req {
        struct Listen {
            std::string addr;
        };
        struct Close { };
        struct SendText {
            std::string msg;
        };
        struct SendBinary {
            Bytes data;
        };
        struct SendPing {
            Bytes data;
        };
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
    }

    using Request = boost::variant<
        srv_req::Listen,
        srv_req::SendText,
        srv_req::SendBinary,
        srv_req::SendPing,
        srv_req::Close>;
    using Reply = boost::variant<
        srv_reply::MessageText,
        srv_reply::MessageBinary,
        srv_reply::MessagePong,
        srv_reply::ClientConnected,
        srv_reply::ClientClosed>;
}
}

using BaseWsServer = FixedSPSCObject<ws::Request, ws::Reply, BaseObject, 16, 20>;

class WsServerImpl;

class NetWsServer : public BaseWsServer {
    std::unique_ptr<WsServerImpl> srv_;

public:
    NetWsServer(const PdArgs& args);
    ~NetWsServer();

    void processRequest(const ws::Request& req, ResultCallback cb) override;
    void processResult(const ws::Reply& res) override;

    void runLoopFor(size_t ms) final;

    void m_close(t_symbol* s, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_listen(t_symbol* s, const AtomListView& lv);

private:
    void outputInfo(const std::string& from, size_t id);
};

void setup_net_ws_server();

#endif // NET_WS_SERVER_H
