/*

————————————————————————————————————————————————————————————————
a - verhulst © andré sier 20030914
		
		distribuicao verhulsta

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
} verhulst;

void* verhulst_new(t_symbol* msg, int argc, t_atom* argv);
void verhulst_bang(verhulst* x);
void verhulst_calc(verhulst* x);
void verhulst_set(verhulst* x, t_symbol* msg, int argc, t_atom* argv);
void verhulst_reset(verhulst* x, t_symbol* msg, int argc, t_atom* argv);

void verhulst_echo(verhulst* x, t_float echo);
void verhulst_float(verhulst* x, t_float n);

void verhulst_lambda(verhulst* x, t_float echo);
void verhulst_seed(verhulst* x, t_float echo);

static t_class* verhulst_class;

void* verhulst_new(t_symbol* msg, int argc, t_atom* argv) //input the args
{
    verhulst* x = (verhulst*)pd_new(verhulst_class);

    x->c_out = outlet_new(&x->x_obj, &s_float);

    x->seed = 0.5f;
    x->seedinit = 0.5f;
    x->lambda = 2.89f;
    x->om = 0;

    verhulst_set(x, msg, argc, argv);

    return (x);
}

void verhulst_set(verhulst* x, t_symbol* msg, int argc, t_atom* argv) //input the args
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

void verhulst_reset(verhulst* x, t_symbol* msg, int argc, t_atom* argv)
{
    x->seed = x->seedinit;
}

void verhulst_calc(verhulst* x)
{
    double lambda, seed;

    lambda = x->lambda;
    seed = x->seed;

    x->seed = seed + (seed * lambda * (1.0 - seed));
}

void verhulst_echo(verhulst* x, t_float echo)
{
    x->lambda = echo;
    if (x->om)
        verhulst_bang(x);
}

void verhulst_float(verhulst* x, t_float n) { verhulst_echo(x, n); }

void verhulst_om(verhulst* x, int n) { x->om = (n > 0); }

void verhulst_bang(verhulst* x)
{
    outlet_float(x->c_out, x->seed);
    verhulst_calc(x); //next
}

void verhulst_lambda(verhulst* x, t_float echo)
{
    x->lambda = echo;
    if (x->om)
        verhulst_bang(x);
}
void verhulst_seed(verhulst* x, t_float echo)
{
    x->seed = echo;
    x->seedinit = echo;
    if (x->om)
        verhulst_bang(x);
}

void verhulst_free() { }

void setup_noise0x2everhulst()
{
    verhulst_class = class_new(gensym("noise.verhulst"),
        (t_newmethod)(verhulst_new),
        (t_method)(verhulst_free),
        sizeof(verhulst), 0, A_GIMME, 0);

    class_addbang(verhulst_class, (t_method)verhulst_bang);

    class_addmethod(verhulst_class, (t_method)verhulst_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(verhulst_class, (t_method)verhulst_set, gensym("set"), A_GIMME, 0);
    class_doaddfloat(verhulst_class, (t_method)verhulst_float);
    class_addmethod(verhulst_class, (t_method)verhulst_lambda, gensym("lambda"), A_DEFFLOAT, 0);
    class_addmethod(verhulst_class, (t_method)verhulst_seed, gensym("seed"), A_DEFFLOAT, 0);
    class_addmethod(verhulst_class, (t_method)verhulst_om, gensym("om"), A_DEFFLOAT, 0);
}
