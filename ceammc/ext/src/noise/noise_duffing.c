/*

————————————————————————————————————————————————————————————————
a-duffing - © andré sier 2004
	
inspired by Paul Bourke, implementation by André Sier	
		
Duffing Attractor
 Written by Paul Bourke
 March 1998

 The solution to the Duffing equations is often used as an example of a classic chaotic system. 
 The Duffing system of differential equations is

 dx / dt = y

 dy / dt = x - x3 - a y + b cos(w t)

 where typically, a = 0.25, b = 0.3, w = 1

 The following shows a "typical" segment of (x,y) values.

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _duffing {
    t_object c_ob;
    t_outlet *c_out1, *c_out2; // outlet
    double a, b, w, t, dt, nx, ny;
    double ainit, binit, winit, tinit, dtinit, nxinit, nyinit;

    bool om;
} duffing;

static void* duffing_new(t_symbol* msg, int argc, t_atom* argv);
static void duffing_calc(duffing* x);
static void duffing_bang(duffing* x);
static void duffing_set(duffing* x, t_symbol* msg, int argc, t_atom* argv);
static void duffing_reset(duffing* x);
static void duffing_a(duffing* x, t_float max);
static void duffing_b(duffing* x, t_float max);
static void duffing_w(duffing* x, t_float max);
static void duffing_dt(duffing* x, t_float max);
static void duffing_om(duffing* x, t_float max);

static t_eclass* duffing_class;

void* duffing_new(t_symbol* msg, int argc, t_atom* argv) //input the args
{
    duffing* x;

    x = (duffing*)eobj_new(duffing_class);

    x->c_out2 = floatout(x);
    x->c_out1 = floatout(x);

    //init
    x->a = 0.25;
    x->b = 0.3;
    x->w = 1.0;
    x->nx = 0;
    x->ny = 0;
    x->dt = 0.01;
    x->t = 0;
    x->ainit = 0.25;
    x->binit = 0.3;
    x->winit = 1.0;
    x->nxinit = 0;
    x->nyinit = 0;
    x->dtinit = 0.01;
    x->tinit = 0;
    x->om = 0;

    if (argc)
        duffing_set(x, msg, argc, argv); // & pass them

    return (x);
}

void duffing_set(duffing* x, t_symbol* msg, int argc, t_atom* argv) //input the args
{
    // super elegant xoaz set method
    if (argc) {
        if (argc > 5) {
            if (argv[5].a_type == A_FLOAT)
                x->nyinit = atom_getfloat(&argv[5]);

            x->ny = x->nyinit;
        }

        if (argc > 4) {
            if (argv[4].a_type == A_FLOAT)
                x->nxinit = atom_getfloat(&argv[4]);

            x->nx = x->nxinit;
        }

        if (argc > 3) {
            if (argv[3].a_type == A_FLOAT)
                x->dtinit = atom_getfloat(&argv[3]);

            x->dt = x->dtinit;
        }

        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->winit = atom_getfloat(&argv[2]);

            x->w = x->winit;
        }

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->binit = atom_getfloat(&argv[1]);

            x->b = x->binit;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->ainit = atom_getfloat(&argv[0]);

            x->a = x->ainit;
        }

    } //end if args
}

void duffing_reset(duffing* x)
{
    //init
    x->t = 0.0f;
    x->a = x->ainit;
    x->b = x->binit;
    x->w = x->winit;
    x->dt = x->dtinit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
}

void duffing_calc(duffing* x)
{
    double nx, ny;
    double a, b, w, t, dt;

    /**
     *  Duffing attractor
     * dx / dt = y
     * dy / dt = x - x3 - a y + b cos(w t)
     */
    a = x->a;
    b = x->b;
    w = x->w;
    t = x->t;
    dt = x->dt;
    nx = x->nx;
    ny = x->ny;

    t += dt;
    x->t = t;

    x->nx = ny;
    x->ny = nx - (nx * nx * nx) - (a * ny) + (b * cos(w * t));
}

void duffing_bang(duffing* x)
{
    outlet_float(x->c_out2, x->nx);
    outlet_float(x->c_out1, x->ny);

    duffing_calc(x);
}

void duffing_a(duffing* x, t_float max)
{
    x->a = max;
    //	x->ainit = max;
    if (x->om)
        duffing_bang(x);
}

void duffing_b(duffing* x, t_float max)
{
    x->b = max;
    //	x->binit = max;
    if (x->om)
        duffing_bang(x);
}

void duffing_w(duffing* x, t_float max)
{
    x->w = max;
    //	x->winit = max;
    if (x->om)
        duffing_bang(x);
}

void duffing_dt(duffing* x, t_float max)
{
    x->dt = max;
    //	x->dtinit = max;
    if (x->om)
        duffing_bang(x);
}

void duffing_om(duffing* x, t_float max)
{
    x->om = (max > 0);
}

void duffing_free(duffing* x)
{
    outlet_free(x->c_out1);
    outlet_free(x->c_out2);
}

void setup_noise0x2eduffing()
{

    duffing_class = eclass_new(("noise.duffing"),
        (t_typ_method)(duffing_new),
        (t_typ_method)(duffing_free),
        sizeof(duffing), 0, A_GIMME, 0);

    eclass_addmethod(duffing_class, (method)duffing_bang, "bang", A_NULL, 0);
    eclass_addmethod(duffing_class, (method)duffing_set, "set", A_GIMME, 0);
    eclass_addmethod(duffing_class, (method)duffing_reset, "reset", A_NULL, 0);
    eclass_addmethod(duffing_class, (method)duffing_a, "a", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_b, "b", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_w, "w", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_dt, "dt", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_om, "om", A_DEFFLOAT, 0);
}
