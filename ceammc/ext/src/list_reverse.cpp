#include <m_pd.h>
#include <algorithm>
#include "ceammc.h"

t_class* list_reverse_class;
typedef struct list_reverse {
    t_object x_obj;
} t_list_reverse;

static void list_reverse_list(t_list_reverse* x, t_symbol* s, int argc, t_atom* argv)
{
    t_atom* copy = ceammc_atoms_alloc_copy(argc, argv);
    std::reverse(copy, copy + argc);
    outlet_list(x->x_obj.te_outlet, s, argc, copy);
    ceammc_atoms_free(copy, argc);
}

static void* list_reverse_new()
{
    t_list_reverse* x = (t_list_reverse*)pd_new(list_reverse_class);
    outlet_new(&x->x_obj, &s_list);
    return (void*)x;
}

extern "C" void setup_list0x2ereverse()
{
    list_reverse_class = class_new(gensym("list.reverse"),
        (t_newmethod)list_reverse_new, (t_method)0,
            sizeof(t_list_reverse), 0, A_NULL);
    class_addlist(list_reverse_class, list_reverse_list);
}

