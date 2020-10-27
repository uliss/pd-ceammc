/*

————————————————————————————————————————————————————————————————
a-ginger - © andré sier 2004
		

A-Chaos Lib

2d output

	--- chaos-ginger.a	
		(a-ginger)

		--inputs
			--  (1 == z/seed) defaults (0.82) // x, y default to (0.7,1.2)
		--outputs
			--  (2d)

		--formulae
			--  dx/dt = 1. -y - z*abs(x)
			--  dy/dt = x


————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>

static t_class* ginger_class;

typedef struct _ginger {
    t_object x_obj;
    t_outlet *c_out_y, *c_out_x; // 2 outlets
    double seed, nx, ny;
    double seedinit, nxinit, nyinit;
} ginger;

void* ginger_new(t_symbol* s, int ac, t_atom* av);
void ginger_calc(ginger* x);
void ginger_bang(ginger* x);
void ginger_set(ginger* x, t_symbol* s, int ac, t_atom* av);
void ginger_reset(ginger* x);

void ginger_seed(ginger* x, t_float max);
void ginger_nx(ginger* x, t_float max);
void ginger_ny(ginger* x, t_float max);
void ginger_om(ginger* x, long max);

void* ginger_new(t_symbol* s, int ac, t_atom* av) //input the args
{
    ginger* x;

    x = (ginger*)pd_new(ginger_class);

    x->c_out_x = outlet_new(&x->x_obj, &s_float);
    x->c_out_y = outlet_new(&x->x_obj, &s_float);

    x->seed = 0.82f;
    x->nx = 0.7f;
    x->ny = 1.2f;
    x->seedinit = 0.82f;
    x->nxinit = 0.7f;
    x->nyinit = 1.2f;

    ginger_set(x, s, ac, av);

    return x;
}

void ginger_set(ginger* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc) {
        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->nyinit = (double)argv[2].a_w.w_float;
            else if (argv[2].a_type == A_FLOAT)
                x->nyinit = (double)argv[2].a_w.w_float;
            x->ny = x->nyinit;
        }

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->nxinit = (double)argv[1].a_w.w_float;
            else if (argv[1].a_type == A_FLOAT)
                x->nxinit = (double)argv[1].a_w.w_float;
            x->nx = x->nxinit;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->seedinit = (double)argv[0].a_w.w_float;
            else if (argv[0].a_type == A_FLOAT)
                x->seedinit = (double)argv[0].a_w.w_float;
            x->seed = x->seedinit;
        }
    }
}

void ginger_reset(ginger* x)
{
    x->seed = x->seedinit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
}

void ginger_calc(ginger* x)
{
    double nx, ny, x1, y1;

    const double seed = x->seed;
    nx = x->nx;
    ny = x->ny;
    // ginger formulae

    y1 = nx;

    if (nx < 0.)
        nx = -nx;
    x1 = 1. - ny - seed * nx;

    x->nx = x1;
    x->ny = y1;
}

void ginger_bang(ginger* x)
{
    outlet_float(x->c_out_x, x->nx);
    outlet_float(x->c_out_y, x->ny);

    ginger_calc(x);
}

void ginger_seed(ginger* x, t_float max)
{
    x->seed = max;
    x->seedinit = max;
}

void ginger_nx(ginger* x, t_float max)
{
    x->nx = max;
    x->nxinit = max;
}

void ginger_ny(ginger* x, t_float max)
{
    x->ny = max;
    x->nyinit = max;
}

void ginger_free() { }

void setup_noise0x2eginger()
{
    ginger_class = class_new(gensym("noise.ginger"),
        (t_newmethod)(ginger_new),
        (t_method)(ginger_free),
        sizeof(ginger), 0, A_GIMME, 0);

    class_addbang(ginger_class, (t_method)ginger_bang);
    class_addmethod(ginger_class, (t_method)ginger_set, gensym("set"), A_GIMME, 0);
    class_addmethod(ginger_class, (t_method)ginger_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(ginger_class, (t_method)ginger_nx, gensym("x"), A_FLOAT, 0);
    class_addmethod(ginger_class, (t_method)ginger_ny, gensym("y"), A_FLOAT, 0);
    class_addmethod(ginger_class, (t_method)ginger_seed, gensym("seed"), A_FLOAT, 0);
}
