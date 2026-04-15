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
#ifndef WITH_ESPHOME
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetEsphomeClient, 1, 1, "compiled without EspHome support");
OBJECT_STUB_SETUP(NetEsphomeClient, net_esphome_client, "net.esphome.client");
#else
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "net_esphome_client.h"
#include "net_esphome_client_args.hpp"
#include "net_rust.hpp"

#define ESPHOME_CAST()                               \
    auto obj = static_cast<NetEsphomeClient*>(user); \
    if (!obj)                                        \
        return;

namespace ceammc {

namespace {
    ceammc_esphome_client_cb on_data(void* user)
    {
        return {
            user,
            [](void* user) {
                ESPHOME_CAST();
                obj->anyTo(0, gensym("pong"), AtomListView());
            },
            [](void* user, std::uint32_t key, bool state, std::uint32_t device) {
                ESPHOME_CAST();
                AtomArray<3> data;
                data[0] = gensym(fmt::format("0x{:08x}", key).c_str());
                data[1] = state;
                data[2] = device;
                obj->anyTo(0, gensym("switch"), data.view());
            },
            [](void* user, std::uint32_t key, bool state, std::uint32_t device, bool missing_state) {
                ESPHOME_CAST();
                AtomArray<4> data;
                data[0] = gensym(fmt::format("0x{:08x}", key).c_str());
                data[1] = state;
                data[2] = device;
                data[3] = missing_state;
                obj->anyTo(0, gensym("binary"), data.view());
            },
        };
    }
} // namespace

NetEsphomeClient::NetEsphomeClient(const PdArgs& args)
    : RustFfiObject<BaseObject, ceammc_esphome_client>(&ceammc_esphome_client_free, args)
{
    createOutlet();

    addr_ = new SymbolProperty("@addr", &s_);
    addProperty(addr_);

    port_ = new IntProperty("@port", 6053);
    port_->checkClosedRange(1, std::numeric_limits<std::uint16_t>::max());
    addProperty(port_);
}

bool NetEsphomeClient::notify(int /*code*/)
{
    return ceammc_esphome_client_process(ffiObject());
}

/// @function "connect to / disconnect from esphome device" {
///     #state bool "state" { default: true }
/// }
void NetEsphomeClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    m_connect_args args;
    if (!args.parse_args(lv, this))
        return;

    if (args.state) {
        createObject(ceammc_esphome_client_new(addr_->cstr(),
            port_->value(),
            on_notify(),
            on_message(),
            on_data(this) //
            ));
    } else {
        createObject(nullptr);
    }
}

void NetEsphomeClient::m_entities(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    ceammc_esphome_client_list_entities(ffiObject());
}

void NetEsphomeClient::m_ping(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    ceammc_esphome_client_ping(ffiObject());
}

void NetEsphomeClient::m_subscribe(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    ceammc_esphome_client_subscribe(ffiObject());
}

/// @function "turn on/off esphome switch" {
///     #key    symbol  "switch ID"     { }
///     #state  bool    "switch state"  {}
/// }
void NetEsphomeClient::m_switch(t_symbol* s, const AtomListView& lv)
{
    m_switch_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(true, s))
        return;

    std::string key = args.key->s_name;
    std::uint32_t id = 0;
    if (!key.empty()) {
        try {
            id = std::stoul(key.substr(1));
        } catch (...) {
            id = 0;
        }
    }

    ceammc_esphome_client_switch(ffiObject(), id, args.state);
}

/// @function "set esphome text sensor" {
///     #key   symbol  "sensor ID"     {}
///     #text  symbol  "sensor text"   {}
/// }
void NetEsphomeClient::m_text(t_symbol* s, const AtomListView& lv)
{
    m_text_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(true, s))
        return;

    std::string key = args.key->s_name;
    std::uint32_t id = 0;
    if (!key.empty()) {
        try {
            id = std::stoul(key.substr(1));
        } catch (...) {
            id = 0;
        }
    }

    ceammc_esphome_client_text(ffiObject(), id, args.text->s_name);
}

void setup_net_esphome_client()
{
    ObjectFactory<NetEsphomeClient> obj("net.esphome.client");

    obj.addMethod("connect", &NetEsphomeClient::m_connect);
    obj.addMethod("entities", &NetEsphomeClient::m_entities);
    obj.addMethod("ping", &NetEsphomeClient::m_ping);
    obj.addMethod("subscribe", &NetEsphomeClient::m_subscribe);
    obj.addMethod("switch", &NetEsphomeClient::m_switch);
    obj.addMethod("text", &NetEsphomeClient::m_text);
}

} // namespace ceammc
#endif
