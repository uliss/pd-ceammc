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
#ifndef FLOW_MULTIPLEX2_TILDE_H
#define FLOW_MULTIPLEX2_TILDE_H

#include "flow_multiplex_tilde.h"
using namespace ceammc;

class Multiplex2Tilde : public MultiplexTilde {
public:
    Multiplex2Tilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;

    const char* annotateInlet(size_t n) const override;
};

void setup_flow_multiplex2_tilde();

#endif // FLOW_MULTIPLEX2_TILDE_H
