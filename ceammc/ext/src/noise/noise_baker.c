/*

————————————————————————————————————————————————————————————————
a-baker - © andré sier 2004
	
a port of Richard's chaos-baker from chaos collection
————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

//#include "ext.h"
//#include "ext_common.h"

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _baker {
    t_eobj c_ob;
    t_outlet* c_out; // outlet
    double init, eval;
    bool fold_cut, om;
} baker;

t_eclass* noise_baker_class;

void* baker_new(t_symbol* msg, short argc, t_atom* argv);
void baker_calc(baker* x);
void baker_bang(baker* x);
void baker_set(baker* x, t_symbol* msg, short argc, t_atom* argv);
void baker_reset(baker* x, t_symbol* msg, short argc, t_atom* argv);

void baker_init(baker* x, double max);
void baker_eval(baker* x, double max);
void baker_fold(baker* x, double max);
void baker_cut(baker* x, double max);
void baker_fold_cut(baker* x, int max);

void baker_assist(baker* x, void* b, long m, long a, char* s);

void* baker_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    baker* x;

    x = (baker*)eobj_new(noise_baker_class);

    x->c_out = floatout(x);

    //init
    x->init = 0.85f;
    x->eval = 0.85f;
    x->fold_cut = 0;
    x->om = 0;

    baker_set(x, msg, argc, argv); // & pass them

    return (x);
}

void baker_free()
{
}

void baker_set(baker* x, t_symbol* msg, short argc, t_atom* argv) //input the args
{

    if (argc) {

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->init = fmod(argv[1].a_w.w_float, 1.);
            if (argv[1].a_type == A_LONG)
                x->fold_cut = argv[1].a_w.w_float;

            x->eval = x->init;
        }
        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->init = fmod(argv[0].a_w.w_float, 1.);
            if (argv[0].a_type == A_LONG)
                x->fold_cut = argv[0].a_w.w_float;

            x->eval = x->init;
        }

        /*  if init is _not_ init of eval
		if (argc==3) {
		
			if (argv[2].a_type == A_LONG)
				x->fold_cut = argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->fold_cut = (long) argv[2].a_w.w_float;


			if (argv[1].a_type == A_LONG)
				x->fold_cut = argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->eval = (double) fmod(argv[1].a_w.w_float, 1.);

			if (argv[0].a_type == A_LONG)
				x->fold_cut = argv[0].a_w.w_long;	//fold_cut
			else if (argv[0].a_type == A_FLOAT) 
				x->init = (double) fmod(argv[0].a_w.w_float, 1.); //init

		}

		if (argc==2) {

			if (argv[1].a_type == A_LONG)
				x->fold_cut = argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->eval = (double) fmod(argv[1].a_w.w_float, 1.);

			if (argv[0].a_type == A_LONG)
				x->fold_cut = argv[0].a_w.w_long;	//fold_cut
			else if (argv[0].a_type == A_FLOAT) 
				x->init = (double) fmod(argv[0].a_w.w_float, 1.); //init

		}

		if (argc==1) {
		
			if (argv[0].a_type == A_LONG)
				x->fold_cut = argv[0].a_w.w_long;	//fold_cut
			else if (argv[0].a_type == A_FLOAT) {
				x->init = (double) fmod(argv[0].a_w.w_float, 1.); //init
				x->eval = x->init; // only if 1 arg is float eval equals init
				}
		}

*/

    } //end if args
}

void baker_reset(baker* x, t_symbol* msg, short argc, t_atom* argv)
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

void baker_eval(baker* x, double max)
{
    x->eval = max;
    x->init = max;
    if (x->om)
        baker_bang(x);
}
void baker_fold(baker* x, double max)
{
    x->fold_cut = 0;
    x->init = fmod(max, 1.);
    x->eval = x->init;
    if (x->om)
        baker_bang(x);
}
void baker_cut(baker* x, double max)
{
    x->fold_cut = 1;
    x->init = fmod(max, 1.);
    x->eval = x->init;
    if (x->om)
        baker_bang(x);
}
void baker_fold_cut(baker* x, int max)
{
    x->fold_cut = (max > 0);
    if (x->om)
        baker_bang(x);
}
void baker_om(baker* x, int max)
{
    x->om = (max > 0);
}

void setup_noise0x2ebaker()
{

    noise_baker_class = eclass_new(("noise.baker"),
        (t_typ_method)(baker_new),
        (t_typ_method)(baker_free),
        sizeof(baker), 0, A_GIMME, 0);

    eclass_addmethod(noise_baker_class, (method)baker_bang, "bang", A_GIMME, 0);
    eclass_addmethod(noise_baker_class, (method)baker_set, "set", A_GIMME, 0);
    eclass_addmethod(noise_baker_class, (method)baker_reset, "reset", A_GIMME, 0);
    eclass_addmethod(noise_baker_class, (method)baker_eval, "seed", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_fold, "fold", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_cut, "cut", A_FLOAT, 0);
    eclass_addmethod(noise_baker_class, (method)baker_fold_cut, "fold_cut", A_LONG, 0);
    eclass_addmethod(noise_baker_class, (method)baker_om, "om", A_LONG, 0);
    //    post("A-Chaos Lib :: a-baker  " __DATE__ " " __TIME__ "                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved", tick, 0);
    post("noise.baker: part of A-Chaos library, (C) 2004 André Sier");
}
