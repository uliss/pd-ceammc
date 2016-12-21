#include "ceammc_controlvalue.h"
#include "ceammc_globaldata.h"
#include <m_pd.h>
#include <queue>

#define OBJ_NAME "data.fifo"
#define MSG_PREFIX "[" OBJ_NAME "] "

static const size_t MAX_FIFO_SIZE = 1024;

typedef ceammc::ControlValue ControlValue;
typedef std::queue<ControlValue> ControlFifo;
typedef ceammc::GlobalData<ControlFifo> SharedFifo;

static t_class* data_fifo_class;
struct t_data_fifo {
    t_object x_obj;
    size_t max_fifo_sz;
    SharedFifo* x_fifo;
};

// output first, remove first
static void fifo_pop(t_data_fifo* x)
{
    if ((*x->x_fifo)->empty())
        return;

    (*x->x_fifo)->front().output(x->x_obj.te_outlet);
    (*x->x_fifo)->pop();
}

// output if fixed size exceeded
static void fifo_maybe_output(t_data_fifo* x)
{
    if ((*x->x_fifo)->size() > x->max_fifo_sz)
        fifo_pop(x);
}

// output all, remove all
static void fifo_flush(t_data_fifo* x)
{
    size_t sz = (*x->x_fifo)->size();
    for (size_t i = 0; i < sz; i++) {
        fifo_pop(x);
    }
}

// dump to pd console
static void fifo_dump(t_data_fifo* x)
{
    startpost(MSG_PREFIX ":");
    size_t sz = (*x->x_fifo)->size();
    for (size_t i = 0; i < sz; i++) {
        poststring(x->x_fifo->name().c_str());
    }
    endpost();
}

// remove all, no output
static void fifo_clear(t_data_fifo* x)
{
    size_t sz = (*x->x_fifo)->size();
    for (size_t i = 0; i < sz; i++) {
        (*x->x_fifo)->pop();
    }
}

static void fifo_set_resize(t_data_fifo* x, t_floatarg sz)
{
    if (sz < 0) {
        pd_error(x, MSG_PREFIX "invalid size value: %f. Using default size: %li", sz, MAX_FIFO_SIZE);
        sz = 0;
    }

    x->max_fifo_sz = static_cast<size_t>(sz);
    if (x->max_fifo_sz == 0)
        x->max_fifo_sz = MAX_FIFO_SIZE;

    // remove all elements from queue
    fifo_clear(x);
}

static void data_fifo_anything(t_data_fifo* x, t_symbol* s, int argc, t_atom* argv)
{
    (*x->x_fifo)->push(ControlValue(s, argc, argv));
    fifo_maybe_output(x);
}

static void data_fifo_bang(t_data_fifo* x)
{
    fifo_flush(x);
}

static void data_fifo_float(t_data_fifo* x, t_floatarg f)
{
    (*x->x_fifo)->push(ControlValue(f));
    fifo_maybe_output(x);
}

static void data_fifo_symbol(t_data_fifo* x, t_symbol* s)
{
    (*x->x_fifo)->push(ControlValue(s));
    fifo_maybe_output(x);
}

static void data_fifo_list(t_data_fifo* x, t_symbol*, int argc, t_atom* argv)
{
    (*x->x_fifo)->push(ControlValue(argc, argv));
    fifo_maybe_output(x);
}

static void* data_fifo_new(t_symbol* id)
{
    t_data_fifo* x = reinterpret_cast<t_data_fifo*>(pd_new(data_fifo_class));
    outlet_new(&x->x_obj, &s_float);
    x->x_fifo = new SharedFifo(id->s_name);
    fifo_set_resize(x, MAX_FIFO_SIZE);
    return static_cast<void*>(x);
}

static void data_fifo_free(t_data_fifo* x)
{
    delete x->x_fifo;
}

extern "C" void setup_data0x2efifo()
{
    data_fifo_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(data_fifo_new),
        reinterpret_cast<t_method>(data_fifo_free),
        sizeof(t_data_fifo), 0, A_DEFSYMBOL, A_NULL);
    class_addanything(data_fifo_class, data_fifo_anything);
    class_addbang(data_fifo_class, data_fifo_bang);
    class_addfloat(data_fifo_class, data_fifo_float);
    class_addsymbol(data_fifo_class, data_fifo_symbol);
    class_addlist(data_fifo_class, data_fifo_list);
    class_addmethod(data_fifo_class, reinterpret_cast<t_method>(fifo_flush), gensym("flush"), A_NULL);
    class_addmethod(data_fifo_class, reinterpret_cast<t_method>(fifo_dump), gensym("dump"), A_NULL);
    class_addmethod(data_fifo_class, reinterpret_cast<t_method>(fifo_clear), gensym("clear"), A_NULL);
    class_addmethod(data_fifo_class, reinterpret_cast<t_method>(fifo_pop), gensym("pop"), A_NULL);
    class_addmethod(data_fifo_class, reinterpret_cast<t_method>(fifo_set_resize),
        gensym("resize"), A_DEFFLOAT, A_NULL);
}
