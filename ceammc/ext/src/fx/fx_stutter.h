#ifndef FX_STUTTER_H
#define FX_STUTTER_H

#include "ceammc_clock.h"
#include "ceammc_property_data.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
#include "datatype_env.h"
using namespace ceammc;

class FxStutter : public SoundExternal {
    enum State {
        ST_PASS,
        ST_FX,
        ST_TO_FX,
        ST_FROM_FX,
    };

    State state_;
    FloatProperty* max_size_ { 0 };
    FloatProperty* t_ { 0 };
    SymbolEnumProperty* mode_ { 0 };
    FloatProperty* speed_ { 0 };
    DataPropertyT<DataTypeEnv>* env_ { 0 };
    std::array<float, 256> env_buf_;
    std::vector<t_sample> buffer_;
    size_t write_phase_ { 0 };
    size_t read_phase_ { 0 };
    ClockLambdaFunction clock_;

public:
    FxStutter(const PdArgs& args);

    void initDone() final;
    void onInlet(size_t n, const AtomListView& lv);

    void setupDSP(t_signal** sig) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void onDataT(const EnvAtom& env);

    // for tests
    const std::vector<t_sample>& buffer() const { return buffer_; }
    size_t stutterSizeSamp() const { return std::min<size_t>(buffer_.size(), (t_->value() * samplerate() * 0.001)); }

private:
    void adjustBufferSize();
    float curveValueAt(size_t pos) const;
};

void setup_fx_stutter_tilde();

#endif // FX_STUTTER_H
