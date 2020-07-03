#ifndef PARAM_FLOAT_H
#define PARAM_FLOAT_H

#include "ceammc_object.h"
#include "preset_base.h"

using namespace ceammc;

class PresetFloat : public PresetBase {
    FloatProperty* init_;
    t_float current_value_;

public:
    PresetFloat(const PdArgs& args);
    void onFloat(t_float f) override;
    void initDone() override;

    void loadFrom(size_t idx) override;
    void storeAt(size_t idx) override;
};

void setup_preset_float();

#endif // PARAM_FLOAT_H
