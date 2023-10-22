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

#include "ceammc_object.h"
#include "ceammc_osc.h"
#include "ceammc_poll_dispatcher.h"

namespace ceammc {
namespace net {
    class OscServer;

    class NetOscReceive : public DispatchedObject<BaseObject> {
        SymbolProperty* server_;
        SymbolProperty* path_;
        SymbolProperty* types_;
        osc::OscMethodPipe pipe_;

    public:
        NetOscReceive(const PdArgs& args);
        ~NetOscReceive();

        void initDone() override;
        bool notify(int code) final;
        void processMessage(const osc::OscRecvMessage& msg);

        void onInlet(size_t n, const AtomListView& lv) override;

    public:
        void updateServer(t_symbol* name, const AtomListView& lv);
        const char* types() const;
        bool subscribe(const osc::OscServerList::OscServerPtr& osc, t_symbol* path);
        bool unsubscribe(const osc::OscServerList::OscServerPtr& osc, t_symbol* path);
    };
}
}

void setup_net_osc_receive();

#endif // NET_OSC_RECEIVE_H
