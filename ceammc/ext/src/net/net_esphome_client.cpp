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
#include "datatype_dict.h"
#ifndef WITH_ESPHOME
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetEsphomeClient, 1, 1, "compiled without EspHome support");
OBJECT_STUB_SETUP(NetEsphomeClient, net_esphome_client, "net.esphome.client");
#else
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "net_esphome_client.h"
#include "net_esphome_client_args.hpp"
#include "net_rust.hpp"

#define ESPHOME_CAST()                               \
    auto obj = static_cast<NetEsphomeClient*>(user); \
    if (!obj)                                        \
        return;

CEAMMC_DEFINE_SYM(pong)
CEAMMC_DEFINE_SYM(connected)

// dark magic
#define APPLY_TO_EACH_1(action, x) action(x)
#define APPLY_TO_EACH_2(action, x, ...) \
    action(x)                           \
        APPLY_TO_EACH_1(action, __VA_ARGS__)
#define APPLY_TO_EACH_3(action, x, ...) \
    action(x)                           \
        APPLY_TO_EACH_2(action, __VA_ARGS__)
#define APPLY_TO_EACH_4(action, x, ...) \
    action(x)                           \
        APPLY_TO_EACH_3(action, __VA_ARGS__)
#define APPLY_TO_EACH_5(action, x, ...) \
    action(x)                           \
        APPLY_TO_EACH_4(action, __VA_ARGS__)

#define GET_APPLY_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define FOR_EACH_SYMBOL(action, ...)                               \
    GET_APPLY_MACRO(__VA_ARGS__, APPLY_TO_EACH_5, APPLY_TO_EACH_4, \
        APPLY_TO_EACH_3, APPLY_TO_EACH_2, APPLY_TO_EACH_1)         \
    (action, __VA_ARGS__)

#define UNWRAP_ARGS(...) __VA_ARGS__

#define DECLARE_SYM(x) t_symbol* x { &s_ };
#define DECLARE_TYPED(type, name) type name {};

#define COPY_SYM(x) \
    , x { gensym(obj.x) }
#define COPY_TYPED(_, x) \
    , x { obj.x }

#define PAIR_COUNT_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define PAIR_COUNT(...) PAIR_COUNT_(__VA_ARGS__, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1)
#define APPLY_PAIR_1(action, a, b) action(a, b)
#define APPLY_PAIR_2(action, a, b, ...) \
    action(a, b)                        \
        APPLY_PAIR_1(action, __VA_ARGS__)
#define APPLY_PAIR_3(action, a, b, ...) \
    action(a, b)                        \
        APPLY_PAIR_2(action, __VA_ARGS__)
#define APPLY_PAIR_4(action, a, b, ...) \
    action(a, b)                        \
        APPLY_PAIR_3(action, __VA_ARGS__)
#define APPLY_PAIR_5(action, a, b, ...) \
    action(a, b)                        \
        APPLY_PAIR_4(action, __VA_ARGS__)
#define APPLY_PAIR_6(action, a, b, ...) \
    action(a, b)                        \
        APPLY_PAIR_5(action, __VA_ARGS__)

#define CONCAT_PAIR(a, b) a##b
#define APPLY_PAIR_N(N, action, ...) CONCAT_PAIR(APPLY_PAIR_, N)(action, __VA_ARGS__)

#define GET_PAIR_MACRO(cnt) APPLY_PAIR_##cnt
#define FOR_EACH_PAIR(action, ...) \
    APPLY_PAIR_N(PAIR_COUNT(__VA_ARGS__), action, __VA_ARGS__)

#define DEFINE_STRUCT(name, info, symbols, others)                         \
    struct name : public EsphomeEntityBase {                               \
        FOR_EACH_SYMBOL(DECLARE_SYM, UNWRAP_ARGS symbols)                  \
        FOR_EACH_PAIR(DECLARE_TYPED, UNWRAP_ARGS others)                   \
        explicit name(const info& obj)                                     \
            : EsphomeEntityBase(obj.id, obj.object_id)                     \
                  FOR_EACH_SYMBOL(COPY_SYM, UNWRAP_ARGS symbols)           \
                      FOR_EACH_PAIR(COPY_TYPED, UNWRAP_ARGS others)        \
        {                                                                  \
        }                                                                  \
        std::unique_ptr<EsphomeEntityBase> clone() const final             \
        {                                                                  \
            return std::unique_ptr<EsphomeEntityBase> { new name(*this) }; \
        }                                                                  \
    }

namespace ceammc {

DEFINE_STRUCT(EsphomeSwitch, ceammc_esphome_switch_info,
    (name, icon, device_class),
    (ceammc_esphome_category, entity_category, bool, assumed_state, bool, disabled_by_default));

DEFINE_STRUCT(EsphomeTime, ceammc_esphome_time_info,
    (name, icon),
    (ceammc_esphome_category, entity_category, bool, disabled_by_default));

DEFINE_STRUCT(EsphomeNumber, ceammc_esphome_number_info,
    (name, icon, unit_of_measurement, device_class),
    (float, min_value,
        float, max_value,
        float, step,
        ceammc_esphome_category, entity_category,
        int32_t, mode,
        bool, disabled_by_default));

DEFINE_STRUCT(EsphomeBinary, ceammc_esphome_binary_info,
    (name, icon, device_class),
    (ceammc_esphome_category, entity_category,
        bool, is_status_binary_sensor,
        bool, disabled_by_default));

DEFINE_STRUCT(EsphomeSensor, ceammc_esphome_sensor_info,
    (name, icon, device_class, unit_of_measurement),
    (ceammc_esphome_category, entity_category,
        int32_t, accuracy_decimals,
        int32_t, state_class,
        bool, disabled_by_default,
        bool, force_update));

DEFINE_STRUCT(EsphomeText, ceammc_esphome_text_info,
    (name, icon, pattern),
    (ceammc_esphome_category, entity_category,
        uint32_t, min_length,
        uint32_t, max_length,
        int32_t, mode,
        bool, disabled_by_default));

DEFINE_STRUCT(EsphomeLightBase, ceammc_esphome_light_info,
    (name, icon),
    (ceammc_esphome_category, entity_category,
        float, min_mireds,
        float, max_mireds,
        bool, disabled_by_default));

struct EsphomeLight : public EsphomeLightBase {
    std::vector<ceammc_esphome_color_mode> color_modes;
    std::vector<t_symbol*> effects;

    explicit EsphomeLight(const ceammc_esphome_light_info& info)
        : EsphomeLightBase(info)
    {
        effects.resize(info.effects_len);
        for (size_t i = 0; i < info.effects_len; i++)
            effects.push_back(gensym(info.effects[i]));

        color_modes.resize(info.color_modes_len);
        for (size_t i = 0; i < info.color_modes_len; i++)
            color_modes.push_back(info.color_modes[i]);
    }
};

DEFINE_STRUCT(EsphomeSelectBase, ceammc_esphome_select_info,
    (name, icon),
    (ceammc_esphome_category, entity_category,
        bool, disabled_by_default));

struct EsphomeSelect : public EsphomeSelectBase {
    std::vector<t_symbol*> options;
    explicit EsphomeSelect(const ceammc_esphome_select_info& info)
        : EsphomeSelectBase(info)
    {
        options.resize(info.options_len);
        for (size_t i = 0; i < info.options_len; i++)
            options.push_back(gensym(info.options[i]));
    }
};

namespace {
    ceammc_esphome_client_cb on_data(void* user)
    {
        return ceammc_esphome_client_cb {
            user,
            [](void* user) {
                ESPHOME_CAST();
                obj->anyTo(0, sym_pong(), AtomListView());
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
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_sensor_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_number_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_select_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_light_state state) {
                ESPHOME_CAST();
                obj->onState(id, state);
            },
            [](void* user, ceammc_esphome_entity_id id, ceammc_esphome_time_state state) {
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
            [](void* user, const ceammc_esphome_sensor_info* s) {
                ESPHOME_CAST();
                obj->onSensorInfo(EsphomeEntityPtr { new EsphomeSensor(*s) });
            },
            [](void* user, const ceammc_esphome_number_info* n) {
                ESPHOME_CAST();
                obj->onNumberInfo(EsphomeEntityPtr { new EsphomeNumber(*n) });
            },
            [](void* user, const ceammc_esphome_select_info* s) {
                ESPHOME_CAST();
                obj->onSelectInfo(EsphomeEntityPtr { new EsphomeSelect(*s) });
            },
            [](void* user, const ceammc_esphome_light_info* l) {
                ESPHOME_CAST();
                obj->onLightInfo(EsphomeEntityPtr { new EsphomeLight(*l) });
            },
            [](void* user, const ceammc_esphome_time_info* t) {
                ESPHOME_CAST();
                obj->onTimeInfo(EsphomeEntityPtr { new EsphomeTime(*t) });
            },
            [](void* user, const ceammc_esphome_device_info* dev) {
                ESPHOME_CAST();
                obj->onDeviceInfo(*dev);
            },
            [](void* user, bool state) {
                ESPHOME_CAST();
                obj->anyTo(0, sym_connected(), Atom(state));
            }
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

void NetEsphomeClient::dump() const
{
    RustFfiObject<BaseObject, ceammc_esphome_client>::dump();

    bins_.foreachEntity(
        [this](t_symbol* id, const EsphomeEntityPtr& e) {
            OBJ_POST << fmt::format("\t'{}': binary", id->s_name);
        });

    nums_.foreachEntity(
        [this](t_symbol* id, const EsphomeEntityPtr& e) {
            OBJ_POST << fmt::format("\t'{}': number", id->s_name);
        });

    texts_.foreachEntity(
        [this](t_symbol* id, const EsphomeEntityPtr& e) {
            OBJ_POST << fmt::format("\t'{}': text", id->s_name);
        });

    sensors_.foreachEntity(
        [this](t_symbol* id, const EsphomeEntityPtr& e) {
            OBJ_POST << fmt::format("\t'{}': sensor", id->s_name);
        });

    time_.foreachEntity(
        [this](t_symbol* id, const EsphomeEntityPtr& e) {
            OBJ_POST << fmt::format("\t'{}': time", id->s_name);
        });
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

/// @function "set esphome number state" {
///     #key    symbol  "number ID"     { }
///     #state  float   "number state"  {}
/// }
void NetEsphomeClient::m_number(t_symbol* s, const AtomListView& lv)
{
    m_number_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(true, s))
        return;

    auto id = nums_.findId(args.key);
    if (!id) {
        METHOD_ERR(s) << fmt::format("number with id '{}' not found", args.key->s_name);
        return;
    }

    auto info = nums_.findInfo<EsphomeNumber>(args.key);
    if (info) {
        if (info->min_value > args.state || args.state > info->max_value) {
            METHOD_ERR(s) << fmt::format("invalid value: {}, expected value in [{}..{}] range",
                args.state, info->min_value, info->max_value);
            return;
        }
    }

    ceammc_esphome_client_number(ffiObject(), id, args.state);
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

void NetEsphomeClient::m_device_info(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    ceammc_esphome_client_device_info(ffiObject());
}

void NetEsphomeClient::m_get_time(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    ceammc_esphome_client_get_time(ffiObject());
}

/// @function "set esphome time" {
///     #key    symbol  "sensor ID" {}
///     #hour   byte    "hour"      {}
///     #min    byte    "minute"    {}
///     #second byte    "second"    {}
/// }
void NetEsphomeClient::m_set_time(t_symbol* s, const AtomListView& lv)
{
    m_set_time_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(true, s))
        return;

    auto id = time_.findId(args.key);
    if (!id) {
        METHOD_ERR(s) << fmt::format("time with id '{}' not found", args.key->s_name);
        return;
    }

    const ceammc_esphome_time_state time {
        args.hour,
        args.min,
        args.second,
        false, // unused
    };
    ceammc_esphome_client_set_time(ffiObject(), id, time);
}

void NetEsphomeClient::onSwitchInfo(EsphomeEntityPtr&& info)
{
    switches_.addInfo(std::move(info));
}

void NetEsphomeClient::onTextInfo(EsphomeEntityPtr&& info)
{
    texts_.addInfo(std::move(info));
}

void NetEsphomeClient::onTimeInfo(EsphomeEntityPtr&& info)
{
    time_.addInfo(std::move(info));
}

void NetEsphomeClient::onBinaryInfo(EsphomeEntityPtr&& info)
{
    bins_.addInfo(std::move(info));
}

void NetEsphomeClient::onLightInfo(EsphomeEntityPtr&& info)
{
    lights_.addInfo(std::move(info));
}

void NetEsphomeClient::onNumberInfo(EsphomeEntityPtr&& info)
{
    nums_.addInfo(std::move(info));
}

void NetEsphomeClient::onSelectInfo(EsphomeEntityPtr&& info)
{
    sels_.addInfo(std::move(info));
}

void NetEsphomeClient::onSensorInfo(EsphomeEntityPtr&& info)
{
    sensors_.addInfo(std::move(info));
}

void NetEsphomeClient::onDeviceInfo(const ceammc_esphome_device_info& info)
{
    DataTypeDict dict;

#define DICT_KEY(name) dict.insert(#name, gensym(info.name))
    DICT_KEY(name);
    DICT_KEY(mac_address);
    DICT_KEY(esphome_version);
    DICT_KEY(compilation_time);
    DICT_KEY(model);
    DICT_KEY(project_name);
    DICT_KEY(project_version);
    DICT_KEY(manufacturer);
    DICT_KEY(friendly_name);
    DICT_KEY(suggested_area);
    DICT_KEY(bluetooth_mac_address);
#undef DICT_KEY

    dict.insert("webserver_port", Atom(info.webserver_port));

    anyTo(0, gensym("device"), DataAtom<DataTypeDict>(std::move(dict)));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_switch_state& state)
{
    auto oid = switches_.setState(id, state);
    if (oid)
        anyTo(0, oid, Atom(state.value));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_text_state& state)
{
    const EsphomeTextState xstate { gensym(state.value), state.missing_state };
    auto oid = texts_.setState(id, xstate);
    if (oid)
        anyTo(0, oid, Atom(xstate.value));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_time_state& state)
{
    auto oid = time_.setState(id, state);
    if (oid) {
        AtomArray<3> data;
        data[0] = state.hour;
        data[1] = state.minute;
        data[2] = state.second;
        anyTo(0, oid, data.view());
    }
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_binary_state& state)
{
    auto oid = bins_.setState(id, state);
    if (oid)
        anyTo(0, oid, Atom(state.value));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_light_state& state)
{
    auto oid = lights_.setState(id, state);
    if (oid)
        anyTo(0, oid, Atom(state.state));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_number_state& state)
{
    auto oid = nums_.setState(id, state);
    if (oid)
        anyTo(0, oid, Atom(state.value));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_select_state& state)
{
    const EsphomeSelectState xstate { gensym(state.value), state.missing_state };
    auto oid = sels_.setState(id, xstate);
    if (oid)
        anyTo(0, oid, Atom(xstate.value));
}

void NetEsphomeClient::onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_sensor_state& state)
{
    auto oid = sensors_.setState(id, state);
    if (oid)
        anyTo(0, oid, Atom(state.value));
}

void setup_net_esphome_client()
{
    ObjectFactory<NetEsphomeClient> obj("net.esphome.client");

    obj.addMethod("connect", &NetEsphomeClient::m_connect);
    obj.addMethod("entities", &NetEsphomeClient::m_entities);
    obj.addMethod("get_time", &NetEsphomeClient::m_get_time);
    obj.addMethod("number", &NetEsphomeClient::m_number);
    obj.addMethod("ping", &NetEsphomeClient::m_ping);
    obj.addMethod("subscribe", &NetEsphomeClient::m_subscribe);
    obj.addMethod("switch", &NetEsphomeClient::m_switch);
    obj.addMethod("text", &NetEsphomeClient::m_text);
    obj.addMethod("time", &NetEsphomeClient::m_set_time);

    obj.addMethod("device?", &NetEsphomeClient::m_device_info);
}

} // namespace ceammc
#endif
