#ifndef PARAM_FLOAT_H
#define PARAM_FLOAT_H

#include "ceammc_object.h"

#include <string>
#include <vector>

using namespace ceammc;

class PresetFloat : public BaseObject {
    FloatProperty* init_;
    SymbolProperty* name_;
    SymbolProperty* path_;
    t_symbol* preset_path_;
    FlagProperty* global_;
    FlagProperty* subpatch_;
    t_float current_value_;

public:
    PresetFloat(const PdArgs& args);
    ~PresetFloat();

    void onFloat(float f);

    void m_clear(t_symbol*, const AtomList&);
    void m_store(t_symbol*, const AtomList& l);
    void m_load(t_symbol*, const AtomList& l);
    void m_update(t_symbol*, const AtomList&);

    t_symbol* makePresetPath() const;
    t_symbol* makePath() const;

    void bindPreset();
    void unbindPreset();
};

void setup_preset_float();

#endif // PARAM_FLOAT_H
