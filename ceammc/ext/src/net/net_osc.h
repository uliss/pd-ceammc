/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef NET_OSC_H
#define NET_OSC_H

#ifndef WIN32
#include <unistd.h>
#endif

#include <cstdarg>
#include <cstdint>

#include "ceammc_poll_dispatcher.h"
#include "readerwriterqueue.h"

using namespace ceammc;

enum NetOscCmdStatus : std::int8_t {
    NET_OSC_SEND_ERROR,
    NET_OSC_SEND_OK,
    NET_OSC_SEND_REQUEST,
};

struct NetOscSendMsg {
    NetOscCmdStatus status;
    std::string error_msg;
};

using NetOscSendBase = NotifiedObject;

struct NetOscSendOscTask;

class NetOscSend : public NetOscSendBase {
public:
    using MsgPipe = moodycamel::ReaderWriterQueue<NetOscSendMsg>;

private:
    SymbolProperty* host_;
    IntProperty* port_;
    MsgPipe msg_pipe_;

public:
    NetOscSend(const PdArgs& args);
    ~NetOscSend();

    void m_send(t_symbol* s, const AtomListView& lv);
    void m_send_bool(t_symbol* s, const AtomListView& lv);

    void processMessage(const NetOscSendMsg& msg);
    bool notify(NotifyEventType code) final;

private:
    void initTask(NetOscSendOscTask& task, const char* path);
};

void setup_net_osc();

#endif // NET_OSC_H
