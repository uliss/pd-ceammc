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

#include "ext.h"
#include "ext_common.h"

#include <math.h>


typedef struct	_duffing
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;		// outlet	
	double a, b, w, t, dt, nx, ny;
	double ainit, binit, winit, tinit, dtinit, nxinit, nyinit;

	Boolean om;
} duffing;



void *duffing_new (Symbol *msg, short argc, Atom *argv);
void duffing_calc (duffing *x); 
void duffing_bang (duffing *x); 
void duffing_set (duffing *x, Symbol *msg, short argc, Atom *argv); 
void duffing_reset (duffing *x, Symbol *msg, short argc, Atom *argv); 

void duffing_a (duffing *x, double max);
void duffing_b (duffing *x, double max);
void duffing_w (duffing *x, double max);
void duffing_dt (duffing *x, double max);
void duffing_om (duffing *x, long max);

void duffing_assist(duffing *x, void *b, long m, long a, char *s);
void *duffing_class;




void *duffing_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
 duffing *x;
 int i;
 
 x=(duffing *)newobject(duffing_class);

x->c_out2=floatout(x);
 x->c_out=floatout(x);

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

 return(x);	
}

void duffing_set (duffing *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	// super elegant xoaz set method
	if (argc) {
		if (argc>5) {
			if (argv[5].a_type == A_LONG)
				x->nyinit = (double) argv[5].a_w.w_long;
			else if (argv[5].a_type == A_FLOAT)
				x->nyinit = (double) argv[5].a_w.w_float;
			x->ny = x->nyinit;
		}

		if (argc>4) {
			if (argv[4].a_type == A_LONG)
				x->nxinit = (double) argv[4].a_w.w_long;
			else if (argv[4].a_type == A_FLOAT)
				x->nyinit = (double) argv[4].a_w.w_float;
			x->nx = x->nxinit;
		}

		if (argc>3) {
			if (argv[3].a_type == A_LONG)
				x->dtinit = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->dtinit = (double) argv[3].a_w.w_float;
			x->dt = x->dtinit;
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->winit = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->winit = (double) argv[2].a_w.w_float;
			x->w = x->winit;
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->binit = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->binit = (double) argv[1].a_w.w_float;
			x->b = x->binit;
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->ainit = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->ainit = (double) argv[0].a_w.w_float;
			x->a = x->ainit;
		}

	} //end if args
}

void duffing_reset (duffing *x, Symbol *msg, short argc, Atom *argv)
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

void  duffing_calc (duffing *x)
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
	x->ny = nx-(nx*nx*nx)-(a*ny)+(b*cos(w*t));


}

void duffing_bang (duffing *x)
{
	outlet_float (x->c_out2, x->nx);
	outlet_float (x->c_out, x->ny);

	duffing_calc(x);
}

void duffing_a (duffing *x, double max)
{
	x->a = max;
//	x->ainit = max;
	if(x->om) duffing_bang(x);
}
void duffing_b (duffing *x, double max)
{
	x->b = max;
//	x->binit = max;
	if(x->om) duffing_bang(x);
}
void duffing_w (duffing *x, double max)
{
	x->w = max;
//	x->winit = max;
	if(x->om) duffing_bang(x);
}
void duffing_dt (duffing *x, double max)
{
	x->dt = max;
//	x->dtinit = max;
	if(x->om) duffing_bang(x);
}
void duffing_om (duffing *x, long max) { x->om = (max>0);}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&duffing_class,(method)duffing_new,0L,(short)sizeof(duffing),0L,
 A_GIMME,0);
 
 addbang((method)duffing_bang);
 addmess((method)duffing_set, "set", A_GIMME, 0);
 addmess((method)duffing_reset, "reset", A_GIMME, 0);
 addmess((method)duffing_a, "a", A_FLOAT, 0);
 addmess((method)duffing_b, "b", A_FLOAT, 0);
 addmess((method)duffing_w, "w", A_FLOAT, 0);
 addmess((method)duffing_dt, "dt", A_FLOAT, 0);
 addmess((method)duffing_om, "om", A_DEFLONG, 0);

 addmess((method)duffing_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-duffing  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void duffing_assist(duffing *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x de duffing     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  de duffing     e             c                h                   o"); }
}


