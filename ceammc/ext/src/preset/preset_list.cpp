#include "preset_list.h"
#include "ceammc_factory.h"
#include "ceammc_preset.h"
#include "preset_iface.h"

PresetList::PresetList(const PdArgs& args)
    : PresetBase(args)
    , init_(nullptr)
{
    // 1st positionalSymbolArgument is preset name - in base class
    // positional arguments from 2 we are using as init arguments
    init_ = new ListProperty("@init", AtomList());
    init_->setArgIndex(1);
    addProperty(init_);

    addTableColumn({ "#", true });
    addTableColumn({ "list", false, 24 });
}

void PresetList::initDone()
{
    PresetBase::initDone();
    current_value_ = init_->value();
}

void PresetList::onList(const AtomListView& lv)
{
    current_value_ = lv;
}

void PresetList::loadFrom(t_float idx)
{
    current_value_ = loadList(idx, init_->value());
    listTo(0, current_value_);
}

void PresetList::storeAt(size_t idx)
{
    storeList(current_value_, idx);
}

AtomList PresetList::editorPresetValue(size_t idx) const
{
    return PresetStorage::instance().listValueAt(presetPath(), idx);
}

bool PresetList::setEditorPreset(size_t idx, const AtomListView& lv)
{
    return PresetStorage::instance().setListValueAt(presetPath(), idx, lv);
}

void setup_preset_list()
{
    PresetIFaceFactory<PresetList> obj("preset.list");
    obj.addAlias("preset.l");

//    PresetList::factoryTableObjectInit(obj);
}
