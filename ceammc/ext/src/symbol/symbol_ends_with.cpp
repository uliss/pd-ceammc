#include "ceammc.h"
#include <boost/algorithm/string/predicate.hpp>
#include <m_pd.h>
#include <string>

static t_class* symbol_ends_with_class;
struct t_symbol_ends_with {
    t_object x_obj;
    t_symbol* suffix;
    t_inlet* in_suffix;
    t_outlet* out_passed;
};

static void symbol_ends_with_symbol(t_symbol_ends_with* x, t_symbol* s)
{
    bool res = boost::algorithm::ends_with(s->s_name, x->suffix->s_name);
    outlet_float(x->x_obj.te_outlet, res ? 1 : 0);

    if (res)
        outlet_symbol(x->out_passed, s);
}

static void* symbol_ends_with_new(t_symbol* s)
{
    t_symbol_ends_with* x = reinterpret_cast<t_symbol_ends_with*>(pd_new(symbol_ends_with_class));
    outlet_new(&x->x_obj, &s_float);
    x->suffix = s;
    x->in_suffix = symbolinlet_new(&x->x_obj, &x->suffix);
    x->out_passed = outlet_new(&x->x_obj, &s_symbol);
    return static_cast<void*>(x);
}

static void symbol_ends_with_free(t_symbol_ends_with* x)
{
    outlet_free(x->out_passed);
    inlet_free(x->in_suffix);
}

extern "C" void setup_symbol0x2eends_with()
{
    symbol_ends_with_class = class_new(gensym("symbol.ends_with"),
        reinterpret_cast<t_newmethod>(symbol_ends_with_new),
        reinterpret_cast<t_method>(symbol_ends_with_free),
        sizeof(t_symbol_ends_with), 0, A_DEFSYMBOL, 0);
    class_addsymbol(symbol_ends_with_class, symbol_ends_with_symbol);
}
