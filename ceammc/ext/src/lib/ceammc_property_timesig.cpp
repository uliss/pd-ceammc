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
#include "ceammc_property_timesig.h"
#include "fmt/core.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr int MAX_DIV = 1024;

namespace ceammc {

TimeSignatureProperty::TimeSignatureProperty(const std::string& name, const music::TimeSignature& sig, PropValueAccess access)
    : SymbolProperty(name, gensym(sig.toString().c_str()), access)
{
}

bool TimeSignatureProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    return setAtom(lv.front());
}

bool TimeSignatureProperty::setAtom(const Atom& a)
{
    if (a.isSymbol())
        return setSymbol(a.asT<t_symbol*>());
    else if (a.isFloat())
        return setFloat(a.asT<t_float>());
    else
        return false;
}

bool TimeSignatureProperty::setFloat(t_float f)
{
    //    int ts = f;
    //    if (dur > 0 && dur <= MAX_DIV) {
    //        dur_ = music::Duration { int(f) };
    //        dirty_ = true;
    //        return true;
    //    } else {
    //        PROP_ERR() << fmt::format("duration value in range [1..{}] is expected, got: {}", MAX_DIV, f);
    //        return false;
    //    }
    return false;
}

bool TimeSignatureProperty::setSymbol(t_symbol* s)
{
    return false;
    //    if (!ts_.parse(s->s_name)) {
    //        PROP_ERR() << "invalid duration value: " << s->s_name;
    //        return false;
    //    }

    //    dirty_ = false;
    //    SymbolProperty::setSymbol(s);
    //    return true;
}

bool TimeSignatureProperty::getSymbol(t_symbol*& s) const
{
    sync();
    return SymbolProperty::getSymbol(s);
}

AtomList TimeSignatureProperty::get() const
{
    sync();
    return { value() };
}

void TimeSignatureProperty::setSignature(const music::TimeSignature& ts) noexcept
{
    ts_ = ts;
    dirty_ = true;
}

void TimeSignatureProperty::sync() const
{
    if (dirty_) {
        auto cthis = const_cast<SymbolProperty*>(static_cast<const SymbolProperty*>(this));
        cthis->setValue(gensym(ts_.toString().c_str()));
        dirty_ = false;
    }
}
}
