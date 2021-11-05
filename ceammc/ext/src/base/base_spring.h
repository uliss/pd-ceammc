/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef BASE_SPRING_H
#define BASE_SPRING_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

class BaseSpring : public BaseObject {
public:
    enum State {
        STATE_INIT,
        STATE_RUNNING,
        STATE_SET
    };

private:
    FloatProperty* value_;
    FloatProperty* total_time_;
    FloatProperty* accel_;
    ClockLambdaFunction clock_;
    t_float start_value_;
    t_float t_;
    State state_;

public:
    BaseSpring(const PdArgs& args);
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomListView& lv) override;

private:
    t_float valueAtTime(t_float t) const;
    void reset();
    void handleMouse(bool on);
};

void setup_base_spring();

#endif // BASE_SPRING_H
