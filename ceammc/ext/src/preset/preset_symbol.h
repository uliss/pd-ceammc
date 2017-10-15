#ifndef PRESET_SYMBOL_H
#define PRESET_SYMBOL_H

#include "ceammc_object.h"
#include "preset_base.h"

using namespace ceammc;

class PresetSymbol : public PresetBase {
    SymbolProperty* init_;
    t_symbol* current_value_;

public:
    PresetSymbol(const PdArgs& args);
    void onSymbol(t_symbol* s);

    void loadFrom(size_t idx);
    void storeAt(size_t idx);
};

void setup_preset_symbol();

#endif // PRESET_SYMBOL_H
