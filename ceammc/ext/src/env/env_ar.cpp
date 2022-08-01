#include "env_ar.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

using namespace ceammc;

class EnvAr : public faust_env_ar_tilde {
    ClockMemberFunction<EnvAr> auto_release_;
    ClockMemberFunction<EnvAr> done_;

    UIProperty* prop_attack_;
    UIProperty* prop_release_;
    UIProperty* prop_gate_;

public:
    EnvAr(const PdArgs& args)
        : faust_env_ar_tilde(args)
        , auto_release_(this, &EnvAr::release)
        , done_(this, &EnvAr::done)
        , prop_attack_((UIProperty*)property(gensym("@attack")))
        , prop_release_((UIProperty*)property(gensym("@release")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@release") });
        addProperty(new CombinedProperty("@ar",
            { property(gensym("@attack")), property(gensym("@release")) }));

        createCbFloatProperty("@length", [this]() -> t_float { return length(); })
            ->setUnitsMs();

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override
    {
        if (sel == gensym("@gate") && lv.boolAt(0, false)) {
            auto_release_.unset();
            done_.delay(length());
        }

        return faust_env_ar_tilde::processAnyProps(sel, lv);
    }

    void onBang() override
    {
        prop_gate_->setValue(1);
        auto_release_.delay(10);
        done_.delay(length());
    }

    void onList(const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK RELEASE values expected: " << lv;
            return;
        }

        if (!set(lv[0].asFloat(), lv[1].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const EnvAtom& env)
    {
        if (!env->isAR()) {
            OBJ_ERR << "not an AR envelope: " << *env;
            return;
        }

        t_float attack = env->pointAt(1).timeMs() - env->pointAt(0).timeMs();
        t_float release = env->pointAt(2).timeMs() - env->pointAt(1).timeMs();

        if (!set(attack, release))
            OBJ_ERR << "can't set envelope";
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        unsetClocks();
    }

    void onClick(t_floatarg, t_floatarg, t_floatarg, t_floatarg, t_floatarg) override
    {
        onBang();
    }

    void m_play(t_symbol*, const AtomListView&)
    {
        onBang();
    }

private:
    void release()
    {
        prop_gate_->setValue(0);
    }

    void done()
    {
        bangTo(1);
    }

    bool checkValues(t_float a, t_float r)
    {
        return a >= 0 && r >= 0;
    }

    void unsetClocks()
    {
        done_.unset();
        auto_release_.unset();
    }

    bool set(t_float a, t_float r)
    {
        if (!checkValues(a, r)) {
            OBJ_ERR << "invalid values: " << a << ", " << r;
            return false;
        }

        prop_attack_->setValue(a);
        prop_release_->setValue(r);
        return true;
    }

    t_float length() const
    {
        return prop_attack_->value() + prop_release_->value();
    }
};

void setup_env_ar_tilde()
{
    SoundExternalFactory<EnvAr> obj("env.ar~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("play", &EnvAr::m_play);
    obj.addMethod("reset", &EnvAr::m_reset);
    obj.useClick();

    obj.setXletsInfo(
        {
            "bang: run envelope\n"
            "list: ATTACK RELEASE\n"
            "play: run envelope\n"
            "reset: reset evenlope\n"
            "signal: input",
        },
        {
            "output signal",
            "bang on done",
        });
}
