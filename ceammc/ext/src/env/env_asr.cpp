#include "env_asr.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"
#include "env_faust_play.h"

using namespace ceammc;

class EnvAsr : public faust_env_asr_tilde {
    ClockMemberFunction<EnvAsr> attack_done_;
    ClockMemberFunction<EnvAsr> release_done_;

    UIProperty* prop_attack_;
    UIProperty* prop_sustain_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAsr(const PdArgs& args)
        : faust_env_asr_tilde(args)
        , attack_done_(this, &EnvAsr::attackDone)
        , release_done_(this, &EnvAsr::releaseDone)
        , prop_attack_((UIProperty*)property(gensym("@attack")))
        , prop_sustain_((UIProperty*)property(gensym("@sustain")))
        , prop_release_((UIProperty*)property(gensym("@release")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@sustain"), gensym("@release") });
        createProperty(new CombinedProperty("@asr",
            { property(gensym("@attack")), property(gensym("@sustain")), property(gensym("@release")) }));

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == gensym("@gate")) {
            if (atomlistToValue<bool>(lst, false)) {
                clockReset();
                attack_done_.delay(prop_attack_->value());
            } else {
                clockReset();
                release_done_.delay(prop_release_->value());
            }
        }

        return faust_env_asr_tilde::processAnyProps(sel, lst);
    }

    void onList(const AtomList& l) override
    {
        if (!checkArgs(l, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK SUSTAIN RELEASE values expected: " << l;
            return;
        }

        if (!set(l[0].asFloat(), l[1].asFloat(), l[2].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const DataTPtr<DataTypeEnv>& dptr)
    {
        if (!dptr->isAR()) {
            OBJ_ERR << "not an AR envelope: " << *dptr;
            return;
        }

        float attack = dptr->pointAt(1).timeMs() - dptr->pointAt(0).timeMs();
        float sustain = dptr->pointAt(1).value * 100;
        float release = dptr->pointAt(2).timeMs() - dptr->pointAt(1).timeMs();

        if (!set(attack, sustain, release))
            OBJ_ERR << "can't set envelope: " << *dptr;
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        dsp_->instanceClear();
        prop_gate_->setValue(0);
        clockReset();
    }

private:
    void attackDone()
    {
        floatTo(1, 1);
    }

    void releaseDone()
    {
        floatTo(1, 0);
    }

    void clockReset()
    {
        attack_done_.unset();
        release_done_.unset();
    }

    bool checkValues(float a, float s, float r)
    {
        return a >= 0 && r >= 0 && s >= 0 && s <= 100;
    }

    bool set(float a, float s, float r)
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

typedef EnvAutoplay<EnvAsr> EnvAsrP;

void setup_env_asr_tilde()
{
    SoundExternalFactory<EnvAsrP> obj("env.asr~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("play", &EnvAsrP::m_play);
    obj.addMethod("reset", &EnvAsrP::m_reset);
}
