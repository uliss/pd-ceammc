#include "env_concat.h"
#include "ceammc_factory.h"

EnvConcat::EnvConcat(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createOutlet();
}

void EnvConcat::onBang()
{
    atomTo(0, res_env_);
}

void EnvConcat::onDataT(const EnvAtom& dptr)
{
    res_env_ = EnvAtom(*dptr + *right_);
    onBang();
}

void EnvConcat::onInlet(size_t n, const AtomList& lst)
{
    if (n == 1) {
        if (!lst.isA<DataTypeEnv>()) {
            OBJ_ERR << "envelope data type expected";
            return;
        }

        right_ = EnvAtom(lst[0]);
    }
}

void setup_env_concat()
{
    ObjectFactory<EnvConcat> env("env.concat");
    env.processData<DataTypeEnv>();
}
