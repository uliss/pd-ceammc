#include "preset_base.h"
#include "ceammc_platform.h"
#include "ceammc_preset.h"
#include "fmt/core.h"

extern "C" {
#include "g_canvas.h"
}

static const std::string SEPARATOR("/");

PresetBase::PresetBase(const PdArgs& args)
    : TableObject<BaseObject>(args)
    , global_(0)
    , subpatch_(nullptr)
    , name_(&s_)
    , path_(&s_)
    , preset_path_(&s_)
{
    createOutlet();

    Property* name = createCbSymbolProperty(
        "@id",
        [this]() -> t_symbol* { return name_; },
        [this](t_symbol* s) -> bool { name_ = s; return true; });
    name->setInitOnly();
    name->setArgIndex(0);

    global_ = new FlagProperty("@global");
    addProperty(global_);

    subpatch_ = new FlagProperty("@subpatch");
    addProperty(subpatch_);

    // virtual @path property
    addProperty(new PointerProperty<t_symbol*>("@path", &preset_path_));
}

PresetBase::~PresetBase()
{
    unbind();
}

void PresetBase::initDone()
{
    BaseObject::initDone();

    path_ = makePath();
    preset_path_ = makePresetPath();

    bind();
}

void PresetBase::m_update(t_symbol*, const AtomListView&)
{
    path_ = makePath();
    t_symbol* new_preset_path = makePresetPath();

    if (preset_path_ != new_preset_path) {
        unbind();
        preset_path_ = new_preset_path;
        bind();
    }
}

void PresetBase::m_clear(t_symbol*, const AtomListView& index)
{
    size_t idx = index.toT<size_t>(0);

    PresetStorage& storage = PresetStorage::instance();

    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    storage.clearValueAt(preset_path_, idx);
}

void PresetBase::bind()
{
    bindReceive(gensym(Preset::SYM_PRESET_ALL));

    if (PresetStorage::instance().hasPreset(preset_path_)) {
        OBJ_DBG << "warning! preset already exists: " << preset_path_->s_name;
    }

    PresetStorage::instance().bindPreset(preset_path_);
}

void PresetBase::unbind()
{
    unbindReceive();
    PresetStorage::instance().unbindPreset(preset_path_);
}

t_symbol* PresetBase::makePath() const
{
    std::string res;

    if (!global_->value()) {
        const t_canvas* c = canvas();
        while (c) {
            // not top window
            if (c->gl_owner) { //
                if (subpatch_->value()) {
                    res.insert(0, SEPARATOR + std::string(c->gl_name->s_name));
                } else if (canvas_isabstraction((t_canvas*)c)) {
                    res.insert(0, SEPARATOR + platform::strip_extension(c->gl_name->s_name));
                } else {
                }
            }

            c = c->gl_owner;
        }
    }

    return gensym(res.c_str());
}

void PresetBase::m_store(t_symbol*, const AtomListView& index)
{
    size_t idx = index.toT<size_t>(0);

    PresetStorage& storage = PresetStorage::instance();

    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    storeAt(idx);
}

void PresetBase::loadFrom(t_float idx)
{
}

void PresetBase::storeAt(size_t idx)
{
}

t_float PresetBase::loadFloat(t_float idx, t_float def)
{
    return PresetStorage::instance().floatValueAt(preset_path_, idx, def);
}

t_symbol* PresetBase::loadSymbol(size_t idx, t_symbol* def)
{
    return PresetStorage::instance().symbolValueAt(preset_path_, idx, def);
}

AtomListView PresetBase::loadList(size_t idx, const AtomListView& def)
{
    return PresetStorage::instance().listValueAt(preset_path_, idx, def);
}

AtomList PresetBase::loadAny(size_t idx, const AtomList& def)
{
    return PresetStorage::instance().anyValueAt(preset_path_, idx, def);
}

void PresetBase::storeFloat(t_float f, size_t idx)
{
    if (!PresetStorage::instance().setFloatValueAt(preset_path_, idx, f))
        OBJ_DBG << "can't save preset: " << preset_path_->s_name;
}

void PresetBase::storeSymbol(t_symbol* s, size_t idx)
{
    if (!PresetStorage::instance().setSymbolValueAt(preset_path_, idx, s))
        OBJ_DBG << "can't save preset: " << preset_path_->s_name;
}

void PresetBase::storeList(const AtomList& l, size_t idx)
{
    if (!PresetStorage::instance().setListValueAt(preset_path_, idx, l))
        OBJ_DBG << "can't save preset: " << preset_path_->s_name;
}

void PresetBase::storeAny(t_symbol* sel, const AtomList& l, size_t idx)
{
    if (!PresetStorage::instance().setAnyValueAt(preset_path_, idx, sel, l))
        OBJ_DBG << "can't save preset: " << preset_path_->s_name;
}

t_symbol* PresetBase::makePresetPath() const
{
    std::string res;
    res += path_->s_name;
    res += SEPARATOR;
    res += name_->s_name;
    return gensym(res.c_str());
}

t_symbol* PresetBase::name() const
{
    return name_;
}

void PresetBase::m_load(t_symbol*, const AtomListView& index)
{
    auto idx = index.intAt(0, 0);

    PresetStorage& storage = PresetStorage::instance();

    if (idx < 0 || idx >= (int)storage.maxPresetCount()) {
        OBJ_ERR << "invalid preset index: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    loadFrom(idx);
}

void PresetBase::m_interp(t_symbol*, const AtomListView& index)
{
    auto idx = index.floatAt(0, 0);

    PresetStorage& storage = PresetStorage::instance();

    if (idx < 0 || idx >= storage.maxPresetCount()) {
        OBJ_ERR << "invalid preset index: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    loadFrom(idx);
}

void PresetBase::tableEditorClear()
{
}

TableLineList PresetBase::getTableContentForEditor() const
{
    TableLineList res;
    auto N = PresetStorage::instance().maxPresetCount();

    for (size_t i = 0; (i + 1) < N; i += 1) {
        if (!PresetStorage::instance().hasValueAt(presetPath(), i))
            continue;

        AtomList line;
        line.append(i);
        line.append(editorPresetValue(i));
        res.push_back(line);
    }
    return res;
}

void PresetBase::tableEditorAddLine(t_symbol* sel, const AtomListView& lv)
{
    if (lv.size() < 1 || !lv[0].isInteger())
        return;

    int idx = lv.intAt(0, 0);
    if (idx < 0) {
        OBJ_ERR << fmt::format("invalid preset index: {}", idx);
        return;
    }

    auto args = lv.subView(1);

    if (!setEditorPreset(idx, args))
        OBJ_ERR << fmt::format("can't set preset [{}]: ", idx) << args;
}

EditorTitleString PresetBase::editorTitle() const
{
    return fmt::format("preset '{}'", name()->s_name).c_str();
}

bool PresetBase::setEditorPreset(size_t idx, const AtomListView& lv)
{
    return false;
}

AtomList PresetBase::editorPresetValue(size_t idx) const
{
    return {};
}
