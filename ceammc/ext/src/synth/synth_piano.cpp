#include "synth_piano.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;

class SynthPiano : public SynthWithFreq<faust_synth_piano_tilde> {
    ClockLambdaFunction clock_;

public:
    SynthPiano(const PdArgs& args)
        : SynthWithFreq<faust_synth_piano_tilde>(args)
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        dsp_->instanceClear();
        clock_.delay(150);
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        gate_->setValue(0);
        dsp_->instanceClear();
    }
};

void setup_synth_piano_tilde()
{
    SoundExternalFactory<SynthPiano> obj("synth.piano~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthPiano::m_reset);
    obj.addMethod("note", &SynthPiano::m_note);

    obj.setDescription("waveguide commuted piano");
    obj.setCategory("synth");
    obj.setKeywords({"piano", "model", "synth"});
}
