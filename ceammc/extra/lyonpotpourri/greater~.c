/* Required Header Files */

#include "MSPd.h"

/* The class pointer */

static t_class *greater_class;

/* The object structure */

typedef struct _greater {
	t_object obj;
	t_float x_f;
    float top;
} t_greater;

#define OBJECT_NAME "greater~"

/* Function prototypes */

void *greater_new(t_symbol *msg, short argc, t_atom *argv);
void greater_dsp(t_greater *x, t_signal **sp, short *count);
t_int *greater_perform(t_int *w);
void greater_top(t_greater *x, t_floatarg top);

/* The object setup function */

void greater_tilde_setup(void)
{
	greater_class = class_new(gensym("greater~"), (t_newmethod)greater_new, 0,sizeof(t_greater),0,A_GIMME,0);
	CLASS_MAINSIGNALIN(greater_class, t_greater, x_f);
	class_addmethod(greater_class, (t_method)greater_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(greater_class, (t_method)greater_top, gensym("top"), A_FLOAT, 0);
	potpourri_announce(OBJECT_NAME);
}



void greater_top(t_greater *x, t_floatarg top)
{
	x->top = top;
}

/* The new instance routine */

void *greater_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_greater *x = (t_greater *)pd_new(greater_class);
	outlet_new(&x->obj, gensym("signal"));
	x->top = atom_getfloatarg(0,argc,argv);
	return x;
}

/* The free memory function*/


/* The perform routine */

t_int *greater_perform(t_int *w)
{
	t_greater *x = (t_greater *) (w[1]);
	t_float *input = (t_float *) (w[2]);
	t_float *output = (t_float *) (w[3]);
	t_int n = w[4];
    t_float top = x->top;
	int i;
	
	for(i=0; i < n; i++){
        output[i] = input[i] > top ? 1 : 0;
	}
	return w + 5;
}

void greater_dsp(t_greater *x, t_signal **sp, short *count)
{
	dsp_add(greater_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}