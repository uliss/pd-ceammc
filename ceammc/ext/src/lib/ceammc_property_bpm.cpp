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
#include "ceammc_property_bpm.h"
#include "ceammc_log.h"
#include "fmt/core.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()
constexpr const char* USAGE_STR = "example values: 120|8bpm, 96|5/8bpm, 60 4, 144";

namespace ceammc {

BpmProperty::BpmProperty(const std::string& name, const music::Tempo& tempo, PropValueAccess access)
    : SymbolProperty(name, gensym(tempo.toString().c_str()), access)
    , tempo_(tempo)
    , dirty_(false)
{
    setUnits(PropValueUnits::BPM);
}

bool BpmProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.isAtom())
        return setAtom(lv.front());
    else if (lv.size() == 2 && lv[0].isFloat() && lv[1].isInteger()) {
        dirty_ = tempo_.set(lv.floatAt(0, 60), lv.intAt(1, 4));
        return dirty_;
    } else {
        PROP_ERR() << "invalid BPM value: " << lv << "\n\t" << USAGE_STR;
        return false;
    }
}

bool BpmProperty::setAtom(const Atom& a)
{
    if (a.isSymbol())
        return setSymbol(a.asT<t_symbol*>());
    else if (a.isFloat())
        return setFloat(a.asT<t_float>());
    else
        return false;
}

bool BpmProperty::setFloat(t_float f)
{
    if (tempo_.set(f, 4, 0)) {
        dirty_ = true;
        return true;
    } else
        return false;
}

bool BpmProperty::setSymbol(t_symbol* s)
{
    if (!tempo_.parse(s->s_name)) {
        PROP_ERR() << fmt::format("invalid BPM value: {}\n\t{}", s->s_name, USAGE_STR);
        return false;
    }

    SymbolProperty::setSymbol(s);
    dirty_ = false;
    return true;
}

bool BpmProperty::getSymbol(t_symbol*& s) const
{
    sync();
    return SymbolProperty::getSymbol(s);
}

AtomList BpmProperty::get() const
{
    sync();
    return { value() };
}

bool BpmProperty::setBpm(t_float bpm)
{
    if (tempo_.setBpm(bpm)) {
        dirty_ = true;
        return true;
    } else {
        PROP_ERR() << fmt::format("invalid BPM value: {}\n\t{}", bpm, USAGE_STR);
        return false;
    }
}

bool BpmProperty::setBeatDivision(int beatDiv)
{
    if (tempo_.setDivision(beatDiv)) {
        dirty_ = true;
        return true;
    } else {
        PROP_ERR() << fmt::format("invalid beat division value: {}\n\t", beatDiv, USAGE_STR);
        return false;
    }
}

void BpmProperty::sync() const
{
    if (dirty_) {
        auto cthis = const_cast<SymbolProperty*>(static_cast<const SymbolProperty*>(this));
        cthis->setValue(gensym(tempo_.toString().c_str()));
        dirty_ = false;
    }
}

}
