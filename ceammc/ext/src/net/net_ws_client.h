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

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "net_rust_struct.hpp"

#include <chrono>
using namespace ceammc;

using BaseWsClient = DispatchedObject<BaseObject>;
using WsClientImpl = net::NetService<ceammc_ws_client, ceammc_ws_client_init, ceammc_ws_client_result_cb>;

class NetWsClient : public BaseWsClient {
    std::unique_ptr<WsClientImpl> cli_; // should be accessed only in worker thread
    SymbolEnumProperty* mode_ { nullptr };

    enum LatencyState : uint8_t {
        LATENCY_NONE,
        LATENCY_START
    };

    LatencyState latency_state_ { LATENCY_NONE };
    std::chrono::steady_clock::time_point latency_measure_begin_;

public:
    NetWsClient(const PdArgs& args);

    bool notify(int code) final;

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
    void outputLatency();
    void processBinary(const std::uint8_t* data, size_t data_len);
    void processClose();
    void processConnect();
    void processDisconnect();
    void processPing(const std::uint8_t* data, size_t data_len);
    void processPong(const std::uint8_t* data, size_t data_len);
    void processText(const char* txt);

private:
    static std::string makeJson(const AtomListView& lv);
};

void setup_net_ws_client();

#endif // NET_WS_CLIENT_H
