#include "MSPd.h"

#define OBJECT_NAME "impulse~"

static t_class *impulse_class;

typedef struct _impulse
{
    t_object x_obj;
    float x_f;
    
	int activated;
    
} t_impulse;

void *impulse_new(void);
t_int *impulse_perform(t_int *w);
void impulse_dsp(t_impulse *x, t_signal **sp);
void impulse_bang(t_impulse *x);

void impulse_bang(t_impulse *x) {
	x->activated = 1 ;
}


void impulse_tilde_setup(void)
{
    impulse_class = class_new(gensym("impulse~"), (t_newmethod)impulse_new,
                              NO_FREE_FUNCTION,sizeof(t_impulse), 0,0);
    CLASS_MAINSIGNALIN(impulse_class, t_impulse, x_f);
    class_addmethod(impulse_class, (t_method)impulse_dsp, gensym("dsp"), 0);
    class_addmethod(impulse_class, (t_method)impulse_bang, gensym("bang"),  0);
    potpourri_announce(OBJECT_NAME);
}


void *impulse_new(void)
{

    t_impulse *x = (t_impulse *)pd_new(impulse_class);
    outlet_new(&x->x_obj, gensym("signal"));
    x->activated = 0;
    
    return (x);
}

t_int *impulse_perform(t_int *w)
{
    t_impulse *x = (t_impulse *) (w[1]);
    t_float *out = (t_float *)(w[2]);
    t_int n = w[3];
    int activated = x->activated;
	while( n-- ) {
		if( activated ) {
			*out++ = 1.0;
			activated = 0;
		}
		else *out++ = 0.0;
	}
	x->activated = activated;
    return (w+4);
}

void impulse_dsp(t_impulse *x, t_signal **sp)
{
    dsp_add(impulse_perform, 3, x, sp[0]->s_vec,sp[0]->s_n);
}

