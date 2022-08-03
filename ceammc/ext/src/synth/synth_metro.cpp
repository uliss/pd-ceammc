#include "synth_metro.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

using namespace ceammc;

constexpr int CLOCK_BANG = 5;
constexpr int DOWNBEAT = 1;
constexpr int UPBEAT = 0;
constexpr int SECTION = 2;

class SynthMetro : public faust_synth_metro_tilde {
    UIProperty* b0_;
    UIProperty* b1_;
    UIProperty* b2_;
    ClockLambdaFunction clock_;
    bool beats_[3];

public:
    SynthMetro(const PdArgs& args)
        : faust_synth_metro_tilde(args)
        , b0_(static_cast<UIProperty*>(property(gensym("@b0"))))
        , b1_(static_cast<UIProperty*>(property(gensym("@b1"))))
        , b2_(static_cast<UIProperty*>(property(gensym("@b2"))))
        , clock_([this]() {
            if (beats_[0]) {
                beats_[0] = false;
                b0_->setValue(0);
            }

            if (beats_[1]) {
                beats_[1] = false;
                b1_->setValue(0);
            }

            if (beats_[2]) {
                beats_[2] = false;
                b2_->setValue(0);
            }
        })
        , beats_ { 0, 0, 0 }
    {
        createInlet();
        createInlet();
    }

    void onBang() final
    {
        b0_->setValue(1);
        beats_[0] = true;
        clock_.delay(CLOCK_BANG);
    }

    void onFloat(t_float v) final
    {
        int b = v;
        if (b < 0 || b > 2) {
            OBJ_ERR << fmt::format("0, 1 or 2 expected, got: {}", v);
            return;
        }

        switch (b) {
        case UPBEAT:
            return onInlet(1, {});
        case SECTION:
            return onInlet(2, {});
        case DOWNBEAT:
        default:
            return onBang();
        }
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (n == 1) {
            b1_->setValue(1);
            beats_[1] = true;
            clock_.delay(CLOCK_BANG);
        } else if (n == 2) {
            b2_->setValue(1);
            beats_[2] = true;
            clock_.delay(CLOCK_BANG);
        }
    }
};

void setup_synth_metro_tilde()
{
    SoundExternalFactory<SynthMetro> obj("synth.metro~", OBJECT_FACTORY_DEFAULT);
}
