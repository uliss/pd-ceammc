/*

————————————————————————————————————————————————————————————————
a - stein © andré sier 20030914
		
		distribuicao steina

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

#define pie 3.14159265358979323846

typedef struct
{
    t_object x_obj;
    t_outlet* c_out;

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
static t_class* stein_class;

void* stein_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    stein* x;
    int i;

    x = (stein*)pd_new(stein_class);

    x->c_out = outlet_new(&x->x_obj, &s_float);

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
    stein_class = class_new(gensym("noise.stein"),
        (t_newmethod)(stein_new),
        (t_method)(stein_free),
        sizeof(stein), 0, A_GIMME, 0);

    class_addbang(stein_class, (t_method)stein_bang);

    class_addmethod(stein_class, (t_method)stein_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(stein_class, (t_method)stein_set, gensym("set"), A_GIMME, 0);
    class_doaddfloat(stein_class, (t_method)stein_float);
    class_addmethod(stein_class, (t_method)stein_lambda, gensym("lambda"), A_DEFFLOAT, 0);
    class_addmethod(stein_class, (t_method)stein_seed, gensym("seed"), A_DEFFLOAT, 0);
    class_addmethod(stein_class, (t_method)stein_om, gensym("om"), A_DEFFLOAT, 0);
}


