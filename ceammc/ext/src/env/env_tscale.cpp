#include "env_tscale.h"
#include "ceammc_factory.h"

EnvTimeScale::EnvTimeScale(const PdArgs& a)
    : BaseObject(a)
    , scale_(0)
{
    scale_ = new FloatProperty("@scale", 1);
    scale_->setArgIndex(0);
    scale_->checkPositive();
    createProperty(scale_);

    createOutlet();
}

void EnvTimeScale::onDataT(const DataTPtr<DataTypeEnv>& dptr)
{
    DataTypeEnv env(*dptr);
    env.scaleTime(scale_->value());
    dataTo(0, DataTPtr<DataTypeEnv>(env));
}

void setup_env_tscale()
{
    ObjectFactory<EnvTimeScale> obj("env.tscale");
    obj.processData<DataTypeEnv>();
}
