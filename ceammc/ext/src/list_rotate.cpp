#include <algorithm>
#include <m_pd.h>

t_class* list_rotate_class;
struct t_list_rotate {
    t_object x_obj;
    t_inlet* in_step;
    t_float r_step;
};

static void list_rotate_list(t_list_rotate* x, t_symbol* s, int argc, t_atom* argv)
{
    int steps = static_cast<int>(x->r_step) % argc;
    t_atom* n_first = argv - steps;
    if (steps >= 0)
        n_first = n_first + argc;

    std::rotate(argv, n_first, argv + argc);
    outlet_list(x->x_obj.te_outlet, s, argc, argv);
}

static void* list_rotate_new(t_floatarg r)
{
    t_list_rotate* x = reinterpret_cast<t_list_rotate*>(pd_new(list_rotate_class));
    outlet_new(&x->x_obj, &s_list);
    x->in_step = floatinlet_new(&x->x_obj, &x->r_step);
    x->r_step = r;
    return static_cast<void*>(x);
}

static void list_rotate_free(t_list_rotate* x)
{
    inlet_free(x->in_step);
}

extern "C" void setup_list0x2erotate()
{
    list_rotate_class = class_new(gensym("list.rotate"),
        reinterpret_cast<t_newmethod>(list_rotate_new),
        reinterpret_cast<t_method>(list_rotate_free),
        sizeof(t_list_rotate), 0, A_DEFFLOAT, 0);
    class_addlist(list_rotate_class, list_rotate_list);
}
