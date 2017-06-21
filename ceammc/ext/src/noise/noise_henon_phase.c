/*

————————————————————————————————————————————————————————————————
a-henonphase - © andré sier 2004


Henon Phase Diagrams
 Written by Paul Bourke
 January 1991
 xn+1 = xn cos(a) - (yn - xn2) sin(a) 
 yn+1 = xn sin(a) + (yn - xn2) cos(a)
————————————————————————————————————————————————————————————————
*/

//#include "ext.h"
//#include "ext_common.h"
//
//#include <math.h>

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

typedef struct _henonphase {
    t_object x_obj;
    t_outlet *c_out, *c_out2;
    double a, b, nx, ny;
    double ainit, binit, nxinit, nyinit;
    double px, py;
    double dt, dtinit, t;

    bool om;
} henonphase;

void* henonphase_new(t_symbol* msg, short argc, t_atom* argv);
void henonphase_calc(henonphase* x);
void henonphase_bang(henonphase* x);
void henonphase_set(henonphase* x, t_symbol* msg, short argc, t_atom* argv);
void henonphase_reset(henonphase* x, t_symbol* msg, short argc, t_atom* argv);

void henonphase_a(henonphase* x, double max);
void henonphase_b(henonphase* x, double max);
void henonphase_om(henonphase* x, long max);

void henonphase_assist(henonphase* x, void* b, long m, long a, char* s);
static t_class* henonphase_class;

void* henonphase_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    henonphase* x;
    //int i;

    x = (henonphase*)pd_new(henonphase_class);

    x->c_out2 = outlet_new(&x->x_obj, &s_float);
    x->c_out = outlet_new(&x->x_obj, &s_float);

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
        henonphase_set(x, msg, argc, argv);

    return (x);
}

void henonphase_set(henonphase* x, t_symbol* s, short ac, t_atom* av) //input the args
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

void henonphase_reset(henonphase* x, t_symbol* s, short ac, t_atom* av) //input the args
{
    x->a = x->ainit;
    x->b = x->binit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
    x->dt = x->dtinit;
}

void henonphase_calc(henonphase* x)
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

    /*

 xn+1 = xn cos(a) - (yn - xn2) sin(a) 
 yn+1 = xn sin(a) + (yn - xn2) cos(a)

	
*/

    x->nx = nx * cos(a) - (ny - nx * nx) * sin(a);
    x->ny = nx * sin(a) - (ny - nx * nx) * cos(a);

    /*

    //henon calc
	lx1 = (ny + 1) - (a * (nx*nx));
	ly1 = b * nx;

    //advance 'a' by dt _AFTER_ calculating henon
	x->t += dt;
	a *= x->t;

    //henon phase
	px = nx*cos(a) - (ny -(nx*nx))*sin(a);
	py = nx*sin(a) - (ny -(nx*nx))*cos(a);


    // henonphase attractor

	lx1 = nx*cos (a) - (ny - (nx*nx))*sin(a);
	ly1 = nx*sin(a) - (ny - (nx*nx))*cos(a);

	x->nx = lx1;
	x->ny = ly1;

    */

    x->px = px;
    x->py = py;
}

void henonphase_bang(henonphase* x)
{

    outlet_float(x->c_out2, x->ny);
    outlet_float(x->c_out, x->nx);

    henonphase_calc(x);
}

void henonphase_a(henonphase* x, double max)
{
    x->a = max;
    if (x->om)
        henonphase_bang(x);
}
void henonphase_b(henonphase* x, double max)
{
    x->b = max;
    if (x->om)
        henonphase_bang(x);
}
void henonphase_nx(henonphase* x, double max)
{
    x->nx = max;
    if (x->om)
        henonphase_bang(x);
}
void henonphase_ny(henonphase* x, double max)
{
    x->ny = max;
    if (x->om)
        henonphase_bang(x);
}
void henonphase_dt(henonphase* x, double max)
{
    x->dt = max;
    if (x->om)
        henonphase_bang(x);
}

void henonphase_om(henonphase* x, long max) { x->om = (max > 0); }

void henonphase_free() {}

void setup_noise0x2ehenon_phase()
{

    henonphase_class = class_new(gensym("noise.henon_phase"),
        (t_newmethod)(henonphase_new),
        (t_method)(henonphase_free),
        sizeof(henonphase), 0, A_GIMME, 0);

    class_addbang(henonphase_class, (t_method)henonphase_bang);
    class_addmethod(henonphase_class, (t_method)henonphase_set, gensym("set"), A_GIMME, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_a, gensym("a"), A_FLOAT, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_b, gensym("b"), A_FLOAT, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_nx, gensym("x"), A_FLOAT, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_ny, gensym("y"), A_FLOAT, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_dt, gensym("dt"), A_FLOAT, 0);
    class_addmethod(henonphase_class, (t_method)henonphase_om, gensym("om"), A_FLOAT, 0);
}
