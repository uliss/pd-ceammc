#include "ceammc_atomlist.h"
#include <m_pd.h>

#define OBJ_NAME "list.first"
#define MSG_PREFIX "[" OBJ_NAME "] "

static t_class* list_first_class;
struct t_list_first {
    t_object x_obj;
};

using namespace ceammc;

static void list_first_list(t_list_first* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    Atom a(argv[0]);
    to_outlet(x->x_obj.te_outlet, a);
}

static void* list_first_new()
{
    t_list_first* x = reinterpret_cast<t_list_first*>(pd_new(list_first_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_list0x2efirst()
{
    list_first_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(list_first_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_list_first), 0, A_NULL);
    class_addlist(list_first_class, list_first_list);
}
