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
#include "lex/parser_music.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

namespace ceammc {

BpmProperty::BpmProperty(const std::string& name, t_float init, std::uint16_t num, BeatDivision div, PropValueAccess access)
    : SymbolProperty(name, gensym(fmt::format("{}|{}bpm", init, div).c_str()), access)
    , bpm_(init)
    , beat_num_(num)
    , beat_div_(div)
{
    setUnits(PropValueUnits::BPM);
}

bool BpmProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    return setAtom(lv.front());
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
    auto ok = setBpm(f) && setBeatNum(1) && setBeatDivision(BEAT_4);
    if (ok)
        return SymbolProperty::setValue(gensym(fmt::format("{}|{}bpm", bpm_, BEAT_4).c_str()));
    else
        return false;
}

bool BpmProperty::setSymbol(t_symbol* s)
{
    using namespace ceammc::parser;
    BpmFullMatch p;
    Bpm bpm;
    if (!p.parse(s->s_name, bpm)) {
        PROP_ERR() << "invalid BPM value: " << s->s_name;
        return false;
    }

    bpm_ = bpm.bpm;
    beat_num_ = bpm.beat_num;
    beat_div_ = bpm.beat_div;
    SymbolProperty::setSymbol(s);
    return true;
}

t_float BpmProperty::ratio() const
{
    return (beat_div_ > 0) ? (t_float(beat_num_) / beat_div_) : 0;
}

bool BpmProperty::setBpm(t_float bpm)
{
    if (bpm >= 0) {
        bpm_ = bpm;
        return true;
    } else
        return false;
}

bool BpmProperty::setBeatNum(std::uint16_t beatNum)
{
    beat_num_ = beatNum;
    return true;
}

bool BpmProperty::setBeatDivision(t_float beatDiv)
{
    if (beatDiv <= 0) {
        PROP_ERR() << "positive duration value expected, got: " << beatDiv;
        return false;
    }

    beat_div_ = beatDiv;
    return true;
}

t_float BpmProperty::beatDurationMs() const
{
    return (60000.0 / bpm_) / beat_num_;
}

t_float BpmProperty::wholeNoteDurationMs() const
{
    return beat_div_ * beatDurationMs();
}

t_float BpmProperty::barDurationMs() const
{
    return beatDurationMs() * beat_num_;
}

}
