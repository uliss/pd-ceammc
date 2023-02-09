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
#include "seq_counter.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_STR(done)
CEAMMC_DEFINE_HASH(wrap)
CEAMMC_DEFINE_HASH(fold)

SeqCounter::SeqCounter(const PdArgs& args)
    : BaseObject(args)
    , from_(nullptr)
    , to_(nullptr)
    , repeat_(nullptr)
    , mode_(nullptr)
    , ri_(0)
    , i_(0)
    , done_(false)
{
    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    from_->setSuccessFn([this](Property*) { i_ = 0; });
    addProperty(from_);

    to_ = new IntProperty("@to", 0);
    to_->setArgIndex(1);
    addProperty(to_);

    repeat_ = new RepeatProperty("@r", -1);
    repeat_->checkMinEq(-1);
    repeat_->setArgIndex(2);
    addProperty(repeat_);

    mode_ = new SymbolEnumProperty("@mode", { str_wrap, str_fold });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@wrap", mode_, gensym(str_wrap)));
    addProperty(new SymbolEnumAlias("@fold", mode_, gensym(str_fold)));

    addProperty(new AliasProperty<RepeatProperty>("@inf", repeat_, -1));
    addProperty(new AliasProperty<RepeatProperty>("@once", repeat_, 1));

    createCbIntProperty(
        "@i", [this]() -> int { return i_; },
        [this](int val) -> bool {
            if (crc32_hash(mode_->value()) == hash_fold)
                i_ = wrapInteger<int>(val, 2 * absRange());
            else
                i_ = wrapInteger<int>(val, absRange() + 1);

            return true;
        });
    createCbIntProperty("@ri", [this]() -> int { return ri_; });
    createCbIntProperty("@value", [this]() -> int { return currentValue(); });

    createInlet();
    createOutlet();
    createOutlet();
}

void SeqCounter::onBang()
{
    if (!shouldRepeat())
        return;

    if (range() == 0) {
        nextConst();
    } else {
        outputCycleCounter();
        outputCurrent();
        next();
    }
}

void SeqCounter::m_next(t_symbol*, const AtomListView& lv)
{
    if (!shouldRepeat())
        return;

    if (range() == 0) {
        nextConst();
    } else {
        next();

        if (shouldRepeat()) {
            outputCycleCounter();
            outputCurrent();
        }
    }
}

void SeqCounter::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty())
        reset();
}

void SeqCounter::m_reset(t_symbol*, const AtomListView& /*lv*/)
{
    reset();
}

void SeqCounter::m_prev(t_symbol*, const AtomListView& lv)
{
    if (!shouldRepeat())
        return;

    if (range() == 0) {
        nextConst();
    } else {
        prev();

        if (shouldRepeat()) {
            outputCycleCounter();
            outputCurrent();
        }
    }
}

void SeqCounter::nextWrapped()
{
    if (i_ < absRange())
        i_++;
    else if (nextCycle())
        i_ = 0;
}

void SeqCounter::nextFolded()
{
    if (i_ < (2 * absRange() - 1))
        i_++;
    else if (nextCycle())
        i_ = 0;
}

void SeqCounter::nextConst()
{
    floatTo(1, ri_);
    floatTo(0, from_->value());

    nextCycle();
}

bool SeqCounter::nextCycle()
{
    if (!repeat_->shouldRepeat(++ri_)) {
        done_ = true;
        anyTo(1, gensym(str_done), AtomListView {});
        return false;
    } else
        return true;
}

void SeqCounter::prev()
{
    switch (crc32_hash(mode_->value())) {
    case hash_wrap:
        return prevWrapped();
    case hash_fold:
        return prevFolded();
    default:
        OBJ_ERR << "unknown wrap mode: " << mode_->value();
        break;
    }
}

void SeqCounter::prevWrapped()
{
    if (i_ > 0)
        i_--;
    else if (nextCycle())
        i_ = absRange();
}

void SeqCounter::prevFolded()
{
    if (i_ > 0)
        i_--;
    else if (nextCycle())
        i_ = (2 * absRange()) - 1;
}

void SeqCounter::reset()
{
    done_ = false;
    ri_ = 0;
    i_ = 0;
}

void SeqCounter::outputCurrent()
{
    floatTo(0, currentValue());
}

void SeqCounter::outputCycleCounter()
{
    if (i_ == 0)
        floatTo(1, ri_);
}

void SeqCounter::next()
{
    switch (crc32_hash(mode_->value())) {
    case hash_wrap:
        return nextWrapped();
    case hash_fold:
        return nextFolded();
    default:
        OBJ_ERR << "unknown wrap mode: " << mode_->value();
        break;
    }
}

int SeqCounter::currentValue() const
{
    const auto R = range();
    const auto rsign = (0 < R) - (R < 0);

    const auto idx = (crc32_hash(mode_->value()) == hash_fold)
        ? std::min(i_, (2 * std::abs(R)) - i_)
        : i_;

    return from_->value() + (idx * rsign);
}

void setup_seq_counter()
{
    ObjectFactory<SeqCounter> obj("seq.counter");

    obj.addMethod("reset", &SeqCounter::m_reset);
    obj.addMethod("next", &SeqCounter::m_next);
    obj.addMethod("prev", &SeqCounter::m_prev);

    obj.setXletsInfo({ "bang: output current element then increment\n"
                       "bang: increment then output\n"
                       "prev: decrement then output\n"
                       "reset: reset counter to start value",
                         "bang: reset counter to start value" },
        { "int: counter value",
            "int: repeat index when iteration starts\n"
            "done: bang when done" });

    obj.setDescription("sequencer counter");
    obj.setCategory("seq");
    obj.setKeywords({"counter", "modulo"});
}
