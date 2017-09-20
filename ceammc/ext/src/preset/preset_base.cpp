#include "preset_base.h"
#include "ceammc_platform.h"
#include "preset_storage.h"

extern "C" {
#include "g_canvas.h"
}

static const std::string SEPARATOR("/");

PresetBase::PresetBase(const PdArgs& args)
    : BaseObject(args)
    , global_(0)
    , subpatch_(0)
    , name_(0)
    , path_(&s_)
    , preset_path_(&s_)
{
    createOutlet();

    global_ = new FlagProperty("@global");
    createProperty(global_);

    subpatch_ = new FlagProperty("@subpatch");
    createProperty(subpatch_);

    // to get @global and @subpatch flags before makeName() call
    parseProperties();

    name_ = new SymbolProperty("@name", positionalSymbolArgument(0, &s_), true);
    createProperty(name_);

    path_ = makePath();
    preset_path_ = makePresetPath();
    bind();
}

PresetBase::~PresetBase()
{
    unbind();
}

void PresetBase::m_update(t_symbol*, const AtomList&)
{
    path_ = makePath();
    t_symbol* new_preset_path = makePresetPath();

    if (preset_path_ != new_preset_path) {
        unbind();
        preset_path_ = new_preset_path;
        bind();
    }
}

void PresetBase::bind()
{
    bindReceive(Preset::makeBindAddress(preset_path_));

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

t_symbol* PresetBase::makePresetPath() const
{
    std::string res;
    res += path_->s_name;
    res += SEPARATOR;
    res += name_->value()->s_name;
    return gensym(res.c_str());
}
