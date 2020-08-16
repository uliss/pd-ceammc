/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef HW_APPLE_SMC_H
#define HW_APPLE_SMC_H

#include "ceammc_object.h"

#ifdef WITH_SMC
#include "apple/smc.h"
#endif

using namespace ceammc;

class HwAppleSMC : public BaseObject {
#ifdef WITH_SMC
    apple_smc::SMC smc_;
#endif

public:
    HwAppleSMC(const PdArgs& args);

    AtomList propKeys() const;
    void m_read(t_symbol* s, const AtomListView& l);
};

void setup_hw_apple_smc();

#endif // HW_APPLE_SMC_H
