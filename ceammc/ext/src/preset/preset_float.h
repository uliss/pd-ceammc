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
    void onFloat(float f);

    void loadFrom(size_t idx);
    void storeAt(size_t idx);
};

void setup_preset_float();

#endif // PARAM_FLOAT_H
