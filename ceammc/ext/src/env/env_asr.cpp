#include "env_asr.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"
#include "env_faust_play.h"

using namespace ceammc;

constexpr int ATTACK_DONE = 1;
constexpr int RELEASE_DONE = 0;

class EnvAsr : public EnvAutoplay<faust_env_asr_tilde> {
    ClockLambdaFunction attack_done_;
    ClockLambdaFunction release_done_;

    UIProperty* prop_attack_;
    UIProperty* prop_sustain_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAsr(const PdArgs& args)
        : EnvAutoplay<faust_env_asr_tilde>(args)
        , attack_done_([this]() { floatTo(1, ATTACK_DONE); })
        , release_done_([this]() { floatTo(1, RELEASE_DONE); })
        , prop_attack_((UIProperty*)property(gensym("@attack")))
        , prop_sustain_((UIProperty*)property(gensym("@sustain")))
        , prop_release_((UIProperty*)property(gensym("@release")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@sustain"), gensym("@release") });
        addProperty(new CombinedProperty("@asr",
            { property(gensym("@attack")), property(gensym("@sustain")), property(gensym("@release")) }));

        createInlet();
        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override
    {
        if (sel == gensym("@gate")) {
            if (lv.boolAt(0, false)) {
                clockReset();
                attack_done_.delay(prop_attack_->value());
            } else {
                clockReset();
                release_done_.delay(prop_release_->value());
            }
        }

        return faust_env_asr_tilde::processAnyProps(sel, lv);
    }

    void onList(const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK SUSTAIN RELEASE values expected: " << lv;
            return;
        }

        if (!set(lv[0].asFloat(), lv[1].asFloat(), lv[2].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const EnvAtom& env)
    {
        if (!env->isAR()) {
            OBJ_ERR << "not an AR envelope: " << *env;
            return;
        }

        float attack = env->pointAt(1).timeMs() - env->pointAt(0).timeMs();
        float sustain = env->pointAt(1).value * 100;
        float release = env->pointAt(2).timeMs() - env->pointAt(1).timeMs();

        if (!set(attack, sustain, release))
            OBJ_ERR << "can't set envelope: " << *env;
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (!checkArgs(lv, ARG_FLOAT)) {
            OBJ_ERR << "float value expected, got: " << lv;
            return;
        }

        sendGate(lv[0].asT<t_float>());
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        dsp_->instanceClear();
        prop_gate_->setValue(0);
        clockReset();
    }

private:
    void clockReset()
    {
        attack_done_.unset();
        release_done_.unset();
    }

    bool checkValues(t_float a, t_float s, t_float r)
    {
        return a >= 0 && r >= 0 && s >= 0 && s <= 100;
    }

    bool set(t_float a, t_float s, t_float r)
    {
        if (!checkValues(a, s, r)) {
            OBJ_ERR << "invalid values: " << a << ", " << s << ", " << r;
            return false;
        }

        prop_attack_->setValue(a);
        prop_sustain_->setValue(s);
        prop_release_->setValue(r);
        return true;
    }
};

void setup_env_asr_tilde()
{
    SoundExternalFactory<EnvAsr> obj("env.asr~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("play", &EnvAsr::m_play);
    obj.addMethod("reset", &EnvAsr::m_reset);

    obj.setXletsInfo(
        { "signal: input", "float: start (if >0) or stop(if =0) envelope" },
        { "signal: output", "float: 1 on attack done, 0 on release done" });

    obj.setDescription("Attack/Sustain/Release envelope generator");
    obj.setCategory("env");
    obj.setKeywords({"envelope", "asr"});
}
