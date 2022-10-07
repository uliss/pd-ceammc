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
#ifndef VLINE_TO_ENV_H
#define VLINE_TO_ENV_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "datatype_env.h"

using namespace ceammc;

class Vline2Env : public BaseObject {
    DataTypeEnv env_;
    double time_;
    size_t len_;
    double v_;
    ClockMemberFunction<Vline2Env> clock_;

public:
    Vline2Env(const PdArgs& args);

    void onBang() override;
    void onList(const AtomListView& lv) override;

private:
    void tick();
};

void setup_vline_to_env();

#endif // VLINE_TO_ENV_H
