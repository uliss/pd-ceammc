#include "env_ar2.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "datatype_env.h"

static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");
static t_symbol* SYM_PROP_TRIGGER = gensym("@trigger");

using namespace ceammc;

class EnvAr2 : public faust_ar2_tilde {
    ClockMemberFunction<EnvAr2> clock_;

public:
    EnvAr2(const PdArgs& args)
        : faust_ar2_tilde(args)
        , clock_(this, &EnvAr2::release)
    {
        bindPositionalArgsToProps({ SYM_PROP_ATTACK, SYM_PROP_RELEASE });
        createProperty(new CombinedProperty("@ar", { property(SYM_PROP_ATTACK), property(SYM_PROP_RELEASE) }));
    }

    void onBang() override
    {
        property(SYM_PROP_TRIGGER)->set(Atom(1));
        clock_.delay(10);
    }

    void onDataT(const DataTypeEnv& env)
    {
        if (!env.isAR()) {
            OBJ_ERR << "not an AR envelope: " << env;
            return;
        }

        float attack = env.pointAt(1).timeMs() - env.pointAt(0).timeMs();
        float release = env.pointAt(2).timeMs() - env.pointAt(1).timeMs();
        property(SYM_PROP_ATTACK)->set(Atom(attack));
        property(SYM_PROP_RELEASE)->set(Atom(release));
    }

private:
    void release()
    {
        property(SYM_PROP_TRIGGER)->set(Atom(0.f));
    }
};

void setup_env_ar2()
{
    SoundExternalFactory<EnvAr2> obj("env.ar2~");
    obj.processData<DataTypeEnv>();
}
