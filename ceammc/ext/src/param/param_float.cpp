#include "param_float.h"
#include "ceammc_factory.h"

ParamFloat::ParamFloat(const PdArgs& args)
    : BaseObject(args)
    , max_(0)
    , init_(0)
{
    max_ = new SizeTProperty("@max", 10, true);
    createProperty(max_);

    init_ = new FloatProperty("@init", 0, true);
    createProperty(init_);

    params_.assign(max_->value(), init_->value());

    createOutlet();
}

void ParamFloat::onFloat(float f)
{
}

void ParamFloat::m_clear(t_symbol*, const AtomList&)
{
    params_.assign(max_->value(), init_->value());
}

void setup_param_float()
{
    ObjectFactory<ParamFloat> obj("param.float");
    obj.addMethod("clear", &ParamFloat::m_clear);
}
