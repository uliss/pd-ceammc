#include <m_pd.h>

#include "ceammc.h"

static t_class* length_class;
typedef struct length {
    t_object x_obj;
} t_length;

static void length_list(t_length* x, t_symbol*, int argc, t_atom*)
{
    outlet_float(x->x_obj.te_outlet, argc);
}

static void length_any(t_length* x, t_symbol*, int argc, t_atom*)
{
    outlet_float(x->x_obj.te_outlet, argc + 1);
}

static void* length_new()
{
    t_length* x = reinterpret_cast<t_length*>(pd_new(length_class));
    outlet_new(&x->x_obj, &s_float);
    return reinterpret_cast<void*>(x);
}

extern "C" void setup_list0x2elength()
{
    length_class = class_new(CEAMMC_LIST_GENSYM(length),
        reinterpret_cast<t_newmethod>(length_new), 0, sizeof(t_length), 0, A_NULL);
    class_addlist(length_class, length_list);
    class_addanything(length_class, length_any);
}
