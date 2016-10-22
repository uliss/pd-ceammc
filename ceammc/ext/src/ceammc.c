/*****************************************************************************
 *
 * Created: 17.10.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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

#include "ceammc.h"

t_atom* ceammc_atoms_alloc(size_t n)
{
    return (t_atom*)getbytes(n * sizeof(t_atom));
}

void ceammc_atoms_free(t_atom* mem, size_t n)
{
    freebytes(mem, n * sizeof(t_atom));
}

void ceammc_atoms_copy(size_t n, t_atom* from, t_atom* to)
{
    for (size_t i = 0; i < n; i++)
        to[i] = from[i];
}

t_atom* ceammc_atoms_alloc_copy(size_t n, t_atom* from)
{
    t_atom* atoms = ceammc_atoms_alloc(n);
    ceammc_atoms_copy(n, from, atoms);
    return atoms;
}

void ceammc_atoms_map_float(size_t n, t_atom* a,
    ceammc_float_unary_func func)
{
    for (size_t i = 0; i < n; i++) {
        if (a[i].a_type == A_FLOAT) {
            a[i].a_w.w_float = func(a[i].a_w.w_float);
        }
    }
}

void ceammc_atoms_map_float_to_outlet(t_outlet* o, t_symbol* s, int n, t_atom* a, ceammc_float_unary_func func)
{
    t_atom * lst = ceammc_atoms_alloc_copy(n, a);
    ceammc_atoms_map_float(n, lst, func);
    outlet_list(o, s, n, lst);
    ceammc_atoms_free(lst, n);
}
