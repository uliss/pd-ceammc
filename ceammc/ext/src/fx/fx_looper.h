#ifndef FX_LOOPER_H
#define FX_LOOPER_H

#include "ceammc_sound_external.h"

using namespace ceammc;

enum FxLooperState {
    STATE_INIT = 0,
    STATE_REC,
    STATE_DUB,
    STATE_PLAY,
    STATE_STOP
};

class FxLooper : public SoundExternal {
    FxLooperState state_;
    FloatProperty* capacity_sec_;
    BoolProperty* loop_bang_;
    BoolProperty* xfade_;
    size_t max_samples_;
    size_t loop_len_;
    size_t play_phase_;
    size_t rec_phase_;
    std::vector<t_sample> buffer_;

public:
    FxLooper(const PdArgs& args);

    void onBang();
    void processBlock(const t_sample** in, t_sample** out);

    void m_record(t_symbol*, const AtomList& lst);
    void m_stop(t_symbol*, const AtomList& lst);
    void m_play(t_symbol*, const AtomList& lst);
    void m_overdub(t_symbol*, const AtomList& lst);
    void m_clear(t_symbol*, const AtomList& lst);
    void m_adjust(t_symbol*, const AtomList& lst);

    AtomList p_length() const;
    AtomList p_play_pos() const;
    AtomList p_play_phase() const;
    AtomList p_state() const;
};

void setup_fx_looper();

#endif // FX_LOOPER_H
