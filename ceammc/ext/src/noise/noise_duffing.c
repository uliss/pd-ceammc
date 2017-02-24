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
    void *c_out, *c_out2; // outlet
    double a, b, w, t, dt, nx, ny;
    double ainit, binit, winit, tinit, dtinit, nxinit, nyinit;

    bool om;
} duffing;

void* duffing_new(t_symbol* msg, short argc, t_atom* argv);
void duffing_calc(duffing* x);
void duffing_bang(duffing* x);
void duffing_set(duffing* x, t_symbol* msg, short argc, t_atom* argv);
void duffing_reset(duffing* x, t_symbol* msg, short argc, t_atom* argv);

void duffing_a(duffing* x, double max);
void duffing_b(duffing* x, double max);
void duffing_w(duffing* x, double max);
void duffing_dt(duffing* x, double max);
void duffing_om(duffing* x, long max);

//void duffing_assist(duffing *x, void *b, long m, long a, char *s);

t_eclass* duffing_class;

void* duffing_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    duffing* x;
    //int i;

    x = (duffing*)eobj_new(duffing_class);

    x->c_out2 = floatout(x);
    x->c_out = floatout(x);

    //init
    x->a = 0.25f;
    x->b = 0.3f;
    x->w = 1.0f;
    x->nx = 0.0f;
    x->ny = 0.0f;
    x->dt = 0.01f;
    x->t = 0.0f;
    x->ainit = 0.25f;
    x->binit = 0.3f;
    x->winit = 1.0f;
    x->nxinit = 0.0f;
    x->nyinit = 0.0f;
    x->dtinit = 0.01f;
    x->tinit = 0.0f;
    x->om = 0;
    if (argc)
        duffing_set(x, msg, argc, argv); // & pass them

    return (x);
}

void duffing_set(duffing* x, t_symbol* msg, short argc, t_atom* argv) //input the args
{
    // super elegant xoaz set method
    if (argc) {
        if (argc > 5) {
            if (argv[5].a_type == A_FLOAT)
                x->nyinit = (double)argv[5].a_w.w_float;
            else if (argv[5].a_type == A_FLOAT)
                x->nyinit = (double)argv[5].a_w.w_float;
            x->ny = x->nyinit;
        }

        if (argc > 4) {
            if (argv[4].a_type == A_FLOAT)
                x->nxinit = (double)argv[4].a_w.w_float;
            else if (argv[4].a_type == A_FLOAT)
                x->nyinit = (double)argv[4].a_w.w_float;
            x->nx = x->nxinit;
        }

        if (argc > 3) {
            if (argv[3].a_type == A_FLOAT)
                x->dtinit = (double)argv[3].a_w.w_float;
            else if (argv[3].a_type == A_FLOAT)
                x->dtinit = (double)argv[3].a_w.w_float;
            x->dt = x->dtinit;
        }

        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->winit = (double)argv[2].a_w.w_float;
            else if (argv[2].a_type == A_FLOAT)
                x->winit = (double)argv[2].a_w.w_float;
            x->w = x->winit;
        }

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->binit = (double)argv[1].a_w.w_float;
            else if (argv[1].a_type == A_FLOAT)
                x->binit = (double)argv[1].a_w.w_float;
            x->b = x->binit;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->ainit = (double)argv[0].a_w.w_float;
            else if (argv[0].a_type == A_FLOAT)
                x->ainit = (double)argv[0].a_w.w_float;
            x->a = x->ainit;
        }

    } //end if args
}

void duffing_reset(duffing* x, t_symbol* msg, short argc, t_atom* argv)
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

    /* 
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
    outlet_float(x->c_out, x->ny);

    duffing_calc(x);
}

void duffing_a(duffing* x, double max)
{
    x->a = max;
    //	x->ainit = max;
    if (x->om)
        duffing_bang(x);
}
void duffing_b(duffing* x, double max)
{
    x->b = max;
    //	x->binit = max;
    if (x->om)
        duffing_bang(x);
}
void duffing_w(duffing* x, double max)
{
    x->w = max;
    //	x->winit = max;
    if (x->om)
        duffing_bang(x);
}
void duffing_dt(duffing* x, double max)
{
    x->dt = max;
    //	x->dtinit = max;
    if (x->om)
        duffing_bang(x);
}
void duffing_om(duffing* x, long max) { x->om = (max > 0); }

void duffing_free() {}

void setup_noise0x2eduffing()
{

    duffing_class = eclass_new(("noise.duffing"),
        (t_typ_method)(duffing_new),
        (t_typ_method)(duffing_free),
        sizeof(duffing), 0, A_GIMME, 0);

    eclass_addmethod(duffing_class, (method)duffing_bang, "bang", A_GIMME, 0);
    eclass_addmethod(duffing_class, (method)duffing_set, "set", A_GIMME, 0);
    eclass_addmethod(duffing_class, (method)duffing_reset, "reset", A_GIMME, 0);
    eclass_addmethod(duffing_class, (method)duffing_a, "a", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_b, "b", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_w, "w", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_dt, "dt", A_FLOAT, 0);
    eclass_addmethod(duffing_class, (method)duffing_om, "om", A_DEFFLOAT, 0);
    
    post("noise.duffing: part of A-Chaos library, (C) 2004 André Sier");
}


