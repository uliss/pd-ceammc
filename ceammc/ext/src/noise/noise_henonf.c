/*

————————————————————————————————————————————————————————————————
a-henonf - © andré sier 2004


basically, i dont know if this is the henon phase diagrams... i just
followed what seemed the proper way. this was inspired by paul bourke's
henon phase, i had to make the obj calc henon and calc the previous result's
phase, advancing it by an amount related to 'a' -- all in the calc method

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _henonf {
    t_object c_ob;
    void *c_out, *c_out2;
    double a, b, nx, ny;
    double ainit, binit, nxinit, nyinit;
    double px, py;
    double dt, dtinit, t;

    bool om;
} henonf;

void* henonf_new(t_symbol* msg, short argc, t_atom* argv);
void henonf_calc(henonf* x);
void henonf_bang(henonf* x);
void henonf_set(henonf* x, t_symbol* msg, short argc, t_atom* argv);
void henonf_reset(henonf* x, t_symbol* msg, short argc, t_atom* argv);

void henonf_a(henonf* x, double max);
void henonf_b(henonf* x, double max);
void henonf_om(henonf* x, long max);

void henonf_assist(henonf* x, void* b, long m, long a, char* s);
static t_eclass* henonf_class;

void* henonf_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    henonf* x;
    int i;

    x = (henonf*)eobj_new(henonf_class);

    x->c_out2 = floatout(x);
    x->c_out = floatout(x);

    //init
    x->a = 1.4f;
    x->b = 0.3f;
    x->nx = 1.0f;
    x->ny = 1.0f;
    x->dt = 0.01f;

    x->ainit = 1.4f;
    x->binit = 0.3f;
    x->nxinit = 1.0f;
    x->nyinit = 1.0f;
    x->dtinit = 0.01f;

    x->t = 0.;

    if (argc)
        henonf_set(x, msg, argc, argv);

    return (x);
}

void henonf_set(henonf* x, t_symbol* s, short ac, t_atom* av) //input the args
{

    if (ac) {

        if (ac > 4) {
            if (av[4].a_type == A_FLOAT)
                x->dtinit = (double)av[4].a_w.w_float;
            else if (av[3].a_type == A_FLOAT)
                x->dtinit = av[4].a_w.w_float;
            x->dt = x->dtinit;
        }

        if (ac > 3) {
            if (av[3].a_type == A_FLOAT)
                x->binit = (double)av[3].a_w.w_float;
            else if (av[3].a_type == A_FLOAT)
                x->binit = av[3].a_w.w_float;
            x->b = x->binit;
        }
        if (ac > 2) {
            if (av[2].a_type == A_FLOAT)
                x->ainit = (double)av[2].a_w.w_float;
            else if (av[2].a_type == A_FLOAT)
                x->ainit = av[2].a_w.w_float;
            x->a = x->ainit;
        }
        if (ac > 1) {
            if (av[1].a_type == A_FLOAT)
                x->nyinit = (double)av[1].a_w.w_float;
            else if (av[1].a_type == A_FLOAT)
                x->nyinit = av[1].a_w.w_float;
            x->ny = x->nyinit;
        }
        if (ac > 0) {
            if (av[0].a_type == A_FLOAT)
                x->nxinit = (double)av[0].a_w.w_float;
            else if (av[0].a_type == A_FLOAT)
                x->nxinit = av[0].a_w.w_float;
            x->nx = x->nxinit;
        }

    } //end if args
}

void henonf_reset(henonf* x, t_symbol* s, short ac, t_atom* av) //input the args
{
    x->a = x->ainit;
    x->b = x->binit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
    x->dt = x->dtinit;
}

void henonf_calc(henonf* x)
{
    double nx, ny, lx1, ly1;
    double a, b;
    double px, py;
    double dt;

    a = x->a;
    b = x->b;
    nx = x->nx;
    ny = x->ny;
    px = x->px;
    py = x->py;
    dt = x->dt;

    //henon calc
    lx1 = (ny + 1) - (a * (nx * nx));
    ly1 = b * nx;

    //advance 'a' by dt _AFTER_ calculating henon
    x->t += dt;
    a *= x->t;

    //henon phase
    px = nx * cos(a) - (ny - (nx * nx)) * sin(a);
    py = nx * sin(a) - (ny - (nx * nx)) * cos(a);

    /*
	// henonf attractor

	lx1 = nx*cos (a) - (ny - (nx*nx))*sin(a);
	ly1 = nx*sin(a) - (ny - (nx*nx))*cos(a);
*/

    x->nx = lx1;
    x->ny = ly1;

    x->px = px;
    x->py = py;
}

void henonf_bang(henonf* x)
{

    outlet_float(x->c_out2, x->py);
    outlet_float(x->c_out, x->px);

    henonf_calc(x);
}

void henonf_a(henonf* x, double max)
{
    x->a = max;
    if (x->om)
        henonf_bang(x);
}
void henonf_b(henonf* x, double max)
{
    x->b = max;
    if (x->om)
        henonf_bang(x);
}
void henonf_nx(henonf* x, double max)
{
    x->nx = max;
    if (x->om)
        henonf_bang(x);
}
void henonf_ny(henonf* x, double max)
{
    x->ny = max;
    if (x->om)
        henonf_bang(x);
}
void henonf_dt(henonf* x, double max)
{
    x->dt = max;
    if (x->om)
        henonf_bang(x);
}

void henonf_om(henonf* x, long max) { x->om = (max > 0); }

void henonf_free() {}

void setup_noise0x2ehenonf()
{

    henonf_class = eclass_new(("noise.henonf"),
        (t_typ_method)(henonf_new),
        (t_typ_method)(henonf_free),
        sizeof(henonf), 0, A_GIMME, 0);

    eclass_addmethod(henonf_class, (method)henonf_bang, "bang", A_GIMME, 0);
    eclass_addmethod(henonf_class, (method)henonf_set, "set", A_GIMME, 0);
    eclass_addmethod(henonf_class, (method)henonf_reset, "reset", A_GIMME, 0);
    eclass_addmethod(henonf_class, (method)henonf_a, "a", A_FLOAT, 0);
    eclass_addmethod(henonf_class, (method)henonf_b, "b", A_FLOAT, 0);
    eclass_addmethod(henonf_class, (method)henonf_nx, "x", A_FLOAT, 0);
    eclass_addmethod(henonf_class, (method)henonf_ny, "y", A_FLOAT, 0);
    eclass_addmethod(henonf_class, (method)henonf_ny, "dt", A_FLOAT, 0);
    eclass_addmethod(henonf_class, (method)henonf_om, "om", A_FLOAT, 0);
}


