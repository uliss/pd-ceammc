#include "env_tshift.h"
#include "ceammc_factory.h"

EnvTimeShift::EnvTimeShift(const PdArgs& args)
    : BaseObject(args)
    , shift_(0)
{

    shift_ = new FloatProperty("@shift", positionalFloatArgument(0, 0));
    createProperty(shift_);
    shift_->info().setUnits(PropertyInfoUnits::MSEC);

    createOutlet();
}

void EnvTimeShift::onDataT(const DataTPtr<DataTypeEnv>& dptr)
{
    DataTypeEnv env(*dptr);

    if (!env.empty()) {
        if (shift_->value() < 0 && env.pointAt(0).timeMs() < (-shift_->value())) {
            OBJ_ERR << "invalid shift value: " << shift_->value();
            return;
        }

        env.shiftTime(shift_->value() * 1000);
    }

    dataTo(0, DataTPtr<DataTypeEnv>(env));
}

void setup_env_tshift()
{
    ObjectFactory<EnvTimeShift> obj("env.tshift");
    obj.processData<DataTypeEnv>();
}
