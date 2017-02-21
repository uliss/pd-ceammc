/*

————————————————————————————————————————————————————————————————
a-henon_heilles - © andré sier 2004

a port of Richard's chaos-henon-heilles from chaos collection
————————————————————————————————————————————————————————————————
*/

#include "ext.h"
#include "ext_common.h"

#include <math.h>


typedef struct	_henon_heilles
{
	t_object 	        c_ob;			
	void 		        *c_out[4];		// outlets	
	double 				nx, ny, nxdot, nydot, e, dt;
	double 				nxinit, nyinit, nxdotinit, nydotinit, einit, dtinit;
	Boolean om;
} henon_heilles;



void *henon_heilles_new (Symbol *msg, short argc, Atom *argv);
void henon_heilles_calc (henon_heilles *x); 
void henon_heilles_bang (henon_heilles *x); 
void henon_heilles_set (henon_heilles *x, Symbol *msg, short argc, Atom *argv);  
void henon_heilles_reset(henon_heilles *x, Symbol *msg, short argc, Atom *argv);

void henon_heilles_nx (henon_heilles *x, double max);
void henon_heilles_ny (henon_heilles *x, double max);
void henon_heilles_nydot (henon_heilles *x, double max);
void henon_heilles_e (henon_heilles *x, double max);
void henon_heilles_dt (henon_heilles *x, double max);
void henon_heilles_om (henon_heilles *x, long max);

void henon_heilles_assist(henon_heilles *x, void *b, long m, long a, char *s);
void *henon_heilles_class;




void *henon_heilles_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
 henon_heilles *x;
 int i;
 
 x=(henon_heilles *)newobject(henon_heilles_class);

 x->c_out[3]=floatout(x);
 x->c_out[2]=floatout(x);
 x->c_out[1]=floatout(x);
 x->c_out[0]=floatout(x);

 //init
	x->nx = 1.0f;
	x->ny = 1.0f;
	x->nydot = 1.4f;
	x->e = 0.125f;
	x->dt = 0.02;
	x->nxinit = 1.0f;
	x->nyinit = 1.0f;
	x->nydotinit = 1.4f;
	x->einit = 0.125f;
	x->dtinit = 0.02;
	
	if(argc)
	 henon_heilles_set(x, msg, argc, argv);

 return(x);	
}

void henon_heilles_set (henon_heilles *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	// super elegant xoaz set method
	if (argc) {
		if (argc>4) {
			if (argv[4].a_type == A_LONG)
				x->dtinit = (double) argv[4].a_w.w_long;
			else if (argv[4].a_type == A_FLOAT)
				x->dtinit = (double) argv[4].a_w.w_float;
			x->dt = x->dtinit;
		}

		if (argc>3) {
			if (argv[3].a_type == A_LONG)
				x->einit = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->einit = (double) argv[3].a_w.w_float;
			x->e = x->einit;
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->nydotinit = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->nydotinit = (double) argv[2].a_w.w_float;
			x->nydot = x->nydotinit;
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->nyinit = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->nyinit = (double) argv[1].a_w.w_float;
			x->ny = x->nyinit;
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->nxinit = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->nxinit = (double) argv[0].a_w.w_float;
			x->nx = x->nxinit;
		}

	} //end if args
}

void  henon_heilles_calc (henon_heilles *x)
{
	double nx, ny, nxdot, nydot, e, dt;
	double xdd, ydd;


	nx = x->nx;
	ny = x->ny;
	nydot = x->nydot;
	dt = x->dt ;
	e = x->e;

	xdd = -nx-(2*nx*ny);
	ydd = ny*ny -ny -(nx*nx);
	x->nxdot += xdd*dt;
	x->nydot += ydd*dt;
	
	nxdot=x->nxdot;
	nydot=x->nydot;
	
	x->nx += nxdot*dt;
	x->ny += nydot*dt; 

}



void henon_heilles_bang (henon_heilles *x) //important, first output values then calc next
{
	outlet_float (x->c_out[3], x->nydot);
	outlet_float (x->c_out[2], x->nxdot);
	outlet_float (x->c_out[1], x->ny);
	outlet_float (x->c_out[0], x->nx);
	
	henon_heilles_calc(x);
}

void henon_heilles_nx (henon_heilles *x, double max)
{
	x->nx = max;
//	x->nxinit = max;
	if (x->om) henon_heilles_bang(x);
}
void henon_heilles_ny (henon_heilles *x, double max)
{
	x->ny = max;
//	x->nyinit = max;
	if (x->om) henon_heilles_bang(x);
}
void henon_heilles_nydot (henon_heilles *x, double max)
{
	x->nydot = max;
//	x->nydotinit = max;
	if (x->om) henon_heilles_bang(x);
}
void henon_heilles_e (henon_heilles *x, double max)
{
	x->e = max;
//	x->einit = max;
	if (x->om) henon_heilles_bang(x);
}
void henon_heilles_dt (henon_heilles *x, double max)
{
	x->dt = max;
//	x->dtinit = max;
	if (x->om) henon_heilles_bang(x);
}

void henon_heilles_reset (henon_heilles *x, Symbol *msg, short argc, Atom *argv)
{
	x->nx = x->nxinit;
	x->ny = x->nyinit;
	x->nydot = x->nydotinit;
	x->e = x->einit;
	x->dt = x->dtinit;

	x->nxdotinit = sqrt((2 * x->e) - (x->ny*x->ny) + (4*x->ny*x->ny*x->ny/3) - (x->nydot * x->nydot));
	x->nxdot = x->nxdotinit;
}


void henon_heilles_om (henon_heilles *x, long max){ x->om = (max>0);}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&henon_heilles_class,(method)henon_heilles_new,0L,(short)sizeof(henon_heilles),0L,
 A_GIMME,0);
 
 addbang((method)henon_heilles_bang);
 addmess((method)henon_heilles_reset, "reset", A_GIMME, 0);
 addmess((method)henon_heilles_set, "set", A_GIMME, 0);
 addmess((method)henon_heilles_nx, "x", A_DEFFLOAT, 0);
 addmess((method)henon_heilles_ny, "y", A_DEFFLOAT, 0);
 addmess((method)henon_heilles_nydot, "y'", A_DEFFLOAT, 0);
 addmess((method)henon_heilles_e, "e", A_DEFFLOAT, 0);
 addmess((method)henon_heilles_dt, "dt", A_DEFFLOAT, 0);

 addmess((method)henon_heilles_om, "om", A_DEFLONG, 0);


 addmess((method)henon_heilles_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-henon_heilles  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}



void henon_heilles_assist(henon_heilles *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  henon_heilles     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  henon_heilles     e            c                h                   o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) x' henon_heilles     e           c                h                   o"); }
    else if (m==2&&a==3) { sprintf(s,"(float) y' henon_heilles     e        c                h                   o"); }
}


