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

#include "ext.h"
#include "ext_common.h"

#include <math.h>


typedef struct	_henonphase
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;			
	double a, b, nx, ny;
	double ainit, binit, nxinit, nyinit;
	double px, py;
	double dt, dtinit, t;

	Boolean om;
} henonphase;



void *henonphase_new (Symbol *msg, short argc, Atom *argv);
void henonphase_calc (henonphase *x); 
void henonphase_bang (henonphase *x); 
void henonphase_set (henonphase *x, Symbol *msg, short argc, Atom *argv); 
void henonphase_reset (henonphase *x, Symbol *msg, short argc, Atom *argv); 

void henonphase_a (henonphase *x, double max);
void henonphase_b (henonphase *x, double max);
void henonphase_om (henonphase *x, long max);

void henonphase_assist(henonphase *x, void *b, long m, long a, char *s);
void *henonphase_class;




void *henonphase_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
 henonphase *x;
 int i;
 
 x=(henonphase *)newobject(henonphase_class);

x->c_out2=floatout(x);
 x->c_out=floatout(x);

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
	 
 return(x);	
}

void henonphase_set (henonphase *x, Symbol *s, short ac, Atom *av) //input the args 
{

	if (ac) {

		if (ac > 4) {
			if (av[4].a_type == A_LONG)
				x->dtinit = (double)av[4].a_w.w_long;
			else if (av[3].a_type == A_FLOAT)
				x->dtinit = av[4].a_w.w_float;
			x->dt = x->dtinit;
		}

		if (ac > 3) {
			if (av[3].a_type == A_LONG)
				x->binit = (double)av[3].a_w.w_long;
			else if (av[3].a_type == A_FLOAT)
				x->binit = av[3].a_w.w_float;
			x->b = x->binit;
		}
		if (ac > 2) {
			if (av[2].a_type == A_LONG)
				x->ainit = (double)av[2].a_w.w_long;
			else if (av[2].a_type == A_FLOAT)
				x->ainit = av[2].a_w.w_float;
			x->a = x->ainit;
		}
		if (ac > 1) {
			if (av[1].a_type == A_LONG)
				x->nyinit = (double)av[1].a_w.w_long;
			else if (av[1].a_type == A_FLOAT)
				x->nyinit = av[1].a_w.w_float;
			x->ny = x->nyinit;
		}	
		if (ac > 0) {
			if (av[0].a_type == A_LONG)
				x->nxinit = (double)av[0].a_w.w_long;
			else if (av[0].a_type == A_FLOAT)
				x->nxinit = av[0].a_w.w_float;
			x->nx = x->nxinit;
		}



	} //end if args
}

void henonphase_reset (henonphase *x, Symbol *s, short ac, Atom *av) //input the args 
{
	x->a = x->ainit;
	x->b = x->binit;
	x->nx = x->nxinit;
	x->ny = x->nyinit;
	x->dt = x->dtinit;
}

void  henonphase_calc (henonphase *x)
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


	x->nx = nx*cos(a)-(ny-nx*nx)*sin(a);
	x->ny = nx*sin(a)-(ny-nx*nx)*cos(a);

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


/*
	// henonphase attractor

	lx1 = nx*cos (a) - (ny - (nx*nx))*sin(a);
	ly1 = nx*sin(a) - (ny - (nx*nx))*cos(a);
*/

/*
	x->nx = lx1;
	x->ny = ly1;
*/
	x->px = px;
	x->py = py;
}

void henonphase_bang (henonphase *x)
{

	outlet_float (x->c_out2, x->ny);
	outlet_float (x->c_out, x->nx);	

	henonphase_calc(x);
	
}

void henonphase_a (henonphase *x, double max)
{
	x->a = max;
	if (x->om) henonphase_bang(x);
}
void henonphase_b (henonphase *x, double max)
{
	x->b = max;
	if (x->om) henonphase_bang(x);
}
void henonphase_nx (henonphase *x, double max)
{
	x->nx = max;
	if (x->om) henonphase_bang(x);
}
void henonphase_ny (henonphase *x, double max)
{
	x->ny = max;
	if (x->om) henonphase_bang(x);
}
void henonphase_dt (henonphase *x, double max)
{
	x->dt = max;
	if (x->om) henonphase_bang(x);
}

void henonphase_om (henonphase *x, long max){ x->om = (max>0);}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&henonphase_class,(method)henonphase_new,0L,(short)sizeof(henonphase),0L,
 A_GIMME,0);
 
 addbang((method)henonphase_bang);
 addmess((method)henonphase_set, "set", A_GIMME, 0);
 addmess((method)henonphase_reset, "reset", A_GIMME, 0);
 addmess((method)henonphase_a, "a", A_FLOAT, 0);
 addmess((method)henonphase_b, "b", A_FLOAT, 0);
 addmess((method)henonphase_nx, "x", A_FLOAT, 0);
 addmess((method)henonphase_ny, "y", A_FLOAT, 0);
 addmess((method)henonphase_dt, "dt", A_FLOAT, 0);
 addmess((method)henonphase_om, "om", A_LONG, 0);
 addmess((method)henonphase_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-henon-phase  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void henonphase_assist(henonphase *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y       e             c                h                   o"); }
}


