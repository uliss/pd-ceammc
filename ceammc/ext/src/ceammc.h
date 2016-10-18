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
#ifndef CEAMMC_H
#define CEAMMC_H

#include <m_pd.h>
#include <stddef.h>

// MATH prefixes
// to get "math.abs" use CEAMMC_MATH_EXT("abs")
#define CEAMMC_MATH_EXT(name) "math." name

// to get extension setup function use CEAMMC_MATH_MODULE(round)
// it expands to: setup_math0x2eround()
#define CEAMMC_MATH_MODULE(name) setup_math0x2e##name()

// LIST prefixes
// to get "list.each" use CEAMMC_LIST_EXT("each")
#define CEAMMC_LIST_EXT(name) "list." name

// to get extension setup function use CEAMMC_LIST_MODULE(each)
// it expands to: setup_list0x2eeach()
#define CEAMMC_LIST_MODULE(name) setup_list0x2e##name()

t_atom* ceammc_atoms_alloc(size_t n);
void ceammc_atoms_free(t_atom* mem, size_t n);
void ceammc_atoms_copy(size_t n, t_atom* from, t_atom* to);
t_atom* ceammc_atoms_alloc_copy(size_t, t_atom* from);

typedef t_float (*ceammc_map_float)(t_float);
void ceammc_atoms_map_float(size_t n, t_atom* a,
    ceammc_map_float func);
void ceammc_atoms_map_float_to_outlet(t_outlet* o, t_symbol *s, int n, t_atom* a, ceammc_map_float func);

#endif // CEAMMC_H
