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
#include "ceammc_property_duration.h"
#include "fmt/core.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr int MAX_DIV = 1024;

namespace ceammc {

DurationProperty::DurationProperty(const std::string& name, const music::Duration& dur, PropValueAccess access)
    : SymbolProperty(name, gensym(dur.toString().c_str()), access)
{
}

bool DurationProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    return setAtom(lv.front());
}

bool DurationProperty::setAtom(const Atom& a)
{
    if (a.isSymbol())
        return setSymbol(a.asT<t_symbol*>());
    else if (a.isFloat())
        return setFloat(a.asT<t_float>());
    else
        return false;
}

bool DurationProperty::setFloat(t_float f)
{
    int dur = f;
    if (dur > 0 && dur <= MAX_DIV) {
        dur_ = music::Duration { std::int16_t(f) };
        dirty_ = true;
        return true;
    } else {
        PROP_ERR() << fmt::format("duration value in range [1..{}] is expected, got: {}", MAX_DIV, f);
        return false;
    }
}

bool DurationProperty::setSymbol(t_symbol* s)
{
    if (!dur_.parse(s->s_name)) {
        PROP_ERR() << "invalid duration value: " << s->s_name;
        return false;
    }

    dirty_ = false;
    SymbolProperty::setSymbol(s);
    return true;
}

bool DurationProperty::getSymbol(t_symbol*& s) const
{
    sync();
    return SymbolProperty::getSymbol(s);
}

AtomList DurationProperty::get() const
{
    sync();
    return { value() };
}

bool DurationProperty::setDots(int dots)
{
    if (dur_.setDots(dots)) {
        dirty_ = true;
        return true;
    } else
        return true;
}

bool DurationProperty::setBeatNum(int beatNum)
{
    if (beatNum <= 0) {
        PROP_ERR() << "positive num value expected, got: " << beatNum;
        return false;
    }

    dur_.setNumerator(beatNum);
    dirty_ = true;
    return true;
}

bool DurationProperty::setBeatDivision(int beatDiv)
{
    if (beatDiv < 1 || beatDiv > MAX_DIV) {
        PROP_ERR() << fmt::format("duration value in range [1..{}] is expected, got: {}", MAX_DIV, beatDiv);
        return false;
    }

    dur_.setDivision(beatDiv);
    dirty_ = true;
    return true;
}

void DurationProperty::setDuration(const music::Duration& dur)
{
    dur_ = dur;
    dirty_ = true;
}

void DurationProperty::sync() const
{
    if (dirty_) {
        auto cthis = const_cast<SymbolProperty*>(static_cast<const SymbolProperty*>(this));
        cthis->setValue(gensym(dur_.toString().c_str()));
        dirty_ = false;
    }
}
}
