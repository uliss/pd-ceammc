#include "synth_harpsichord.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;

class SynthHarpsichord : public SynthWithFreq<faust_synth_harpsichord_tilde> {
    ClockLambdaFunction clock_;

public:
    SynthHarpsichord(const PdArgs& args)
        : SynthWithFreq<faust_synth_harpsichord_tilde>(args)
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(150);
    }
};

void setup_synth_harpsichord_tilde()
{
    SynthHarpsichord::initSymTab();

    SoundExternalFactory<SynthHarpsichord> obj("synth.harpsichord~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthHarpsichord::m_reset);
    obj.addMethod("note", &SynthHarpsichord::m_note);
}
