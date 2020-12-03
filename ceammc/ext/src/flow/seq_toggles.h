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
#ifndef SEQ_TOGGLES_H
#define SEQ_TOGGLES_H

#include "seq_bangs.h"

class SeqToggles : public SeqBangs {
    FloatProperty* duration_;
    ClockLambdaFunction clock_off_;

public:
    SeqToggles(const PdArgs& args);

//    void output

protected:
//    void schedNext() override;
//    void outputEvent() override;
//    void stop() override;
//    void reset() override;
};

void setup_seq_toggles();

#endif // SEQ_TOGGLES_H
