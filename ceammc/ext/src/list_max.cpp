#include "ceammc_atomlist.h"
#include <m_pd.h>

#define OBJ_NAME "list.max"
#define MSG_PREFIX "[" OBJ_NAME "] "

static t_class* list_max_class;
typedef struct list_max {
    t_object x_obj;
} t_list_max;

using namespace ceammc;

static void list_max_list(t_list_max* x, t_symbol* /*s*/, int argc, t_atom* argv)
{
    AtomList lst(static_cast<size_t>(argc), argv);
    Atom* a = lst.max();
    if (a == 0) {
        pd_error(x, MSG_PREFIX "invalid list");
    }

    to_outlet(x->x_obj.te_outlet, *a);
}

static void* list_max_new()
{
    t_list_max* x = reinterpret_cast<t_list_max*>(pd_new(list_max_class));
    outlet_new(&x->x_obj, &s_anything);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2emax()
{
    list_max_class = class_new(gensym("list.max"),
        reinterpret_cast<t_newmethod>(list_max_new),
        static_cast<t_method>(0),
        sizeof(t_list_max), 0, A_NULL);
    class_addlist(list_max_class, list_max_list);
}
