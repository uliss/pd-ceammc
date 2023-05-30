/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ceammc_faust_ui.h"
#include "ceammc_crc32.h"
#include "ceammc_log.h"
#include "fmt/core.h"
#include "lex/parser_units.h"

using namespace ceammc::faust;
using namespace ceammc;

static t_symbol* makePropertyGet(const char* name)
{
    char buf[MAXPDSTRING];
    sprintf(buf, "@%s?", name);
    return gensym(buf);
}

static t_symbol* makePropertySet(const char* name)
{
    char buf[MAXPDSTRING];
    sprintf(buf, "@%s", name);
    return gensym(buf);
}

PropValueUnits ceammc::faust::to_units(const char* u)
{
    parser::UnitTypeFullMatch p;
    if (!p.parse(u))
        return PropValueUnits::NONE;

    switch (p.type()) {
    case parser::TYPE_HZ:
        return PropValueUnits::HZ;
    case parser::TYPE_MSEC:
        return PropValueUnits::MSEC;
    case parser::TYPE_SEC:
        return PropValueUnits::SEC;
    case parser::TYPE_SAMP:
        return PropValueUnits::SAMP;
    case parser::TYPE_PERCENT:
        return PropValueUnits::PERCENT;
    case parser::TYPE_DB:
        return PropValueUnits::DB;
    case parser::TYPE_RADIAN:
        return PropValueUnits::RAD;
    case parser::TYPE_DEGREE:
        return PropValueUnits::DEG;
    case parser::TYPE_CENT:
        return PropValueUnits::CENT;
    case parser::TYPE_SEMITONE:
        return PropValueUnits::SEMITONE;
    case parser::TYPE_BPM:
        return PropValueUnits::BPM;
    default:
        return PropValueUnits::NONE;
    }
}

PropValueType ceammc::faust::to_value_type(const char* v)
{
    switch (crc32_hash(v)) {
    case "float"_hash:
        return PropValueType::FLOAT;
    case "int"_hash:
        return PropValueType::INTEGER;
    case "bool"_hash:
        return PropValueType::BOOLEAN;
    default:
        LIB_ERR << fmt::format("unsupported value type: {}, valid values are: float,int,bool", v);
        return PropValueType::FLOAT;
    }
}

UIElement::UIElement(UIElementType t, t_symbol* label)
    : type_(t)
    , label_(label)
    , init_(0)
    , min_(0)
    , max_(1)
    , step_(0)
    , vptr_(0)
    , set_prop_symbol_(makePropertySet(label->s_name))
    , get_prop_symbol_(makePropertyGet(label->s_name))
    , pinfo_(set_prop_symbol_, PropValueType::FLOAT)
{
    initProperty();
}

UIElement::UIElement(UIElementType t, const char* label)
    : UIElement(t, gensym(label))
{
}

t_symbol* UIElement::typeSymbol() const
{
    switch (type_) {
    case UI_BUTTON:
        return gensym("button");
    case UI_CHECK_BUTTON:
        return gensym("checkbox");
    case UI_V_SLIDER:
        return gensym("vslider");
    case UI_H_SLIDER:
        return gensym("hslider");
    case UI_NUM_ENTRY:
        return gensym("nentry");
    case UI_V_BARGRAPH:
        return gensym("vbargraph");
    case UI_H_BARGRAPH:
        return gensym("hbargraph");
    default:
        return 0;
    }
}

FAUSTFLOAT UIElement::value(FAUSTFLOAT def) const
{
    if (!vptr_)
        return std::min(max_, std::max(min_, def));

    return std::min(max_, std::max(min_, *vptr_));
}

void UIElement::setValue(FAUSTFLOAT v, bool clip)
{
    if (!vptr_)
        return;

    if (v < min_) {
        if (clip)
            *vptr_ = min_;

        return;
    }

    if (v > max_) {
        if (clip)
            *vptr_ = max_;

        return;
    }

    *vptr_ = v;
}

void UIElement::initProperty()
{
    // set type and view
    switch (type_) {
    case UI_CHECK_BUTTON:
        pinfo_.setType(PropValueType::BOOLEAN);
        pinfo_.setView(PropValueView::TOGGLE);
        pinfo_.setDefault(init_ != 0);
        break;
    case UI_V_SLIDER:
    case UI_H_SLIDER:
        pinfo_.setType(PropValueType::FLOAT);
        pinfo_.setView(PropValueView::SLIDER);
        break;
    case UI_NUM_ENTRY:
        pinfo_.setType(PropValueType::FLOAT);
        pinfo_.setView(PropValueView::NUMBOX);
        break;
    default:
        pinfo_.setType(PropValueType::FLOAT);
        pinfo_.setView(PropValueView::SLIDER);
        break;
    }

    if (type_ != UI_CHECK_BUTTON) {
        if (!pinfo_.setConstraints(PropValueConstraints::CLOSED_RANGE))
            LIB_ERR << set_prop_symbol_ << " can't set constraints";

        if (!pinfo_.setRangeFloat(min_, max_))
            LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;

        pinfo_.setDefault(init_);
        pinfo_.setStep(step_);
    }
}

void UIElement::setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    assert(min <= init && init <= max);

    init_ = init;
    min_ = min;
    max_ = max;
    step_ = step;

    if (type() != UI_CHECK_BUTTON) {
        if (pinfo_.type() != PropValueType::BOOLEAN && !pinfo_.setConstraints(PropValueConstraints::CLOSED_RANGE))
            LIB_ERR << set_prop_symbol_ << " can't set constraints";

        if (pinfo_.type() == PropValueType::FLOAT && !pinfo_.setRangeFloat(min_, max_))
            LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;
        else if (pinfo_.type() == PropValueType::INTEGER && !pinfo_.setRangeInt(min_, max_))
            LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;

        if (pinfo_.type() == PropValueType::FLOAT)
            pinfo_.setDefault(init_);
        else if (pinfo_.type() == PropValueType::INTEGER)
            pinfo_.setDefault(static_cast<int>(init_));
        else if (pinfo_.type() == PropValueType::BOOLEAN)
            pinfo_.setDefault(static_cast<bool>(init_));

        pinfo_.setStep(step_);
    }
}

size_t PdUI::uiCount() const { return ui_elements_.size(); }

void PdUI::addSoundfile(const char*, const char*, Soundfile**) { }

PdUI::PdUI(const char* name)
    : name_(gensym(name))
{
    osc_segs_.push_back(name_);
}

UIElement* PdUI::uiAt(size_t pos)
{
    return pos < ui_elements_.size() ? ui_elements_[pos].get() : 0;
}

const UIElement* PdUI::uiAt(size_t pos) const
{
    return pos < ui_elements_.size() ? ui_elements_[pos].get() : 0;
}

void PdUI::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone)
{
    UIElementPtr elem(new UIElement(type, label));
    last_declare_.update(zone, elem);

    elem->setValuePtr(zone);
    ui_elements_.push_back(std::move(elem));
}

void PdUI::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
    FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    UIElementPtr elem(new UIElement(type, label));
    last_declare_.update(zone, elem);

    elem->setContraints(init, min, max, step);
    elem->setValuePtr(zone);
    ui_elements_.push_back(std::move(elem));
}

void PdUI::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
    FAUSTFLOAT min, FAUSTFLOAT max)
{
    UIElementPtr elem(new UIElement(type, label));
    last_declare_.update(zone, elem);

    elem->setContraints(0.0, min, max, 0.0);
    elem->setValuePtr(zone);
    ui_elements_.push_back(std::move(elem));
}

void PdUI::addButton(const char* label, FAUSTFLOAT* zone)
{
    UIElementPtr elem(new UIElement(UI_BUTTON, gensym(label)));
    last_declare_.update(zone, elem);

    elem->setContraints(0, 0, 1, 1);
    elem->setValuePtr(zone);
    ui_elements_.push_back(std::move(elem));
}

void PdUI::addCheckButton(const char* label, FAUSTFLOAT* zone)
{
    UIElementPtr elem(new UIElement(UI_CHECK_BUTTON, gensym(label)));
    last_declare_.update(zone, elem);

    elem->setContraints(0, 0, 1, 1);
    elem->setValuePtr(zone);
    ui_elements_.push_back(std::move(elem));
}

void PdUI::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    add_elem(UI_V_SLIDER, gensym(label), zone, init, min, max, step);
}

void PdUI::addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    add_elem(UI_H_SLIDER, gensym(label), zone, init, min, max, step);
}

void PdUI::addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    add_elem(UI_NUM_ENTRY, gensym(label), zone, init, min, max, step);
}

void PdUI::addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{
    add_elem(UI_H_BARGRAPH, gensym(label), zone, min, max);
}

void PdUI::addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{
    add_elem(UI_V_BARGRAPH, gensym(label), zone, min, max);
}

void PdUI::openTabBox(const char* label)
{
    osc_segs_.push_back(gensym(label));
}

void PdUI::openHorizontalBox(const char* label)
{
    osc_segs_.push_back(gensym(label));
}

void PdUI::openVerticalBox(const char* label)
{
    osc_segs_.push_back(gensym(label));
}

void PdUI::closeBox()
{
    osc_segs_.pop_back();
}

void PdUI::declare(FAUSTFLOAT* v, const char* name, const char* value)
{
    last_declare_.reset();
    last_declare_.setValue(v);

    switch (crc32_hash(name)) {
    case "unit"_hash:
        last_declare_.setUnits(to_units(value));
        break;
    case "type"_hash:
        last_declare_.setType(to_value_type(value));
        break;
    case "style"_hash:
        last_declare_.setStyle(value);
        break;
    default:
        break;
    }
}

std::vector<FAUSTFLOAT> PdUI::uiValues() const
{
    std::vector<FAUSTFLOAT> res;
    for (size_t i = 0; i < uiCount(); i++)
        res.push_back(uiAt(i)->value());

    return res;
}

void PdUI::setUIValues(const std::vector<FAUSTFLOAT>& v)
{
    size_t max = std::min(v.size(), uiCount());
    for (size_t i = 0; i < max; i++)
        uiAt(i)->setValue(v[i]);
}

PropValueType to_value_type(const char* str)
{
    switch (crc32_hash(str)) {
    case "int"_hash:
        return PropValueType::INTEGER;
    case "bool"_hash:
        return PropValueType::BOOLEAN;
    case "float"_hash:
        return PropValueType::FLOAT;
    default:
        LIB_ERR << fmt::format("[dev][faust] unsupported value type: '{}'", str);
        return PropValueType::FLOAT;
    }
}

void DeclareValue::reset()
{
    value_ = nullptr;
    update_flags_ = UPDATE_NONE;
}

void DeclareValue::update(const FAUSTFLOAT* value, UIElementPtr& e)
{
    if (!e || value != value_)
        return;

    if (update_flags_ & UPDATE_TYPE)
        e->setType(type_);

    if (update_flags_ & UPDATE_UNITS)
        e->setUnits(units_);
}

void DeclareValue::setValue(const FAUSTFLOAT* v)
{
    value_ = v;
}

void DeclareValue::setType(PropValueType t)
{
    type_ = t;
    update_flags_ |= UPDATE_TYPE;
}

void DeclareValue::setUnits(PropValueUnits u)
{
    units_ = u;
    update_flags_ |= UPDATE_UNITS;
}

void DeclareValue::setStyle(const char* str)
{
    LIB_DBG << fmt::format("style is not supported yet: {}", str);
}

UIEnumEntry::UIEnumEntry(const char* name)
    : name_(name)
    , hash_(crc32_hash(name))
{
}

bool UIEnumEntry::operator==(const char* name) const
{
    // strcmp in our case is faster then crc32_hash
    return std::strcmp(name_.c_str(), name) == 0;
}
