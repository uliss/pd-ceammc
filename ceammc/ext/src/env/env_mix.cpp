#include "env_mix.h"
#include "ceammc_dataatom.h"
#include "ceammc_datatypes.h"
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
    if (env0_.isEqual(&env1_))
        return;

    if (v < 0 || v > 1) {
        OBJ_ERR << "value in range: [0-1] expected";
        return;
    }

    DataTypeEnv env;
    if (!DataTypeEnv::interpolated(env0_, env1_, v, env)) {
        OBJ_ERR << "can't interpolate envelopes";
        return;
    }

    dataTo(0, DataPtr(env.clone()));
}

void EnvMix::onInlet(size_t n, const AtomList& lst)
{
    if (n == 1 || n == 2) {
        if (lst.empty())
            return;

        DataAtom a(lst[0]);

        if (!a.isData()) {
            OBJ_ERR << "envelope data type expected";
            return;
        }

        DataPtr dptr = a.data();
        if (dptr->type() != data::DATA_ENVELOPE) {
            OBJ_ERR << "envelope data type expected";
            return;
        }

        if (n == 1)
            env0_ = *dptr->as<DataTypeEnv>();
        else if (n == 2)
            env1_ = *dptr->as<DataTypeEnv>();
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
