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
#include "ceammc_log.h"

namespace ceammc {

bool outletAtom(t_outlet* o, const Atom& a)
{
    if (a.isFloat()) {
        outlet_float(o, a.asFloat());
        return true;
    } else if (a.isSymbol()) {
        outlet_symbol(o, a.asSymbol());
        return true;
    } else if (a.isData()) {
        outlet_list(o, &s_list, 1, const_cast<t_atom*>(&a.atom()));
        return true;
    } else
        return false;
}

bool outletAtomList(t_outlet* o, const AtomList& l, bool simplifyType)
{
    if (!o) {
        LIB_DBG << "ERROR! NULL outlet pointer: " << __FUNCTION__;
        return false;
    }

    if (simplifyType) {
        if (l.isBang())
            outlet_bang(o);
        else if (l.isFloat())
            outlet_float(o, l[0].asFloat());
        else if (l.isSymbol())
            outlet_symbol(o, l[0].asSymbol());
        else
            outlet_list(o, &s_list, static_cast<int>(l.size()), l.toPdData());
    } else {
        outlet_list(o, &s_list, static_cast<int>(l.size()), l.toPdData());
    }

    return true;
}

bool outletAtomListView(t_outlet* o, const AtomListView& v, bool simplifyType)
{
    if (!o) {
        LIB_DBG << "ERROR! NULL outlet pointer: " << __FUNCTION__;
        return false;
    }

    if (simplifyType) {
        if (v.empty())
            outlet_bang(o);
        else if (v.isFloat())
            outlet_float(o, v.asFloat());
        else if (v.isSymbol())
            outlet_symbol(o, v.asSymbol());
        else
            outlet_list(o,
                &s_list,
                static_cast<int>(v.size()),
                const_cast<t_atom*>(&v.begin()->atom()));
    } else {
        outlet_list(o,
            &s_list,
            static_cast<int>(v.size()),
            const_cast<t_atom*>(&v.begin()->atom()));
    }

    return true;
}

bool outletAny(t_outlet* o, t_symbol* s, const Atom& a)
{
    outlet_anything(o, s, 1, const_cast<t_atom*>(&a.atom()));
    return true;
}

bool outletAny(t_outlet* o, t_symbol* s, const AtomList& l)
{
    outlet_anything(o, s, static_cast<int>(l.size()), l.toPdData());
    return true;
}

bool outletAny(_outlet* o, t_symbol* s, const AtomListView& v)
{
    outlet_anything(o, s,
        static_cast<int>(v.size()),
        const_cast<t_atom*>(&v.begin()->atom()));

    return true;
}

bool outletAny(t_outlet* o, const AtomList& l)
{
    if (l.size() < 1)
        return false;

    // check for valid selector
    if (!l.first()->isSymbol())
        return false;

    outlet_anything(o,
                    l.first()->asSymbol(),
                    static_cast<int>(l.size() - 1),
                    l.size() == 1 ? nullptr : l.toPdData() + 1);
    return true;
}

bool outletAny(t_outlet* o, const AtomListView& l)
{
    if (l.size() < 1)
        return false;

    // check for valid selector
    if (!l.front().isSymbol())
        return false;

    outlet_anything(o,
                    l.front().asSymbol(),
                    static_cast<int>(l.size() - 1),
                    l.size() == 1 ? nullptr : l.toPdData() + 1);
    return true;
}

}
