#include "ceammc.h"

#include "memrss.h"
#include "memsize.h"

#include <float.h>
#include <math.h>
#include <string.h>

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

void ceammc_atoms_map_float(size_t n, t_atom* a, ceammc_float_unary_func func)
{
    for (size_t i = 0; i < n; i++) {
        if (a[i].a_type == A_FLOAT) {
            a[i].a_w.w_float = func(a[i].a_w.w_float);
        }
    }
}

void ceammc_atoms_map_float_to_outlet(t_outlet* o, t_symbol* s, int n, t_atom* a, ceammc_float_unary_func func)
{
    t_atom* lst = ceammc_atoms_alloc_copy(n, a);
    ceammc_atoms_map_float(n, lst, func);
    outlet_list(o, s, n, lst);
    ceammc_atoms_free(lst, n);
}

t_float ceammc_atoms_reduce_float(size_t n, t_atom* a, t_float init, ceammc_float_binary_func func)
{
    t_float accum = init;
    for (size_t i = 0; i < n; i++) {
        accum = func(accum, atom_getfloat(&a[i]));
    }
    return accum;
}

int ceammc_atoms_compare(const t_atom* a1, const t_atom* a2)
{
    if (a1->a_type == a2->a_type) {
        switch (a1->a_type) {
        case A_FLOAT:
            if (a1->a_w.w_float < a2->a_w.w_float)
                return -1;
            if (a1->a_w.w_float > a2->a_w.w_float)
                return 1;
            else
                return 0;
        case A_SYMBOL:
        case A_DEFSYMBOL: {
            int r = strcmp(a1->a_w.w_symbol->s_name, a2->a_w.w_symbol->s_name);
            if (r < 0)
                return -1;
            if (r > 0)
                return 1;
            else
                return 0;
            return r;
        }
        default:
            return 0;
        }
    }

    return (a1->a_type < a2->a_type) ? -1 : 1;
}

void output_atom(t_outlet* out, t_atom* atom)
{
    if (!out || !atom)
        return;
    switch (atom->a_type) {
    case A_FLOAT:
        outlet_float(out, atom->a_w.w_float);
        break;
    case A_SYMBOL:
        outlet_symbol(out, atom->a_w.w_symbol);
        break;
    case A_POINTER:
        outlet_pointer(out, atom->a_w.w_gpointer);
        break;
    default:
        break;
    }
}

int ceammc_atoms_equal(const t_atom* a1, const t_atom* a2)
{
    return ceammc_atoms_compare(a1, a2) == 0 ? 1 : 0;
}

size_t ceammc_memory_size()
{
    return getMemorySize();
}

size_t ceammc_memory_current_rss()
{
    return getCurrentRSS();
}

size_t ceammc_memory_peak_rss()
{
    return getPeakRSS();
}

int ceammc_floats_equal(float a, float b)
{
    // Calculate the difference.
    float diff = fabs(a - b);
    a = fabs(a);
    b = fabs(b);
    // Find the largest
    float largest = (b > a) ? b : a;

    if (diff <= largest * FLT_EPSILON)
        return 1;
    return 0;
}
