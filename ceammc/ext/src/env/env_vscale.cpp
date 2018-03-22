#include "env_vscale.h"
#include "ceammc_factory.h"

EnvValueScale::EnvValueScale(const PdArgs& args)
    : BaseObject(args)
    , scale_(0)
{
    scale_ = new FloatProperty("@scale", positionalFloatArgument(0, 1));
    createProperty(scale_);

    createOutlet();
}

void EnvValueScale::onDataT(const DataTypeEnv& env)
{
    DataTypeEnv res(env);
    res.scaleValue(scale_->value());

    dataTo(0, DataPtr(res.clone()));
}

void setup_env_vscale()
{
    ObjectFactory<EnvValueScale> obj("env.vscale");
    obj.processData<DataTypeEnv>();
}
