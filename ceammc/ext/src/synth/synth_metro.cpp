#include "synth_metro.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthMetro : public faust_synth_metro_tilde {
    enum BeatType {
        BEAT_DOWN = 0,
        BEAT_ON,
        BEAT_OFF,
        BEAT_MARK,
        BEAT_MAX = BEAT_MARK,
    };

    UIProperty* beats_[4] = { 0 };
    ClockLambdaFunction clock_;

public:
    SynthMetro(const PdArgs& args)
        : faust_synth_metro_tilde(args)
        , beats_ { findUIProperty("@down"), findUIProperty("@on"), findUIProperty("@off"), findUIProperty("@mark") }
        , clock_([this]() {
            for (auto x : beats_) {
                if (x)
                    x->setValue(0, true);
            }
        })
    {
        createInlet();
        createInlet();
        createInlet();
    }

    void onBang() override
    {
        bangBeat(BEAT_DOWN);
    }

    void onFloat(t_float f) override
    {
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (n < BEAT_DOWN || n > BEAT_MAX)
            return;

        bangBeat(static_cast<BeatType>(n));
    }

private:
    void bangBeat(BeatType t)
    {
        if (beats_[t]) {
            beats_[t]->setValue(1, true);
            clock_.delay(10);
        }
    }
};

void setup_synth_metro_tilde()
{
    SoundExternalFactory<SynthMetro> obj("synth.metro~", OBJECT_FACTORY_DEFAULT);

    //    obj.setDescription("bird singing generator");
    //    obj.setCategory("synth");
    //    obj.setKeywords({ "birds" });
}
