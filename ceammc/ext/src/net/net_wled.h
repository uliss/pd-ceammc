/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef NET_WLED_H
#define NET_WLED_H

#ifdef WITH_WLED

#include "ceammc_rs_msg_object.h"
#include "net_rust.hpp"

namespace ceammc {
class NetWledUdp : public RustFfiObject<BaseObject, ceammc_wled_udp> {
    SymbolProperty* addr_ { nullptr };
    IntProperty* port_ { nullptr };
    IntProperty* size_ { nullptr };

public:
    explicit NetWledUdp(const PdArgs& args);

    bool notify(int code) final;

    void m_brightness(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_set_pixel(t_symbol* s, const AtomListView& lv);
};

} // namespace ceammc


void setup_net_wled_udp();

#else

#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(NetWledUdp, 1, 1, "compiled without WLED support");
OBJECT_STUB_SETUP(NetWledUdp, net_wled_udp, "net.wled");


#endif

#endif // NET_WLED_H
