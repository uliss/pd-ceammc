#include "env_concat.h"
#include "ceammc_factory.h"

EnvConcat::EnvConcat(const PdArgs& args)
    : BaseObject(args)
    , right_(0)
{
    createInlet();
    createOutlet();
}

void EnvConcat::onBang()
{
    dataTo(0, DataPtr(res_env_.clone()));
}

void EnvConcat::onDataT(const DataTypeEnv& env)
{
    if (right_.isValid())
        res_env_ = env + *right_.data();
    else
        res_env_ = env;

    onBang();
}

void EnvConcat::onInlet(size_t n, const AtomList& lst)
{
    if (n == 1) {
        if (lst.empty())
            return;

        right_ = DataTPtr<DataTypeEnv>(lst[0]);

        if (!right_.isValid()) {
            OBJ_ERR << "envelope data type expected";
            return;
        }
    }
}

void setup_env_concat()
{
    ObjectFactory<EnvConcat> env("env.concat");
    env.processData<DataTypeEnv>();
}
