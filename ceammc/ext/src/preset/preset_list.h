#ifndef PRESET_LIST_H
#define PRESET_LIST_H

#include "preset_base.h"

class PresetList : public PresetBase {
    ListProperty* init_;
    AtomList current_value_;

public:
    PresetList(const PdArgs& args);
    void initDone() override;

    void onList(const AtomList& lst) override;
    void loadFrom(t_float idx) override;
    void storeAt(size_t idx) override;
};

void setup_preset_list();

#endif // PRESET_LIST_H
