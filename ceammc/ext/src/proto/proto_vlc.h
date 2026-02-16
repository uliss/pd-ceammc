#ifndef PROTO_VLC_H
#define PROTO_VLC_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "proto_rust.hpp"

#include <memory>

using namespace ceammc;

class ProtoVlc : public DispatchedObject<BaseObject> {
    SymbolProperty* host_;
    IntProperty* port_;
    AtomProperty* pass_;

    std::unique_ptr<ceammc_vlc, typeof(&ceammc_vlc_free)> vlc_;

public:
    ProtoVlc(const PdArgs& args);

    void initDone() override;

    void m_add(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fullscreen(t_symbol* s, const AtomListView& lv);
    void m_loop(t_symbol* s, const AtomListView& lv);
    void m_next(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_play(t_symbol* s, const AtomListView& lv);
    void m_prev(t_symbol* s, const AtomListView& lv);
    void m_repeat(t_symbol* s, const AtomListView& lv);
    void m_seek(t_symbol* s, const AtomListView& lv);
    void m_sort(t_symbol* s, const AtomListView& lv);
    void m_speed(t_symbol* s, const AtomListView& lv);
    void m_status(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_volume(t_symbol* s, const AtomListView& lv);

    void m_browse(t_symbol* s, const AtomListView& lv);
    void m_delete(t_symbol* s, const AtomListView& lv);
    void m_playlist(t_symbol* s, const AtomListView& lv);


    bool notify(int code) final;
};

void setup_proto_vlc();

#endif // PROTO_VLC_H
