#ifndef PRESET_BASE_H
#define PRESET_BASE_H

#include "ceammc_object.h"

using namespace ceammc;

class PresetBase : public BaseObject {
    FlagProperty* global_;
    FlagProperty* subpatch_;

    SymbolProperty* name_;
    t_symbol* path_;
    t_symbol* preset_path_;

public:
    PresetBase(const PdArgs& args);
    ~PresetBase();

    t_symbol* makePath() const;
    t_symbol* makePresetPath() const;

    t_symbol* path() { return path_; }
    t_symbol* presetPath() { return preset_path_; }

public:
    void m_update(t_symbol*, const AtomList&);

private:
    void bind();
    void unbind();
};

#endif // PRESET_BASE_H
