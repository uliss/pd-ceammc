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
#include "ceammc_factory.h"

#include <algorithm>

SeqToggles::SeqToggles(const PdArgs& args)
    : SeqBangs(args)
    , duration_(nullptr)
    , clock_off_([this]() { floatTo(0, 0); })
{
    duration_ = new FloatProperty("@dur", 0.75);
    duration_->checkClosedRange(0, 1);
    addProperty(duration_);
}

//void SeqToggles::schedNext()
//{
//    const auto ms = currentEventDurationMs();
//    if (ms < 0)
//        return;

//    const t_float toff_ms = std::max<t_float>(1, ms * duration_->value());
//    clock_off_.delay(toff_ms);

////    SeqBangs::schedNext();
//}

//void SeqToggles::outputEvent()
//{
//    floatTo(0, 1);
//}

//void SeqToggles::stop()
//{
//    SeqBangs::stop();
//    if (clock_off_.isActive()) {
//        clock_off_.exec();
//        clock_off_.unset();
//    }
//}

//void SeqToggles::reset()
//{
//    if (clock_off_.isActive()) {
//        clock_off_.exec();
//        clock_off_.unset();
//    }

//    SeqBangs::reset();
//}

void setup_seq_toggles()
{
    ObjectFactory<SeqToggles> obj("seq.toggles");
    obj.addAlias("seq.t");

    obj.addMethod("stop", &SeqToggles::m_stop);
    obj.addMethod("reset", &SeqToggles::m_reset);
}
