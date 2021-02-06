#include "synth_harpsichord.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;

class SynthHarpsichord : public SynthWithFreq<faust_synth_harpsichord_tilde> {
    UIProperty* gain_;
    ClockLambdaFunction clock_;

public:
    SynthHarpsichord(const PdArgs& args)
        : SynthWithFreq<faust_synth_harpsichord_tilde>(args)
        , gain_(static_cast<UIProperty*>(property(PROP_GAIN)))
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(150);
    }

    void m_note(t_symbol* s, const AtomListView& lv)
    {
        if (!pitch_ || !gate_ || !gain_) {
            METHOD_ERR(s) << "dev error: @pitch or @gate or @gain property not found";
            return;
        }

        if (lv.size() != 2 && !lv.allOf(isFloat)) {
            METHOD_ERR(s) << "usage: " << s->s_name << " NOTE VELOCITY";
            return;
        }

        const auto note = lv.floatAt(0, -1);
        const auto vel = lv.floatAt(1, -1);
        const auto nmin = pitch_->infoT().minFloat();
        const auto nmax = pitch_->infoT().maxFloat();

        if (note < nmin || note > nmax) {
            METHOD_ERR(s) << "note value is out of [" << nmin << ".." << nmax << "] range: " << note;
            return;
        }

        if (vel < 0 || vel > 127) {
            METHOD_ERR(s) << "velocity value is out of [0..127] range: " << vel;
            return;
        }

        pitch_->setValue(note, true);
        gain_->setValue(vel / 127, true);
        gate_->setValue(vel > 0, true);
    }
};

void setup_synth_harpsichord_tilde()
{
    SynthHarpsichord::initSymTab();

    SoundExternalFactory<SynthHarpsichord> obj("synth.harpsichord~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthHarpsichord::m_reset);
    obj.addMethod("note", &SynthHarpsichord::m_note);
}
