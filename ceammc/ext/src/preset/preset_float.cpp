#include "preset_float.h"
#include "ceammc_factory.h"

PresetFloat::PresetFloat(const PdArgs& args)
    : PresetBase(args)
    , init_(0)
{
    init_ = new FloatProperty("@init", 0, true);
    current_value_ = init_->value();
    createProperty(init_);
}

void PresetFloat::onFloat(float f)
{
    current_value_ = f;
}

void PresetFloat::loadFrom(size_t idx)
{
    current_value_ = loadFloat(idx, init_->value());
    floatTo(0, current_value_);
}

void PresetFloat::storeAt(size_t idx)
{
    storeFloat(current_value_, idx);
}

void setup_preset_float()
{
    ObjectFactory<PresetFloat> obj("preset.float");
    //    obj.addMethod("clear", &PresetFloat::m_clear);
    obj.addMethod("store", &PresetFloat::m_store);
    obj.addMethod("load", &PresetFloat::m_load);
    obj.addMethod("update", &PresetFloat::m_update);
}
