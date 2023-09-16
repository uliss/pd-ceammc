#include "preset_storage.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "ceammc_preset.h"

PresetExternal::PresetExternal(const PdArgs& args)
    : BaseObject(args)
    , patch_dir_(".")
{
    createCbListProperty("@keys", []() -> AtomList { return PresetStorage::instance().keys(); });

    createOutlet();

    patch_dir_ = canvasDir(CanvasType::TOPLEVEL)->s_name;
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
    size_t idx = l.toT<size_t>(0);
    pd::send_message(gensym(Preset::SYM_PRESET_ALL), gensym("clear"), Atom(idx));
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
    auto cnv = canvas(CanvasType::TOPLEVEL);

    if (cnv) {
        res += platform::strip_extension(canvas_info_name(cnv)->s_name);
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
