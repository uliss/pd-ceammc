#include "preset_storage.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "ceammc_preset.h"

#include <cstring>
#include <fstream>

extern "C" {
#include "g_canvas.h"
}

PresetExternal::PresetExternal(const PdArgs& args)
    : BaseObject(args)
    , root_cnv_(rootCanvas())
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
    size_t idx = l.asSizeT(0);
    PresetStorage::instance().loadAll(idx);
}

void PresetExternal::m_store(t_symbol*, const AtomList& l)
{
    size_t idx = l.asSizeT(0);
    PresetStorage::instance().storeAll(idx);
}

void PresetExternal::m_clear(t_symbol*, const AtomList& l)
{
    size_t idx = l.asSizeT(0);

    if (!Preset::SYM_PRESET_ALL->s_thing)
        return;

    t_atom a;
    SETFLOAT(&a, idx);
    pd_typedmess(Preset::SYM_PRESET_ALL->s_thing, Preset::SYM_CLEAR, 1, &a);
}

void PresetExternal::m_write(t_symbol*, const AtomList& l)
{
    PresetStorage::instance().write(canvas(), to_string(l));
}

void PresetExternal::m_read(t_symbol*, const AtomList& l)
{
    PresetStorage::instance().read(canvas(), to_string(l));
}

void PresetExternal::m_update(t_symbol*, const AtomList&)
{
    PresetStorage::instance().updateAll();
}

std::string PresetExternal::makeDefaultPresetPath() const
{
    std::string res;

    if (root_cnv_) {
        res += platform::strip_extension(root_cnv_->gl_name->s_name);
        res += "-preset.txt";
    }

    return res;
}

void setup_preset_storage()
{
    ObjectFactory<PresetExternal> obj("preset.storage");
    obj.addMethod("load", &PresetExternal::m_load);
    obj.addMethod("store", &PresetExternal::m_store);
    obj.addMethod("read", &PresetExternal::m_read);
    obj.addMethod("write", &PresetExternal::m_write);
    obj.addMethod("clear", &PresetExternal::m_clear);
    obj.addMethod("update", &PresetExternal::m_update);
}
