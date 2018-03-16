#ifndef PARAM_STORAGE_H
#define PARAM_STORAGE_H

#include "ceammc_object.h"

using namespace ceammc;

class PresetExternal : public BaseObject {
    t_canvas* root_cnv_;
    std::string patch_dir_;

public:
    PresetExternal(const PdArgs& args);

    AtomList p_keys() const;

    void m_load(t_symbol*, const AtomList& l);
    void m_store(t_symbol*, const AtomList& l);
    void m_clear(t_symbol*, const AtomList& l);
    void m_write(t_symbol*, const AtomList& fname);
    void m_read(t_symbol*, const AtomList& fname);
    void m_update(t_symbol*, const AtomList&);

    std::string makeDefaultPresetPath() const;
};

void setup_preset_storage();

#endif // PARAM_STORAGE_H
