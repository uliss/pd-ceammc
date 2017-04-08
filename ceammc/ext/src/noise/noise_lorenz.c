/*

————————————————————————————————————————————————————————————————
a - lorenz © andré sier 20030914
		
		lorenz chaos by heuns method
		extra accuracy
		
modified when i got to see the src! a big thank you to Richard Dudas and Mikhail Malt
for great work implementing non-linearity in max

sigma replaced with a
b replaced with c

————————————————————————————————————————————————————————————————
*/


// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

#define lorx(x, y, a) (a) * ((y) - (x));
#define lory(x, y, z, a) ((x) * ((r) - (z))) - (y);
#define lorz(x, y, z, c) ((x) * (y)) - ((z) * (c));

typedef struct
{
    t_object c_ob;
    void* c_out; // value
    void* c_out2; // value
    void* c_out3; // value

    double a, r, c, nx, ny, nz, dt; //eq variables
    double ainit, rinit, cinit, nxinit, nyinit, nzinit, dtinit; //init eq variables

    bool om;
} lorenz;

void* lorenz_new(t_symbol* msg, short argc, t_atom* argv);
void lorenz_bang(lorenz* x);

//user variables
void lorenz_a(lorenz* x, double echo);
void lorenz_r(lorenz* x, double echo);
void lorenz_c(lorenz* x, double echo);
void lorenz_nx(lorenz* x, double echo);
void lorenz_ny(lorenz* x, double echo);
void lorenz_nz(lorenz* x, double echo);
void lorenz_dt(lorenz* x, double echo);
void lorenz_param(lorenz* x, double a, double reynolds, double beta, double dt);

//int, double -> dt
void lorenz_echo(lorenz* x, double echo);
void lorenz_int(lorenz* x, int n);
void lorenz_float(lorenz* x, double n);

//calc
void lorenz_calc(lorenz* x);
void lorenz_reset(lorenz* x, t_symbol* msg, short argc, t_atom* argv);
void lorenz_set(lorenz* x, t_symbol* msg, short argc, t_atom* argv);

static t_eclass* lorenz_class;

void* lorenz_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    lorenz* x;
    //int i;

    x = (lorenz*)eobj_new(lorenz_class);

    x->c_out3 = floatout(x);
    x->c_out2 = floatout(x);
    x->c_out = floatout(x);

    //classic lorenz
    x->a = 10.0;
    x->r = 28.0;
    x->c = 2.67;
    x->nx = 0.1;
    x->ny = 0.1;
    x->nz = 0.1;
    x->dt = 0.01;

    x->ainit = 10.0;
    x->rinit = 28.0;
    x->cinit = 2.67;
    x->nxinit = 0.1;
    x->nyinit = 0.1;
    x->nzinit = 0.1;
    x->dtinit = 0.01;

    x->om = 0;

    lorenz_set(x, msg, argc, argv);

    return (x);
}

void lorenz_free() {}

void lorenz_om(lorenz* x, int n) { x->om = (n > 0); }

void lorenz_set(lorenz* x, t_symbol* msg, short ac, t_atom* av) //input the args
{
    if (ac) {

        if (ac > 6) {
            if (av[6].a_type == A_LONG)
                x->dtinit = (double)av[6].a_w.w_float;
            else if (av[6].a_type == A_FLOAT)
                x->dtinit = av[6].a_w.w_float;
            x->dt = x->dtinit;
        }
        if (ac > 5) {
            if (av[5].a_type == A_LONG)
                x->cinit = (double)av[5].a_w.w_float;
            else if (av[5].a_type == A_FLOAT)
                x->cinit = av[5].a_w.w_float;
            x->c = x->cinit;
        }
        if (ac > 4) {
            if (av[4].a_type == A_LONG)
                x->rinit = (double)av[4].a_w.w_float;
            else if (av[4].a_type == A_FLOAT)
                x->rinit = av[4].a_w.w_float;
            x->r = x->rinit;
        }
        if (ac > 3) {
            if (av[3].a_type == A_LONG)
                x->ainit = (double)av[3].a_w.w_float;
            else if (av[3].a_type == A_FLOAT)
                x->ainit = av[3].a_w.w_float;
            x->a = x->ainit;
        }
        if (ac > 2) {
            if (av[2].a_type == A_LONG)
                x->nzinit = (double)av[2].a_w.w_float;
            else if (av[2].a_type == A_FLOAT)
                x->nzinit = av[2].a_w.w_float;
            x->nz = x->nzinit;
        }
        if (ac > 1) {
            if (av[1].a_type == A_LONG)
                x->nyinit = (double)av[1].a_w.w_float;
            else if (av[1].a_type == A_FLOAT)
                x->nyinit = av[1].a_w.w_float;
            x->ny = x->nyinit;
        }
        if (ac > 0) {
            if (av[0].a_type == A_LONG)
                x->nxinit = (double)av[0].a_w.w_float;
            else if (av[0].a_type == A_FLOAT)
                x->nxinit = av[0].a_w.w_float;
            x->nx = x->nxinit;
        }

    } // end if args
}

void lorenz_reset(lorenz* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->nx = x->nxinit;
    x->ny = x->nyinit;
    x->nz = x->nzinit;
    x->a = x->ainit;
    x->r = x->rinit;
    x->c = x->cinit;
    x->dt = x->dtinit;
}

void lorenz_calc(lorenz* x)
{
    double a, r, c, dt, x0, y0, z0;
    double fx, fy, fz, ffx, ffy, ffz, dt2;
    double dtfx, dtfy, dtfz;

    //local copies
    a = x->a;
    r = x->r;
    c = x->c;
    dt = x->dt;
    dt2 = dt / 2.;
    x0 = x->nx;
    y0 = x->ny;
    z0 = x->nz;

    fx = lorx(x0, y0, a);
    fz = lorz(x0, y0, z0, c);
    fy = lory(x0, y0, z0, r);

    dtfx = x0 + (dt * fx);
    dtfy = y0 + (dt * fy);
    dtfz = z0 + (dt * fz);

    ffx = lorx(dtfx, dtfy, a);
    ffz = lorz(dtfx, dtfy, dtfz, c);
    ffy = lory(dtfx, dtfy, dtfz, r);

    x->nx += (dt2 * (fx + ffx));
    x->ny += (dt2 * (fy + ffy));
    x->nz += (dt2 * (fz + ffz));
}

void lorenz_echo(lorenz* x, double echo)
{
    x->dt = echo;
    // 	lorenz_bang(x); //prevent increase in speed if undesired
}

void lorenz_int(lorenz* x, int n) { lorenz_echo(x, n * 0.01); }
void lorenz_float(lorenz* x, double n) { lorenz_echo(x, n); }

void lorenz_a(lorenz* x, double echo)
{
    x->a = (double)echo;
    //	x->ainit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_r(lorenz* x, double echo)
{
    x->r = (double)echo;
    //	x->rinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_c(lorenz* x, double echo)
{
    x->c = (double)echo;
    //	x->cinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_nx(lorenz* x, double echo)
{
    x->nx = (double)echo;
    //	x->nxinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_ny(lorenz* x, double echo)
{
    x->ny = (double)echo;
    //	x->nyinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_nz(lorenz* x, double echo)
{
    x->nz = (double)echo;
    //	x->nzinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_dt(lorenz* x, double echo)
{
    x->dt = (double)echo;
    //	x->dtinit = (double)echo;
    if (x->om)
        lorenz_bang(x);
}

void lorenz_bang(lorenz* x)
{

    outlet_float(x->c_out3, x->nz);
    outlet_float(x->c_out2, x->ny);
    outlet_float(x->c_out, x->nx);

    lorenz_calc(x); //next
}

void setup_noise0x2elorenz()
{

    lorenz_class = eclass_new(("noise.lorenz"),
        (t_typ_method)(lorenz_new),
        (t_typ_method)(lorenz_free),
        sizeof(lorenz), 0, A_GIMME, 0);

    eclass_addmethod(lorenz_class, (method)lorenz_bang, "bang", A_GIMME, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_set, "set", A_GIMME, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_reset, "reset", A_GIMME, 0);
    //
    eclass_addmethod(lorenz_class, (method)lorenz_dt, "dt", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_a, "a", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_r, "r", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_c, "c", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_nx, "x", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_ny, "y", A_DEFFLOAT, 0);
    eclass_addmethod(lorenz_class, (method)lorenz_nz, "z", A_DEFFLOAT, 0);
    // addint((method)lorenz_int);
    // addfloat((method)lorenz_float);
    eclass_addmethod(lorenz_class, (method)lorenz_om, "om", A_DEFFLOAT, 0);
}

