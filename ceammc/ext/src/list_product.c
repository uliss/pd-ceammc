#include <m_pd.h>
#include "ceammc.h"

t_class* list_product_class;
typedef struct list_product {
    t_object x_obj;
} t_list_product;

static void list_product_float(t_list_product* x, t_floatarg f)
{
    outlet_float(x->x_obj.te_outlet, f);
}

static t_float product(t_float f1, t_float f2) { return f1 * f2; }

static void list_product_list(t_list_product* x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_float(x->x_obj.te_outlet, ceammc_atoms_reduce_float(argc, argv, 1, &product));
}

static void* list_product_new()
{
    t_list_product* x = (t_list_product*)pd_new(list_product_class);
    outlet_new(&x->x_obj, &s_float);  
    return (void*)x;
}

void setup_list0x2eproduct()
{
    list_product_class = class_new(gensym("list.product"),
        (t_newmethod)list_product_new, (t_method)0,
            sizeof(t_list_product), 0, A_NULL);
    class_addfloat(list_product_class, list_product_float);
    class_addlist(list_product_class, list_product_list);
}

