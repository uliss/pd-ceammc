#include "flt_c_lowshelf.h"

static void* faust_new(t_symbol* s, int argc, t_atom* argv)
{
    t_faust* x = reinterpret_cast<t_faust*>(pd_new(faust_class));

    const char* id = NULL;
    get_nth_symbol_arg(argc, argv, 1, &id);
    faust_new_internal(x, id);

    t_float param1 = 0;
    get_nth_float_arg(argc, argv, 1, &param1);
    pd_float((t_pd*)x, param1);

    t_float param2 = 0;
    get_nth_float_arg(argc, argv, 2, &param2);
    x->ui->setElementValue("param2", param2);

    return x;
}

extern "C" void setup_flt0x2ec_lowshelf_tilde()
{
    internal_setup(gensym("flt.c_lowshelf~"));
}

