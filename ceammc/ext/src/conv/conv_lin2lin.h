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
#ifndef CONV_LIN2LIN_H
#define CONV_LIN2LIN_H

#include "baserangeconverter.h"

using namespace ceammc;

class Lin2Lin : public BaseRangeConverter {
public:
    Lin2Lin(const PdArgs& a);
    void onFloat(t_float value) override;
    void onList(const AtomListView& lv) override;
};

void setup_conv_lin2lin();

#endif // CONV_LIN2LIN_H
