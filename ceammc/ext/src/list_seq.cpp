#include "ceammc.hpp"
#include <m_pd.h>

t_class* list_seq_class;
struct t_list_seq {
    t_object x_obj;
    t_inlet* i_times;
    t_inlet* i_step;
    t_float start;
    t_float times;
    t_float step;
};

static void list_seq_bang(t_list_seq* x)
{
    if (!ceammc::math::is_natural(x->times)) {
        pd_error(x, "sequence length should be natural number. %f", x->times);
        return;
    }

    if (x->times < 1)
        return;

    ceammc::pd::atom_list lst;
    lst.reserve(x->times);

    for (int i = 0; i < x->times; i++) {
        t_float item = x->start + i * x->step;
        t_atom v;
        SETFLOAT(&v, item);
        lst.push_back(v);
    }

    ceammc::pd::output(x->x_obj.te_outlet, lst);
}

static void list_seq_list(t_list_seq* x, t_symbol* s, int argc, t_atom* argv)
{
}

static void* list_seq_new(t_floatarg start, t_floatarg num, t_floatarg step)
{
    t_list_seq* x = reinterpret_cast<t_list_seq*>(pd_new(list_seq_class));
    outlet_new(&x->x_obj, &s_list);
    x->i_times = floatinlet_new(&x->x_obj, &x->times);
    x->i_step = floatinlet_new(&x->x_obj, &x->step);
    x->start = start;
    x->times = num;
    x->step = (step == 0) ? 1 : step;
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2eseq()
{
    list_seq_class = class_new(gensym("list.seq"),
        reinterpret_cast<t_newmethod>(list_seq_new),
        static_cast<t_method>(0),
        sizeof(t_list_seq), 0, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(list_seq_class, list_seq_bang);
    class_addlist(list_seq_class, list_seq_list);
}
