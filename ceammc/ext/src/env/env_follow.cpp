#include "env_follow.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");

class EnvFollow : public faust_env_follow_tilde {
    UIProperty* prop_attack_;
    UIProperty* prop_release_;

public:
    EnvFollow(const PdArgs& args)
        : faust_env_follow_tilde(args)
        , prop_attack_((UIProperty*)property(SYM_PROP_ATTACK))
        , prop_release_((UIProperty*)property(SYM_PROP_RELEASE))
    {
        bindPositionalArgsToProps({ SYM_PROP_ATTACK, SYM_PROP_RELEASE });
    }
};

void setup_env_follow_tilde()
{
    SoundExternalFactory<EnvFollow> obj("env.follow~");
}
