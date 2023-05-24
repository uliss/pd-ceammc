#include "ceammc_preset.h"
#include "ceammc_containers.h"
#include "ceammc_log.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"

extern "C" {
#include "g_canvas.h"
}

#include <algorithm>
#include <cmath>
#include <cstring>

namespace ceammc {

static const size_t MAX_PRESET_COUNT = 256;
const char* Preset::SYM_PRESET_ALL = ".preset update all";

PresetStorage::PresetStorage()
    : indexes_(MAX_PRESET_COUNT, PresetNameSet())
{
    SYM_PRESET_UPDATE_INDEX_ADDR = gensym(".preset index update addr");
    SYM_PRESET_INDEX_ADD = gensym(".preset index add");
    SYM_PRESET_INDEX_REMOVE = gensym(".preset index remove");
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

bool PresetStorage::setFloatValueAt(t_symbol* name, size_t presetIdx, t_float v)
{
    PresetPtr param = getOrCreate(name);
    addPresetIndex(name, presetIdx);
    return param->setFloatAt(presetIdx, v);
}

bool PresetStorage::clearValueAt(t_symbol* name, size_t presetIdx)
{
    auto it = params_.find(name);
    if (it == params_.end())
        return false;

    removePresetIndex(name, presetIdx);
    return it->second->clearAt(presetIdx);
}

bool PresetStorage::setSymbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* v)
{
    PresetPtr param = getOrCreate(name);
    addPresetIndex(name, presetIdx);
    return param->setSymbolAt(presetIdx, v);
}

t_symbol* PresetStorage::symbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* def) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return def;

    return it->second->symbolAt(presetIdx, def);
}

bool PresetStorage::setListValueAt(t_symbol* name, size_t presetIdx, const AtomList& l)
{
    PresetPtr param = getOrCreate(name);
    addPresetIndex(name, presetIdx);
    return param->setListAt(presetIdx, l);
}

AtomListView PresetStorage::listValueAt(t_symbol* name, size_t presetIdx, const AtomListView& def) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return def;

    return it->second->listAt(presetIdx, def);
}

bool PresetStorage::setAnyValueAt(t_symbol* name, size_t presetIdx, t_symbol* sel, const AtomList& l)
{
    PresetPtr param = getOrCreate(name);
    addPresetIndex(name, presetIdx);
    return param->setAnyAt(presetIdx, sel, l);
}

AtomList PresetStorage::anyValueAt(t_symbol* name, size_t presetIdx, const AtomList& def) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return def;

    return it->second->anyAt(presetIdx, def);
}

t_float PresetStorage::floatValueAt(t_symbol* name, t_float presetIdx, t_float def) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return def;

    return it->second->floatAt(presetIdx, def);
}

bool PresetStorage::hasValueAt(t_symbol* name, size_t presetIdx) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return false;

    return it->second->hasDataAt(presetIdx);
}

bool PresetStorage::hasValueTypeAt(t_symbol* name, Message::Type t, size_t presetIdx) const
{
    auto it = params_.find(name);
    if (it == params_.end())
        return false;

    return it->second->hasDataTypeAt(presetIdx, t);
}

bool PresetStorage::hasFloatValueAt(t_symbol* name, size_t presetIdx)
{
    return hasValueTypeAt(name, Message::FLOAT, presetIdx);
}

static t_canvas* rootCanvas(t_canvas* cnv)
{
    if (!cnv)
        return 0;

    t_canvas* c = cnv;
    while (c->gl_owner)
        c = c->gl_owner;

    return c;
}

static std::string makePresetFilename(t_canvas* c, const std::string& path)
{
    if (path.empty()) {
        if (c)
            return platform::strip_extension(c->gl_name->s_name) + "-preset.txt";
        else
            return "";
    } else
        return path;
}

static std::string makeFullPresetPath(t_canvas* c, const std::string& name)
{
    if (!sys_isabsolutepath(name.c_str())) {
        if (c)
            return std::string(canvas_getdir(c)->s_name) + "/" + name;
        else
            return "./" + name;
    } else
        return name;
}

bool PresetStorage::write(t_canvas* c, const std::string& path) const
{
    auto root_cnv = rootCanvas(c);
    std::string name = makePresetFilename(root_cnv, path);
    std::string full_path = makeFullPresetPath(root_cnv, name);

    bool rc = write(full_path.c_str());
    if (!rc)
        LIB_ERR << "can't write presets to " << full_path;
    else
        LIB_DBG << "presets written to: " << full_path;

    return rc;
}

bool PresetStorage::write(const char* path) const
{
    if (params_.empty()) {
        LIB_DBG << "no presets in storage";
        return false;
    }

    auto bb = binbuf_new();

    // sort keys
    std::vector<t_symbol*> keys;
    keys.reserve(params_.size());
    for (auto& p : params_)
        keys.push_back(p.first);

    std::sort(keys.begin(), keys.end(), [](t_symbol* a, t_symbol* b) { return strcmp(a->s_name, b->s_name) < 0; });

    for (auto k : keys) {
        auto it = params_.find(k);
        if (params_.end() == it)
            continue;

        auto& ptr = it->second;

        for (size_t i = 0; i < maxPresetCount(); i++) {
            if (!ptr->hasDataAt(i))
                continue;

            binbuf_addv(bb, "sf", ptr->name(), double(i));

            if (ptr->hasFloatAt(i))
                binbuf_addv(bb, "sf", &s_float, ptr->floatAt(i));
            else if (ptr->hasSymbolAt(i)) {
                binbuf_addv(bb, "ss", &s_symbol, ptr->symbolAt(i));
            } else if (ptr->hasListAt(i)) {
                auto lv = ptr->listAt(i);
                binbuf_addv(bb, "s", &s_list);
                binbuf_add(bb, lv.size(), lv.toPdData());
            } else if (ptr->hasAnyAt(i)) {
                AtomList l = ptr->anyAt(i);
                binbuf_add(bb, l.size(), l.toPdData());
            }

            binbuf_addsemi(bb);
        }
    }

    int rc = binbuf_write(bb, path, "", 0);
    binbuf_free(bb);

    return rc == 0;
}

bool PresetStorage::read(t_canvas* c, const std::string& path)
{
    t_canvas* root_cnv = rootCanvas(c);
    std::string name = makePresetFilename(root_cnv, path);
    std::string full_path = makeFullPresetPath(root_cnv, name);

    bool rc = read(full_path.c_str());
    if (!rc)
        LIB_ERR << "can't read presets from " << full_path;
    else
        LIB_DBG << "presets readed from " << full_path;

    return rc;
}

bool PresetStorage::read(const char* path)
{
    using SmallAtomList = SmallAtomListN<16>;
    using LineList = boost::container::small_vector<SmallAtomList, 64>;
    // RAII
    using BinbufPtr = std::unique_ptr<t_binbuf, void (*)(t_binbuf*)>;
    BinbufPtr content(binbuf_new(), binbuf_free);
    BinbufPtr bb_sym(binbuf_new(), binbuf_free);

    int err = binbuf_read(content.get(), (char*)path, (char*)"", 0);

    if (err)
        return false;

    LineList lines;
    lines.push_back({});

    const int N = binbuf_getnatom(content.get());
    lines.reserve(N);

    t_atom* lst = binbuf_getvec(content.get());

    for (int i = 0; i < N; i++) {
        lines.back().push_back(lst[i]);

        if (lst[i].a_type == A_SEMI) {
            lines.push_back({});
            continue;
        }
    }

    content.reset();

    // remove last empty list
    if (!lines.empty() && lines.back().empty())
        lines.pop_back();

    for (size_t i = 0; i < lines.size(); i++) {
        auto& line = lines[i];

        if (line.size() < 4)
            continue;

        if (line[0].isSymbol() && line[1].isFloat() && line[2].isSymbol()) {
            auto name = line[0].asT<t_symbol*>();
            auto index = line[1].asT<size_t>();
            auto sel = line[2].asT<t_symbol*>();

            if (!hasPreset(name))
                continue;

            if (sel == &s_float) {
                setFloatValueAt(name, index, line[3].asFloat());
            } else if (sel == &s_symbol) {
                setSymbolValueAt(name, index, line[3].asSymbol());
            } else {
                auto lv = line.view().subView(3);
                if (!lv.empty() && lv.back().isSemicolon())
                    lv = lv.subView(0, lv.size() - 1);

                if (sel == &s_list) {
                    setListValueAt(name, index, lv);
                } else {
                    setAnyValueAt(name, index, sel, lv);
                }
            }

        } else {
            LIB_ERR << "invalid preset line: " << line.view();
        }
    }

    return true;
}

AtomList PresetStorage::keys() const
{
    AtomList res;
    res.reserve(params_.size());

    for (auto& p : params_)
        res.append(p.first);

    return res;
}

bool PresetStorage::hasIndex(size_t sz) const
{
    if (sz >= MAX_PRESET_COUNT)
        return false;

    return !indexes_[sz].empty();
}

bool PresetStorage::hasPreset(t_symbol* name)
{
    return params_.find(name) != params_.end();
}

void PresetStorage::bindPreset(t_symbol* name)
{
    auto it = params_.find(name);

    // create new preset
    if (it == params_.end()) {
        PresetPtr ptr = std::make_shared<Preset>(name);
        auto res = params_.insert(PresetMap::value_type(name, ptr));
        if (!res.second) {
            LIB_ERR << "can't create preset: " << name;
            return;
        }

        it = res.first;
    }

    it->second->refCountUp();
}

void PresetStorage::unbindPreset(t_symbol* name)
{
    auto it = params_.find(name);

    if (it == params_.end()) {
        LIB_ERR << "preset is not found: " << name;
        return;
    }

    it->second->refCountDown();
    int n = it->second->refCount();

    if (n == 0) {
        params_.erase(it);
        for (size_t i = 0; i < indexes_.size(); i++) {
            auto& idx_set = indexes_[i];
            idx_set.erase(name);
            if (idx_set.empty())
                pd::send_message(SYM_PRESET_UPDATE_INDEX_ADDR, SYM_PRESET_INDEX_REMOVE, Atom(i));
        }

    } else if (n < 0) {
        LIB_ERR << "preset ref count error: " << name;
    }
}

void PresetStorage::clearAll()
{
    params_.clear();

    for (auto& x : indexes_)
        x.clear();
}

void PresetStorage::clearAll(size_t idx)
{
    if (idx >= maxPresetCount()) {
        LIB_ERR << "[preset] "
                << "invalid preset index: " << idx
                << ". Should be less then: " << maxPresetCount();
        return;
    }

    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("clear"), Atom(idx));
}

void PresetStorage::loadAll(size_t idx)
{
    if (idx >= maxPresetCount()) {
        LIB_ERR << "[preset] "
                << "invalid preset index: " << idx
                << ". Should be less then: " << maxPresetCount();
        return;
    }

    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("load"), Atom(idx));
}

void PresetStorage::storeAll(size_t idx)
{
    if (idx >= maxPresetCount()) {
        LIB_ERR << "[preset] "
                << "invalid preset index: " << idx
                << ". Should be less then: " << maxPresetCount();
        return;
    }

    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("store"), Atom(idx));
}

void PresetStorage::interpAll(t_float idx)
{
    if (idx >= maxPresetCount()) {
        LIB_ERR << "[preset] "
                << "invalid preset index: " << idx
                << ". Should be less then: " << maxPresetCount();
        return;
    }

    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("interp"), Atom(idx));
}

void PresetStorage::updateAll()
{
    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("update"), {});
}

void PresetStorage::duplicateAll()
{
    // all indexed keys
    PresetNameSet u;
    for (auto& p : indexes_)
        u.insert(p.begin(), p.end());

    for (auto& p : u) {
        for (size_t i = 0; i < indexes_.size(); i++) {
            auto& idx_set = indexes_[i];
            if (idx_set.empty())
                continue;

            if (idx_set.find(p) == idx_set.end()) {
                auto it = params_.find(p);
                if (it != params_.end()) {
                    (*it).second->duplicate();
                    idx_set.insert(p);
                }
            }
        }
    }
}

PresetPtr PresetStorage::getOrCreate(t_symbol* name)
{
    auto it = params_.find(name);
    if (it != params_.end())
        return it->second;

    auto new_param = std::make_shared<Preset>(name);
    params_.insert(PresetMap::value_type(name, new_param));
    return new_param;
}

void PresetStorage::addPresetIndex(t_symbol* name, size_t idx)
{
    if (idx >= MAX_PRESET_COUNT)
        return;

    bool new_preset_index = indexes_[idx].empty();
    indexes_[idx].insert(name);

    if (new_preset_index)
        pd::send_message(SYM_PRESET_UPDATE_INDEX_ADDR, SYM_PRESET_INDEX_ADD, Atom(idx));
}

void PresetStorage::removePresetIndex(t_symbol* name, size_t idx)
{
    if (idx >= MAX_PRESET_COUNT)
        return;

    indexes_[idx].erase(name);
    if (indexes_[idx].empty())
        pd::send_message(SYM_PRESET_UPDATE_INDEX_ADDR, SYM_PRESET_INDEX_REMOVE, Atom(idx));
}

Preset::Preset(t_symbol* name)
    : name_(name)
    , ref_count_(0)
{
    data_.assign(MAX_PRESET_COUNT, Message());
}

t_symbol* Preset::name() const
{
    return name_;
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

bool Preset::copyData(size_t src_idx, size_t dst_idx)
{
    if (src_idx >= data_.size() || dst_idx >= data_.size())
        return false;

    if (src_idx != dst_idx)
        data_[src_idx] = data_[dst_idx];

    return true;
}

bool Preset::duplicate()
{
    auto b = data_.begin();
    auto e = data_.end();
    auto it = std::find_if(b, e, [](decltype(data_.front())& el) { return !el.isNone(); });

    if (it == e)
        return false;

    for (auto& el : data_) {
        if (el.isNone())
            el = *it;
    }

    return true;
}

t_float Preset::floatAt(t_float fidx, t_float def) const
{
    if (fidx < 0)
        return def;

    auto idx = static_cast<size_t>(fidx);
    if (idx >= data_.size() || !data_[idx].isFloat())
        return def;

    const bool is_int = (fidx == idx);
    if (is_int) {
        const auto v = data_[idx].atomValue().asFloat();
        return (std::isnan(v) || std::isinf(v)) ? def : v;
    } else {
        const auto i0 = idx;
        const auto i1 = std::min<size_t>(idx + 1, data_.size() - 1);
        const auto v0 = data_[i0].atomValue().asFloat();
        const auto v1 = data_[i1].atomValue().asFloat();

        const bool nan0 = std::isnan(v0) || std::isinf(v0);
        const bool nan1 = std::isnan(v1) || std::isinf(v1);

        if (nan0 && !nan1)
            return v1;
        else if (!nan0 && nan1)
            return v0;
        else if (nan0 && nan1)
            return def;
        else {
            // assert (fidx > idx)
            t_float frac = fidx - t_float(idx);
            return ((1 - frac) * v0) + frac * v1;
        }
    }
}

bool Preset::setFloatAt(size_t idx, t_float v)
{
    if (idx >= data_.size())
        return false;

    if (std::isnan(v) || std::isinf(v))
        v = 0;

    data_[idx].setFloat(v);
    return true;
}

bool Preset::setSymbolAt(size_t idx, t_symbol* v)
{
    if (idx >= data_.size())
        return false;

    data_[idx].setSymbol(v);
    return true;
}

bool Preset::setListAt(size_t idx, const AtomList& l)
{
    if (idx >= data_.size())
        return false;

    data_[idx].setList(l);
    return true;
}

bool Preset::setAnyAt(size_t idx, t_symbol* sel, const AtomList& args)
{
    if (idx >= data_.size())
        return false;

    data_[idx].setAny(sel, args);
    return true;
}

t_symbol* Preset::symbolAt(size_t idx, t_symbol* def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isSymbol() ? data_[idx].atomValue().asSymbol() : def;
}

AtomListView Preset::listAt(size_t idx, const AtomListView& def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isList() ? data_[idx].listValue().view() : def;
}

AtomList Preset::anyAt(size_t idx, const AtomList& def) const
{
    if (idx >= data_.size())
        return def;

    return data_[idx].isAny() ? data_[idx].anyValue() : def;
}

bool Preset::clearAt(size_t idx)
{
    if (idx >= data_.size())
        return false;

    data_[idx] = Message();
    return true;
}
}
