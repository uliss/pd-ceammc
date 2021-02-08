#include "synth_harpsichord.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;

using HarpBase = SynthWithFreq<faust_synth_harpsichord_tilde>;

class SynthHarpsichord : public HarpBase {
    ClockLambdaFunction clock_;

public:
    SynthHarpsichord(const PdArgs& args)
        : HarpBase(args)
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

void setup_synth_harpsichord_tilde()
{
    SynthHarpsichord::initSymTab();

    SoundExternalFactory<SynthHarpsichord> obj("synth.harpsichord~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthHarpsichord::m_reset);
    obj.addMethod("note", &SynthHarpsichord::m_note);
}
