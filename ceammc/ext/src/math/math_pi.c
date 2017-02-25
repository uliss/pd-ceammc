#include "ceammc.h"
#include <m_pd.h>
#include <math.h>

#define OBJ_NAME "math.pi"
#define MSG_PREFIX "[" OBJ_NAME "]"

static t_class* math_pi_class;
typedef struct math_pi {
    t_object x_obj;
} t_math_pi;

static void math_pi_bang(t_math_pi* x)
{
    outlet_float(x->x_obj.te_outlet, M_PI);
}

static void* math_pi_new()
{
    t_math_pi* x = (t_math_pi*)pd_new(math_pi_class);
    outlet_new(&x->x_obj, &s_float);
    
    return (void*)x;
}

void setup_math0x2epi()
{
    math_pi_class = class_new(gensym(OBJ_NAME),
        (t_newmethod)math_pi_new, (t_method)0,
        sizeof(t_math_pi), 0, A_NULL);
    class_addbang(math_pi_class, math_pi_bang);
}

