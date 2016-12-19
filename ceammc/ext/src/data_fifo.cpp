#include "ceammc.hpp"
#include <m_pd.h>
#include <queue>

typedef std::queue<t_atom> atom_fifo;

t_class* data_fifo_class;
struct t_data_fifo {
    t_object x_obj;
    atom_fifo* fifo;
    int fifo_sz;
};

static void data_fifo_anything(t_data_fifo* x, t_symbol* s, int argc, t_atom* argv)
{
}

static void data_fifo_bang(t_data_fifo* x)
{
    size_t sz = x->fifo->size();

    for (size_t i = 0; i < sz; i++) {
        output_atom(x->x_obj.te_outlet, &x->fifo->front());
        x->fifo->pop();
    }
}

static void data_fifo_float(t_data_fifo* x, t_floatarg f)
{
    t_atom a;
    SETFLOAT(&a, f);
    x->fifo->push(a);
    if (x->fifo->size() > x->fifo_sz) {
        outlet_float(x->x_obj.te_outlet, atom_getfloat(&x->fifo->front()));
        x->fifo->pop();
    }
}

static void data_fifo_list(t_data_fifo* x, t_symbol* s, int argc, t_atom* argv)
{
}

static void* data_fifo_new(t_floatarg sz)
{
    if (sz < 0) {
        error("invalid argument: %f", sz);
        return 0;
    }

    t_data_fifo* x = reinterpret_cast<t_data_fifo*>(pd_new(data_fifo_class));
    outlet_new(&x->x_obj, &s_float);
    x->fifo_sz = static_cast<int>(sz);
    x->fifo = new atom_fifo();
    return static_cast<void*>(x);
}

static void data_fifo_free(t_data_fifo* x)
{
    delete x->fifo;
}

extern "C" void setup_data0x2efifo()
{
    data_fifo_class = class_new(gensym("data.fifo"),
        reinterpret_cast<t_newmethod>(data_fifo_new),
        reinterpret_cast<t_method>(data_fifo_free),
        sizeof(t_data_fifo), 0, A_DEFFLOAT, A_NULL);
    class_addanything(data_fifo_class, data_fifo_anything);
    class_addbang(data_fifo_class, data_fifo_bang);
    class_addfloat(data_fifo_class, data_fifo_float);
    class_addlist(data_fifo_class, data_fifo_list);
}
