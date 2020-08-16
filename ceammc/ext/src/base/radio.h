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
#ifndef RADIO_H
#define RADIO_H

#include "ceammc_object.h"
using namespace ceammc;

class Radio : public BaseObject {
    IntProperty* n_;

public:
    Radio(const PdArgs& args);

    void onFloat(t_float f) override;
    void m_reset(t_symbol* s, const AtomListView&);
};

void setup_base_radio();

#endif // RADIO_H
