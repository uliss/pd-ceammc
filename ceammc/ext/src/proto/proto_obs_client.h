/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_PROTO_OBS_CLIENT_H_
#define CEAMMC_PROTO_OBS_CLIENT_H_

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "proto_rust_struct.hpp"

#include <memory>

using namespace ceammc;

using BaseOBSClient = DispatchedObject<BaseObject>;
using ObsClientImpl = proto::ProtoService<ceammc_obs_client, ceammc_obs_init, ceammc_obs_result_cb>;

namespace ceammc {

class ProtoObsClient : public BaseOBSClient {
    std::unique_ptr<ObsClientImpl> cli_; // should be accessed only in worker thread
    IntProperty* port_ { nullptr };
    SymbolProperty* host_ { nullptr };
    SymbolProperty* passwd_ { nullptr };

public:
    explicit ProtoObsClient(const PdArgs& args);

    bool notify(int code) final;

    void m_info(t_symbol* s, const AtomListView& lv);
    void m_connect(t_symbol* s, const AtomListView& lv);
    void m_disconnect(t_symbol* s, const AtomListView& lv);
    void m_scenes(t_symbol* s, const AtomListView& lv);
    void m_current(t_symbol* s, const AtomListView& lv);
    void m_monitors(t_symbol* s, const AtomListView& lv);
    void m_items(t_symbol* s, const AtomListView& lv);
    void m_collection(t_symbol* s, const AtomListView& lv);

private:
    void processCollectionList(const ceammc_obs_collection_list* coll);
    void processConnect(bool state);
    void processCurrentScene(const char* scene);
    void processCurrentConnection(const char* scene);
    void processItemsList(const ceammc_obs_scene_item_list* items);
    void processMonitorList(const ceammc_obs_monitor_list* mons);
    void processSceneList(const ceammc_obs_scene_list* scl);
    void processVersion(const ceammc_obs_version* ver);
};

} // namespace ceammc

void setup_proto_obs_client();

#endif // CEAMMC_PROTO_OBS_CLIENT_H_
