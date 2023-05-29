#include "env_adsr.h"
#include "ceammc_clock.h"
#include "ceammc_faust_factory.h"
#include "datatype_env.h"
#include "env_faust_play.h"

using namespace ceammc;

class EnvAdsr : public faust_env_adsr_tilde {
    ClockMemberFunction<EnvAdsr> ad_done_;
    ClockMemberFunction<EnvAdsr> release_done_;

    UIProperty* prop_attack_;
    UIProperty* prop_decay_;
    UIProperty* prop_sustain_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAdsr(const PdArgs& args)
        : faust_env_adsr_tilde(args)
        , ad_done_(this, &EnvAdsr::attackDecayDone)
        , release_done_(this, &EnvAdsr::releaseDone)
        , prop_attack_(findUIProperty("@attack"))
        , prop_decay_(findUIProperty("@decay"))
        , prop_sustain_(findUIProperty("@sustain"))
        , prop_release_(findUIProperty("@release"))
        , prop_gate_(findUIProperty("@gate"))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@decay"), gensym("@sustain"), gensym("@release") });
        addProperty(new CombinedProperty("@adsr",
            { property(gensym("@attack")), property(gensym("@decay")),
                property(gensym("@sustain")), property(gensym("@release")) }));

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override
    {
        // intercept @gate call
        if (sel == gensym("@gate")) {
            if (lv.boolAt(0, false)) {
                clockReset();
                ad_done_.delay(prop_attack_->value() + prop_decay_->value());
            } else {
                clockReset();
                release_done_.delay(prop_release_->value());
            }
        }

        return faust_env_adsr_tilde::processAnyProps(sel, lv);
    }

    void onList(const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK DECAY SUSTAIN RELEASE values expected: " << lv;
            return;
        }

        if (!set(lv[0].asFloat(), lv[1].asFloat(), lv[2].asFloat(), lv[3].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const EnvAtom& env)
    {
        if (!env->isADSR()) {
            OBJ_ERR << "not an ADSR envelope: " << *env;
            return;
        }

        t_float attack = env->pointAt(1).timeMs() - env->pointAt(0).timeMs();
        t_float decay = env->pointAt(2).timeMs() - env->pointAt(1).timeMs();
        t_float sustain = env->pointAt(2).value * 100;
        t_float release = env->pointAt(3).timeMs() - env->pointAt(2).timeMs();

        if (!set(attack, decay, sustain, release))
            OBJ_ERR << "can't set envelope: " << *env;
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        clockReset();
    }

private:
    void attackDecayDone()
    {
        floatTo(1, 1);
    }

    void releaseDone()
    {
        floatTo(1, 0);
    }

    void durationDone()
    {
        processAnyProps(gensym("@gate"), Atom(0.f));
    }

    void clockReset()
    {
        ad_done_.unset();
        release_done_.unset();
    }

    bool checkValues(t_float a, t_float d, t_float s, t_float r)
    {
        return a >= 0 && d >= 0 && r >= 0 && s >= 0 && s <= 100 && (a + d + r) > 0;
    }

    bool set(t_float a, t_float d, t_float s, t_float r)
    {
        if (!checkValues(a, d, s, r)) {
            OBJ_ERR << "invalid values: " << a << ", " << d << ", " << s << ", " << r;
            return false;
        }

        prop_attack_->setValue(a);
        prop_decay_->setValue(d);
        prop_sustain_->setValue(s);
        prop_release_->setValue(r);
        return true;
    }
};

typedef EnvAutoplay<EnvAdsr> EnvADSR2;

void setup_env_adsr_tilde()
{
    FaustFactory<EnvADSR2> obj("env.adsr~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("play", &EnvADSR2::m_play);

    obj.setDescription("ADSR (Attack, Decay, Sustain, Release) envelope generator");
    obj.setCategory("env");
    obj.setKeywords({ "envelope", "adsr" });
}
