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
    void onSymbol(t_symbol* s) override;
    void initDone() override;

    void loadFrom(t_float idx) override;
    void storeAt(size_t idx) override;

    AtomList editorPresetValue(size_t idx) const final;
    bool setEditorPreset(size_t idx, const AtomListView& lv) final;
};

void setup_preset_symbol();

#endif // PRESET_SYMBOL_H
