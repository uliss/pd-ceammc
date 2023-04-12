#include "synth_metro.h"
#include "args/argcheck2.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "ceammc_property_bpm.h"
#include "ceammc_property_timesig.h"

#include <cstdint>

using namespace ceammc;

class SynthMetro : public faust_synth_metro_tilde {
    std::array<UIProperty*, 4> beats_;
    ClockLambdaFunction clock_on_, clock_off_;
    music::BeatList pattern_;
    std::uint32_t pattern_idx_ { 0 };
    BpmProperty* tempo_ { 0 };
    TimeSignatureProperty* tsig_ { 0 };

public:
    SynthMetro(const PdArgs& args)
        : faust_synth_metro_tilde(args)
        , beats_ { findUIProperty("@.down"), findUIProperty("@.on"), findUIProperty("@.off"), findUIProperty("@.mark") }
        , clock_on_([this]() {
            if (pattern_.empty())
                return;

            if (pattern_idx_ >= pattern_.size())
                pattern_idx_ = 0;

            auto beat = pattern_[pattern_idx_++];
            bangBeat(beat.type);
            if (!tempo_->isNull())
                clock_on_.delay(tempo_->wholeNoteDurationMs() / beat.division);
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

        for (auto p : beats_)
            p->setInternal();

        tempo_ = new BpmProperty("@tempo", 60);
        tempo_->setArgIndex(0);
        addProperty(tempo_);

        tsig_ = new TimeSignatureProperty("@ts");
        tsig_->setArgIndex(1);
        tsig_->setSuccessFn([this](Property*) { syncPattern(); });
        addProperty(tsig_);
    }

    void initDone() final
    {
        syncPattern();
    }

    void syncPattern()
    {
        pattern_.clear();
        pattern_ = tsig_->signature().beatList();
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
        switch (n) {
        case 1:
            return bangBeat(lv.intAt(0, music::BEAT_DOWN));
        case 2:
            tempo_->setList(lv);
            break;
        default:
            break;
        }
    }

    void m_tempo(t_symbol* s, const AtomListView& lv)
    {
        static const args::ArgChecker chk("TEMPO:f>=0");
        if (!chk.check(lv, this))
            return chk.usage(this, s);

        tempo_->setBpm(lv.asT<t_float>());
    }

    void m_down(t_symbol*, const AtomListView&) { bangBeat(music::BEAT_DOWN); }
    void m_on(t_symbol*, const AtomListView&) { bangBeat(music::BEAT_ON); }
    void m_off(t_symbol*, const AtomListView&) { bangBeat(music::BEAT_OFF); }
    void m_mark(t_symbol*, const AtomListView&) { bangBeat(music::BEAT_MARK); }

private:
    void bangBeat(int t)
    {
        if (t < music::BEAT_DOWN || t > music::BEAT_MAX)
            return;

        try {
            if (beats_.at(t - 1)) {
                beats_[t - 1]->setValue(1, true);
                clock_off_.delay(10);
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    }
};

void setup_synth_metro_tilde()
{
    SoundExternalFactory<SynthMetro> obj("synth.metro~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("tempo", &SynthMetro::m_tempo);
    obj.addMethod("down", &SynthMetro::m_down);
    obj.addMethod("on", &SynthMetro::m_on);
    obj.addMethod("off", &SynthMetro::m_off);
    obj.addMethod("mark", &SynthMetro::m_mark);

    obj.setXletsInfo({ "bool: on/off metro", "int: bang beat", "float: set bpm" }, { "signal: out" });

    obj.setDescription("ready to use metronome synth");
    obj.setCategory("synth");
    obj.setKeywords({ "metro" });
}
