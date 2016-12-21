#include "ceammc_atomlist.h"
#include <m_pd.h>

#define OBJ_NAME "list.min"
#define MSG_PREFIX "[" OBJ_NAME "] "

static t_class* list_min_class;
typedef struct list_min {
    t_object x_obj;
} t_list_min;

using namespace ceammc;

static void list_min_list(t_list_min* x, t_symbol*, int argc, t_atom* argv)
{
    AtomList lst(static_cast<size_t>(argc), argv);
    Atom* a = lst.min();
    if (a == 0) {
        pd_error(x, MSG_PREFIX "invalid list");
        return;
    }

    to_outlet(x->x_obj.te_outlet, *a);
}

static void* list_min_new()
{
    t_list_min* x = reinterpret_cast<t_list_min*>(pd_new(list_min_class));
    outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2emin()
{
    list_min_class = class_new(gensym("list.min"),
        reinterpret_cast<t_newmethod>(list_min_new),
        static_cast<t_method>(0),
        sizeof(t_list_min), 0, A_NULL);
    class_addlist(list_min_class, list_min_list);
}
