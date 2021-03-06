#ifndef PARAM_STORAGE_H
#define PARAM_STORAGE_H

#include "ceammc_object.h"

using namespace ceammc;

class PresetExternal : public BaseObject {
    std::string patch_dir_;

public:
    PresetExternal(const PdArgs& args);

    AtomList p_keys() const;

    void m_load(t_symbol*, const AtomListView& l);
    void m_interp(t_symbol*, const AtomListView& l);
    void m_store(t_symbol*, const AtomListView& l);
    void m_clear(t_symbol*, const AtomListView& l);
    void m_write(t_symbol*, const AtomListView& fname);
    void m_read(t_symbol*, const AtomListView& fname);
    void m_update(t_symbol*, const AtomListView&);
    void m_duplicate(t_symbol*, const AtomListView& l);

    std::string makeDefaultPresetPath() const;
};

void setup_preset_storage();

#endif // PARAM_STORAGE_H
