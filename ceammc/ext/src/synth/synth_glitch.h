#ifndef SYNTH_GLITCH_H
#define SYNTH_GLITCH_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"
#include "glitchxx.h"

#include <future>

using namespace ceammc;

class SynthGlitch : public SoundExternal {
    double t_;
    Glitch glitch_;
    ListProperty* expr_ { 0 };
    BoolProperty* clip_ { 0 };

    std::future<std::string> read_content_;
    ClockLambdaFunction read_clock_;
    std::string file_path_;

public:
    SynthGlitch(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
    void samplerateChanged(size_t sr) final;

    void m_byte(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_read(t_symbol* s, const AtomListView& lv);
    void m_speed(t_symbol* s, const AtomListView& lv);

private:
    static float resample_;
};

void setup_synth_glitch();

#endif // SYNTH_GLITCH_H
