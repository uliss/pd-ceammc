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
#include "lex/parser_music.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr int MAX_DIV = 1024;
constexpr int MAX_DOTS = 3;
constexpr const char* STR_DOTS = "...";

namespace {
const char* dots2str(int n)
{
    return STR_DOTS + std::min(MAX_DOTS, n);
}

t_symbol* dur2sym(int n, int div, int dots)
{
    return gensym(fmt::format("{}/{}", n, div, dots2str(dots)).c_str());
}
}

namespace ceammc {

DurationProperty::DurationProperty(
    const std::string& name,
    std::uint16_t num,
    std::uint16_t div,
    std::uint8_t dots,
    PropValueAccess access)

    : SymbolProperty(name, dur2sym(num, div, dots), access)
    , num_(num)
    , div_(div)
    , dots_(dots)
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
        num_ = 1;
        div_ = f;
        dots_ = 0;
        dirty_ = true;
        return true;
    } else {
        PROP_ERR() << fmt::format("duration value in range [1..{}] is expected, got: {}", MAX_DIV, f);
        return false;
    }
}

bool DurationProperty::setSymbol(t_symbol* s)
{
    using namespace ceammc::parser;

    DurationFullMatch p;
    if (!p.parse(s->s_name)) {
        PROP_ERR() << "invalid duration value: " << s->s_name;
        return false;
    }

    auto& res = p.result();
    if (!res.isAbs()) {
        PROP_ERR() << "only absolute duration added";
        return false;
    }

    num_ = res.num;
    if (res.repeats > 0)
        num_ *= res.repeats;

    div_ = res.den;
    dots_ = res.dots;
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

bool DurationProperty::setBeatNum(int beatNum)
{
    if (beatNum <= 0) {
        PROP_ERR() << "positive num value expected, got: " << beatNum;
        return false;
    }

    num_ = beatNum;
    dirty_ = true;
    return true;
}

bool DurationProperty::setBeatDivision(int beatDiv)
{
    if (beatDiv < 1 || beatDiv > MAX_DIV) {
        PROP_ERR() << fmt::format("duration value in range [1..{}] is expected, got: {}", MAX_DIV, beatDiv);
        return false;
    }

    dirty_ = true;
    div_ = beatDiv;
    return true;
}

t_float DurationProperty::durationMs(t_float bpm, int num, int div) const
{
    return ((((60000.0 / bpm) * div) / num) * num_) / div_;
}

t_float DurationProperty::divDurationMs(t_float bpm, int num, int div) const
{
    return (((60000.0 / bpm) * div) / num) / div_;
}

void DurationProperty::sync() const
{
    if (dirty_) {
        auto cthis = const_cast<SymbolProperty*>(static_cast<const SymbolProperty*>(this));
        cthis->setValue(dur2sym(num_, div_, dots_));
        dirty_ = false;
    }
}
}
