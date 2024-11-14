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
#include "proto_obs_client.h"
#include "args/argcheck.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

namespace ceammc {

ProtoObsClient::ProtoObsClient(const PdArgs& args)
    : BaseOBSClient(args)
{
    port_ = new IntProperty("@port", 4455);
    port_->checkClosedRange(1, std::numeric_limits<std::int16_t>::max());
    addProperty(port_);

    host_ = new SymbolProperty("@host", gensym("localhost"));
    addProperty(host_);

    passwd_ = new SymbolProperty("@passwd", &s_);
    addProperty(passwd_);

    createOutlet();
}

bool ProtoObsClient::notify(int code)
{
    if (cli_)
        cli_->processResults();

    return true;
}

void ProtoObsClient::m_info(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        METHOD_ERR(s) << "not connected";
        return;
    }

    ceammc_obs_get_version(cli_->handle());
}

void ProtoObsClient::m_connect(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PASSWD:s?");
    if (!chk.check(lv, this))
        return;

#define PROC_FN(ptr, fn, ...)                           \
    {                                                   \
        auto this_ = static_cast<ProtoObsClient*>(ptr); \
        if (this_)                                      \
            this_->fn(__VA_ARGS__);                     \
    }

    cli_.reset(new ObsClientImpl(
        {
            host_->value()->s_name,
            lv.symbolAt(0, passwd_->value())->s_name,
            static_cast<uint16_t>(port_->value()),
        },
        ceammc_obs_new,
        ceammc_obs_free,
        ceammc_obs_process_events,
        ceammc_obs_result_cb {
            this,
            [](void* user, const ceammc_obs_version* ver) {
                PROC_FN(user, processVersion, ver);
            },
            [](void* user, const ceammc_obs_collection_list* coll) {
                PROC_FN(user, processCollectionList, coll);
            },
            [](void* user, const ceammc_obs_scene_list* scl) {
                PROC_FN(user, processSceneList, scl);
            },
            [](void* user, const ceammc_obs_scene_item_list* items) {
                PROC_FN(user, processItemsList, items);
            },
            [](void* user, const ceammc_obs_monitor_list* mons) {
                PROC_FN(user, processMonitorList, mons);
            },
            [](void* user, const char* name) {
                PROC_FN(user, processCurrentConnection, name);
            },
            [](void* user, const char* name) {
                PROC_FN(user, processCurrentScene, name);
            },
            [](void* user, bool state) {
                PROC_FN(user, processConnect, state);
            },
        },
        ceammc_callback_notify { reinterpret_cast<size_t>(this), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
}

void ProtoObsClient::m_disconnect(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    cli_.release();
}

void ProtoObsClient::m_scenes(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    if (lv.size() >= 2 && lv[0] == "current" && lv[1] == "get") {
        ceammc_obs_get_current_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "next") {
        ceammc_obs_next_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "prev") {
        ceammc_obs_prev_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "first") {
        ceammc_obs_first_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "last") {
        ceammc_obs_last_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "create") {
        static const args::ArgChecker chk_create("s=create NAME:s");

        if (chk_create.check(lv, this))
            ceammc_obs_create_scene(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (!lv.empty() && lv[0] == "remove") {
        static const args::ArgChecker chk_create("s=remove NAME:s");

        if (chk_create.check(lv, this))
            ceammc_obs_remove_scene(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (lv.empty() || lv[0] == &s_list) {
        ceammc_obs_list_scenes(cli_->handle());
    }
}

void ProtoObsClient::m_current(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    ceammc_obs_set_current_scene(cli_->handle(), lv.symbolAt(0, &s_)->s_name);
}

void ProtoObsClient::m_monitors(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    ceammc_obs_list_monitors(cli_->handle());
}

void ProtoObsClient::m_items(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    if (lv.size() >= 1 && lv[0] == &s_list) {
        ceammc_obs_list_scene_items(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (lv.size() >= 1 && lv[0] == "enable") {
        ceammc_obs_enable_scene_item(cli_->handle(),
            lv.symbolAt(1, &s_)->s_name,
            lv.intAt(2, 0),
            lv.boolAt(3, true));
    } else if (lv.size() >= 1 && lv[0] == "remove") {
        ceammc_obs_remove_scene_item(cli_->handle(),
            lv.symbolAt(1, &s_)->s_name,
            lv.intAt(2, 0));
    }
}

void ProtoObsClient::m_collection(t_symbol* s, const AtomListView& lv)
{
    if (!cli_) {
        OBJ_ERR << "not connected";
        return;
    }

    if (lv.size() >= 1 && lv[0] == &s_list) {
        ceammc_obs_list_collections(cli_->handle());
    } else if (lv.size() >= 1 && lv[0] == "current") {
        ceammc_obs_set_current_collection(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    }
}

void ProtoObsClient::processCollectionList(const ceammc_obs_collection_list* coll)
{
    auto current = ceammc_obs_get_collection_current(coll);
    anyTo(0, gensym("collection"), AtomList { gensym("current"), gensym(current) });

    auto N = ceammc_obs_get_collection_count(coll);
    AtomList res;
    res.reserve(N + 1);
    res.push_back(&s_list);
    for (size_t i = 0; i < N; i++) {
        auto c = ceammc_obs_get_collection_at(coll, i);
        if (!c)
            continue;

        res.append(gensym(c));
    }

    anyTo(0, gensym("collection"), res);
}

void ProtoObsClient::processVersion(const ceammc_obs_version* ver)
{
    {
        uint64_t maj, min, patch;
        ceammc_obs_version_server(ver, &maj, &min, &patch);
        AtomArray<4> msg { gensym("obs"), maj, min, patch };
        anyTo(0, gensym("version"), msg.view());
    }

    {
        uint64_t maj, min, patch;
        ceammc_obs_version_websocket(ver, &maj, &min, &patch);
        AtomArray<4> msg { gensym("obs"), maj, min, patch };
        anyTo(0, gensym("version"), msg.view());
    }

    {
        AtomArray<2> msg { gensym("rpc"), ceammc_obs_version_rpc(ver) };
        anyTo(0, gensym("version"), msg.view());
    }

    {
        AtomArray<2> msg { gensym("platform"), gensym(ceammc_obs_version_platform(ver)) };
        anyTo(0, gensym("version"), msg.view());
    }

    {
        AtomArray<2> msg { gensym("platform_desc"), gensym(ceammc_obs_version_platform_desc(ver)) };
        anyTo(0, gensym("version"), msg.view());
    }

    {
        AtomList image_fmts;
        auto fmt_count = ceammc_obs_version_image_fmt_num(ver);
        image_fmts.reserve(fmt_count + 1);
        image_fmts.append(gensym("image_formats"));
        for (size_t i = 0; i < fmt_count; i++) {
            image_fmts.append(gensym(ceammc_obs_version_image_fmt_at(ver, i)));
        }

        anyTo(0, gensym("info"), image_fmts);
    }
}

void ProtoObsClient::processSceneList(const ceammc_obs_scene_list* scl)
{
    auto sc_cur = ceammc_obs_scene_current(scl);
    anyTo(0, gensym("current"), Atom(gensym(ceammc_obs_scene_name(sc_cur))));

    AtomList scenes;
    for (size_t i = 0; i < ceammc_obs_scene_list_length(scl); i++) {
        auto sc = ceammc_obs_scene_list_at(scl, i);
        if (sc)
            scenes.append(gensym(ceammc_obs_scene_name(sc)));
    }

    anyTo(0, gensym("scenes"), scenes);
}

void ProtoObsClient::processItemsList(const ceammc_obs_scene_item_list* items)
{
    for (size_t i = 0; i < ceammc_obs_get_scene_item_count(items); i++) {
        auto item = ceammc_obs_get_scene_item_at(items, i);
        if (item) {
            auto name = ceammc_obs_get_scene_item_name(item);
            auto idx = ceammc_obs_get_scene_item_index(item);
            auto type = ceammc_obs_get_scene_item_type(item);
            auto input_kind = ceammc_obs_get_scene_item_input_kind(item);
            AtomArray<4> msg { idx, gensym(type), gensym(input_kind), gensym(name) };
            anyTo(0, gensym("item"), msg.view());
        }
    }
}

void ProtoObsClient::processConnect(bool state)
{
    anyTo(0, gensym("connected"), Atom(state));
}

void ProtoObsClient::processMonitorList(const ceammc_obs_monitor_list* mons)
{
    auto N = ceammc_obs_monitor_count(mons);
    for (size_t i = 0; i < N; i++) {
        auto m = ceammc_obs_monitor_at(mons, i);
        if (!m)
            break;

        uint16_t x, y, w, h;
        ceammc_obs_monitor_geom(m, &x, &y, &w, &h);

        const AtomArray<6> msg {
            ceammc_obs_monitor_index(m),
            w, h, x, y,
            gensym(ceammc_obs_monitor_name(m))
        };
        anyTo(0, gensym("monitor"), msg.view());
    }
}

void ProtoObsClient::processCurrentScene(const char* scene)
{
    AtomArray<2> msg { gensym("current"), gensym(scene) };
    anyTo(0, gensym("scene"), msg.view());
}

void ProtoObsClient::processCurrentConnection(const char* scene)
{
    AtomArray<2> msg { gensym("current"), gensym(scene) };
    anyTo(0, gensym("collection"), msg.view());
}

} // namespace ceammc

void setup_proto_obs_client()
{
    ObjectFactory<ProtoObsClient> obj("proto.obs");
    obj.addMethod("collection", &ProtoObsClient::m_collection);
    obj.addMethod("connect", &ProtoObsClient::m_connect);
    obj.addMethod("current", &ProtoObsClient::m_current);
    obj.addMethod("disconnect", &ProtoObsClient::m_disconnect);
    obj.addMethod("info", &ProtoObsClient::m_info);
    obj.addMethod("items", &ProtoObsClient::m_items);
    obj.addMethod("monitors", &ProtoObsClient::m_monitors);
    obj.addMethod("scene", &ProtoObsClient::m_scenes);
}
