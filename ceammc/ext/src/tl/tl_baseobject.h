/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TL_BASEOBJECT_H
#define TL_BASEOBJECT_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

#include <memory>

namespace ceammc {
namespace tl {
    class TimelineData;
}
}

using namespace ceammc;

class TlBaseObject : public BaseObject {
    std::unique_ptr<tl::TimelineData> tl_;
    ClockMemberFunction<TlBaseObject> update_;

public:
    TlBaseObject(const PdArgs& args);
    ~TlBaseObject() override;

    virtual void onCueEnter() = 0;
    virtual void onCueLeave() = 0;

    void updatePos();

public:
    static void setDisplaceFn(t_class* c);
};

#endif // TL_BASEOBJECT_H
