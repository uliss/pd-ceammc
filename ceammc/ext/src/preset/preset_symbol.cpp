#include "preset_symbol.h"
#include "ceammc_factory.h"
#include "ceammc_preset.h"
#include "preset_iface.h"

PresetSymbol::PresetSymbol(const PdArgs& args)
    : PresetBase(args)
    , init_(nullptr)
    , current_value_(nullptr)
{
    // 1st positionalSymbolArgument is preset name - in base class
    // 2nd positionalSymbolArgument we are using as init value
    init_ = new SymbolProperty("@init", &s_);
    init_->setArgIndex(1);
    init_->setInitOnly();
    addProperty(init_);

    addTableColumn({ "#", true });
    addTableColumn(TableColumnParam { "symbol", false, 24 }.alignRight());
}

void PresetSymbol::onSymbol(t_symbol* s)
{
    current_value_ = s;
}

void PresetSymbol::initDone()
{
    PresetBase::initDone();
    current_value_ = init_->value();
}

void PresetSymbol::loadFrom(t_float idx)
{
    current_value_ = loadSymbol(idx, init_->value());
    symbolTo(0, current_value_);
}

void PresetSymbol::storeAt(size_t idx)
{
    storeSymbol(current_value_, idx);
}

AtomList PresetSymbol::editorPresetValue(size_t idx) const
{
    return Atom(PresetStorage::instance().symbolValueAt(presetPath(), idx, &s_));
}

bool PresetSymbol::setEditorPreset(size_t idx, const AtomListView& lv)
{
    if (!lv.isSymbol())
        return false;

    return PresetStorage::instance().setSymbolValueAt(presetPath(), idx, lv.asSymbol());
}

void setup_preset_symbol()
{
    PresetIFaceFactory<PresetSymbol> obj("preset.symbol");
    obj.addAlias("preset.s");

    PresetSymbol::factoryTableObjectInit(obj);
}
