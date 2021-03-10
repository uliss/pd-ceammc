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
    , patch_dir_(".")
{
    createCbListProperty("@keys", []() -> AtomList { return PresetStorage::instance().keys(); });

    createOutlet();

    if (rootCanvas())
        patch_dir_ = canvas_getdir(rootCanvas())->s_name;
}

void PresetExternal::m_load(t_symbol*, const AtomListView& l)
{
    size_t idx = l.toT<size_t>(0);
    PresetStorage::instance().loadAll(idx);
}

void PresetExternal::m_interp(t_symbol*, const AtomListView& l)
{
    t_float idx = l.toT<t_float>(0);
    PresetStorage::instance().interpAll(idx);
}

void PresetExternal::m_store(t_symbol*, const AtomListView& l)
{
    size_t idx = l.toT<size_t>(0);
    PresetStorage::instance().storeAll(idx);
}

void PresetExternal::m_clear(t_symbol*, const AtomListView& l)
{
    t_symbol* SYM_PRESET_ALL = gensym(Preset::SYM_PRESET_ALL);

    size_t idx = l.toT<size_t>(0);

    if (!SYM_PRESET_ALL->s_thing)
        return;

    t_atom a;
    SETFLOAT(&a, idx);
    pd_typedmess(SYM_PRESET_ALL->s_thing, gensym("clear"), 1, &a);
}

void PresetExternal::m_write(t_symbol*, const AtomListView& l)
{
    PresetStorage::instance().write(canvas(), to_string(l));
}

void PresetExternal::m_read(t_symbol*, const AtomListView& l)
{
    PresetStorage::instance().read(canvas(), to_string(l));
}

void PresetExternal::m_update(t_symbol*, const AtomListView&)
{
    PresetStorage::instance().updateAll();
}

void PresetExternal::m_duplicate(t_symbol*, const AtomListView& l)
{
    if (l.empty())
        PresetStorage::instance().duplicateAll();
}

std::string PresetExternal::makeDefaultPresetPath() const
{
    std::string res;

    if (rootCanvas()) {
        res += platform::strip_extension(rootCanvas()->gl_name->s_name);
        res += "-preset.txt";
    }

    return res;
}

void setup_preset_storage()
{
    ObjectFactory<PresetExternal> obj("preset.storage");
    obj.addMethod("load", &PresetExternal::m_load);
    obj.addMethod("interp", &PresetExternal::m_interp);
    obj.addMethod("store", &PresetExternal::m_store);
    obj.addMethod("read", &PresetExternal::m_read);
    obj.addMethod("write", &PresetExternal::m_write);
    obj.addMethod("clear", &PresetExternal::m_clear);
    obj.addMethod("update", &PresetExternal::m_update);
    obj.addMethod("duplicate", &PresetExternal::m_duplicate);
}
