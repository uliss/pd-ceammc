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

typedef struct
{
    t_object x_obj;
    t_outlet* c_out;

    double lambda, seed, seedinit;
    bool om;
} logistic;

void* logistic_new(t_symbol* msg, short argc, t_atom* argv);
void logistic_bang(logistic* x);
void logistic_calc(logistic* x);
void logistic_set(logistic* x, t_symbol* msg, short argc, t_atom* argv);
void logistic_reset(logistic* x, t_symbol* msg, short argc, t_atom* argv);

void logistic_echo(logistic* x, float echo);
void logistic_int(logistic* x, int n);
void logistic_float(logistic* x, float n);

void logistic_lambda(logistic* x, float echo);
void logistic_seed(logistic* x, float echo);

void logistic_assist(logistic* x, void* b, long m, long a, char* s);
static t_class* logistic_class;

void* logistic_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    logistic* x;
    int i;

    x = (logistic*)pd_new(logistic_class);

    x->c_out = outlet_new(&x->x_obj, &s_float);

    x->seed = 0.777f;
    x->seedinit = 0.777f;
    x->lambda = 3.9f;
    x->om = 0;

    logistic_set(x, msg, argc, argv);

    return (x);
}

void logistic_set(logistic* x, t_symbol* msg, short argc, t_atom* argv) //input the args
{

    if (argc) {

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->lambda = argv[1].a_w.w_float;
            if (argv[1].a_type == A_FLOAT)
                x->lambda = (double)argv[1].a_w.w_float;
        }
        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->seedinit = argv[0].a_w.w_float;
            if (argv[0].a_type == A_FLOAT)
                x->seedinit = (double)argv[0].a_w.w_float;

            x->seed = x->seedinit;
        }

    } //end if args
}

void logistic_reset(logistic* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->seed = x->seedinit;
}

void logistic_calc(logistic* x)
{
    double lambda, seed;

    lambda = x->lambda;
    seed = x->seed;

    x->seed = lambda * seed * (1.0 - seed);
}

void logistic_echo(logistic* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        logistic_bang(x);
}

void logistic_float(logistic* x, float n) { logistic_echo(x, n); }

void logistic_om(logistic* x, int n) { x->om = (n > 0); }

void logistic_bang(logistic* x)
{

    outlet_float(x->c_out, x->seed);

    logistic_calc(x); //next
}

void logistic_lambda(logistic* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        logistic_bang(x);
}
void logistic_seed(logistic* x, float echo)
{
    x->seed = echo;
    x->seedinit = echo;
    if (x->om)
        logistic_bang(x);
}

void logistic_free() {}

void setup_noise0x2elogistic()
{

    logistic_class = class_new(gensym("noise.logistic"),
        (t_newmethod)(logistic_new),
        (t_method)(logistic_free),
        sizeof(logistic), 0, A_GIMME, 0);

    class_addbang(logistic_class, (t_method)logistic_bang);

    class_addmethod(logistic_class, (t_method)logistic_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(logistic_class, (t_method)logistic_set, gensym("set"), A_GIMME, 0);
    class_addfloat(logistic_class, (t_method)logistic_float);
    class_addmethod(logistic_class, (t_method)logistic_lambda, gensym("lambda"), A_DEFFLOAT, 0);
    class_addmethod(logistic_class, (t_method)logistic_seed, gensym("seed"), A_DEFFLOAT, 0);
    class_addmethod(logistic_class, (t_method)logistic_om, gensym("om"), A_DEFFLOAT, 0);
}


