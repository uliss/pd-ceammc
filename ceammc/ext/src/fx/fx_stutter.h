#ifndef FX_STUTTER_H
#define FX_STUTTER_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class FxStutter : public SoundExternal {
    enum State {
        ST_PASS,
        ST_FX,
        ST_TO_FX,
        ST_FROM_FX,
    };

    State state_;
    FloatProperty* max_size_;
    FloatProperty* size_;
    std::vector<t_sample> buffer_;
    size_t buf_phase_;
    ClockLambdaFunction clock_;

public:
    FxStutter(const PdArgs& args);

    void initDone() final;
    void onInlet(size_t n, const AtomListView& lv);

    void setupDSP(t_signal** sig) final;
    void processBlock(const t_sample** in, t_sample** out) final;

private:
    void adjustBufferSize();
};

void setup_fx_stutter_tilde();

#endif // FX_STUTTER_H
