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
#include "seq_base.h"
#include "ceammc_crc32.h"
#include "lex/parser_units.h"

CEAMMC_DEFINE_SYM(inf);
CEAMMC_DEFINE_SYM_HASH(inc);
CEAMMC_DEFINE_SYM_HASH(dec);
CEAMMC_DEFINE_SYM_HASH(tri);

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

static constexpr const int DIR_UP = 1;
static constexpr const int DIR_DOWN = 0;

RepeatProperty::RepeatProperty(const std::string& name, int defValue)
    : IntProperty(name, defValue)
{
    checkMinEq(-1);
}

AtomList RepeatProperty::get() const
{
    if (value() < 0)
        return Atom(sym_inf());
    else
        return IntProperty::get();
}

bool RepeatProperty::setList(const AtomListView& lv)
{
    static t_symbol* SYM_INF = gensym("inf");

    if (value() >= 0)
        return IntProperty::setList(lv);
    else if (lv.isFloat()) // <0, inf
        return setValue(lv[0].asT<t_float>());
    else if (lv == SYM_INF)
        return true;
    else {
        PROP_ERR() << "invalid value: " << lv;
        return false;
    }
}

SeqBase::SeqBase(const PdArgs& args, t_float defBeatDuration)
    : BaseObject(args)
    , clock_([this]() {
        const auto ms = calcNextTick();
        if (tick())
            clock_.delay(ms);
    })
    , inc_(DIR_UP)
    , r0_ouput_(false)
{
    beat_duration_ = new SeqTimeGrain("@t", defBeatDuration);
    addProperty(beat_duration_);

    repeat_ = new RepeatProperty("@r", 1);
    addProperty(repeat_);

    mode_ = new SymbolEnumProperty("@mode", { sym_inc(), sym_dec(), sym_tri() });
    addProperty(mode_);

    addProperty(new AliasProperty<RepeatProperty>("@inf", repeat_, -1));
    addProperty(new AliasProperty<RepeatProperty>("@once", repeat_, 1));

    createCbIntProperty("@i", [this]() -> int { return sequence_counter_; });
    createCbIntProperty("@ri", [this]() -> int { return num_repeats_; });

    upbeat_ = new FloatProperty("@upbeat", 0);
    upbeat_->checkNonNegative();
    addProperty(upbeat_);
}

void SeqBase::outputSequenceFirst() { }

void SeqBase::outputSequenceLast() { }

void SeqBase::clockStart()
{
    if (upbeat_->value() > 0)
        clock_.delay(upbeat_->value());
    else
        clock_.exec();
}

void SeqBase::clockStop()
{
    clock_.unset();
}

void SeqBase::initDone()
{
    const auto N = sequenceSize();
    if (N < 1)
        return;
    else if (mode_->isEqual(str_dec))
        sequence_counter_ = N - 1;
}

void SeqBase::resetSequenceCounter()
{
    const auto N = sequenceSize();
    if (mode_->isEqual(str_dec) && N > 1)
        sequence_counter_ = N - 1;
    else
        sequence_counter_ = 0;
}

size_t SeqBase::sequenceCounter() const
{
    return sequence_counter_;
}

bool SeqBase::tickForward(bool output)
{
    const auto N = sequenceSize();
    const auto LAST = N - 1;

    if (shouldRepeat()) {
        if (output && isSeqBegin()) {
            r0_ouput_ = true;
            outputRepeat(num_repeats_);
        }

        if (output)
            outputTick();

        const auto m = crc32_hash(mode_->value());
        if (m == hash_inc) {
            if (++sequence_counter_ >= N) {
                sequence_counter_ = 0;
                num_repeats_++;
            }
        } else if (m == hash_dec) {
            if (sequence_counter_ == 0) {
                sequence_counter_ = LAST;
                num_repeats_++;
            } else
                sequence_counter_--;
        } else if (m == hash_tri) {
            if (sequence_counter_ == 0) {
                sequence_counter_++;
                inc_ = DIR_UP;
            } else if (sequence_counter_ == LAST) {
                sequence_counter_--;
                num_repeats_++;
                inc_ = DIR_DOWN;
            } else if (inc_ == DIR_UP)
                sequence_counter_++;
            else
                sequence_counter_--;
        }

        return true;
    } else {
        outputRepeatDone();
        return false;
    }
}

bool SeqBase::isSeqBegin() const
{
    const auto N = sequenceSize();

    if (N == 0)
        return false;
    else if (N == 1)
        return true;
    else if (!r0_ouput_ && num_repeats_ == 0)
        return true;
    else if (mode_->isEqual(str_inc) && sequence_counter_ == 0)
        return true;
    else if (mode_->isEqual(str_dec) && sequence_counter_ == N - 1)
        return true;
    else if (mode_->isEqual(str_tri) && sequence_counter_ == 0)
        return true;
    else
        return false;
}

bool SeqBase::tick(bool output)
{
    if (repeat_->value() == 0)
        return false;

    if (sequenceSize() == 0)
        return false;

    return tickForward(output);
}

bool SeqBase::shouldRepeat() const
{
    const bool fin_continue = repeat_->isFinite() && (num_repeats_ < size_t(repeat_->value()));
    const bool inf_continue = !repeat_->isFinite();

    return fin_continue || inf_continue;
}

void SeqBase::resetCycleCounter()
{
    num_repeats_ = 0;
}

void SeqBase::setSequenceCounter(size_t i)
{
    sequence_counter_ = (i % sequenceSize());
}

void SeqBase::moveSequenceCounter(long n)
{
    const auto N = sequenceSize();
    if (N < 1)
        return;

    const auto LAST = N - 1;
    const long nn = static_cast<long>(sequence_counter_) + n;

    if (nn < 0)
        sequence_counter_ = LAST - (static_cast<size_t>(-nn - 1) % N);
    else
        sequence_counter_ = static_cast<size_t>(nn) % N;
}

void SeqBase::reset()
{
    r0_ouput_ = 0;
    resetSequenceCounter();
    resetCycleCounter();
    clockStop();
}

void SeqBase::start()
{
    clockStart();
}

void SeqBase::stop()
{
    clockStop();
}

SeqTimeGrain::SeqTimeGrain(const std::string& name, t_float f)
    : FloatProperty(name, f)
{
    setUnits(PropValueUnits::MSEC);
    checkClosedRange(MIN_INTERVAL, MAX_INTERVAL);
}

bool SeqTimeGrain::setList(const AtomListView& lv)
{
    if (lv.isFloat())
        return FloatProperty::setList(lv);
    else if (lv.isSymbol()) {
        try {
            using namespace parser;
            UnitsFullMatch p;

            if (p.parse(lv[0])) {

                switch (p.type()) {
                case TYPE_BPM:
                    return setValue(p.bpm().beatPeriodMs());
                case TYPE_MSEC:
                    return setValue(p.value());
                case TYPE_SEC:
                    return setValue(p.value() * 1000);
                case TYPE_HZ:
                    return setValue(1000 / p.value());
                default:
                    return false;
                }

            } else {
                PROP_ERR() << "parse error";
                return false;
            }
        } catch (const std::exception& e) {
            PROP_ERR() << "parser error: " << e.what();
            return false;
        }
    } else
        return FloatProperty::setList(lv);
}

bool SeqTimeGrain::setBpm(t_float bpm)
{
    if (bpm <= 0) {
        PROP_ERR() << "invalid bpm: " << bpm;
        return false;
    }

    const t_float interval_ms = 60000 / bpm;

    if (interval_ms < MIN_INTERVAL) {
        PROP_ERR() << "too high bpm value: " << bpm;
        return false;
    } else if (interval_ms > MAX_INTERVAL) {
        PROP_ERR() << "too slow bmp value: " << bpm;
        return false;
    } else
        return setValue(interval_ms);
}
