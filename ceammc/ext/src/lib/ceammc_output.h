/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_OUTPUT_H
#define CEAMMC_OUTPUT_H

#include "ceammc_macro.h"

namespace ceammc {

class Atom;
class AtomList;
class AtomListView;

bool outletAtom(t_outlet* o, const Atom& a);
bool outletAtomList(t_outlet* o, const AtomListView& v, bool simplifyType = false);

bool outletAny(t_outlet* o, t_symbol* s, const Atom& a);
bool outletAny(t_outlet* o, t_symbol* s, const AtomListView& v);
CEAMMC_WARN_UNUSED bool outletAny(t_outlet* o, const AtomListView& l);

}

#endif // CEAMMC_OUTPUT_H
