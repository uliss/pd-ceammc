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
#include "ceammc_factory.h"

static t_symbol* SYM_DONE;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

constexpr int R_INFINITE = -1;

SeqCounter::SeqCounter(const PdArgs& args)
    : BaseObject(args)
    , from_(nullptr)
    , to_(nullptr)
    , repeat_(nullptr)
    , mode_(nullptr)
    , ri_(0)
    , i_(0)
    , done_(false)
    , dir_(DIR_FORWARD)
{
    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    from_->setSuccessFn([this](Property*) { i_ = from_->value(); });
    addProperty(from_);

    to_ = new IntProperty("@to", 0);
    to_->setArgIndex(1);
    addProperty(to_);

    repeat_ = new RepeatProperty("@r", -1);
    repeat_->checkMinEq(-1);
    repeat_->setArgIndex(2);
    addProperty(repeat_);

    mode_ = new SymbolEnumProperty("@mode", { SYM_WRAP, SYM_FOLD });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@wrap", mode_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@fold", mode_, SYM_FOLD));

    addProperty(new AliasProperty<RepeatProperty>("@inf", repeat_, -1));
    addProperty(new AliasProperty<RepeatProperty>("@once", repeat_, 1));

    createCbIntProperty("@i", [this]() -> int { return i_; });
    createCbIntProperty("@ri", [this]() -> int { return ri_; });

    createInlet();
    createOutlet();
    createOutlet();
}

void SeqCounter::onBang()
{
    const auto IS_CONST = (to_->value() - from_->value()) == 0;

    if (!IS_CONST && mode_->value() == SYM_WRAP)
        nextWrapped();
    else if (!IS_CONST && mode_->value() == SYM_FOLD)
        nextFolded();
    else
        nextConst();
}

void SeqCounter::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        reset();
}

void SeqCounter::m_reset(t_symbol*, const AtomListView& lv)
{
    reset();
}

void SeqCounter::nextWrapped()
{
    if (!shouldRepeat())
        return;

    if (i_ == from_->value())
        floatTo(1, ri_);

    floatTo(0, i_);

    const auto N = to_->value() - from_->value();

    if (i_ == to_->value()) {
        const auto nr = repeat_->value();

        if (nr == R_INFINITE || ri_ < nr) {
            ri_++;

            if (ri_ == nr) { // should stop
                done_ = true;
                anyTo(1, SYM_DONE, AtomListView {});
            } else { // continue: reset counter
                i_ = from_->value();
            }
        }
    } else {
        if (N > 0)
            i_++;
        else if (N < 0)
            i_--;
    }
}

void SeqCounter::nextFolded()
{
    if (!shouldRepeat())
        return;

    const auto RANGE = to_->value() - from_->value();
    const auto NR = repeat_->value();
    const auto BEGIN = from_->value();
    const auto END = to_->value();

    int LAST = 0;
    if (RANGE > 1)
        LAST = BEGIN + 1;
    else if (RANGE < -1)
        LAST = END + 1;
    else {
        dir_ = DIR_BACK;
        LAST = END;
    }

    // repeat counter
    if (i_ == BEGIN) {
        dir_ = DIR_FORWARD;
        floatTo(1, ri_);
    }

    // output sequence counter
    floatTo(0, i_);

    // update
    if (i_ == LAST && dir_ == DIR_BACK) { // last element in folded sequence
        dir_ = DIR_FORWARD;

        if (NR == R_INFINITE || ri_ < NR) {
            if (++ri_ == NR) { // should stop
                done_ = true;
                anyTo(1, SYM_DONE, AtomListView {});
            }
        }

        i_ = from_->value();
    } else if (i_ == to_->value()) { // going down
        dir_ = DIR_BACK;

        if (RANGE > 0)
            i_ = to_->value() - 1;
        else if (RANGE < 0)
            i_ = to_->value() + 1;
    } else {
        const bool up = (RANGE > 0 && dir_) || (RANGE < 0 && !dir_);
        const bool down = (RANGE > 0 && !dir_) || (RANGE < 0 && dir_);

        if (up)
            i_++;
        else if (down)
            i_--;
    }
}

void SeqCounter::nextConst()
{
    if (!shouldRepeat())
        return;

    floatTo(1, ri_++);
    floatTo(0, from_->value());

    if (ri_ == repeat_->value()) {
        done_ = true;
        anyTo(1, SYM_DONE, AtomListView {});
    }
}

void SeqCounter::reset()
{
    done_ = false;
    dir_ = DIR_FORWARD;
    ri_ = 0;
    i_ = from_->value();
}

bool SeqCounter::shouldRepeat() const
{
    const auto n = repeat_->value();
    return !done_ && (n == R_INFINITE || ri_ < n);
}

void setup_seq_counter()
{
    SYM_DONE = gensym("done");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<SeqCounter> obj("seq.counter");
    obj.addAlias("counter");

    obj.addMethod("reset", &SeqCounter::m_reset);

    obj.setXletsInfo({ "bang: increment and output\n"
                       "reset: reset counter to start value",
                         "bang: reset counter to start value" },
        { "int: counter value",
            "int: repeat index when iteration starts\n"
            "done: bang when done" });
}
