#include "proto_vlc.h"
#include "ceammc_args.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"

#include "fmt/format.h"
#include "httplib.h"

#define VLC_BROWSE "/requests/browse.json"
#define VLC_STATUS "/requests/status.json"
#define VLC_PLAYLIST "/requests/playlist.json"

ProtoVlc::ProtoVlc(const PdArgs& args)
    : ProtoVlcBase(args)
    , host_(nullptr)
    , port_(nullptr)
    , pass_(nullptr)
    , logger_ {}
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

    Dispatcher::instance().subscribe(this, subscriberId());
}

void ProtoVlc::m_play(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_PLAY;
    if (lv.size() == 1 && lv[0].isInteger())
        cmd.data = lv[0].asT<int>();

    sendCommand(s, cmd);
}

void ProtoVlc::m_stop(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_STOP;
    sendCommand(s, cmd);
}

void ProtoVlc::m_pause(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_PAUSE;
    cmd.data = lv.boolAt(0, true);
    sendCommand(s, cmd);
}

void ProtoVlc::m_fullscreen(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_FULLSCREEN;
    cmd.data = lv.boolAt(0, true);

    sendCommand(s, cmd);
}

void ProtoVlc::m_seek(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_SEEK;
    sendCommand(s, cmd);
}

void ProtoVlc::m_clear(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_CLEAR;
    cmd.data = lv.boolAt(0, true);

    sendCommand(s, cmd);
}

void ProtoVlc::m_next(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_NEXT;

    sendCommand(s, cmd);
}

void ProtoVlc::m_prev(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_NEXT;

    sendCommand(s, cmd);
}

void ProtoVlc::m_loop(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_LOOP;

    sendCommand(s, cmd);
}

void ProtoVlc::m_volume(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_VOLUME;

    auto conv = [](int x) -> int {
        return std::round(convert::lin2lin_clip<float, -100, 100>(x, -255, 255));
    };

    if (lv.isInteger()) {
        auto x = lv[0].asT<int>();
        if (x >= 0) { // absolute volume
            cmd.data = fmt::format("{}", conv(x));
        } else // relative negative
            cmd.data = fmt::format("-{}", conv(-x));
    } else if (lv.isSymbol()) {
        auto str = lv[0].asT<t_symbol*>()->s_name;
        if (str[0] != '+' && str[0] != '-') {
            METHOD_ERR(s) << "VOL, +VOL or -VOL expected";
            return;
        }

        try {
            auto v = conv(std::stoi(str));
            if (v < 0)
                cmd.data = fmt::format("{}", v);
            else
                cmd.data = fmt::format("%2b{}", v);
        } catch (std::exception& e) {
            METHOD_ERR(s) << e.what();
            return;
        }
    }

    sendCommand(s, cmd);
}

void ProtoVlc::m_delete(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT))
        return;

    VlcCommand cmd;
    cmd.code = VLC_CMD_DELETE;
    cmd.data = lv[0].asInt();

    sendCommand(s, cmd);
}

void ProtoVlc::m_add(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    VlcCommand cmd;
    cmd.code = VLC_CMD_ADD;
    cmd.data = to_string(lv);
    sendCommand(s, cmd);
}

void ProtoVlc::m_status(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_STATUS;
    sendCommand(s, cmd);
}

void ProtoVlc::m_playlist(t_symbol* s, const AtomListView& lv)
{
    VlcCommand cmd;
    cmd.code = VLC_CMD_PLAYLIST;
    sendCommand(s, cmd);
}

void ProtoVlc::m_browse(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "URI expected";
        return;
    }

    VlcCommand cmd;
    cmd.code = VLC_CMD_BROWSE;
    cmd.data = to_string(lv);
    sendCommand(s, cmd);
}

void ProtoVlc::processMessage(const VlcResponse& msg)
{
    try {
        atomTo(1, DictAtom(msg.resp));
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }

    enum {
        HTTP_OK = 200,
        HTTP_UNAUTH = 401,
        HTTP_NOT_FOUND = 404,
    };

    switch (msg.status) {
    case HTTP_OK:
        return boolTo(0, true);
    case HTTP_UNAUTH:
        OBJ_ERR << "authorization error";
        break;
    case HTTP_NOT_FOUND:
        OBJ_ERR << "not found";
        break;
    default:
        OBJ_ERR << "unknown http code: " << msg.status;
        break;
    }

    boolTo(0, false);
}

static std::string make_vlc_request(const VlcCommand& cmd)
{
    switch (cmd.code) {
    case VLC_CMD_PLAY: {
        auto id = boost::get<int>(&cmd.data);
        if (id)
            return fmt::format(VLC_STATUS "?command=pl_play&id={}", *id);
        else
            return VLC_STATUS "?command=pl_play";
    }
    case VLC_CMD_DELETE:
        return fmt::format(VLC_STATUS "?command=pl_delete&id={}", boost::get<int>(cmd.data));
    case VLC_CMD_STOP:
        return VLC_STATUS "?command=pl_stop";
    case VLC_CMD_NEXT:
        return VLC_STATUS "?command=pl_next";
    case VLC_CMD_PREV:
        return VLC_STATUS "?command=pl_prev";
    case VLC_CMD_PAUSE: {
        auto on = boost::get<bool>(&cmd.data);
        if (on && *on)
            return VLC_STATUS "?command=pl_forcepause";
        else
            return VLC_STATUS "?command=pl_forceresume";
    }
    case VLC_CMD_FULLSCREEN:
        return VLC_STATUS "?command=fullscreen";
    case VLC_CMD_CLEAR:
        return VLC_STATUS "?command=pl_empty";
    case VLC_CMD_LOOP:
        return VLC_STATUS "?command=pl_loop";
    case VLC_CMD_VOLUME:
        return fmt::format(VLC_STATUS "?command=volume&val={}", boost::get<std::string>(cmd.data));
    case VLC_CMD_STATUS:
        return VLC_STATUS;
    case VLC_CMD_PLAYLIST:
        return VLC_PLAYLIST;
    case VLC_CMD_ADD:
        return fmt::format(VLC_STATUS "?command=in_enqueue&input={}",
            httplib::detail::encode_query_param(boost::get<std::string>(cmd.data)));
    case VLC_CMD_BROWSE:
        return fmt::format(VLC_BROWSE "?uri={}",
            httplib::detail::encode_query_param(boost::get<std::string>(cmd.data)));
    }
}

static VlcResponse make_vlc_response(const VlcCommand& cmd, const httplib::Result& res)
{
    try {
        DataTypeDict dict;
        dict.fromJSON(res->body);
        return { dict, res->status };
    } catch (std::exception& e) {
        return { {}, -1 };
    }
}

ProtoVlc::Future ProtoVlc::createTask()
{
    return std::async(std::launch::async, [this]() {
        try {
            VlcCommand cmd;

            while (inPipe().try_dequeue(cmd)) {
                httplib::Client cli(host_->value()->s_name, port_->value());
                cli.set_basic_auth("", to_string(pass_->value()).c_str());
                cli.set_connection_timeout(1);
                cli.set_tcp_nodelay(true);
                cli.set_keep_alive(false);
                auto req = make_vlc_request(cmd);
                logger_.debug(req);
                auto res = cli.Get(req.c_str());
                if (res) {
                    outPipe().enqueue(make_vlc_response(cmd, res));
                } else {
                    logger_.error(fmt::format("http request error: '{}'", to_string(res.error())));
                }
            }

            Dispatcher::instance().send({ subscriberId(), NOTIFY_UPDATE });

        } catch (std::exception& e) {
            logger_.error(fmt::format("http request exception: '{}'", e.what()));
        }
    });
}

void ProtoVlc::sendCommand(t_symbol* m, const VlcCommand& cmd)
{
    if (!inPipe().try_enqueue(std::move(cmd))) {
        METHOD_ERR(m) << "can't send command to worker";
        return;
    }

    if (!runTask())
        METHOD_ERR(m) << "can't run worker";
}

void setup_proto_vlc()
{
    ObjectFactory<ProtoVlc> obj("proto.vlc");

    obj.addMethod("play", &ProtoVlc::m_play);
    obj.addMethod("stop", &ProtoVlc::m_stop);
    obj.addMethod("pause", &ProtoVlc::m_pause);
    obj.addMethod("fs", &ProtoVlc::m_fullscreen);
    obj.addMethod("seek", &ProtoVlc::m_seek);
    obj.addMethod("clear", &ProtoVlc::m_clear);
    obj.addMethod("next", &ProtoVlc::m_next);
    obj.addMethod("prev", &ProtoVlc::m_prev);
    obj.addMethod("delete", &ProtoVlc::m_delete);
    obj.addMethod("loop", &ProtoVlc::m_loop);
    obj.addMethod("volume", &ProtoVlc::m_volume);

    obj.addMethod("add", &ProtoVlc::m_add);
    obj.addMethod("browse", &ProtoVlc::m_browse);
    obj.addMethod("playlist", &ProtoVlc::m_playlist);
    obj.addMethod("status", &ProtoVlc::m_status);
}
