#include "env_tshift.h"
#include "ceammc_factory.h"

EnvTimeShift::EnvTimeShift(const PdArgs& args)
    : BaseObject(args)
    , shift_(0)
{

    shift_ = new FloatProperty("@shift", positionalFloatArgument(0, 0));
    createProperty(shift_);

    createOutlet();
}

void EnvTimeShift::onDataT(const DataTypeEnv& env)
{
    DataTypeEnv* e = env.clone();

    if (!e->empty()) {
        if (shift_->value() < 0 && e->pointAt(0).timeMs() < (-shift_->value())) {
            OBJ_ERR << "invalid shift value: " << shift_->value();
            return;
        }

        e->shiftTime(shift_->value() * 1000);
    }

    dataTo(0, DataPtr(e));
}

void setup_env_tshift()
{
    ObjectFactory<EnvTimeShift> obj("env.tshift");
    obj.processData<DataTypeEnv>();
}
