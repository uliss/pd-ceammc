#include "ceammc.hpp"
#include <m_pd.h>
#include <math.h>

#define PREFIX "[list.repeat] "

const static t_float REPEAT_MAX = 10000;

t_class* list_repeat_class;
typedef struct list_repeat {
    t_object x_obj;
    t_inlet* n_inlet;
    t_float n;
} t_list_repeat;

template <class T>
static inline bool is_valid(T v)
{
    return (ceammc::math::is_natural(v) && v < REPEAT_MAX) ? true : false;
}

static void list_repeat_float(t_list_repeat* x, t_floatarg f)
{
    if (!is_valid(x->n)) {
        pd_error(x, PREFIX "invalid repeat value: %f", x->n);
        return;
    }

    if (x->n == 0.0)
        return;

    size_t n = static_cast<size_t>(x->n);
    t_atom v;
    SETFLOAT(&v, f);
    ceammc::pd::output(x->x_obj.te_outlet, ceammc::pd::atom_list(n, v));
}

static void list_repeat_list(t_list_repeat* x, t_symbol* s, int argc, t_atom* argv)
{
    if (!is_valid(x->n)) {
        pd_error(x, PREFIX "invalid repeat value: %f", x->n);
        return;
    }

    if (x->n == 0.0)
        return;

    size_t n = static_cast<size_t>(x->n);
    ceammc::pd::atom_list result;
    result.reserve(n * argc);
    for (size_t i = 0; i < n; i++) {
        for (int j = 0; j < argc; j++) {
            result.push_back(argv[j]);
        }
    }

    ceammc::pd::output(x->x_obj.te_outlet, result);
}

static void* list_repeat_new(t_floatarg f)
{
    t_list_repeat* x = reinterpret_cast<t_list_repeat*>(pd_new(list_repeat_class));
    x->n_inlet = floatinlet_new(&x->x_obj, &x->n);

    if (!is_valid(f)) {
        pd_error(x, PREFIX "invalid repeat value: %f, setting to 1", f);
        x->n = 1;
    } else {
        x->n = static_cast<int>(f);
    }

    outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2erepeat()
{
    list_repeat_class = class_new(gensym("list.repeat"),
        reinterpret_cast<t_newmethod>(list_repeat_new),
        static_cast<t_method>(0),
        sizeof(t_list_repeat), CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addfloat(list_repeat_class, list_repeat_float);
    class_addlist(list_repeat_class, list_repeat_list);
}
