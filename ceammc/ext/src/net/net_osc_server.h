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
#ifndef NET_OSC_SERVER_H
#define NET_OSC_SERVER_H

#include "ceammc_osc.h"
#include "ceammc_object.h"
#include "ceammc_property.h"
#include "osc_property.h"

namespace ceammc {
namespace net {

    class NetOscServer : public BaseObject {
        SymbolProperty* name_;
        OscUrlProperty* url_;
        BoolProperty* dump_;
        BoolProperty* auto_start_;
        std::weak_ptr<osc::OscServer> server_;

    public:
        NetOscServer(const PdArgs& args);
        ~NetOscServer();

        void dump() const override;

        void initDone() final;
        void m_start(t_symbol* s, const AtomListView& lv);
        void m_stop(t_symbol* s, const AtomListView& lv);
    };
}
}

void setup_net_osc_server();

#endif // NET_OSC_SERVER_H
