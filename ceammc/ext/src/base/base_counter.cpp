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
#include "base_counter.h"
#include "ceammc_factory.h"

static t_symbol* SYM_DONE;
constexpr int R_INFINITE = -1;

BaseCounter::BaseCounter(const PdArgs& args)
    : BaseObject(args)
    , from_(nullptr)
    , to_(nullptr)
    , repeat_(nullptr)
    , ri_(0)
    , i_(0)
    , done_(false)
{
    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    from_->setSuccessFn([this](Property*) { i_ = from_->value(); });
    addProperty(from_);

    to_ = new IntProperty("@to", 0);
    to_->setArgIndex(1);
    addProperty(to_);

    repeat_ = new IntProperty("@r", -1);
    repeat_->checkMinEq(-1);
    repeat_->setArgIndex(2);
    addProperty(repeat_);

    createInlet();
    createOutlet();
    createOutlet();
}

void BaseCounter::onBang()
{
    next();
}

void BaseCounter::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        reset();
}

void BaseCounter::m_reset(t_symbol*, const AtomListView& lv)
{
    reset();
}

void BaseCounter::next()
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

void BaseCounter::reset()
{
    done_ = false;
    ri_ = 0;
    i_ = from_->value();
}

bool BaseCounter::shouldRepeat() const
{
    const auto n = repeat_->value();
    return !done_ && (n == R_INFINITE || ri_ < n);
}

void setup_base_counter()
{
    SYM_DONE = gensym("done");

    ObjectFactory<BaseCounter> obj("counter");
    obj.addMethod("reset", &BaseCounter::m_reset);

    obj.setXletsInfo({ "bang: increment and output\n"
                       "reset: reset counter to start value",
                         "bang: reset counter to start value" },
        { "int: counter value",
            "int: repeat index when iteration starts\n"
            "done: bang when done" });
}
