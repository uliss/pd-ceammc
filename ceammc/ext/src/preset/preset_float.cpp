#include "preset_float.h"
#include "ceammc_factory.h"
#include "preset_storage.h"

extern "C" {
#include "g_canvas.h"
}

static t_symbol* SYM_EMPTY = gensym("");
static t_symbol* SYM_FLOAT = gensym("float");

PresetFloat::PresetFloat(const PdArgs& args)
    : BaseObject(args)
    , init_(0)
{
    init_ = new FloatProperty("@init", 0, true);
    current_value_ = init_->value();
    createProperty(init_);

    global_ = new FlagProperty("@global");
    createProperty(global_);

    subpatch_ = new FlagProperty("@subpatch");
    createProperty(subpatch_);

    // to get @global and @subpatch flags before makeName() call
    parseProperties();

    name_ = new SymbolProperty("@name", positionalSymbolArgument(0, SYM_FLOAT), true);
    createProperty(name_);

    path_ = new SymbolProperty("@path", positionalSymbolArgument(0, SYM_EMPTY), true);
    createProperty(path_);

    path_->setValue(makePath());
    preset_path_ = makePresetPath();

    bindPreset();

    createOutlet();
}

PresetFloat::~PresetFloat()
{
    unbindPreset();
}

void PresetFloat::onFloat(float f)
{
    current_value_ = f;
}

void PresetFloat::m_clear(t_symbol*, const AtomList&)
{
    PresetStorage::instance().remove(name_->value());
}

void PresetFloat::m_store(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_NATURAL)) {
        OBJ_ERR << "single float value expected: " << l;
        return;
    }

    size_t idx = l[0].asSizeT(0);

    PresetStorage& storage = PresetStorage::instance();
    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    if (!storage.setFloatValueAt(name_->value(), idx, current_value_))
        OBJ_DBG << "can't save preset: " << name_->str();
}

void PresetFloat::m_load(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT)) {
        OBJ_ERR << "single float value expected: " << l;
        return;
    }

    size_t idx = l[0].asSizeT(0);

    PresetStorage& storage = PresetStorage::instance();
    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    current_value_ = storage.floatValueAt(name_->value(), idx, init_->value());
    floatTo(0, current_value_);
}

void PresetFloat::m_update(t_symbol*, const AtomList&)
{
    path_->setValue(makePath());
    t_symbol* new_preset_path = makePresetPath();

    if (preset_path_ != new_preset_path) {
        unbindPreset();
        preset_path_ = new_preset_path;
        bindPreset();
    }
}

t_symbol* PresetFloat::makePresetPath() const
{
    std::string res;
    res += path_->value()->s_name;
    res += name_->value()->s_name;
    return gensym(res.c_str());
}

t_symbol* PresetFloat::makePath() const
{
    std::string res;

    if (!global_->value()) {
        const t_canvas* c = canvas();
        while (c) {
            // not top window
            if (c->gl_owner) { //
                if (subpatch_->value()) {
                    //                    OBJ_DBG << "subpatch";
                    res = std::string("/") + std::string(c->gl_name->s_name) + res;
                } else if (canvas_isabstraction((t_canvas*)c)) {
                    //                    OBJ_DBG << "abstra";
                    res = std::string("/") + std::string(c->gl_name->s_name) + res;
                } else {
                    //                    OBJ_DBG << "ignore";
                }
            }

            c = c->gl_owner;
        }

        res += '/';
    }

    return gensym(res.c_str());
}

void PresetFloat::bindPreset()
{
    bindReceive(Preset::makeBindAddress(preset_path_));

    if (PresetStorage::instance().hasPreset(preset_path_)) {
        OBJ_DBG << "warning! preset already exists: " << preset_path_->s_name;
    }

    PresetStorage::instance().bindPreset(preset_path_);
}

void PresetFloat::unbindPreset()
{
    OBJ_DBG << "unbind: " << preset_path_->s_name;
    //    unbindReceive();
    PresetStorage::instance().unbindPreset(preset_path_);
}

void setup_preset_float()
{
    ObjectFactory<PresetFloat> obj("preset.float");
    //    obj.addMethod("clear", &PresetFloat::m_clear);
    obj.addMethod("store", &PresetFloat::m_store);
    obj.addMethod("load", &PresetFloat::m_load);
    obj.addMethod("update", &PresetFloat::m_update);
}
