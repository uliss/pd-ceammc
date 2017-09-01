#include "preset_storage.h"

#include <boost/make_shared.hpp>
#include <fstream>

static const size_t MAX_PRESET_COUNT = 16;
t_symbol* Preset::SYM_NONE = gensym("");
t_symbol* Preset::SYM_LOAD = gensym("load");
t_symbol* Preset::SYM_STORE = gensym("store");

PresetStorage::PresetStorage()
{
}

PresetStorage& PresetStorage::instance()
{
    static PresetStorage instance_;
    return instance_;
}

size_t PresetStorage::maxPresetCount() const
{
    return MAX_PRESET_COUNT;
}

bool PresetStorage::setFloatValueAt(t_symbol* name, size_t presetIdx, float v)
{
    PresetPtr param = getOrCreateParam(name);
    return param->setFloatAt(presetIdx, v);
}

bool PresetStorage::clearValueAt(t_symbol* name, size_t presetIdx)
{
    PresetMap::iterator it = params_.find(name);
    if (it == params_.end())
        return false;

    return it->second->clearAt(presetIdx);
}

float PresetStorage::floatValueAt(t_symbol* name, size_t presetIdx, float def) const
{
    PresetMap::const_iterator it = params_.find(name);
    if (it == params_.end())
        return def;

    return it->second->floatAt(presetIdx, def);
}

bool PresetStorage::hasValueAt(t_symbol* name, size_t presetIdx) const
{
    PresetMap::const_iterator it = params_.find(name);
    if (it == params_.end())
        return false;

    return it->second->hasDataAt(presetIdx);
}

bool PresetStorage::hasValueTypeAt(t_symbol* name, Message::Type t, size_t presetIdx) const
{
    PresetMap::const_iterator it = params_.find(name);
    if (it == params_.end())
        return false;

    return it->second->hasDataTypeAt(presetIdx, t);
}

bool PresetStorage::hasFloatValueAt(t_symbol* name, size_t presetIdx)
{
    return hasValueTypeAt(name, Message::FLOAT, presetIdx);
}

void PresetStorage::remove(t_symbol* name)
{
    params_.erase(name);
}

bool PresetStorage::loadAll(size_t presetIdx)
{
    if (presetIdx >= maxPresetCount())
        return false;

    PresetMap::iterator it;
    for (it = params_.begin(); it != params_.end(); ++it) {
        it->second->loadAt(presetIdx);
    }

    return true;
}

bool PresetStorage::storeAll(size_t presetIdx)
{
    if (presetIdx >= maxPresetCount())
        return false;

    PresetMap::iterator it;
    for (it = params_.begin(); it != params_.end(); ++it) {
        it->second->storeAt(presetIdx);
    }

    return true;
}

bool PresetStorage::write(const char* path) const
{
    t_binbuf* content = binbuf_new();

    PresetMap::const_iterator it;
    for (it = params_.begin(); it != params_.end(); ++it) {
        PresetPtr ptr = it->second;

        for (size_t i = 0; i < maxPresetCount(); i++) {
            if (!ptr->hasDataAt(i))
                continue;

            binbuf_addv(content, "ss", &s_symbol, ptr->name());

            if (ptr->hasFloatAt(i)) {
                binbuf_addv(content, "sf", &s_float, ptr->floatAt(i));
            }

            binbuf_addsemi(content);
        }
    }

    int rc = binbuf_write(content, (char*)path, NULL, 0);
    binbuf_free(content);

    return rc == 0;
}

bool PresetStorage::read(const char* path)
{
    t_binbuf* content = binbuf_new();
    int rc = binbuf_read(content, (char*)path, NULL, 0);

    //    binbuf_eval();

    binbuf_free(content);
    return rc == 0;
}

PresetPtr PresetStorage::getOrCreateParam(t_symbol* name)
{
    PresetMap::iterator it = params_.find(name);
    if (it != params_.end())
        return it->second;

    PresetPtr new_param = boost::make_shared<Preset>(name);
    params_.insert(PresetMap::value_type(name, new_param));
    return new_param;
}

Preset::Preset(t_symbol* name)
    : name_(name)
    , bind_addr_(makeBindAddress(name))
{
    data_.assign(MAX_PRESET_COUNT, Message());
}

t_symbol* Preset::name() const
{
    return name_;
}

t_symbol* Preset::bindName() const
{
    return bind_addr_;
}

std::vector<Message>& Preset::data()
{
    return data_;
}

const std::vector<Message>& Preset::data() const
{
    return data_;
}

bool Preset::hasDataAt(size_t idx) const
{
    if (idx >= data_.size())
        return false;

    return !data_[idx].isNone();
}

bool Preset::hasDataTypeAt(size_t idx, Message::Type t) const
{
    if (idx >= data_.size())
        return false;

    return data_[idx].type() == t;
}

float Preset::floatAt(size_t idx, float def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isFloat() ? data_[idx].atomValue().asFloat() : def;
}

bool Preset::setFloatAt(size_t idx, float v)
{
    if (idx >= data_.size())
        return false;

    data_[idx].setFloat(v);
    return true;
}

t_symbol* Preset::symbolAt(size_t idx, t_symbol* def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isSymbol() ? data_[idx].atomValue().asSymbol() : def;
}

AtomList Preset::listAt(size_t idx, const AtomList& def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isList() ? data_[idx].listValue() : def;
}

AtomList Preset::anyAt(size_t idx, const AtomList& def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isAny() ? data_[idx].anyValue() : def;
}

bool Preset::storeAt(size_t idx)
{
    if (idx >= data_.size())
        return false;

    if (!bind_addr_->s_thing)
        return false;

    t_atom a;
    SETFLOAT(&a, idx);
    pd_typedmess(bind_addr_->s_thing, SYM_STORE, 1, &a);
    return true;
}

bool Preset::loadAt(size_t idx)
{
    if (idx >= data_.size())
        return false;

    if (!bind_addr_->s_thing)
        return false;

    t_atom a;
    SETFLOAT(&a, idx);
    pd_typedmess(bind_addr_->s_thing, SYM_LOAD, 1, &a);
    return true;
}

bool Preset::clearAt(size_t idx)
{
    if (idx >= data_.size())
        return false;

    data_[idx] = Message();
    return true;
}

t_symbol* Preset::makeBindAddress(t_symbol* name)
{
    std::string res("preset: ");
    res += name->s_name;
    return gensym(res.c_str());
}
