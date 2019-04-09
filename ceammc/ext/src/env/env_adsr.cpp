#include "env_adsr.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
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
        , prop_attack_((UIProperty*)property(gensym("@attack")))
        , prop_decay_((UIProperty*)property(gensym("@decay")))
        , prop_sustain_((UIProperty*)property(gensym("@sustain")))
        , prop_release_((UIProperty*)property(gensym("@release")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@decay"), gensym("@sustain"), gensym("@release") });
        createProperty(new CombinedProperty("@adsr",
            { property(gensym("@attack")), property(gensym("@decay")),
                property(gensym("@sustain")), property(gensym("@release")) }));

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        // intercept @gate call
        if (sel == gensym("@gate")) {
            if (atomlistToValue<bool>(lst, false)) {
                clockReset();
                ad_done_.delay(prop_attack_->value() + prop_decay_->value());
            } else {
                clockReset();
                release_done_.delay(prop_release_->value());
            }
        }

        return faust_env_adsr_tilde::processAnyProps(sel, lst);
    }

    void onList(const AtomList& l) override
    {
        if (!checkArgs(l, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK DECAY SUSTAIN RELEASE values expected: " << l;
            return;
        }

        if (!set(l[0].asFloat(), l[1].asFloat(), l[2].asFloat(), l[3].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const DataTypeEnv& env)
    {
        if (!env.isADSR()) {
            OBJ_ERR << "not an ADSR envelope: " << env;
            return;
        }

        float attack = env.pointAt(1).timeMs() - env.pointAt(0).timeMs();
        float decay = env.pointAt(2).timeMs() - env.pointAt(1).timeMs();
        float sustain = env.pointAt(2).value * 100;
        float release = env.pointAt(3).timeMs() - env.pointAt(2).timeMs();

        if (!set(attack, decay, sustain, release))
            OBJ_ERR << "can't set envelope: " << env;
    }

    void m_reset(t_symbol*, const AtomList&)
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

    bool checkValues(float a, float d, float s, float r)
    {
        return a >= 0 && d >= 0 && r >= 0 && s >= 0 && s <= 100 && (a + d + r) > 0;
    }

    bool set(float a, float d, float s, float r)
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
    SoundExternalFactory<EnvADSR2> obj("env.adsr~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("reset", &EnvADSR2::m_reset);
    obj.addMethod("play", &EnvADSR2::m_play);
}
