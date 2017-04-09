/*

————————————————————————————————————————————————————————————————
a - verhulst © andré sier 20030914
		
		distribuicao verhulsta

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct
{
    t_object c_ob;
    void* c_out;

    double lambda, seed, seedinit;
    bool om;
} verhulst;

void* verhulst_new(t_symbol* msg, short argc, t_atom* argv);
void verhulst_bang(verhulst* x);
void verhulst_calc(verhulst* x);
void verhulst_set(verhulst* x, t_symbol* msg, short argc, t_atom* argv);
void verhulst_reset(verhulst* x, t_symbol* msg, short argc, t_atom* argv);

void verhulst_echo(verhulst* x, float echo);
void verhulst_int(verhulst* x, int n);
void verhulst_float(verhulst* x, float n);

void verhulst_lambda(verhulst* x, float echo);
void verhulst_seed(verhulst* x, float echo);

void verhulst_assist(verhulst* x, void* b, long m, long a, char* s);
static t_eclass* verhulst_class;

void* verhulst_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    verhulst* x;
    int i;

    x = (verhulst*)eobj_new(verhulst_class);

    x->c_out = floatout(x);

    x->seed = 0.5f;
    x->seedinit = 0.5f;
    x->lambda = 2.89f;
    x->om = 0;

    verhulst_set(x, msg, argc, argv);

    return (x);
}

void verhulst_set(verhulst* x, t_symbol* msg, short argc, t_atom* argv) //input the args
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

void verhulst_reset(verhulst* x, t_symbol* msg, short argc, t_atom* argv)
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

void verhulst_echo(verhulst* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        verhulst_bang(x);
}

void verhulst_int(verhulst* x, int n) { verhulst_echo(x, n * 0.01); }
void verhulst_float(verhulst* x, float n) { verhulst_echo(x, n); }

void verhulst_om(verhulst* x, int n) { x->om = (n > 0); }

void verhulst_bang(verhulst* x)
{

    outlet_float(x->c_out, x->seed);

    verhulst_calc(x); //next
}

void verhulst_lambda(verhulst* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        verhulst_bang(x);
}
void verhulst_seed(verhulst* x, float echo)
{
    x->seed = echo;
    x->seedinit = echo;
    if (x->om)
        verhulst_bang(x);
}

void verhulst_free() {}

void setup_noise0x2everhulst()
{
    verhulst_class = eclass_new(("noise.verhulst"),
        (t_typ_method)(verhulst_new),
        (t_typ_method)(verhulst_free),
        sizeof(verhulst), 0, A_GIMME, 0);

    eclass_addmethod(verhulst_class, (method)verhulst_bang, "bang", A_GIMME, 0);

    eclass_addmethod(verhulst_class, (method)verhulst_reset, "reset", A_GIMME, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_set, "set", A_GIMME, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_int, "int", A_GIMME, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_float, "float", A_GIMME, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_lambda, "lambda", A_DEFFLOAT, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_seed, "seed", A_DEFFLOAT, 0);
    eclass_addmethod(verhulst_class, (method)verhulst_om, "om", A_DEFFLOAT, 0);
}


