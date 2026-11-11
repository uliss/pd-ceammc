#ifndef PROTO_VLC_H
#define PROTO_VLC_H

#ifdef WITH_VLC

#include "ceammc_rs_msg_object.h"

#include <memory>

struct ceammc_vlc;

using namespace ceammc;

class ProtoVlc : public RustMessageObject<BaseObject> {
    SymbolProperty* host_;
    IntProperty* port_;
    AtomProperty* pass_;

    std::unique_ptr<ceammc_vlc, void (*)(ceammc_vlc*)> vlc_;

public:
    ProtoVlc(const PdArgs& args);

    void initDone() override;

    void m_add_dir(t_symbol* s, const AtomListView& lv);
    void m_add_uri(t_symbol* s, const AtomListView& lv);
    void m_browse(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_delete(t_symbol* s, const AtomListView& lv);
    void m_fullscreen(t_symbol* s, const AtomListView& lv);
    void m_get_current(t_symbol* s, const AtomListView& lv);
    void m_get_playlist(t_symbol* s, const AtomListView& lv);
    void m_get_status(t_symbol* s, const AtomListView& lv);
    void m_loop(t_symbol* s, const AtomListView& lv);
    void m_next(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_play(t_symbol* s, const AtomListView& lv);
    void m_prev(t_symbol* s, const AtomListView& lv);
    void m_repeat(t_symbol* s, const AtomListView& lv);
    void m_seek(t_symbol* s, const AtomListView& lv);
    void m_sort(t_symbol* s, const AtomListView& lv);
    void m_speed(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_volume(t_symbol* s, const AtomListView& lv);

    bool notify(int code) final;
};

void setup_proto_vlc();

#else
#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(ProtoVlc, 1, 2, "compiled without VLC support");
OBJECT_STUB_SETUP(ProtoVlc, proto_vlc, "proto.vlc");

#endif

#endif // PROTO_VLC_H
