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

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "net_rust.hpp"
#include "net_rust_struct.hpp"

#include <cstdint>
#include <vector>

using namespace ceammc;

using Bytes = std::vector<std::uint8_t>;
using BaseWsServer = DispatchedObject<BaseObject>;
using WsServerImpl = net::NetService<ceammc_ws_server, ceammc_ws_server_init, ceammc_ws_server_result_cb>;

class NetWsServer : public BaseWsServer {
    std::unique_ptr<WsServerImpl> srv_;
    SymbolEnumProperty* mode_ { nullptr };

    enum RequestArgs {
        REQ_ARGS_EQ_0, // equal to zero
        REQ_ARGS_GE_0, // >= 0
        REQ_ARGS_GE_1, // >= 1
    };

public:
    NetWsServer(const PdArgs& args);

    void m_listen(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);

    void m_clients(t_symbol* s, const AtomListView& lv);
    void m_close(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_send_binary(t_symbol* s, const AtomListView& lv);
    void m_send_json(t_symbol* s, const AtomListView& lv);
    void m_shutdown(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;

private:
    static AtomList fromBinary(const std::uint8_t* data, size_t len);
    static Bytes toBinary(const AtomListView& lv);
    static std::string toJson(const AtomListView& lv);

private:
    void outputInfo(const ceammc_ws_peer_info* peer);
    void processText(const char* msg, const ceammc_ws_peer_info* peer);
    void processBinary(const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer);
    void processPing(const std::uint8_t* data, size_t len, const ceammc_ws_peer_info* peer);
    void processConnected(bool connected, const ceammc_ws_peer_info* peer);

    bool checkClientSelector(t_symbol* s, const AtomListView& lv, RequestArgs req);
};

void setup_net_ws_server();

#endif // NET_WS_SERVER_H
