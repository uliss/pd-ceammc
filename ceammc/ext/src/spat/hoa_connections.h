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
#ifndef HOA_CONNECTIONS_H
#define HOA_CONNECTIONS_H

#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class HoaIn : public BaseObject {
    IntPropertyMin* extra_;

public:
    HoaIn(const PdArgs& args);
    void onBang() override;
    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomList& l) override;
    int extra() const { return extra_->value(); }

public:
    static HoaIn* fromObject(t_object* obj);
};

t_class* hoa_in_class();

void setup_spat_hoa_connections();

#endif // HOA_CONNECTIONS_H
