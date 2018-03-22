#include "env_tscale.h"
#include "ceammc_factory.h"

EnvTimeScale::EnvTimeScale(const PdArgs& a)
    : BaseObject(a)
    , scale_(0)
{
    scale_ = new FloatPropertyMin("@scale", positionalFloatArgument(0, 1), 0);
    createProperty(scale_);

    createOutlet();
}

void EnvTimeScale::onDataT(const DataTypeEnv& env)
{
    DataTypeEnv* e = env.clone();
    e->scaleTime(scale_->value());
    dataTo(0, DataPtr(e));
}

void setup_env_tscale()
{
    ObjectFactory<EnvTimeScale> obj("env.tscale");
    obj.processData<DataTypeEnv>();
}
