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

struct WsCliConnect {
    std::string url;
};
struct WsCliClose { };
struct WsCliPing { };
struct WsCliPong { };
struct WsCliSendMsg {
    std::string msg;
};

struct WsCliReplyText {
    std::string msg;
};
struct WsCliReplyBinary {
    std::vector<std::uint8_t> data;
};

using WsCliRequest = boost::variant<WsCliSendMsg, WsCliConnect, WsCliClose, WsCliPing>;
using WsCliReply = boost::variant<WsCliReplyText, WsCliReplyBinary, WsCliPong>;

using BaseWsClient = FixedSPSCObject<WsCliRequest, WsCliReply>;

class WsClientImpl;

class NetWsClient : public BaseWsClient {
    std::unique_ptr<WsClientImpl> cli_;

public:
    NetWsClient(const PdArgs& args);
    ~NetWsClient();

    void processRequest(const WsCliRequest& req, ResultCallback cb) override;
    void processResult(const WsCliReply& res) override;

    void runLoopFor(size_t ms) final;

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
};

void setup_net_ws_client();

#endif // NET_WS_CLIENT_H
