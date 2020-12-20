#include "preset_list.h"
#include "ceammc_factory.h"
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
}

void PresetList::initDone()
{
    PresetBase::initDone();
    current_value_ = init_->value();
}

void PresetList::onList(const AtomList& lst)
{
    current_value_ = lst;
}

void PresetList::loadFrom(size_t idx)
{
    current_value_ = loadList(idx, init_->value());
    listTo(0, current_value_);
}

void PresetList::storeAt(size_t idx)
{
    storeList(current_value_, idx);
}

void setup_preset_list()
{
    PresetIFaceFactory<PresetList> obj("preset.list");
    obj.addAlias("preset.l");
}
