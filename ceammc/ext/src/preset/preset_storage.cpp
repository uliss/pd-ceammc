#include "preset_storage.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <boost/make_shared.hpp>
#include <fstream>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

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

            binbuf_addv(content, "sf", ptr->name(), double(i));

            if (ptr->hasFloatAt(i))
                binbuf_addv(content, "sf", &s_float, ptr->floatAt(i));
            else if (ptr->hasSymbolAt(i))
                binbuf_addv(content, "ss", &s_symbol, ptr->symbolAt(i));

            binbuf_addsemi(content);
        }
    }

    int rc = binbuf_write(content, (char*)path, (char*)"", 0);
    binbuf_free(content);

    return rc == 0;
}

bool PresetStorage::read(const char* path)
{
    t_binbuf* content = binbuf_new();
    int rc = binbuf_read(content, (char*)path, (char*)"", 0);

    std::vector<AtomList> lines;
    lines.push_back(AtomList());

    const int n = binbuf_getnatom(content);
    t_atom* lst = binbuf_getvec(content);
    for (int i = 0; i < n; i++) {
        lines.back().append(lst[i]);

        if (lst[i].a_type == A_SEMI) {
            lines.push_back(AtomList());
            continue;
        }
    }

    binbuf_free(content);

    // remove last empty list
    if (!lines.empty() && lines.back().empty())
        lines.pop_back();

    LIB_DBG << lines;

    for (size_t i = 0; i < lines.size(); i++) {
        AtomList& line = lines[i];

        if (line.size() < 4)
            continue;

        if (line[0].isSymbol() && line[1].isFloat() && line[2].isSymbol()) {
            t_symbol* name = line[0].asSymbol();
            size_t index = line[1].asSizeT();
            t_symbol* sel = line[2].asSymbol();

            if (sel == &s_float) {
                PresetStorage::instance().setFloatValueAt(name, index, line[3].asFloat());
            }

            if (sel == &s_symbol) {
//                PresetStorage::instance().setSyValueAt(name, index, line[3].asSymbol());
            }
        } else {
            LIB_ERR << "invalid preset line: " << line;
        }
    }

    return rc == 0;
}

AtomList PresetStorage::keys() const
{
    AtomList res;
    res.reserve(params_.size());

    PresetMap::const_iterator it;
    for (it = params_.begin(); it != params_.end(); ++it)
        res.append(it->first);

    return res;
}

void PresetStorage::createPreset(t_symbol* name)
{
    if (!hasPreset(name)) {
        PresetPtr ptr = boost::make_shared<Preset>(name);
        params_.insert(PresetMap::value_type(name, ptr));
    }
}

bool PresetStorage::hasPreset(t_symbol* name)
{
    return params_.find(name) != params_.end();
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

PresetExternal::PresetExternal(const PdArgs& args)
    : BaseObject(args)
    , cnv_(canvas_getcurrent())
    , root_cnv_(canvas_getrootfor(canvas_getcurrent()))
    , patch_dir_(".")
{
    createCbProperty("@keys", &PresetExternal::p_keys);

    createOutlet();

    if (root_cnv_)
        patch_dir_ = canvas_getdir(root_cnv_)->s_name;
}

AtomList PresetExternal::p_keys() const
{
    return PresetStorage::instance().keys();
}

void PresetExternal::m_load(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT))
        return;

    PresetStorage& s = PresetStorage::instance();

    size_t idx = l[0].asSizeT();
    if (idx >= s.maxPresetCount()) {
        OBJ_ERR << "invalid preset index: " << idx;
        return;
    }

    PresetStorage::instance().loadAll(idx);
}

void PresetExternal::m_store(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT))
        return;

    PresetStorage& s = PresetStorage::instance();

    size_t idx = l[0].asSizeT();
    if (idx >= s.maxPresetCount()) {
        OBJ_ERR << "invalid preset index: " << idx;
        return;
    }

    PresetStorage::instance().storeAll(idx);
}

void PresetExternal::m_write(t_symbol*, const AtomList& l)
{
    std::string fname;

    if (l.empty()) {
        if (root_cnv_) {
            fname += std::string(root_cnv_->gl_name->s_name);
            fname += "-preset.txt";
        }
    } else
        fname = to_string(l[0]);

    if (!sys_isabsolutepath(fname.c_str()))
        fname = patch_dir_ + "/" + fname;

    bool rc = PresetStorage::instance().write(fname.c_str());
    if (!rc) {
        OBJ_ERR << "can't write presets to " << fname;
        return;
    }

    OBJ_DBG << "presets written to: " << fname;
}

void PresetExternal::m_read(t_symbol*, const AtomList& l)
{
    std::string fname;

    if (l.empty()) {
        if (root_cnv_) {
            fname += std::string(root_cnv_->gl_name->s_name);
            fname += "-preset.txt";
        }
    } else
        fname = to_string(l[0]);

    if (!sys_isabsolutepath(fname.c_str()))
        fname = patch_dir_ + "/" + fname;

    OBJ_DBG << "presets read from: " << fname;

    bool rc = PresetStorage::instance().read(fname.c_str());
    if (!rc) {
        OBJ_ERR << "can't read presets from " << fname;
        return;
    }

    OBJ_DBG << "presets read from: " << fname;
}

void setup_preset_storage()
{
    ObjectFactory<PresetExternal> obj("preset.storage");
    obj.addMethod("load", &PresetExternal::m_load);
    obj.addMethod("store", &PresetExternal::m_store);
    obj.addMethod("read", &PresetExternal::m_read);
    obj.addMethod("write", &PresetExternal::m_write);
}
