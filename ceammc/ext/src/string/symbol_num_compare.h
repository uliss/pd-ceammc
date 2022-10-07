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
#ifndef SYMBOL_NUM_COMPARE_H
#define SYMBOL_NUM_COMPARE_H

#include "ceammc_object.h"

using namespace ceammc;

class SymbolNumCompare : public BaseObject {
    t_symbol* cmp_with_;

public:
    SymbolNumCompare(const PdArgs& a);
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;

public:
    static int numPart(t_symbol* s);
};

void setup_symbol_num_compare();

#endif // SYMBOL_NUM_COMPARE_H
