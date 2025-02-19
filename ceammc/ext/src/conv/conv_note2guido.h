/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef CONV_NOTE2GUIDO_H
#define CONV_NOTE2GUIDO_H

#include <array>

#include "ceammc_datatypes.h"
#include "ceammc_object.h"
using namespace ceammc;

class ConvNote2Guido : public BaseObject {
    using GuidoNote = BoostStaticString<15>;

public:
    ConvNote2Guido(const PdArgs& args);

    void onFloat(t_float f) final;
    void onList(const AtomListView& lv) override;

    void m_note(t_symbol* s, const AtomListView& lv);
public:
    GuidoNote guidoNote(int midiPitch, int dur = -1, int dots = 0) const;
};

void setup_conv_note2guido();

#endif // CONV_NOTE2GUIDO_H
