#include "ceammc.h"
#include <algorithm>
#include <m_pd.h>

t_class* list_shuffle_class;
typedef struct list_shuffle {
    t_object x_obj;
} t_list_shuffle;

static void list_shuffle_list(t_list_shuffle* x, t_symbol* s, int argc, t_atom* argv)
{
    t_atom* copy = ceammc_atoms_alloc_copy(argc, argv);
    std::random_shuffle(copy, copy + argc);
    outlet_list(x->x_obj.te_outlet, s, argc, copy);
    ceammc_atoms_free(copy, argc);
}

static void* list_shuffle_new()
{
    t_list_shuffle* x = (t_list_shuffle*)pd_new(list_shuffle_class);
    outlet_new(&x->x_obj, &s_float);
    return (void*)x;
}

extern "C" void setup_list0x2eshuffle()
{
    list_shuffle_class = class_new(gensym("list.shuffle"),
        (t_newmethod)list_shuffle_new, (t_method)0,
        sizeof(t_list_shuffle), 0, A_NULL);
    class_addlist(list_shuffle_class, list_shuffle_list);
}
