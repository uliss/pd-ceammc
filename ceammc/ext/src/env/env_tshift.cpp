#include "env_tshift.h"
#include "ceammc_factory.h"

EnvTimeShift::EnvTimeShift(const PdArgs& args)
    : BaseObject(args)
    , shift_(nullptr)
{
    shift_ = new FloatProperty("@shift", 0);
    shift_->setArgIndex(0);
    shift_->setUnitsMs();

    addProperty(shift_);

    createOutlet();
}

void EnvTimeShift::onDataT(const EnvAtom& env)
{
    if (env->empty() || shift_->value() == 0) {
        atomTo(0, env);
        return;
    }

    if (shift_->value() < 0 && env->pointAt(0).timeMs() < (-shift_->value())) {
        OBJ_ERR << "invalid shift value: " << shift_->value();
        return;
    }

    EnvAtom res = env;
    res.detachData();
    res->shiftTime(shift_->value() * 1000);
    atomTo(0, res);
}

void setup_env_tshift()
{
    ObjectFactory<EnvTimeShift> obj("env.tshift");
    obj.processData<DataTypeEnv>();

    obj.setDescription("Envelope time-shift (horizontal)");
    obj.setCategory("env");
    obj.setKeywords({"time", "shift"});
}
