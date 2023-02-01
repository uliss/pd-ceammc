/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef ARRAY_P2S_H
#define ARRAY_P2S_H

#include "array_base.h"

class ArrayPhaseToSample : public ArrayBase {
public:
    ArrayPhaseToSample(const PdArgs& a);

    /**
     * @brief converts float value of phase to array sample position
     * @param phase - in range 0..1
     */
    void onFloat(t_float phase) override;

    void onList(const AtomListView& lv) override;
};

void setup_array_p2s();

#endif // ARRAY_P2S_H
