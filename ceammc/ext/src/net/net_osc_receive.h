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
#ifndef NET_OSC_RECEIVE_H
#define NET_OSC_RECEIVE_H

#include "ceammc_poll_dispatcher.h"
#include "net_osc_server.h"

namespace ceammc {
namespace net {
    class OscServer;

    template <typename T>
    class DispatcherSubscriber {
        T* ptr_;

    public:
        DispatcherSubscriber(T* p)
            : ptr_(p)
        {
            Dispatcher::instance().subscribe(ptr_, id());
        }

        ~DispatcherSubscriber()
        {
            Dispatcher::instance().unsubscribe(ptr_);
        }

        SubscriberId id() const { return reinterpret_cast<SubscriberId>(ptr_); }
    };

    class NetOscReceive : public BaseObject, public NotifiedObject {
        SymbolProperty* server_;
        SymbolProperty* path_;
        SymbolProperty* types_;
        DispatcherSubscriber<NetOscReceive> disp_;
        OscMethodPipe pipe_;

    public:
        NetOscReceive(const PdArgs& args);
        ~NetOscReceive();

        void initDone() override;
        bool notify(NotifyEventType code) final;
        void processMessage(const OscMessage& msg);
    };
}
}

void setup_net_osc_receive();

#endif // NET_OSC_RECEIVE_H
