/*

————————————————————————————————————————————————————————————————
a-baker - © andré sier 2004
	
a port of Richard's chaos-baker from chaos collection
————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version
#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _baker {
    t_eobj c_ob;
    t_outlet* c_out; // outlet
    double init, eval;
    bool fold_cut, om;
} baker;

static t_eclass* noise_baker_class;

static void* baker_new(t_symbol* msg, int argc, t_atom* argv);
static void baker_bang(baker* x);
static void baker_set(baker* x, t_symbol* msg, int argc, t_atom* argv);
static void baker_reset(baker* x);
static void baker_eval(baker* x, t_float max);
static void baker_fold(baker* x, t_float max);
static void baker_cut(baker* x, t_float max);
static void baker_fold_cut(baker* x, t_float max);

void* baker_new(t_symbol* msg, int argc, t_atom* argv) //input the args
{
    baker* x = (baker*)eobj_new(noise_baker_class);

    x->c_out = floatout(x);

    //init
    x->init = 0.85;
    x->eval = 0.85;
    x->fold_cut = 0;
    x->om = 0;

    baker_set(x, msg, argc, argv); // & pass them

    return (x);
}

void baker_free(baker* x)
{
    outlet_free(x->c_out);
}

void baker_set(baker* x, t_symbol* msg, int argc, t_atom* argv) //input the args
{
    if (argc) {
        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->init = fmod(argv[1].a_w.w_float, 1.);

            x->eval = x->init;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->init = fmod(argv[0].a_w.w_float, 1.);

            x->eval = x->init;
        }
    } //end if args
}

void baker_reset(baker* x)
{
    //init
    x->eval = x->init;
}

void baker_bang(baker* x)
{
    outlet_float(x->c_out, x->eval);

    if (!x->fold_cut) /* fold */
    {
        if (x->eval > 0.5)
            x->eval = 2. - (x->eval * 2.);
        else
            x->eval *= 2.;
    } else /* cut */
    {
        if (x->eval < 0.5)
            x->eval *= 2.;
        else
            x->eval = (x->eval * 2.) - 1.;
    }
}

void baker_eval(baker* x, t_float max)
{
    x->eval = max;
    x->init = max;
    if (x->om)
        baker_bang(x);
}

void baker_fold(baker* x, t_float max)
{
    x->fold_cut = 0;
    x->init = fmod(max, 1.);
    x->eval = x->init;
    if (x->om)
        baker_bang(x);
}

void baker_cut(baker* x, t_float max)
{
    x->fold_cut = 1;
    x->init = fmod(max, 1.);
    x->eval = x->init;
    if (x->om)
        baker_bang(x);
}

void baker_fold_cut(baker* x, t_float max)
{
    x->fold_cut = (max > 0);
    if (x->om)
        baker_bang(x);
}

void baker_om(baker* x, t_float max)
{
    x->om = (max > 0);
}

void setup_noise0x2ebaker()
{
    noise_baker_class = eclass_new(("noise.baker"),
        (t_typ_method)(baker_new),
        (t_typ_method)(baker_free),
        sizeof(baker), 0, A_GIMME, 0);

    eclass_addmethod(noise_baker_class, (method)baker_bang, "bang", A_NULL, 0);
    eclass_addmethod(noise_baker_class, (method)baker_set, "set", A_GIMME, 0);
    eclass_addmethod(noise_baker_class, (method)baker_reset, "reset", A_NULL, 0);
    eclass_addmethod(noise_baker_class, (method)baker_eval, "seed", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_fold, "fold", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_cut, "cut", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_fold_cut, "fold_cut", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_om, "om", A_FLOAT, 0);
}
