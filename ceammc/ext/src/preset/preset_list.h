#ifndef PRESET_LIST_H
#define PRESET_LIST_H

#include "preset_base.h"

class PresetList : public PresetBase {
    ListProperty* init_;
    AtomList current_value_;

public:
    PresetList(const PdArgs& args);

    void onList(const AtomList& lst);
    void loadFrom(size_t idx);
    void storeAt(size_t idx);
};

void setup_preset_list();

#endif // PRESET_LIST_H
