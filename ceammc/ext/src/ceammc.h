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

#ifdef __cplusplus
extern "C" {
#endif

#include <m_pd.h>
#include <stddef.h>

// module prefixes
#define CEAMMC_LIST_GENSYM(name) gensym("list." #name)
#define CEAMMC_MATH_GENSYM(name) gensym("math." #name)
#define CEAMMC_PATH_GENSYM(name) gensym("path." #name)

// to get extension setup function use CEAMMC_LIST_MODULE(each)
// it expands to: setup_list0x2eeach()
#define CEAMMC_DOT_MODULE(prefix, name) setup_##prefix##0x2e##name()
#define CEAMMC_MATH_MODULE(name) CEAMMC_DOT_MODULE(math, name)
#define CEAMMC_LIST_MODULE(name) CEAMMC_DOT_MODULE(list, name)
#define CEAMMC_PATH_MODULE(name) CEAMMC_DOT_MODULE(path, name)

/**
 * @brief compare atoms
 * @return 0 - if a1 == a2, _-1); -1 - if a1 < a2; +1 - if a1 > a2
 * @see ceammc_atoms_equal
 */
int ceammc_atoms_compare(const t_atom* a1, const t_atom* a2);

/**
 * @brief check for atoms equality
 * @return 1 if equal, 0 - otherwise
 * @see ceammc_atoms_compare
 */
int ceammc_atoms_equal(const t_atom* a1, const t_atom* a2);

/**
 * @brief checks is floats are equal with using machine epsilon
 * @return 1 if given floats are almost equal, 0 otherwise
 */
int ceammc_floats_equal(float a, float b);

t_atom* ceammc_atoms_alloc(size_t n);
void ceammc_atoms_free(t_atom* mem, size_t n);
void ceammc_atoms_copy(size_t n, t_atom* from, t_atom* to);
t_atom* ceammc_atoms_alloc_copy(size_t, t_atom* from);
void output_atom(t_outlet* out, t_atom* atom);

typedef t_float (*ceammc_float_unary_func)(t_float);
typedef t_float (*ceammc_float_binary_func)(t_float, t_float);
void ceammc_atoms_map_float(size_t n, t_atom* a, ceammc_float_unary_func func);
void ceammc_atoms_map_float_to_outlet(t_outlet* o, t_symbol* s, int n, t_atom* a, ceammc_float_unary_func func);
t_float ceammc_atoms_reduce_float(size_t n, t_atom* a, t_float init, ceammc_float_binary_func func);

size_t ceammc_memory_size();
size_t ceammc_memory_current_rss();
size_t ceammc_memory_peak_rss();

#ifdef __cplusplus
}
#endif

#endif // CEAMMC_H
