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
#ifndef REPLACE_H
#define REPLACE_H

#include "ceammc_factory.h"
#include "ceammc_object.h"

namespace ceammc {
class Replace : public BaseObject {
    Atom from_;
    Atom to_;

public:
    Replace(const PdArgs& a);

    void onInlet(size_t n, const AtomList& l) final;
    void onAny(t_symbol* sel, const AtomListView& l) final;
    void onList(const AtomList& l) final;
    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    bool validateArgs() const;
};
}

void setup_base_replace();

#endif // REPLACE_H
