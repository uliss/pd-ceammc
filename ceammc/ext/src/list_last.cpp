#include "ceammc_atom.h"
#include <m_pd.h>

static t_class* list_last_class;
struct t_list_last {
    t_object x_obj;
};

using namespace ceammc;

static void list_last_list(t_list_last* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    Atom a(argv[argc - 1]);
    to_outlet(x->x_obj.te_outlet, a);
}

static void* list_last_new()
{
    t_list_last* x = reinterpret_cast<t_list_last*>(pd_new(list_last_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2elast()
{
    list_last_class = class_new(gensym("list.last"),
        reinterpret_cast<t_newmethod>(list_last_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_last), 0, A_NULL);
    class_addlist(list_last_class, list_last_list);
}
