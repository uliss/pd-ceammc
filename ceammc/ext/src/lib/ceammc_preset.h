#ifndef CEAMMC_PRESET_H
#define CEAMMC_PRESET_H

#include "ceammc_message.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ceammc {

class Preset {
    mutable t_symbol* name_;
    std::vector<Message> data_;
    int ref_count_;

public:
    static const char* SYM_PRESET_ALL;

public:
    Preset(t_symbol* name);

    t_symbol* name() const;

    std::vector<Message>& data();
    const std::vector<Message>& data() const;

    bool hasDataAt(size_t idx) const;
    bool hasDataTypeAt(size_t idx, Message::Type t) const;
    bool hasFloatAt(size_t idx) { return hasDataTypeAt(idx, Message::FLOAT); }
    bool hasSymbolAt(size_t idx) { return hasDataTypeAt(idx, Message::SYMBOL); }
    bool hasListAt(size_t idx) { return hasDataTypeAt(idx, Message::LIST); }
    bool hasAnyAt(size_t idx) { return hasDataTypeAt(idx, Message::ANY); }

    bool copyData(size_t src_idx, size_t dst_idx);
    bool duplicate();

    t_float floatAt(t_float fidx, t_float def = 0) const;
    t_symbol* symbolAt(size_t idx, t_symbol* def = &s_) const;
    AtomListView listAt(size_t idx, const AtomListView& def = AtomListView()) const;
    AtomList anyAt(size_t idx, const AtomList& def = AtomList()) const;

    bool setFloatAt(size_t idx, t_float v);
    bool setSymbolAt(size_t idx, t_symbol* v);
    bool setListAt(size_t idx, const AtomList& l);
    bool setAnyAt(size_t idx, t_symbol* sel, const AtomList& args);

    bool clearAt(size_t idx);

    void refCountUp() { ref_count_++; }
    void refCountDown() { ref_count_--; }
    int refCount() const { return ref_count_; }
};

typedef std::shared_ptr<Preset> PresetPtr;

class PresetStorage {
    PresetStorage();
    PresetStorage(const PresetStorage&);
    void operator=(const PresetStorage&);

private:
    typedef std::unordered_map<t_symbol*, PresetPtr> PresetMap;
    typedef std::unordered_set<t_symbol*> PresetNameSet;
    typedef std::vector<PresetNameSet> IndexMap;
    PresetMap params_;
    IndexMap indexes_;

public:
    static PresetStorage& instance();
    size_t maxPresetCount() const;

    bool setFloatValueAt(t_symbol* name, size_t presetIdx, t_float v);
    t_float floatValueAt(t_symbol* name, t_float presetIdx, t_float def = 0) const;
    bool clearValueAt(t_symbol* name, size_t presetIdx);

    bool setSymbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* v);
    t_symbol* symbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* def) const;

    bool setListValueAt(t_symbol* name, size_t presetIdx, const AtomList& l);
    AtomListView listValueAt(t_symbol* name, size_t presetIdx, const AtomListView& def = AtomListView()) const;

    bool setAnyValueAt(t_symbol* name, size_t presetIdx, t_symbol* sel, const AtomList& l);
    AtomList anyValueAt(t_symbol* name, size_t presetIdx, const AtomList& def = AtomList()) const;

    bool hasValueAt(t_symbol* name, size_t presetIdx) const;
    bool hasValueTypeAt(t_symbol* name, Message::Type t, size_t presetIdx) const;
    bool hasFloatValueAt(t_symbol* name, size_t presetIdx);

    bool write(t_canvas* c, const std::string& path) const;
    bool write(const char* path) const;
    bool read(t_canvas* c, const std::string& path);
    bool read(const char* path);

    AtomList keys() const;
    bool hasIndex(size_t sz) const;

    bool hasPreset(t_symbol* name);
    void bindPreset(t_symbol* name);
    void unbindPreset(t_symbol* name);

    void clearAll();
    void clearAll(size_t idx);
    void loadAll(size_t idx);
    void storeAll(size_t idx);
    void updateAll();
    void duplicateAll();

public:
    t_symbol* SYM_PRESET_UPDATE_INDEX_ADDR;
    t_symbol* SYM_PRESET_INDEX_ADD;
    t_symbol* SYM_PRESET_INDEX_REMOVE;

private:
    PresetPtr getOrCreate(t_symbol* name);
    void addPresetIndex(t_symbol* name, size_t idx);
    void removePresetIndex(t_symbol* name, size_t idx);
};
}

#endif // CEAMMC_PRESET_H
