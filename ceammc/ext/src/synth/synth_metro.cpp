#include "synth_metro.h"
#include "args/argcheck2.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "ceammc_music_theory_timesig.h"
#include "ceammc_property_bpm.h"

#include <cstdint>

using namespace ceammc;

class SynthMetro : public faust_synth_metro_tilde {
    UIProperty* beats_[4] = { 0 };
    ClockLambdaFunction clock_on_, clock_off_;
    music::BeatList pattern_;
    std::uint32_t pattern_idx_ { 0 };
    BpmProperty* tempo_ { 0 };
    music::TimeSignature time_sig_;

public:
    SynthMetro(const PdArgs& args)
        : faust_synth_metro_tilde(args)
        , beats_ { findUIProperty("@down"), findUIProperty("@on"), findUIProperty("@off"), findUIProperty("@mark") }
        , clock_on_([this]() {
            if (pattern_.empty())
                return;

            if (pattern_idx_ >= pattern_.size())
                pattern_idx_ = 0;

            auto beat = pattern_[pattern_idx_++];
            bangBeat(beat.type);
            auto t = tempo_->tempo().wholeNoteDurationMs() / beat.division;
            clock_on_.delay(t);
        })
        , clock_off_([this]() {
            for (auto x : beats_) {
                if (x)
                    x->setValue(0, true);
            }
        })
    {
        createInlet();
        createInlet();
        createInlet();

        tempo_ = new BpmProperty("@t", 60);
        tempo_->setSuccessFn([this](Property*) { syncPattern(); });
        tempo_->setArgIndex(0);
        addProperty(tempo_);
    }

    void initDone() final
    {
        syncPattern();
    }

    void syncPattern()
    {
        pattern_.clear();
        pattern_ = time_sig_.beatList();
    }

    void onBang() override
    {
        bangBeat(music::BEAT_DOWN);
    }

    void onFloat(t_float f) override
    {
        if (f > 0)
            clock_on_.exec();
        else
            clock_on_.unset();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (n < music::BEAT_DOWN || n > music::BEAT_OFF)
            return;

        bangBeat(static_cast<music::BeatType>(n));
    }

    void m_tempo(t_symbol* s, const AtomListView& lv)
    {
        static const args::ArgChecker chk("TEMPO:f>=0");
        if (!chk.check(lv, this))
            return chk.usage(this, s);

        tempo_->setBpm(lv.asT<t_float>());
    }

private:
    void bangBeat(music::BeatType t)
    {
        if (beats_[t - 1]) {
            beats_[t - 1]->setValue(1, true);
            clock_off_.delay(10);
        }
    }
};

void setup_synth_metro_tilde()
{
    SoundExternalFactory<SynthMetro> obj("synth.metro~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("tempo", &SynthMetro::m_tempo);

    //    obj.setDescription("bird singing generator");
    //    obj.setCategory("synth");
    //    obj.setKeywords({ "birds" });
}
