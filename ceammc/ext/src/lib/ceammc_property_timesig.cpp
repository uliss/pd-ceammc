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
#include "ceammc_numeric.h"
#include "fmt/core.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr const char* USAGE_STR = "example values are: |6/8|, |3/8+1/16|, 5 4, 6 etc.";

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

    if (lv.isAtom())
        return setAtom(lv.front());
    else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isInteger()) {
        dirty_ = ts_.set(lv.intAt(0, 4), lv.intAt(1, 4));
        return dirty_;
    } else {
        PROP_ERR() << "invalid time signature: " << lv << ", " << USAGE_STR;
        return false;
    }
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
    if (!math::is_natural(f)) {
        PROP_ERR() << fmt::format("natural number expected, got: {}\n\t{}", f, USAGE_STR);
        return false;
    }

    dirty_ = ts_.set(f, 4);
    return dirty_;
}

bool TimeSignatureProperty::setSymbol(t_symbol* s)
{
    if (!ts_.parse(s->s_name)) {
        PROP_ERR() << fmt::format("invalid time signature value: {}\n\t{}", s->s_name, USAGE_STR);
        return false;
    }

    dirty_ = false;
    SymbolProperty::setSymbol(s);
    return true;
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
