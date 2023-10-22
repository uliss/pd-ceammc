#include "env_vscale.h"
#include "ceammc_factory.h"

EnvValueScale::EnvValueScale(const PdArgs& args)
    : BaseObject(args)
    , scale_(nullptr)
{
    scale_ = new FloatProperty("@scale", 1);
    scale_->setArgIndex(0);
    addProperty(scale_);

    createOutlet();
}

void EnvValueScale::onDataT(const EnvAtom& env)
{
    DataTypeEnv res(*env);
    res.scaleValue(scale_->value());

    atomTo(0, EnvAtom(res));
}

void setup_env_vscale()
{
    ObjectFactory<EnvValueScale> obj("env.vscale");
    obj.processData<DataTypeEnv>();

    obj.setDescription("Envelope value-scale");
    obj.setCategory("env");
    obj.setKeywords({"value", "scale"});
}
