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
#ifndef ARRAY_DO_H
#define ARRAY_DO_H

#include "array_base.h"
#include "ceammc_object.h"

class ArrayDo : public ArrayMod {
    size_t idx_;

public:
    ArrayDo(const PdArgs& a);
    void onBang() override;
    void onSymbol(t_symbol* s) override;
    void onInlet(size_t n, const AtomListView& l) override;

private:
    void output();
    void finish();
};

void setup_array_do();

#endif // ARRAY_DO_H
