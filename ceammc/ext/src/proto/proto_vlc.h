#ifndef PROTO_VLC_H
#define PROTO_VLC_H

#include "ceammc_pollthread_object.h"
#include "ceammc_thread.h"
#include "datatype_dict.h"
using namespace ceammc;

#include <boost/variant.hpp>

enum VlcCommandCode {
    VLC_CMD_PLAY,
    VLC_CMD_STOP,
    VLC_CMD_PAUSE,
    VLC_CMD_FULLSCREEN,
    VLC_CMD_SEEK,
    VLC_CMD_CLEAR,
    VLC_CMD_NEXT,
    VLC_CMD_PREV,
    VLC_CMD_DELETE,
    VLC_CMD_ADD,
    VLC_CMD_LOOP,
    VLC_CMD_VOLUME,
    VLC_CMD_STATUS,
    VLC_CMD_PLAYLIST,
    VLC_CMD_BROWSE,
};

using VlcCommandData = boost::variant<bool, float, int, std::string>;

struct VlcCommand {
    VlcCommandCode code;
    VlcCommandData data;
};

struct VlcResponse {
    DataTypeDict resp;
    int status;
};

using ProtoVlcBase = PollThreadQueueObject<VlcCommand, VlcResponse>;

class ProtoVlc : public ProtoVlcBase {
    SymbolProperty* host_;
    IntProperty* port_;
    AtomProperty* pass_;
    ThreadPdLogger logger_;

public:
    ProtoVlc(const PdArgs& args);

    void m_play(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_fullscreen(t_symbol* s, const AtomListView& lv);
    void m_seek(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_next(t_symbol* s, const AtomListView& lv);
    void m_prev(t_symbol* s, const AtomListView& lv);

    void m_add(t_symbol* s, const AtomListView& lv);
    void m_delete(t_symbol* s, const AtomListView& lv);
    void m_loop(t_symbol* s, const AtomListView& lv);
    void m_volume(t_symbol* s, const AtomListView& lv);

    void m_status(t_symbol* s, const AtomListView& lv);
    void m_playlist(t_symbol* s, const AtomListView& lv);
    void m_browse(t_symbol* s, const AtomListView& lv);

    void processMessage(const VlcResponse& msg) final;
    Future createTask() final;

private:
    void sendCommand(t_symbol* s, const VlcCommand& cmd);
};

void setup_proto_vlc();

#endif // PROTO_VLC_H
