/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef PROTO_MPV_H
#define PROTO_MPV_H

#include "ceammc_clock.h"
#include "ceammc_data.h"
#include "ceammc_object.h"
#include "mpv_ipc.h"
#include "readerwriterqueue.h"
using namespace ceammc;

class ProtoMpv : public BaseObject {
    SymbolProperty* mpv_ipc_path_;
    std::atomic_bool sig_quit_;
    mpv::MpvIpcFuture ipc_result_;
    mpv::StringQueue queue_to_mpv_;
    mpv::StringQueue queue_from_mpv_;
    ClockLambdaFunction clock_;

public:
    ProtoMpv(const PdArgs& args);
    ~ProtoMpv();

    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_play(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_quit(t_symbol* s, const AtomListView& lv);
    void m_load(t_symbol* s, const AtomListView& lv);
    void m_next(t_symbol* s, const AtomListView& lv);
    void m_prev(t_symbol* s, const AtomListView& lv);
    void m_seek(t_symbol* s, const AtomListView& lv);
    void m_text(t_symbol* s, const AtomListView& lv);
    void m_fullscreen(t_symbol* s, const AtomListView& lv);
    void m_playlist(t_symbol* s, const AtomListView& lv);

    void onDataT(const DictAtom& dict);

private:
    bool write(const char* str);
    void quit();
};

void setup_proto_mpv();

#endif // PROTO_MPV_H
