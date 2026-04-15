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
#ifndef NET_ESPHOME_CLIENT_H
#define NET_ESPHOME_CLIENT_H

#include "ceammc_object.h"
#include "ceammc_rs_msg_object.h"

struct ceammc_esphome_client;

namespace ceammc {
class NetEsphomeClient : public RustFfiObject<BaseObject, ceammc_esphome_client> {
    SymbolProperty* addr_ { nullptr };
    IntProperty* port_ { nullptr };

public:
    explicit NetEsphomeClient(const PdArgs& args);

    bool notify(int code) final;

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_entities(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_switch(t_symbol* s, const AtomListView& lv);
    void m_text(t_symbol* s, const AtomListView& lv);
};

void setup_net_esphome_client();

} // namespace ceammc

#endif // NET_ESPHOME_CLIENT_H
