#include "env_vscale.h"
#include "ceammc_factory.h"

EnvValueScale::EnvValueScale(const PdArgs& args)
    : BaseObject(args)
    , scale_(0)
{
    scale_ = new FloatProperty("@scale", 1);
    scale_->setArgIndex(0);
    addProperty(scale_);

    createOutlet();
}

void EnvValueScale::onDataT(const DataTPtr<DataTypeEnv>& dptr)
{
    DataTypeEnv res(*dptr);
    res.scaleValue(scale_->value());

    dataTo(0, DataTPtr<DataTypeEnv>(res));
}

void setup_env_vscale()
{
    ObjectFactory<EnvValueScale> obj("env.vscale");
    obj.processData<DataTypeEnv>();
}
