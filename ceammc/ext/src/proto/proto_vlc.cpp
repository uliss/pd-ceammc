#include "proto_vlc.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "parser_vlc.h"

// #include "httplib.h"

// #define VLC_BROWSE "/requests/browse.json"
// #define VLC_STATUS "/requests/status.json"
// #define VLC_PLAYLIST "/requests/playlist.json"

using opt_i16 = ceammc::proto_rust::Option<std::int16_t>;

ProtoVlc::ProtoVlc(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
    , host_(nullptr)
    , port_(nullptr)
    , pass_(nullptr)
    , vlc_ { nullptr, &ceammc_vlc_free }
{
    createOutlet();
    createOutlet();

    host_ = new SymbolProperty("@host", gensym("localhost"));
    host_->setArgIndex(0);
    addProperty(host_);

    port_ = new IntProperty("@port", 8080);
    port_->checkClosedRange(1, 1024 * 32);
    addProperty(port_);

    pass_ = new AtomProperty("@pass", Atom(&s_));
    addProperty(pass_);
}

void ProtoVlc::initDone()
{
    auto pass = to_string(pass_->value());
    vlc_.reset(ceammc_vlc_create( //
        host_->cstr(),
        port_->value(),
        pass.c_str(),
        { this, [](void* user, const char* msg) {} },
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }));
}

void ProtoVlc::m_play(t_symbol* s, const AtomListView& lv)
{
    opt_i16 id;
    if (lv.isInteger())
        id.set(lv[0].asInt());

    ceammc_vlc_play(vlc_.get(), id.get());
}

void ProtoVlc::m_stop(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_stop(vlc_.get());
}

void ProtoVlc::m_pause(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_pause(vlc_.get(), value.get());
}

void ProtoVlc::m_fullscreen(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_fullscreen(vlc_.get(), value.get());
}

void ProtoVlc::m_seek(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "TIME expected";
        return;
    }

    if (!parser::check_vlc_seek(lv[0])) {
        METHOD_ERR(s) << "invalid TIME: " << lv;
        return;
    }

    // VlcCommand cmd;
    // cmd.code = VLC_CMD_SEEK;
    // cmd.data = to_string(lv[0]);
    // sendCommand(s, cmd);
}

void ProtoVlc::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_clear(vlc_.get());
}

void ProtoVlc::m_next(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_next(vlc_.get());
}

void ProtoVlc::m_prev(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_prev(vlc_.get());
}

void ProtoVlc::m_sort(t_symbol* s, const AtomListView& lv)
{
    enum ceammc_vlc_sort_order order = ceammc_vlc_sort_order::Normal;
    AtomListView sort_order;
    if (lv.getProperty(gensym("@order"), sort_order)) {
        if (sort_order.asBool(false))
            order = ceammc_vlc_sort_order::Reversed;
    }

    auto sort_by_sym = &s_;
    AtomListView sort_by;
    if (lv.getProperty(gensym("@by"), sort_by)) {
        sort_by_sym = sort_by.asSymbol(&s_);
    }

    ceammc_vlc_sort(vlc_.get(), sort_by_sym->s_name, order);
}

void ProtoVlc::m_loop(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_loop(vlc_.get(), value.get());
}

void ProtoVlc::m_repeat(t_symbol* s, const AtomListView& lv)
{
    proto_rust::Option<bool> value;
    if (lv.isBool())
        value.set(lv.asBool(true));

    ceammc_vlc_repeat(vlc_.get(), value.get());
}

void ProtoVlc::m_volume(t_symbol* s, const AtomListView& lv)
{
    // VlcCommand cmd;
    // cmd.code = VLC_CMD_VOLUME;

    // auto conv = [](int x) -> int {
    //     return std::round(convert::lin2lin_clip<float, -100, 100>(x, -255, 255));
    // };

    // if (lv.isInteger()) {
    //     auto x = lv[0].asT<t_int>();
    //     if (x >= 0) { // absolute volume
    //         cmd.data = fmt::format("{}", conv(x));
    //     } else // relative negative
    //         cmd.data = fmt::format("-{}", conv(-x));
    // } else if (lv.isSymbol()) {
    //     auto str = lv[0].asT<t_symbol*>()->s_name;
    //     if (str[0] != '+' && str[0] != '-') {
    //         METHOD_ERR(s) << "VOL, +VOL or -VOL expected";
    //         return;
    //     }

    //     try {
    //         auto v = conv(std::stoi(str));
    //         if (v < 0)
    //             cmd.data = fmt::format("{}", v);
    //         else
    //             cmd.data = fmt::format("%2b{}", v);
    //     } catch (std::exception& e) {
    //         METHOD_ERR(s) << e.what();
    //         return;
    //     }
    // }

    // sendCommand(s, cmd);
}

void ProtoVlc::m_delete(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT))
        return;

    // VlcCommand cmd;
    // cmd.code = VLC_CMD_DELETE;
    // cmd.data = static_cast<int>(lv[0].asInt());

    // sendCommand(s, cmd);
}

void ProtoVlc::m_add(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    // VlcCommand cmd;
    // cmd.code = VLC_CMD_ADD;
    // cmd.data = to_string(lv);
    // sendCommand(s, cmd);
}

void ProtoVlc::m_status(t_symbol* s, const AtomListView& lv)
{
    ceammc_vlc_get_status(vlc_.get());
}

void ProtoVlc::m_playlist(t_symbol* s, const AtomListView& lv)
{
    // VlcCommand cmd;
    // cmd.code = VLC_CMD_PLAYLIST;
    // sendCommand(s, cmd);
}

void ProtoVlc::m_browse(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    // VlcCommand cmd;
    // cmd.code = VLC_CMD_BROWSE;
    // cmd.data = to_string(lv);
    // sendCommand(s, cmd);
}

bool ProtoVlc::notify(int code)
{
    return ceammc_vlc_poll(vlc_.get(),
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<ProtoVlc*>(user);
             Error(obj) << msg;
         } },
        { this, [](void* user, const ceammc_vlc_status* stat) {
             auto obj = static_cast<ProtoVlc*>(user);
             if (!obj)
                 return;

             DictAtom da;

             da->insert("apiversion", stat->apiversion);
             da->insert("loop", stat->has_loop);
             da->insert("repeat", stat->repeat);
             da->insert("random", stat->random);
             da->insert("fullscreen", stat->fullscreen);
             da->insert("position", stat->position);

             obj->atomTo(1, da);
         } });
}

// void ProtoVlc::processMessage(const VlcResponse& msg)
// {
//     try {
//         atomTo(1, DictAtom(msg.resp));
//     } catch (std::exception& e) {
//         OBJ_ERR << e.what();
//     }

//     enum {
//         HTTP_OK = 200,
//         HTTP_UNAUTH = 401,
//         HTTP_NOT_FOUND = 404,
//     };

//     switch (msg.status) {
//     case HTTP_OK:
//         return boolTo(0, true);
//     case HTTP_UNAUTH:
//         OBJ_ERR << "authorization error";
//         break;
//     case HTTP_NOT_FOUND:
//         OBJ_ERR << "not found";
//         break;
//     default:
//         OBJ_ERR << "unknown http code: " << msg.status;
//         break;
//     }

//     boolTo(0, false);
// }

// static std::string make_vlc_request(const VlcCommand& cmd)
// {
//     switch (cmd.code) {
//     case VLC_CMD_PLAY: {
//         auto id = boost::get<int>(&cmd.data);
//         if (id)
//             return fmt::format(VLC_STATUS "?command=pl_play&id={}", *id);
//         else
//             return VLC_STATUS "?command=pl_play";
//     }
//     case VLC_CMD_DELETE:
//         return fmt::format(VLC_STATUS "?command=pl_delete&id={}", boost::get<int>(cmd.data));
//     case VLC_CMD_STOP:
//         return VLC_STATUS "?command=pl_stop";
//     case VLC_CMD_NEXT:
//         return VLC_STATUS "?command=pl_next";
//     case VLC_CMD_PREV:
//         return VLC_STATUS "?command=pl_prev";
//     case VLC_CMD_PAUSE: {
//         auto on = boost::get<bool>(&cmd.data);
//         if (on && *on)
//             return VLC_STATUS "?command=pl_forcepause";
//         else
//             return VLC_STATUS "?command=pl_forceresume";
//     }
//     case VLC_CMD_FULLSCREEN:
//         return VLC_STATUS "?command=fullscreen";
//     case VLC_CMD_CLEAR:
//         return VLC_STATUS "?command=pl_empty";
//     case VLC_CMD_LOOP:
//         return VLC_STATUS "?command=pl_loop";
//     case VLC_CMD_VOLUME:
//         return fmt::format(VLC_STATUS "?command=volume&val={}", boost::get<std::string>(cmd.data));
//     case VLC_CMD_STATUS:
//         return VLC_STATUS;
//     case VLC_CMD_PLAYLIST:
//         return VLC_PLAYLIST;
//     case VLC_CMD_ADD:
//         return fmt::format(VLC_STATUS "?command=in_enqueue&input={}",
//             httplib::detail::encode_query_param(boost::get<std::string>(cmd.data)));
//     case VLC_CMD_BROWSE:
//         return fmt::format(VLC_BROWSE "?uri={}",
//             httplib::detail::encode_query_param(boost::get<std::string>(cmd.data)));
//     case VLC_CMD_SEEK:
//         return fmt::format(VLC_STATUS "?command=seek&val={}", boost::get<std::string>(cmd.data));
//     default:
//         return {};
//     }
// }

void setup_proto_vlc()
{
    ObjectFactory<ProtoVlc> obj("proto.vlc");

    obj.addMethod("play", &ProtoVlc::m_play);
    obj.addMethod("stop", &ProtoVlc::m_stop);
    obj.addMethod("pause", &ProtoVlc::m_pause);
    obj.addMethod("fs", &ProtoVlc::m_fullscreen);
    obj.addMethod("clear", &ProtoVlc::m_clear);
    obj.addMethod("next", &ProtoVlc::m_next);
    obj.addMethod("prev", &ProtoVlc::m_prev);
    obj.addMethod("loop", &ProtoVlc::m_loop);
    obj.addMethod("repeat", &ProtoVlc::m_repeat);

    obj.addMethod("volume", &ProtoVlc::m_volume);
    obj.addMethod("sort", &ProtoVlc::m_sort);
    obj.addMethod("delete", &ProtoVlc::m_delete);
    obj.addMethod("seek", &ProtoVlc::m_seek);

    obj.addMethod("add", &ProtoVlc::m_add);
    obj.addMethod("browse", &ProtoVlc::m_browse);
    obj.addMethod("playlist", &ProtoVlc::m_playlist);
    obj.addMethod("status", &ProtoVlc::m_status);
}
