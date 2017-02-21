/*

————————————————————————————————————————————————————————————————
a-clifford - © andré sier 2004
	
inspired by Paul Bourke, implementation by André Sier	
		
clifford Attractors
 Attributed to Cliff Pickover

 Graphics by Paul Bourke
 February 2004

 See also Peter de Jong attractors 
 xn+1 = sin(a yn) + c cos(a xn)
 yn+1 = sin(b xn) + d cos(b yn)
 where a, b, c, d are variabies that define each attractor.

 a = -1.4, b = 1.6, c = 1.0, d = 0.7 
 a = 1.1, b = -1.0, c = 1.0, d = 1.5
 a = 1.6, b = -0.6, c = -1.2, d = 1.6
 a = 1.7, b = 1.7, c = 0.06, d = 1.2
 a = 1.3, b = 1.7, c = 0.5, d = 1.4
 
 
 vers 1.0.2
 
 updated init variables & calc equation & set methods & reset & info

————————————————————————————————————————————————————————————————
*/

#include "ext.h"
#include "ext_common.h"

#include <math.h>

#define clif_calc(a,b,c,d) (sin((a)*(b))+(c)*cos((a)*(d)))



typedef struct	_clifford
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;		// outlets	
	double 				a, b, c, d, nx, ny;
	double 				ainit, binit, cinit, dinit, nxinit, nyinit;

	Boolean				om;
} clifford;



void *clifford_new (Symbol *msg, short argc, Atom *argv);
void clifford_calc (clifford *x); 
void clifford_bang (clifford *x); 
void clifford_set (clifford *x, Symbol *msg, short argc, Atom *argv);  
void clifford_reset(clifford *x, Symbol *msg, short argc, Atom *argv);
void clifford_info(clifford *x, Symbol *msg, short argc, Atom *argv);

void clifford_a (clifford *x, double max);
void clifford_b (clifford *x, double max);
void clifford_c (clifford *x, double max);
void clifford_d (clifford *x, double max);
void clifford_nx (clifford *x, double max);
void clifford_ny (clifford *x, double max);
void clifford_om (clifford *x, long max);

void clifford_assist(clifford *x, void *b, long m, long a, char *s);
void *clifford_class;




void *clifford_new (Symbol *msg, short argc, Atom *argv) 
{
 clifford *x;
 int i;
 
 x=(clifford *)newobject(clifford_class);

 x->c_out2=floatout(x);
 x->c_out=floatout(x);

 //init
	x->a = -1.4f;
	x->ainit = -1.4f;
	x->b = 1.6f;
	x->binit = 1.6f;
	x->c = 1.0f;
	x->cinit = 1.0f;
	x->d = 0.7f;
	x->dinit = 0.7f;
//do not init x and y for more chaotic results
	x->nx = 0.0f;
	x->nxinit = 0.0f;
	x->ny = 0.0f;
	x->nyinit = 0.0f;
	
	x->om = 0;

	if (argc)
	clifford_set(x, msg, argc, argv);
 	return(x);	
}

void clifford_set (clifford *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	//supa elegant xoaz set method
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
				x->dinit = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->dinit = (double) argv[3].a_w.w_float;
			x->d = x->dinit;
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->cinit = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->cinit = (double) argv[2].a_w.w_float;
			x->c = x->cinit;
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

void  clifford_calc (clifford *x)
{
	double nx,ny;
	double a, b, c, d;
	
	a = x->a;
	b = x->b;
	c = x->c;
	d = x->d;
	nx = x->nx;
	ny = x->ny;

	// clifford attractor

//	lx1 = sin (a*ny) + c*cos(a*nx);
//	ly1 = sin (b*nx) + d*cos(b*ny);

	x->nx = clif_calc(a, ny, c, nx);
	x->ny = clif_calc(b, nx, d, ny);
	
}

void clifford_bang (clifford *x)  //important, first output values then calc next
{	
	outlet_float (x->c_out2, x->nx);
	outlet_float (x->c_out, x->ny);
	clifford_calc(x);	
}

void clifford_nx (clifford *x, double max)
{
	x->nx = max;
//	x->nxinit = max;
	if(x->om) clifford_bang(x);
}
void clifford_ny (clifford *x, double max)
{
	x->ny = max;
//	x->nyinit = max;
	if(x->om) clifford_bang(x);
}
void clifford_a (clifford *x, double max)
{
	x->a = max;
//	x->ainit = max;
	if(x->om) clifford_bang(x);
}
void clifford_b (clifford *x, double max)
{
	x->b = max;
//	x->binit = max;
	if(x->om) clifford_bang(x);
}
void clifford_c (clifford *x, double max)
{
	x->c = max;
//	x->cinit = max;
	if(x->om) clifford_bang(x);
}
void clifford_d (clifford *x, double max)
{
	x->d = max;
//	x->dinit = max;
	if(x->om) clifford_bang(x);
}
void clifford_om (clifford *x, long max) { x->om = (max>0);}

void clifford_reset (clifford *x, Symbol *msg, short argc, Atom *argv)
{
	x->a = x->ainit;
	x->b = x->binit;
	x->c = x->cinit;
	x->d = x->dinit;
	x->nx = x->nxinit;
	x->ny = x->nyinit;
}
void clifford_info (clifford *x, Symbol *msg, short argc, Atom *argv)
{
	post ("__________________________________");
	post ("A-Chaos Lib::a-clifford info 102");
	post ("(a,b,c,d,x,y)init variables: %f, %f, %f, %f, %f, %f", x->ainit, x->binit, x->cinit, x->dinit, x->nxinit, x->nyinit,0);
	post ("(a,b,c,d,x,y) variables: %f, %f, %f, %f, %f, %f", x->a, x->b, x->c, x->d, x->nx, x->ny,0);
	post ("__________________________________");
}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&clifford_class,(method)clifford_new,0L,(short)sizeof(clifford),0L,
 A_GIMME,0);
 
 addbang((method)clifford_bang);
 addmess((method)clifford_reset, "reset", A_GIMME, 0);
 addmess((method)clifford_set, "set", A_GIMME, 0);
// addmess((method)clifford_info, "info", A_GIMME, 0);
 addmess((method)clifford_nx, "x", A_DEFFLOAT, 0);
 addmess((method)clifford_ny, "y", A_DEFFLOAT, 0);
 addmess((method)clifford_a, "a", A_DEFFLOAT, 0);
 addmess((method)clifford_b, "b", A_DEFFLOAT, 0);
 addmess((method)clifford_c, "c", A_DEFFLOAT, 0);
 addmess((method)clifford_d, "d", A_DEFFLOAT, 0);
 addmess((method)clifford_om, "om", A_DEFLONG, 0);



 addmess((method)clifford_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-clifford  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void clifford_assist(clifford *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x clifford     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  clifford     e             c                h                   o"); }
}


