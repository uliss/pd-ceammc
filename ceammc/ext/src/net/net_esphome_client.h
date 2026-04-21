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
#include "net_rust.hpp"

#include <functional>
#include <unordered_map>

namespace ceammc {

struct EsphomeEntityIdHash {
    std::size_t operator()(const ceammc_esphome_entity_id& id) const;
};

struct EsphomeEntityEqual {
    bool operator()(const ceammc_esphome_entity_id& a, const ceammc_esphome_entity_id& b) const;
};

class EsphomeEntityBase;
using EsphomeEntityPtr = std::unique_ptr<EsphomeEntityBase>;
class EsphomeEntityBase {
    ceammc_esphome_entity_id id_ {};
    t_symbol* object_id_ { &s_ };
    EsphomeEntityBase& operator=(const EsphomeEntityBase&) = delete;
    EsphomeEntityBase& operator=(EsphomeEntityBase&&) = delete;
    EsphomeEntityBase(EsphomeEntityBase&&) = delete;

public:
    EsphomeEntityBase(const ceammc_esphome_entity_id& id, const char* object_id);
    EsphomeEntityBase(const EsphomeEntityBase& x);
    virtual ~EsphomeEntityBase() = default;

    t_symbol* object_id() { return object_id_; }
    const ceammc_esphome_entity_id& id() const { return id_; }
    virtual EsphomeEntityPtr clone() const = 0;
};

template <typename State>
struct EsphomeEntityState {
    t_symbol* object_id { &s_ };
    State state;
};

struct EsphomeTextState {
    t_symbol* value { &s_ };
    bool missing_value {};
};

struct EsphomeSelectState {
    t_symbol* value { &s_ };
    bool missing_value {};
};

template <typename State>
struct EsphomeEntities {
    std::unordered_map<t_symbol*, EsphomeEntityPtr> entities;
    std::unordered_map<ceammc_esphome_entity_id, EsphomeEntityState<State>, EsphomeEntityIdHash, EsphomeEntityEqual> states;
    std::unordered_map<ceammc_esphome_entity_id, t_symbol*, EsphomeEntityIdHash, EsphomeEntityEqual> ids;

    void addInfo(EsphomeEntityPtr&& info)
    {
        auto obj_id = info->object_id();
        auto id = info->id();
        entities.insert({ obj_id, std::move(info) });
        ids.insert({ id, obj_id });
    }

    t_symbol* setState(const ceammc_esphome_entity_id& id, const State& state)
    {
        auto it = states.find(id);
        if (it == states.end()) {
            auto oid = ids.find(id);
            if (oid == ids.end())
                return nullptr;

            states.insert({ id, { oid->second, state } });
            return oid->second;
        } else {
            it->second.state = state;
            return it->second.object_id;
        }
    }

    const ceammc_esphome_entity_id* findId(t_symbol* object_id) const
    {
        auto it = entities.find(object_id);
        return it == entities.end() ? nullptr : &it->second->id();
    }

    template <typename Info>
    const Info* findInfo(t_symbol* object_id) const
    {
        auto it = entities.find(object_id);
        return it == entities.end() ? nullptr : dynamic_cast<const Info*>(it->second.get());
    }

    void foreachEntity(const std::function<void(t_symbol*, const EsphomeEntityPtr&)>& fn) const
    {
        for (auto& e : entities)
            fn(e.first, e.second);
    }

    void clear()
    {
        entities.clear();
        states.clear();
        ids.clear();
    }
};

class NetEsphomeClient : public RustFfiObject<BaseObject, ceammc_esphome_client> {
    SymbolProperty* addr_ { nullptr };
    IntProperty* port_ { nullptr };

    EsphomeEntities<ceammc_esphome_binary_state> bins_;
    EsphomeEntities<ceammc_esphome_number_state> nums_;
    EsphomeEntities<EsphomeSelectState> sels_;
    EsphomeEntities<ceammc_esphome_sensor_state> sensors_;
    EsphomeEntities<ceammc_esphome_switch_state> switches_;
    EsphomeEntities<ceammc_esphome_time_state> time_;
    EsphomeEntities<EsphomeTextState> texts_;

public:
    explicit NetEsphomeClient(const PdArgs& args);

    bool notify(int code) final;
    void dump() const final;

    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_entities(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_number(t_symbol* s, const AtomListView& lv);
    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_switch(t_symbol* s, const AtomListView& lv);
    void m_text(t_symbol* s, const AtomListView& lv);
    void m_device_info(t_symbol* s, const AtomListView& lv);
    void m_get_time(t_symbol* s, const AtomListView& lv);
    void m_set_time(t_symbol* s, const AtomListView& lv);

public:
    void onBinaryInfo(EsphomeEntityPtr&& info);
    void onNumberInfo(EsphomeEntityPtr&& info);
    void onSelectInfo(EsphomeEntityPtr&& info);
    void onSensorInfo(EsphomeEntityPtr&& info);
    void onSwitchInfo(EsphomeEntityPtr&& info);
    void onTextInfo(EsphomeEntityPtr&& info);
    void onTimeInfo(EsphomeEntityPtr&& info);

    void onDeviceInfo(const ceammc_esphome_device_info& info);

    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_binary_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_number_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_select_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_sensor_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_switch_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_text_state& state);
    void onState(const ceammc_esphome_entity_id& id, const ceammc_esphome_time_state& state);
};

void setup_net_esphome_client();

} // namespace ceammc

#endif // NET_ESPHOME_CLIENT_H
