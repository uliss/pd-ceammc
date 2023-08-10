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
#ifndef FLOW_DEMULTIPLEX2_TILDE_H
#define FLOW_DEMULTIPLEX2_TILDE_H

#include "flow_demultiplex_tilde.h"
using namespace ceammc;

class Demultiplex2Tilde : public DemultiplexTilde {
public:
    Demultiplex2Tilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;

    const char* annotateOutlet(size_t n) const final;
};

void setup_flow_demultiplex2_tilde();

#endif // FLOW_DEMULTIPLEX2_TILDE_H
