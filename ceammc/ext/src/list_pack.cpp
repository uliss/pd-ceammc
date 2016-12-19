#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_pack_class;
struct t_list_pack {
    t_object x_obj;
    t_inlet* in_count;
    ceammc::pd::atom_list* lst;
    size_t pack_size;
};

static void list_clear(t_list_pack* x)
{
    x->lst->clear();
}

static void list_flush(t_list_pack* x)
{
    if (x->lst->size() < 1)
        return;

    ceammc::pd::output(x->x_obj.te_outlet, *x->lst);
    list_clear(x);
}

static void list_set_size(t_list_pack* x, t_floatarg sz)
{
    if (sz < 0) {
        pd_error(x, "invalid pack count: %f", sz);
        return;
    }

    x->pack_size = static_cast<int>(sz);
    list_clear(x);
    x->lst->reserve(x->pack_size);
}

static void list_pack_float(t_list_pack* x, t_floatarg f)
{
    t_atom v;
    SETFLOAT(&v, f);
    x->lst->push_back(v);

    if (x->lst->size() >= x->pack_size)
        list_flush(x);
}

static void list_pack_symbol(t_list_pack* x, t_symbol* s)
{
    t_atom v;
    SETSYMBOL(&v, s);
    x->lst->push_back(v);

    if (x->lst->size() >= x->pack_size)
        list_flush(x);
}

static void list_pack_list(t_list_pack* x, t_symbol* s, int argc, t_atom* argv)
{
    for (int i = 0; i < argc; i++) {
        t_atom* a = argv + i;
        switch (a->a_type) {
        case A_FLOAT:
            list_pack_float(x, atom_getfloat(a));
            break;
        case A_SYMBOL:
            list_pack_symbol(x, atom_getsymbol(a));
            break;
        default:
            break;
        }
    }
}

static void* list_pack_new(t_floatarg n)
{
    t_list_pack* x = reinterpret_cast<t_list_pack*>(pd_new(list_pack_class));
    outlet_new(&x->x_obj, &s_float);
    x->in_count = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("set_size"));
    x->pack_size = 0;
    x->lst = new ceammc::pd::atom_list();
    list_set_size(x, n);
    return static_cast<void*>(x);
}

static void list_pack_free(t_list_pack* x)
{
    inlet_free(x->in_count);
    delete x->lst;
}

extern "C" void setup_list0x2epack()
{
    list_pack_class = class_new(gensym("list.pack"),
        reinterpret_cast<t_newmethod>(list_pack_new),
        reinterpret_cast<t_method>(list_pack_free),
        sizeof(t_list_pack), 0, A_DEFFLOAT, A_NULL);
    class_addfloat(list_pack_class, list_pack_float);
    class_addsymbol(list_pack_class, list_pack_symbol);
    class_addlist(list_pack_class, list_pack_list);
    class_addmethod(list_pack_class,
        reinterpret_cast<t_method>(list_set_size),
        gensym("set_size"), A_DEFFLOAT, 0);
    class_addmethod(list_pack_class,
        reinterpret_cast<t_method>(list_flush),
        gensym("flush"), A_NULL);
    class_addmethod(list_pack_class,
        reinterpret_cast<t_method>(list_clear),
        gensym("clear"), A_NULL);
}
