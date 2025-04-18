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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM(app)
CEAMMC_DEFINE_SYM(collection)
CEAMMC_DEFINE_SYM(connected)
CEAMMC_DEFINE_SYM(current)
CEAMMC_DEFINE_SYM(image_formats)
CEAMMC_DEFINE_SYM(info)
CEAMMC_DEFINE_SYM(localhost)
CEAMMC_DEFINE_SYM(monitor)
CEAMMC_DEFINE_SYM(platform)
CEAMMC_DEFINE_SYM(platform_desc)
CEAMMC_DEFINE_SYM(remove)
CEAMMC_DEFINE_SYM(rpc)
CEAMMC_DEFINE_SYM(scene)
CEAMMC_DEFINE_SYM(version)
CEAMMC_DEFINE_SYM(ws)

#define CHECK_CONNECT(cli, m)             \
    if (!cli || !cli->handle()) {         \
        METHOD_ERR(m) << "not connected"; \
        return;                           \
    }

namespace ceammc {

ProtoObsClient::ProtoObsClient(const PdArgs& args)
    : BaseOBSClient(args)
{
    port_ = new IntProperty("@port", 4455);
    port_->checkClosedRange(1, std::numeric_limits<std::int16_t>::max());
    addProperty(port_);

    host_ = new SymbolProperty("@host", sym_localhost());
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
    CHECK_CONNECT(cli_, s);

    ceammc_obs_request_version(cli_->handle());
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
            [](void* user, const ceammc_obs_info* info) {
                PROC_FN(user, processInfo, info);
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

    if (!cli_->handle()) {
        cli_.reset();
        msg_connected(false);
        return;
    }

    cli_->setErrorCallback([this](const char* msg) { OBJ_ERR << msg; });
    cli_->setDebugCallback([this](const char* msg) { OBJ_DBG << msg; });
}

void ProtoObsClient::m_disconnect(t_symbol* s, const AtomListView& lv)
{
    CHECK_CONNECT(cli_, s);

    cli_.reset();
    msg_connected(false);
}

void ProtoObsClient::m_scene(t_symbol* s, const AtomListView& lv)
{
    CHECK_CONNECT(cli_, s);

    if (lv.size() >= 2 && lv[0] == "current" && lv[1] == "get") { // scene current get
        ceammc_obs_request_current_scene(cli_->handle());
    } else if (lv.size() >= 2 && lv[0] == "current" && lv[1] == "set") { // scene current set
        ceammc_obs_set_current_scene(cli_->handle(), lv.symbolAt(2, &s_)->s_name);
    } else if (!lv.empty() && lv[0] == "next") { // scene next
        ceammc_obs_next_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "prev") { // scene prev
        ceammc_obs_prev_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "first") { // scene first
        ceammc_obs_first_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "last") { // scene last
        ceammc_obs_last_scene(cli_->handle());
    } else if (!lv.empty() && lv[0] == "create") { // scene create
        static const args::ArgChecker chk_create("s=create NAME:s");

        if (chk_create.check(lv, this))
            ceammc_obs_create_scene(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (!lv.empty() && lv[0] == "remove") { // scene remove
        static const args::ArgChecker chk_create("s=remove NAME:s");

        if (chk_create.check(lv, this))
            ceammc_obs_remove_scene(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (lv.empty() || lv[0] == "list") { // scene list
        ceammc_obs_list_scenes(cli_->handle());
    }
}

void ProtoObsClient::m_monitor(t_symbol* s, const AtomListView& lv)
{
    CHECK_CONNECT(cli_, s);

    if (lv.size() > 0 || lv[0] == "list")
        ceammc_obs_list_monitors(cli_->handle());
}

void ProtoObsClient::m_item(t_symbol* s, const AtomListView& lv)
{
    CHECK_CONNECT(cli_, s);

    if (lv.size() >= 1 && lv[0] == "list") { // item list SCENE
        ceammc_obs_list_scene_items(cli_->handle(), lv.symbolAt(1, &s_)->s_name);
    } else if (lv.size() >= 1 && lv[0] == "enable") { // item enable SCENE IDX VALUE?
        ceammc_obs_enable_scene_item(cli_->handle(),
            lv.symbolAt(1, &s_)->s_name,
            lv.intAt(2, 0),
            lv.boolAt(3, true));
    } else if (lv.size() >= 1 && lv[0] == "remove") { // item remove SCENE IDX
        ceammc_obs_remove_scene_item(cli_->handle(),
            lv.symbolAt(1, &s_)->s_name,
            lv.intAt(2, 0));
    }
}

void ProtoObsClient::m_collection(t_symbol* s, const AtomListView& lv)
{
    CHECK_CONNECT(cli_, s);

    if (lv.size() >= 1 && lv[0] == "list") { // collection list
        ceammc_obs_list_collections(cli_->handle());
    } else if (lv.size() >= 1 && lv[0] == "current" && lv[1] == "set") { // collection current set SCENE
        ceammc_obs_set_current_collection(cli_->handle(), lv.symbolAt(2, &s_)->s_name);
    } else if (lv.size() >= 1 && lv[0] == "current" && lv[1] == "get") { // collection current get
        ceammc_obs_request_current_collection(cli_->handle());
    }
}

void ProtoObsClient::processCollectionList(const ceammc_obs_collection_list* coll)
{
    auto current = ceammc_obs_get_collection_current(coll);
    anyTo(0, sym_collection(), AtomList { sym_current(), gensym(current) });

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

    anyTo(0, sym_collection(), res);
}

void ProtoObsClient::processInfo(const ceammc_obs_info* info)
{
    {
        uint64_t maj, min, patch;
        ceammc_obs_get_app_version(info, &maj, &min, &patch);
        AtomArray<4> msg { sym_app(), maj, min, patch };
        anyTo(0, sym_version(), msg.view());
    }

    {
        uint64_t maj, min, patch;
        ceammc_obs_get_websocket_version(info, &maj, &min, &patch);
        AtomArray<4> msg { sym_ws(), maj, min, patch };
        anyTo(0, sym_version(), msg.view());
    }

    {
        AtomArray<2> msg { sym_rpc(), ceammc_obs_get_rpc_version(info) };
        anyTo(0, sym_version(), msg.view());
    }

    {
        AtomArray<2> msg { sym_platform(), gensym(ceammc_obs_get_platform(info)) };
        anyTo(0, sym_version(), msg.view());
    }

    {
        AtomArray<2> msg { sym_platform_desc(), gensym(ceammc_obs_get_platform_desc(info)) };
        anyTo(0, sym_version(), msg.view());
    }

    {
        AtomList image_fmts;
        auto N = ceammc_obs_get_image_format_count(info);
        image_fmts.reserve(N + 1);
        image_fmts.append(sym_image_formats());
        for (size_t i = 0; i < N; i++) {
            image_fmts.append(gensym(ceammc_obs_get_image_format_at(info, i)));
        }

        anyTo(0, sym_info(), image_fmts);
    }
}

void ProtoObsClient::msg_connected(bool value)
{
    anyTo(0, sym_connected(), Atom(value));
}

void ProtoObsClient::processSceneList(const ceammc_obs_scene_list* scl)
{
    {
        auto sc_cur = ceammc_obs_scene_current(scl);
        auto name = ceammc_obs_get_scene_name(sc_cur);
        AtomArray<2> msg { sym_current(), gensym(name) };
        anyTo(0, sym_scene(), msg.view());
    }

    auto N = ceammc_obs_get_scene_count(scl);
    AtomList scenes;
    scenes.reserve(N + 1);
    scenes.push_back(&s_list);

    for (size_t i = 0; i < N; i++) {
        auto sc = ceammc_obs_get_scene_at(scl, i);
        if (sc)
            scenes.append(gensym(ceammc_obs_get_scene_name(sc)));
    }

    anyTo(0, sym_scene(), scenes);
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
    msg_connected(state);
}

void ProtoObsClient::processMonitorList(const ceammc_obs_monitor_list* mons)
{
    auto N = ceammc_obs_get_monitor_count(mons);
    for (size_t i = 0; i < N; i++) {
        auto m = ceammc_obs_get_monitor_at(mons, i);
        if (!m)
            break;

        uint16_t x, y, w, h;
        ceammc_obs_get_monitor_geom(m, &x, &y, &w, &h);

        const AtomArray<6> msg {
            ceammc_obs_get_monitor_index(m),
            w, h, x, y,
            gensym(ceammc_obs_get_monitor_name(m))
        };
        anyTo(0, sym_monitor(), msg.view());
    }
}

void ProtoObsClient::processCurrentScene(const char* scene)
{
    AtomArray<2> msg { sym_current(), gensym(scene) };
    anyTo(0, sym_scene(), msg.view());
}

void ProtoObsClient::processCurrentConnection(const char* scene)
{
    AtomArray<2> msg { sym_current(), gensym(scene) };
    anyTo(0, sym_collection(), msg.view());
}

} // namespace ceammc

void setup_proto_obs_client()
{
    ObjectFactory<ProtoObsClient> obj("proto.obs");
    obj.addMethod("collection", &ProtoObsClient::m_collection);
    obj.addMethod("connect", &ProtoObsClient::m_connect);
    obj.addMethod("disconnect", &ProtoObsClient::m_disconnect);
    obj.addMethod("info", &ProtoObsClient::m_info);
    obj.addMethod("item", &ProtoObsClient::m_item);
    obj.addMethod("monitor", &ProtoObsClient::m_monitor);
    obj.addMethod("scene", &ProtoObsClient::m_scene);
}
