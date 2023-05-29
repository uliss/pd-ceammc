#include "env_follow.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class EnvFollow : public faust_env_follow_tilde {
    UIProperty* prop_attack_;
    UIProperty* prop_release_;

public:
    EnvFollow(const PdArgs& args)
        : faust_env_follow_tilde(args)
        , prop_attack_(findUIProperty("@attack"))
        , prop_release_(findUIProperty("@release"))
    {
        bindPositionalArgsToProps({ gensym("@attack"), gensym("@release") });
    }
};

void setup_env_follow_tilde()
{
    FaustFactory<EnvFollow> obj("env.follow~");

    obj.setDescription("Envelope follower with independent attack and release times");
    obj.setCategory("env");
    obj.setKeywords({ "envelope", "amplitude", "follower" });
}
