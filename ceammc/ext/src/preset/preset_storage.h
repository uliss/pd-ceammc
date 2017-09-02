#ifndef PARAM_STORAGE_H
#define PARAM_STORAGE_H

#include "ceammc_atomlist.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <boost/shared_ptr.hpp>
#include <map>

using namespace ceammc;

class Preset {
    mutable t_symbol* name_;
    mutable t_symbol* bind_addr_;
    std::vector<Message> data_;

public:
    static t_symbol* SYM_NONE;
    static t_symbol* SYM_LOAD;
    static t_symbol* SYM_STORE;

public:
    Preset(t_symbol* name);

    t_symbol* name() const;
    t_symbol* bindName() const;

    std::vector<Message>& data();
    const std::vector<Message>& data() const;

    bool hasDataAt(size_t idx) const;
    bool hasDataTypeAt(size_t idx, Message::Type t) const;
    bool hasFloatAt(size_t idx) { return hasDataTypeAt(idx, Message::FLOAT); }
    bool hasSymbolAt(size_t idx) { return hasDataTypeAt(idx, Message::SYMBOL); }
    bool hasListAt(size_t idx) { return hasDataTypeAt(idx, Message::LIST); }
    bool hasAnyAt(size_t idx) { return hasDataTypeAt(idx, Message::ANY); }

    float floatAt(size_t idx, float def = 0) const;
    bool setFloatAt(size_t idx, float v);
    t_symbol* symbolAt(size_t idx, t_symbol* def = SYM_NONE) const;
    AtomList listAt(size_t idx, const AtomList& def = AtomList()) const;
    AtomList anyAt(size_t idx, const AtomList& def = AtomList()) const;

    bool storeAt(size_t idx);
    bool loadAt(size_t idx);
    bool clearAt(size_t idx);

public:
    static t_symbol* makeBindAddress(t_symbol* name);
};

typedef boost::shared_ptr<Preset> PresetPtr;

class PresetStorage {
    PresetStorage();
    PresetStorage(const PresetStorage&);
    void operator=(const PresetStorage&);

private:
    typedef std::map<t_symbol*, PresetPtr> PresetMap;
    PresetMap params_;

public:
    static PresetStorage& instance();
    size_t maxPresetCount() const;

    bool setFloatValueAt(t_symbol* name, size_t presetIdx, float v);
    float floatValueAt(t_symbol* name, size_t presetIdx, float def = 0) const;
    bool clearValueAt(t_symbol* name, size_t presetIdx);

    bool hasValueAt(t_symbol* name, size_t presetIdx) const;
    bool hasValueTypeAt(t_symbol* name, Message::Type t, size_t presetIdx) const;
    bool hasFloatValueAt(t_symbol* name, size_t presetIdx);

    void remove(t_symbol* name);
    bool loadAll(size_t presetIdx);
    bool storeAll(size_t presetIdx);

    bool write(const char* path) const;
    bool read(const char* path);

    AtomList keys() const;

    void createPreset(t_symbol* name);
    bool hasPreset(t_symbol* name);

private:
    PresetPtr
    getOrCreateParam(t_symbol* name);
};

class PresetExternal : public BaseObject {
    t_canvas* cnv_;
    t_canvas* root_cnv_;
    std::string patch_dir_;

public:
    PresetExternal(const PdArgs& args);

    AtomList p_keys() const;

    void m_load(t_symbol*, const AtomList& l);
    void m_store(t_symbol*, const AtomList& l);
    void m_write(t_symbol*, const AtomList& fname);
    void m_read(t_symbol*, const AtomList& fname);

    std::string makePresetPath() const;
};

void setup_preset_storage();

#endif // PARAM_STORAGE_H
