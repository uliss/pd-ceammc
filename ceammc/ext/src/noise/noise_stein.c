/*

————————————————————————————————————————————————————————————————
a - stein © andré sier 20030914
		
		distribuicao steina

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

#define pie 3.14159265358979323846

typedef struct
{
    t_object c_ob;
    void* c_out;

    double lambda, seed, seedinit;
    bool om;
} stein;

void* stein_new(t_symbol* msg, short argc, t_atom* argv);
void stein_bang(stein* x);
void stein_calc(stein* x);
void stein_set(stein* x, t_symbol* msg, short argc, t_atom* argv);
void stein_reset(stein* x, t_symbol* msg, short argc, t_atom* argv);

void stein_echo(stein* x, float echo);
void stein_int(stein* x, int n);
void stein_float(stein* x, float n);

void stein_lambda(stein* x, float echo);
void stein_seed(stein* x, float echo);

void stein_assist(stein* x, void* b, long m, long a, char* s);
static t_eclass* stein_class;

void* stein_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    stein* x;
    int i;

    x = (stein*)eobj_new(stein_class);

    x->c_out = floatout(x);

    x->seed = 0.777f;
    x->seedinit = 0.777f;
    x->lambda = 1.7f;
    x->om = 0;

    stein_set(x, msg, argc, argv);

    return (x);
}

void stein_set(stein* x, t_symbol* msg, short argc, t_atom* argv) //input the args
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

void stein_reset(stein* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->seed = x->seedinit;
}

void stein_calc(stein* x)
{
    double lambda, seed;

    lambda = x->lambda;
    seed = x->seed;

    x->seed = lambda * sin(pie * seed);
}

void stein_echo(stein* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        stein_bang(x);
}

//void stein_int(stein* x, int n) { stein_echo(x, n * 0.01); }
void stein_float(stein* x, float n) { stein_echo(x, n); }

void stein_om(stein* x, int n) { x->om = (n > 0); }

void stein_bang(stein* x)
{

    outlet_float(x->c_out, x->seed);

    stein_calc(x); //next
}

void stein_lambda(stein* x, float echo)
{
    x->lambda = echo;
    if (x->om)
        stein_bang(x);
}
void stein_seed(stein* x, float echo)
{
    x->seed = echo;
    x->seedinit = echo;
    if (x->om)
        stein_bang(x);
}

void stein_free() {}

void setup_noise0x2estein()
{
    stein_class = eclass_new(("noise.stein"),
        (t_typ_method)(stein_new),
        (t_typ_method)(stein_free),
        sizeof(stein), 0, A_GIMME, 0);

    eclass_addmethod(stein_class, (method)stein_bang, "bang", A_GIMME, 0);

    eclass_addmethod(stein_class, (method)stein_reset, "reset", A_GIMME, 0);
    eclass_addmethod(stein_class, (method)stein_set, "set", A_GIMME, 0);
    //     eclass_addmethod(stein_class, (method)stein_int, "int", A_GIMME, 0);
    eclass_addmethod(stein_class, (method)stein_float, "float", A_GIMME, 0);
    eclass_addmethod(stein_class, (method)stein_lambda, "lambda", A_DEFFLOAT, 0);
    eclass_addmethod(stein_class, (method)stein_seed, "seed", A_DEFFLOAT, 0);
    eclass_addmethod(stein_class, (method)stein_om, "om", A_DEFFLOAT, 0);
}


