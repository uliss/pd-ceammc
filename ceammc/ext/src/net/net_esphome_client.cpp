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
        return ceammc_esphome_client_cb {
            user,
            [](void* user) {
                ESPHOME_CAST();
                obj->anyTo(0, gensym("pong"), AtomListView());
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_switch_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_binary_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_text_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, const ceammc_esphome_switch_info* s) {
                ESPHOME_CAST();
                obj->onSwitchInfo(EsphomeEntityPtr { new EsphomeSwitch(*s) });
            },
            [](void* user, const ceammc_esphome_text_info* t) {
                ESPHOME_CAST();
                obj->onTextInfo(EsphomeEntityPtr { new EsphomeText(*t) });
            },
            [](void* user, const ceammc_esphome_binary_info* b) {
                ESPHOME_CAST();
                obj->onBinaryInfo(EsphomeEntityPtr { new EsphomeBinary(*b) });
            },

        };
    }
} // namespace

EsphomeEntityBase::EsphomeEntityBase(const ceammc_esphome_entity_id& id, const char* object_id)
    : id_(id)
    , object_id_(gensym(object_id))
{
}

EsphomeEntityBase::EsphomeEntityBase(const EsphomeEntityBase& x)
    : id_(x.id_)
    , object_id_(x.object_id_)
{
}

std::size_t EsphomeEntityIdHash::operator()(const ceammc_esphome_entity_id& id) const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, id.key);
    boost::hash_combine(seed, id.device_id);
    return seed;
}

bool EsphomeEntityEqual::operator()(const ceammc_esphome_entity_id& a, const ceammc_esphome_entity_id& b) const
{
    return a.key == b.key && a.device_id == b.device_id;
}

#define MSYM_INIT(obj, name) name(gensym(obj.name))
#define M_INIT(obj, name) name(obj.name)

EsphomeSwitch::EsphomeSwitch(const ceammc_esphome_switch_info& s)
    : EsphomeEntityBase(s.id, s.object_id)
    , MSYM_INIT(s, name)
    , MSYM_INIT(s, icon)
    , MSYM_INIT(s, device_class)
    , M_INIT(s, entity_category)
    , M_INIT(s, assumed_state)
    , M_INIT(s, disabled_by_default)
{
}

std::unique_ptr<EsphomeEntityBase> EsphomeSwitch::clone() const
{
    return std::unique_ptr<EsphomeEntityBase> { new EsphomeSwitch(*this) };
}

EsphomeBinary::EsphomeBinary(const ceammc_esphome_binary_info& s)
    : EsphomeEntityBase(s.id, s.object_id)
    , MSYM_INIT(s, name)
    , MSYM_INIT(s, icon)
    , MSYM_INIT(s, device_class)
    , M_INIT(s, entity_category)
    , M_INIT(s, disabled_by_default)
    , M_INIT(s, is_status_binary_sensor)
{
}

std::unique_ptr<EsphomeEntityBase> EsphomeBinary::clone() const
{
    return std::unique_ptr<EsphomeEntityBase> { new EsphomeBinary(*this) };
}

EsphomeText::EsphomeText(const ceammc_esphome_text_info& t)
    : EsphomeEntityBase(t.id, t.object_id)
    , MSYM_INIT(t, name)
    , MSYM_INIT(t, icon)
    , MSYM_INIT(t, pattern)
    , M_INIT(t, entity_category)
    , M_INIT(t, min_length)
    , M_INIT(t, max_length)
    , M_INIT(t, mode)
    , M_INIT(t, disabled_by_default)
{
}

std::unique_ptr<EsphomeEntityBase> EsphomeText::clone() const
{
    return std::unique_ptr<EsphomeEntityBase> { new EsphomeText(*this) };
}

NetEsphomeClient::NetEsphomeClient(const PdArgs& args)
    : RustFfiObject<BaseObject, ceammc_esphome_client>(&ceammc_esphome_client_free, args)
{
    createOutlet();

    addr_ = new SymbolProperty { "@addr", &s_ };
    addProperty(addr_);

    port_ = new IntProperty { "@port", ceammc_ESPHOME_DEFAULT_PORT };
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

    auto id = switches_.findId(args.key);
    if (!id) {
        METHOD_ERR(s) << fmt::format("switch with id '{}' not found", args.key->s_name);
        return;
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

    auto id = texts_.findId(args.key);
    if (!id) {
        METHOD_ERR(s) << fmt::format("text with id '{}' not found", args.key->s_name);
        return;
    }

    ceammc_esphome_client_text(ffiObject(), id, args.text->s_name);
}

void NetEsphomeClient::onSwitchInfo(EsphomeEntityPtr&& info)
{
    switches_.addInfo(std::move(info));
}

void NetEsphomeClient::onTextInfo(EsphomeEntityPtr&& info)
{
    texts_.addInfo(std::move(info));
}

void NetEsphomeClient::onBinaryInfo(EsphomeEntityPtr&& info)
{
    bins_.addInfo(std::move(info));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_switch_state& state)
{
    auto oid = switches_.setState(id, state);
    if (oid) {
        anyTo(0, oid, Atom(state.value));
    }
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_text_state& state)
{
    const EsphomeTextState xstate { gensym(state.value), state.missing_state };
    auto oid = texts_.setState(id, xstate);
    if (oid) {
        anyTo(0, oid, Atom(xstate.value));
    }
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_binary_state& state)
{
    auto oid = bins_.setState(id, state);
    if (oid) {
        anyTo(0, oid, Atom(state.value));
    }
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
