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
using namespace ceammc;

struct WsSrvListen {
    std::string addr;
};
struct WsSrvSendText {
    std::string msg;
};
struct WsSrvClose {
};

struct WsSrvReplyText {
    std::string msg;
    std::string from;
    std::size_t id;
};
struct WsSrvReplyBinary {
    std::vector<std::uint8_t> data;
    std::string from;
    std::size_t id;
};
struct WsSrvReplyConn {
    std::string from;
    std::size_t id;
};
struct WsSrvReplyClose {
    std::string from;
    std::size_t id;
};

using WsSrvRequest = boost::variant<WsSrvListen, WsSrvSendText, WsSrvClose>;
using WsSrvReply = boost::variant<WsSrvReplyText, WsSrvReplyBinary, WsSrvReplyConn, WsSrvReplyClose>;

using BaseWsServer = FixedSPSCObject<WsSrvRequest, WsSrvReply>;

class WsServerImpl;

class NetWsServer : public BaseWsServer {
    std::unique_ptr<WsServerImpl> srv_;

public:
    NetWsServer(const PdArgs& args);
    ~NetWsServer();

    void processRequest(const WsSrvRequest& req, ResultCallback cb) override;
    void processResult(const WsSrvReply& res) override;

    void runLoopFor(size_t ms) final;

    //    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_listen(t_symbol* s, const AtomListView& lv);

private:
    void outputInfo(const std::string& from, size_t id);
};

void setup_net_ws_server();

#endif // NET_WS_SERVER_H
