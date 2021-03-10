#include "env_mix.h"
#include "ceammc_factory.h"

EnvMix::EnvMix(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createInlet();
    createOutlet();
}

void EnvMix::onFloat(t_float v)
{
    // no interpoltaion needed
    if (env0_ == env1_)
        return;

    if (v < 0 || v > 1) {
        OBJ_ERR << "value in range: [0-1] expected, got: " << v;
        return;
    }

    EnvAtom env;
    if (!DataTypeEnv::interpolated(*env0_, *env1_, v, *env)) {
        OBJ_ERR << "can't interpolate envelopes";
        return;
    }

    atomTo(0, env);
}

void EnvMix::onInlet(size_t n, const AtomListView& lst)
{
    if (n == 1 || n == 2) {
        if (!lst.isA<DataTypeEnv>()) {
            OBJ_ERR << "envelope data type expected";
            return;
        }

        if (n == 1)
            env0_ = EnvAtom(lst[0]);
        else if (n == 2)
            env1_ = EnvAtom(lst[0]);
    }
}

void EnvMix::dump() const
{
    BaseObject::dump();

    OBJ_DBG << "first:  " << env0_;
    OBJ_DBG << "second: " << env1_;
}

void setup_env_mix()
{
    ObjectFactory<EnvMix> obj("env.mix");
}
