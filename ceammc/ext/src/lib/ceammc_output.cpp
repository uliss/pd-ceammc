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
#include "ceammc_output.h"
#include "ceammc_atomlist.h"

namespace ceammc {

bool outletAtom(t_outlet* o, const Atom& a)
{
    if (a.isFloat()) {
        outlet_float(o, a.asT<t_float>());
        return true;
    } else if (a.isSymbol()) {
        outlet_symbol(o, a.asT<t_symbol*>());
        return true;
    } else if (a.isData()) {
        outlet_list(o, &s_list, 1, const_cast<t_atom*>(&a.atom()));
        return true;
    } else
        return false;
}

bool outletAny(t_outlet* o, t_symbol* s, const Atom& a)
{
    outlet_anything(o, s, 1, const_cast<t_atom*>(&a.atom()));
}

}
