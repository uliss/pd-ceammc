#include "env_smooth.h"
#include "ceammc_clock.h"
#include "ceammc_faust_factory.h"
#include "env_faust_play.h"

using namespace ceammc;

constexpr const char* PROP_GATE = "@gate";
constexpr const char* PROP_DURATION = "@duration";

static bool inline operator==(t_symbol& s1, const char* s2)
{
    return std::strcmp(s1.s_name, s2) == 0;
}

class EnvSmooth : public faust_env_smooth_tilde {
    ClockLambdaFunction done_;

    UIProperty* prop_duration_;
    UIProperty* prop_gate_;

public:
    EnvSmooth(const PdArgs& args)
        : faust_env_smooth_tilde(args)
        , done_([this]() { bangTo(1); })
        , prop_duration_(findUIProperty(PROP_DURATION))
        , prop_gate_(findUIProperty(PROP_GATE))
    {
        bindPositionalArgsToProps({ PROP_DURATION });
        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override
    {
        if (*sel == PROP_GATE && !lv.boolAt(0, false)) {
            done_.delay(prop_duration_->value());
        }

        return faust_env_smooth_tilde::processAnyProps(sel, lv);
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        resetClocks();
    }

private:
    void resetClocks()
    {
        done_.unset();
    }
};

void setup_env_smooth_tilde()
{
    using EnvSmooth2 = EnvAutoplay<EnvSmooth>;

    FaustFactory<EnvSmooth2> obj("env.smooth~");
    obj.addMethod("play", &EnvSmooth2::m_play);
    obj.setXletsInfo({ "input signal" }, { "enveloped signal", "bang when done" });

    obj.setDescription("An envelope with an exponential attack and release");
    obj.setCategory("env");
    obj.setKeywords({ "envelope", "smooth" });
}
