#include "env_ar.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

#include "env_faust_base.h"

using namespace ceammc;

class EnvAr : public EnvFaustBase<faust_env_ar_tilde> {
public:
    EnvAr(const PdArgs& args)
        : EnvFaustBase<faust_env_ar_tilde>(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_ATTACK, SYM_PROP_RELEASE });
        createProperty(new CombinedProperty("@ar", { property(SYM_PROP_ATTACK), property(SYM_PROP_RELEASE) }));
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

private:
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
};

void setup_env_ar_tilde()
{
    SoundExternalFactory<EnvAr> obj("env.ar~");
    obj.processData<DataTypeEnv>();
    obj.addMethod("reset", &EnvAr::m_reset);
    obj.addClick(&EnvAr::m_click);
}
