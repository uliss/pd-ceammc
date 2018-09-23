#include "env_asr.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_SUSTAIN = gensym("@sustain");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");
static t_symbol* SYM_PROP_GATE = gensym("@gate");

using namespace ceammc;

class EnvAsr : public faust_env_asr_tilde {
    ClockMemberFunction<EnvAsr> attack_done_;
    ClockMemberFunction<EnvAsr> release_done_;
    ClockMemberFunction<EnvAsr> duration_done_;

    UIProperty* prop_attack_;
    UIProperty* prop_sustain_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAsr(const PdArgs& args)
        : faust_env_asr_tilde(args)
        , attack_done_(this, &EnvAsr::attackDone)
        , release_done_(this, &EnvAsr::releaseDone)
        , duration_done_(this, &EnvAsr::durationDone)
        , prop_attack_((UIProperty*)property(SYM_PROP_ATTACK))
        , prop_sustain_((UIProperty*)property(SYM_PROP_SUSTAIN))
        , prop_release_((UIProperty*)property(SYM_PROP_RELEASE))
        , prop_gate_((UIProperty*)property(SYM_PROP_GATE))
    {
        bindPositionalArgsToProps({ SYM_PROP_ATTACK, SYM_PROP_SUSTAIN, SYM_PROP_RELEASE });
        createProperty(new CombinedProperty("@asr",
            { property(SYM_PROP_ATTACK), property(SYM_PROP_SUSTAIN), property(SYM_PROP_RELEASE) }));

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == SYM_PROP_GATE) {
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

    void onDataT(const DataTypeEnv& env)
    {
        if (!env.isAR()) {
            OBJ_ERR << "not an AR envelope: " << env;
            return;
        }

        float attack = env.pointAt(1).timeMs() - env.pointAt(0).timeMs();
        float sustain = env.pointAt(1).value * 100;
        float release = env.pointAt(2).timeMs() - env.pointAt(1).timeMs();

        if (!set(attack, sustain, release))
            OBJ_ERR << "can't set envelope: " << env;
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        dsp_->instanceClear();
        prop_gate_->setValue(0);
        clockReset();
    }

    void m_duration(t_symbol* sel, const AtomList& l)
    {
        bool ok = (l.size() == 1) && (l[0].isFloat()) && (l[0].asFloat() >= 0);
        if (!ok) {
            OBJ_ERR << "duration TIME_MS expected: " << l;
            return;
        }

        processAnyProps(SYM_PROP_GATE, Atom(1));
        duration_done_.delay(l[0].asFloat());
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

    void durationDone()
    {
        processAnyProps(SYM_PROP_GATE, Atom(0.f));
    }

    void clockReset()
    {
        attack_done_.unset();
        release_done_.unset();
        duration_done_.unset();
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

void setup_env_asr_tilde()
{
    SoundExternalFactory<EnvAsr> obj("env.asr~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("reset", &EnvAsr::m_reset);
    obj.addMethod("duration", &EnvAsr::m_duration);
}
