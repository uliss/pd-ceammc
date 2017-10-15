#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.e"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_e_class;
typedef struct math_e {
    t_object x_obj;
} t_math_e;

static void math_e_bang(t_math_e* x)
{
    outlet_float(x->x_obj.te_outlet, M_E);
}

static void* math_e_new()
{
    t_math_e* x = (t_math_e*)pd_new(math_e_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2ee()
{
    math_e_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_e_new, (t_method)0,
        sizeof(t_math_e), 0, A_NULL);
    class_addbang(math_e_class, math_e_bang);
}

