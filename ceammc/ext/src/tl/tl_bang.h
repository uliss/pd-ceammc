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
#ifndef TL_BANG_H
#define TL_BANG_H

#include "ceammc_clock.h"
#include "tl_baseobject.h"

class TlBang : public TlBaseObject {
    ClockMemberFunction<TlBang> clock_;
    FloatProperty* delay_;

public:
    TlBang(const PdArgs& args);
    void tick();

    void onCueEnter() override;
    void onCueLeave() override;
};

void setup_tl_bang();

#endif // TL_BANG_H
