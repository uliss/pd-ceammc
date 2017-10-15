#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.nan"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_nan_class;
typedef struct math_nan {
    t_object x_obj;
} t_math_nan;

static void math_nan_bang(t_math_nan* x)
{
    outlet_float(x->x_obj.te_outlet, NAN);
}

static void* math_nan_new()
{
    t_math_nan* x = (t_math_nan*)pd_new(math_nan_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2enan()
{
    math_nan_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_nan_new, (t_method)0,
        sizeof(t_math_nan), 0, A_NULL);
    class_addbang(math_nan_class, math_nan_bang);
}

