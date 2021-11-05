/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "seq_toggles.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "lex/parser_units.h"

#include <algorithm>
#include <cstdio>

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

static t_symbol* SYM_EVENT_DUR;
static t_symbol* SYM_EVENT_LEN;
static t_symbol* SYM_IDX;

constexpr int MIN_NOTE_LEN = 1;

LengthProperty::LengthProperty(const std::string& name, t_float def)
    : FloatProperty(name, def)
    , percent_value_(&s_)
{
    char buf[32];
    snprintf(buf, sizeof(buf) - 1, "%g%%", def);
    percent_value_ = gensym(buf);
}

t_float LengthProperty::calcValue(t_float v) const
{
    if (mode_ == PERCENT)
        return v * value() / 100;
    else if (mode_ == SUBTRACT)
        return clip_min<t_float, 0>(value() + v);
    else
        return clip_max<t_float>(value(), v);
}

bool LengthProperty::setList(const AtomListView& lv)
{
    if (lv.size() == 1) {
        using namespace parser;

        const Atom& a = lv[0];
        UnitsFullMatch p;
        if (!p.parse(a)) {
            PROP_ERR() << "float, int, % or ms value expected, got: " << a;
            return false;
        }

        const auto& v = p.result();
        switch (v.type) {
        case AtomType::TYPE_FLOAT:
        case AtomType::TYPE_INT:
            if (setValue(v.value)) {
                mode_ = (v.value >= 0) ? FIXED : SUBTRACT;
                return true;
            } else
                return false;
            break;
        case AtomType::TYPE_PERCENT:
            if (v.value < 0 || v.value > 100) {
                PROP_ERR() << "positive percent value in 0..100 range expected, got: " << a;
                return false;
            } else {
                const auto rc = setValue(v.value);
                if (rc) {
                    percent_value_ = a.asT<t_symbol*>();
                    mode_ = PERCENT;
                    return true;
                } else
                    return false;
            }
            break;
        case AtomType::TYPE_MSEC:
            if (setValue(v.value)) {
                mode_ = (v.value >= 0) ? FIXED : SUBTRACT;
                return true;
            } else
                return false;
            break;
        case AtomType::TYPE_SEC:
            if (setValue(v.value * 1000)) {
                mode_ = (v.value >= 0) ? FIXED : SUBTRACT;
                return true;
            } else
                return false;
            break;
        default:
            return FloatProperty::setList(lv);
        }
    } else if (FloatProperty::setList(lv)) {
        mode_ = FIXED;
        return true;
    } else
        return false;
}

AtomList LengthProperty::get() const
{
    if (mode_ == PERCENT) {
        return Atom(percent_value_);
    } else
        return FloatProperty::get();
}

SeqToggles::SeqToggles(const PdArgs& args)
    : SeqBangs(args)
    , length_(nullptr)
    , clock_off_([this]() { outputOff(); })
{
    length_ = new LengthProperty("@length", 75);
    addProperty(length_);
}

void SeqToggles::outputTick()
{
    const auto event_dur_ms = calcNextTick();
    // setting minimal note length = 1ms
    const t_float event_len_ms = clip_min<t_float, MIN_NOTE_LEN>(length_->calcValue(event_dur_ms));

    Atom l[2] = { sequenceCounter(), sequenceSize() };
    anyTo(1, SYM_IDX, AtomListView(l, 2));
    anyTo(1, SYM_EVENT_DUR, Atom(event_dur_ms));
    anyTo(1, SYM_EVENT_LEN, Atom(event_len_ms));
    floatTo(0, 1);

    // schedule off event
    clock_off_.delay(event_len_ms);
}

void SeqToggles::clockStop()
{
    SeqBangs::clockStop();

    if (clock_off_.isActive()) {
        clock_off_.unset();
        outputOff();
    }
}

void SeqToggles::outputOff()
{
    floatTo(0, 0);
}

void setup_seq_toggles()
{
    SYM_EVENT_DUR = gensym("ed");
    SYM_EVENT_LEN = gensym("el");
    SYM_IDX = gensym("i");

    SequencerIFaceFactory<ObjectFactory, SeqToggles> obj("seq.toggles");
    obj.addAlias("seq.t");

    obj.setXletsInfo({ "bang:  start playing sequence\n"
                       "stop:  stop sequencer\n",
                         "list: set new pattern" },
        { "1 or 0", "\\[i IDX N( - sequence iteration\n"
                    "\\[ri IDX N( - repeat iteration\n"
                    "\\[ed MS( - event duration\n"
                    "\\[el MS( - event length\n"
                    "\\[done( - when done" });
}
