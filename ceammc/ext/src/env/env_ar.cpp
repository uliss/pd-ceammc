#include "env_ar.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");
static t_symbol* SYM_PROP_GATE = gensym("@gate");

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
        , prop_attack_((UIProperty*)property(SYM_PROP_ATTACK))
        , prop_release_((UIProperty*)property(SYM_PROP_RELEASE))
        , prop_gate_((UIProperty*)property(SYM_PROP_GATE))
    {
        bindPositionalArgsToProps({ SYM_PROP_ATTACK, SYM_PROP_RELEASE });
        createProperty(new CombinedProperty("@ar", { property(SYM_PROP_ATTACK), property(SYM_PROP_RELEASE) }));
        createCbProperty("@length", &EnvAr::propLength);

        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == SYM_PROP_GATE && atomlistToValue<bool>(lst, false))
            done_.delay(length());

        return faust_env_ar_tilde::processAnyProps(sel, lst);
    }

    void onBang() override
    {
        done_.unset();
        prop_gate_->setValue(1);
        auto_release_.delay(10);
        done_.delay(length());
    }

    void onList(const AtomList& l) override
    {
        if (!checkArgs(l, ARG_FLOAT, ARG_FLOAT)) {
            OBJ_ERR << "ATTACK RELEASE values expected: " << l;
            return;
        }

        if (!set(l[0].asFloat(), l[1].asFloat()))
            OBJ_ERR << "can't set envelope";
    }

    void onDataT(const DataTypeEnv& env)
    {
        if (!env.isAR()) {
            OBJ_ERR << "not an AR envelope: " << env;
            return;
        }

        float attack = env.pointAt(1).timeMs() - env.pointAt(0).timeMs();
        float release = env.pointAt(2).timeMs() - env.pointAt(1).timeMs();

        if (!set(attack, release))
            OBJ_ERR << "can't set envelope";
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        dsp_->instanceClear();
        auto_release_.unset();
    }

    void m_click(t_symbol*, const AtomList& l)
    {
        onBang();
    }

    AtomList propLength() const
    {
        return Atom(length());
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

    bool checkValues(float a, float r)
    {
        return a >= 0 && r >= 0;
    }

    bool set(float a, float r)
    {
        if (!checkValues(a, r)) {
            OBJ_ERR << "invalid values: " << a << ", " << r;
            return false;
        }

        prop_attack_->setValue(a);
        prop_release_->setValue(r);
        return true;
    }

    float length() const
    {
        return prop_attack_->value() + prop_release_->value();
    }
};

void setup_env_ar_tilde()
{
    SoundExternalFactory<EnvAr> obj("env.ar~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("reset", &EnvAr::m_reset);
    obj.addClick(&EnvAr::m_click);
}
