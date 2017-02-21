/*

————————————————————————————————————————————————————————————————
a-henon - © andré sier 2004

updated to use init vars method ported from chaos collection		
————————————————————————————————————————————————————————————————
*/

#include "ext.h"
#include "ext_common.h"

#include <math.h>


typedef struct	_henon
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;			
	double a, b, nx, ny;
	double ainit, binit, nxinit, nyinit;

	Boolean om;
} henon;



void *henon_new (Symbol *msg, short argc, Atom *argv);
void henon_calc (henon *x); 
void henon_bang (henon *x); 
void henon_set (henon *x, Symbol *msg, short argc, Atom *argv); 
void henon_reset (henon *x, Symbol *msg, short argc, Atom *argv); 

void henon_a (henon *x, double max);
void henon_b (henon *x, double max);
void henon_om (henon *x, long max);

void henon_assist(henon *x, void *b, long m, long a, char *s);
void *henon_class;




void *henon_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
 henon *x;
 int i;
 
 x=(henon *)newobject(henon_class);

x->c_out2=floatout(x);
 x->c_out=floatout(x);

 //init
	x->a = 1.4f;
	x->b = 0.3f;
	x->nx = 1.0f;
	x->ny = 1.0f;

	x->ainit = 1.4f;
	x->binit = 0.3f;
	x->nxinit = 1.0f;
	x->nyinit = 1.0f;

	if (argc)
	 henon_set(x, msg, argc, argv);
	 
 return(x);	
}

void henon_set (henon *x, Symbol *s, short ac, Atom *av) //input the args 
{

	if (ac) {

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

void henon_reset (henon *x, Symbol *s, short ac, Atom *av) //input the args 
{
	x->a = x->ainit;
	x->b = x->binit;
	x->nx = x->nxinit;
	x->ny = x->nyinit;
}
void  henon_calc (henon *x)
{
	double nx, ny, lx1, ly1;
	double a, b;
	
	a = x->a;
	b = x->b;
	nx = x->nx;
	ny = x->ny;

	x->nx = (ny + 1) - (a * (nx*nx));
	x->ny = b * nx;


}

void henon_bang (henon *x)
{
	
	outlet_float (x->c_out2, x->ny);
	outlet_float (x->c_out, x->nx);

	henon_calc(x);
	
}

void henon_a (henon *x, double max)
{
	x->a = max;
	if (x->om) henon_bang(x);
}
void henon_b (henon *x, double max)
{
	x->b = max;
	if (x->om) henon_bang(x);
}
void henon_nx (henon *x, double max)
{
	x->nx = max;
	if (x->om) henon_bang(x);
}
void henon_ny (henon *x, double max)
{
	x->ny = max;
	if (x->om) henon_bang(x);
}

void henon_om (henon *x, long max){ x->om = (max>0);}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&henon_class,(method)henon_new,0L,(short)sizeof(henon),0L,
 A_GIMME,0);
 
 addbang((method)henon_bang);
 addmess((method)henon_set, "set", A_GIMME, 0);
 addmess((method)henon_reset, "reset", A_GIMME, 0);
 addmess((method)henon_a, "a", A_FLOAT, 0);
 addmess((method)henon_b, "b", A_FLOAT, 0);
 addmess((method)henon_nx, "x", A_FLOAT, 0);
 addmess((method)henon_ny, "y", A_FLOAT, 0);
 addmess((method)henon_om, "om", A_LONG, 0);
 addmess((method)henon_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-henon  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void henon_assist(henon *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y       e             c                h                   o"); }
}


