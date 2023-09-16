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
#include "chrono_datetime.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

#include <ctime>

ChronoDatetime::ChronoDatetime(const PdArgs& args)
    : BaseObject(args)
    , utc_(nullptr)
{
    createOutlet();
    createOutlet();

    utc_ = new BoolProperty("@utc", false);
    addProperty(utc_);
}

void ChronoDatetime::onBang()
{
    auto t = time(nullptr);
    std::tm* tm = nullptr;

    if (utc_->value())
        tm = std::gmtime(&t);
    else
        tm = std::localtime(&t);

    AtomArray<3> time_msg { tm->tm_hour, tm->tm_min, tm->tm_sec };
    listTo(1, time_msg.view());

    AtomArray<3> date_msg { tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday };
    listTo(0, date_msg.view());
}

void setup_chrono_datetime()
{
    ObjectFactory<ChronoDatetime> obj("chrono.datetime");
    obj.addAlias("datetime");

    obj.setXletsInfo(
        { "bang: output date and time" },
        { "list: YEAR MONTH DAY", "list: HOUR MINUTE SECOND" });
}
