/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef CONV_BITS2NOTE_H
#define CONV_BITS2NOTE_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <bitset>

using namespace ceammc;

class ConvBits2Note : public BaseObject {
    FlagProperty* poly_;
    ListProperty* keys_;
    FloatProperty* vel_;
    SymbolEnumProperty* mode_;
    std::bitset<128> notes_;

public:
    ConvBits2Note(const PdArgs& args);

    void initDone() override;
    void onList(const AtomList& lst) override;

private:
    void noteTo(size_t n, t_float note, t_float vel);
    void processNote(size_t n, t_float note, bool on);
};

void setup_conv_bits2note();

#endif // CONV_BITS2NOTE_H
