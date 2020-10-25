/*

————————————————————————————————————————————————————————————————
a - logistic © andré sier 20030914
		
		distribuicao logistica

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

typedef enum OutputMode_ {
    OUTPUT_NONE,
    OUTPUT_IMMIDIATLY
} OutputMode;

typedef struct
{
    t_object x_obj;
    t_outlet* c_out;

    double lambda, seed, seedinit;
    OutputMode om;
} logistic;

static t_class* logistic_class;

void* logistic_new(t_symbol* msg, int argc, t_atom* argv);
void logistic_bang(logistic* x);
void logistic_calc(logistic* x);
void logistic_set(logistic* x, t_symbol* msg, int argc, t_atom* argv);
void logistic_reset(logistic* x, t_symbol* msg, int argc, t_atom* argv);

void logistic_float(logistic* x, t_float n);

void logistic_lambda(logistic* x, t_float v);
void logistic_seed(logistic* x, t_float v);

void* logistic_new(t_symbol* msg, int argc, t_atom* argv) //input the args
{
    logistic* x = (logistic*)pd_new(logistic_class);

    x->c_out = outlet_new(&x->x_obj, &s_float);

    x->seed = 0.777f;
    x->seedinit = 0.777f;
    x->lambda = 3.9f;
    x->om = OUTPUT_NONE;

    logistic_set(x, msg, argc, argv);

    return x;
}

void logistic_set(logistic* x, t_symbol* msg, int argc, t_atom* argv) //input the args
{
    if (argc) {
        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->lambda = (double)argv[1].a_w.w_float;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->seedinit = (double)argv[0].a_w.w_float;

            x->seed = x->seedinit;
        }
    } //end if args
}

void logistic_reset(logistic* x, t_symbol* msg, int argc, t_atom* argv)
{
    x->seed = x->seedinit;
}

void logistic_calc(logistic* x)
{
    const double lambda = x->lambda;
    const double seed = x->seed;

    x->seed = lambda * seed * (1.0 - seed);
}

void logistic_float(logistic* x, t_float n)
{
    x->lambda = n;
    if (x->om == OUTPUT_IMMIDIATLY)
        logistic_bang(x);
}

void logistic_om(logistic* x, t_float n)
{
    x->om = (n > 0) ? OUTPUT_IMMIDIATLY : OUTPUT_NONE;
}

void logistic_bang(logistic* x)
{
    outlet_float(x->c_out, x->seed);

    logistic_calc(x); //next
}

void logistic_lambda(logistic* x, t_float v)
{
    x->lambda = v;
    if (x->om == OUTPUT_IMMIDIATLY)
        logistic_bang(x);
}
void logistic_seed(logistic* x, t_float v)
{
    x->seed = v;
    x->seedinit = v;
    if (x->om == OUTPUT_IMMIDIATLY)
        logistic_bang(x);
}

void logistic_free() { }

void setup_noise0x2elogistic()
{

    logistic_class = class_new(gensym("chaos.logistic"),
        (t_newmethod)(logistic_new),
        (t_method)(logistic_free),
        sizeof(logistic), 0, A_GIMME, 0);

    class_addcreator((t_newmethod)(logistic_new), gensym("noise.logistic"), A_GIMME, 0);

    class_addbang(logistic_class, (t_method)logistic_bang);
    class_doaddfloat(logistic_class, (t_method)logistic_float);

    class_addmethod(logistic_class, (t_method)logistic_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(logistic_class, (t_method)logistic_set, gensym("set"), A_GIMME, 0);

    class_addmethod(logistic_class, (t_method)logistic_lambda, gensym("lambda"), A_DEFFLOAT, 0);
    class_addmethod(logistic_class, (t_method)logistic_seed, gensym("seed"), A_DEFFLOAT, 0);
    class_addmethod(logistic_class, (t_method)logistic_om, gensym("om"), A_DEFFLOAT, 0);
}
