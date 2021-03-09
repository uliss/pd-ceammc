#include "preset_float.h"
#include "ceammc_factory.h"
#include "preset_iface.h"

PresetFloat::PresetFloat(const PdArgs& args)
    : PresetBase(args)
    , init_(nullptr)
{
    // 1st positionalSymbolArgument is preset name - in base class
    // 2nd positionalFloatArgument we are using as init value
    init_ = new FloatProperty("@init", 0);
    init_->setArgIndex(1);
    init_->setInitOnly();
    addProperty(init_);
}

void PresetFloat::initDone()
{
    PresetBase::initDone();
    current_value_ = init_->value();
}

void PresetFloat::onFloat(t_float f)
{
    current_value_ = f;
}

void PresetFloat::loadFrom(t_float idx)
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
    PresetIFaceFactory<PresetFloat> obj("preset.float");
    obj.addAlias("preset.f");
}
