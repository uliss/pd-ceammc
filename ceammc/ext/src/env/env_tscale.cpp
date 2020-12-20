#include "env_tscale.h"
#include "ceammc_factory.h"

EnvTimeScale::EnvTimeScale(const PdArgs& a)
    : BaseObject(a)
    , scale_(0)
{
    scale_ = new FloatProperty("@scale", 1);
    scale_->setArgIndex(0);
    scale_->checkPositive();
    addProperty(scale_);

    createOutlet();
}

void EnvTimeScale::onDataT(const EnvAtom& env)
{
    EnvAtom res = env;
    res.detachData();
    res->scaleTime(scale_->value());
    atomTo(0, res);
}

void setup_env_tscale()
{
    ObjectFactory<EnvTimeScale> obj("env.tscale");
    obj.processData<DataTypeEnv>();
}
