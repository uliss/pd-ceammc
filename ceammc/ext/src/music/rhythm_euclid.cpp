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
#include "rhythm_euclid.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "fmt/core.h"

constexpr int MAX_LENGTH = 1024 * 8;

RhythmEuclid::RhythmEuclid(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
    , beats_(nullptr)
    , offset_(nullptr)
    , changed_(false)
{
    createInlet();
    createInlet();
    createOutlet();

    n_ = new IntProperty("@n", 8);
    n_->checkClosedRange(1, MAX_LENGTH);
    n_->setArgIndex(0);
    n_->setSuccessFn([this](Property*) { changed_ = true; });
    addProperty(n_);

    beats_ = new IntProperty("@beats", 3);
    beats_->checkClosedRange(0, MAX_LENGTH);
    beats_->setArgIndex(1);
    beats_->setSuccessFn([this](Property*) { changed_ = true; });
    addProperty(beats_);

    offset_ = new IntProperty("@offset", 0);
    offset_->setArgIndex(2);
    offset_->setSuccessFn([this](Property*) { changed_ = true; });
    addProperty(offset_);
}

void RhythmEuclid::initDone()
{
    pattern_.reserve(n_->value());
}

void RhythmEuclid::onBang()
{
    if (!updatePattern())
        return;

    listTo(0, pattern_);
}

void RhythmEuclid::onList(const AtomListView& lv)
{
    auto N = lv.size();

    if (N > 0)
        n_->set(lv.subView(0, 1));

    if (N > 1)
        beats_->set(lv.subView(1, 1));

    if (N > 2)
        offset_->set(lv.subView(2, 1));
}

void RhythmEuclid::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        beats_->set(lv);
    else if (n == 2)
        offset_->set(lv);
}

bool RhythmEuclid::updatePattern()
{
    if (!changed_)
        return true;

    if (beats_->value() > n_->value()) {
        OBJ_ERR << fmt::format("number of beats expected to be <= {}, got {}", n_->value(), beats_->value());
        return false;
    }

    pattern_ = list::bresenham(n_->value(), beats_->value());
    if (offset_->value())
        pattern_ = list::rotate(pattern_, -offset_->value());

    changed_ = false;
    return true;
}

void setup_rhythm_euclid()
{
    ObjectFactory<RhythmEuclid> obj("rhythm.euclid");
}
