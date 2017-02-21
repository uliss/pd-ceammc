/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a-ginger - © andrŽ sier 2004
		

A-Chaos Lib

2d output

	--- chaos-ginger.a	
		(a-ginger)

		--inputs
			--  (1 == z/seed) defaults (0.82) // x, y default to (0.7,1.2)
		--outputs
			--  (2d)

		--formulae
			--  dx/dt = 1. -y - z*abs(x)
			--  dy/dt = x


ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"


#define ABS(a) ((a)>0)?(a):(-a) 
//#include <math.h>


typedef struct	_ginger
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;		// 2 outlets	
	double seed, nx, ny;	
	double seedinit, nxinit, nyinit;	
	Boolean	om;
} ginger;



void *ginger_new (Symbol *s, int ac, Atom *av);
void ginger_calc (ginger *x); 
void ginger_bang (ginger *x); 
void ginger_set(ginger *x, Symbol *s, int ac, Atom *av); 
void ginger_reset(ginger *x); 

void ginger_seed (ginger *x, double max);
void ginger_nx (ginger *x, double max);
void ginger_ny (ginger *x, double max);
void ginger_om (ginger *x, long max);

void ginger_assist(ginger *x, void *b, long m, long a, char *s);
void *ginger_class;




void *ginger_new (Symbol *s, int ac, Atom *av) //input the args 
{
 ginger *x;
 int i;
 
 x=(ginger *)newobject(ginger_class);

 x->c_out2=floatout(x);
 x->c_out=floatout(x);

	x->seed = 0.82f;
	x->nx = 0.7f;
	x->ny = 1.2f;
	x->seedinit = 0.82f;
	x->nxinit = 0.7f;
	x->nyinit = 1.2f;

	ginger_set(x, s, ac, av);
	
 return(x);	
}

void ginger_set(ginger *x, Symbol *s, int argc, Atom *argv)
{
	if (argc) {

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->nyinit = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->nyinit = (double) argv[2].a_w.w_float;
			x->ny = x->nyinit;
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->nxinit = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->nxinit = (double) argv[1].a_w.w_float;
			x->nx = x->nxinit;
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->seedinit = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->seedinit = (double) argv[0].a_w.w_float;
			x->seed = x->seedinit;
		}


	}
}

void ginger_reset (ginger *x)
{
	x->seed = x->seedinit;
	x->nx = x->nxinit;
	x->ny = x->nyinit;
}

void  ginger_calc (ginger *x)
{
	double nx, ny, x1, y1;
	double seed;
	
	seed = x->seed;
	nx = x->nx;
	ny = x->ny;
	// ginger formulae

	y1 = nx;

	if (nx<0.) nx = -nx;
	x1 = 1. - ny - seed*nx;

	x->nx = x1;
	x->ny = y1;

}

void ginger_bang (ginger *x)
{
	outlet_float (x->c_out2, x->nx);
	outlet_float (x->c_out, x->ny);
	
	ginger_calc(x);	
}

void ginger_seed (ginger *x, double max)
{
	x->seed = max;
	x->seedinit = max;
}
void ginger_nx (ginger *x, double max)
{
	x->nx = max;
	x->nxinit = max;
}
void ginger_ny (ginger *x, double max)
{
	x->ny = max;
	x->nyinit = max;
}
void ginger_om (ginger *x, long max)
{
	x->om = (max>0);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&ginger_class,(method)ginger_new,0L,(short)sizeof(ginger),0L,
 A_GIMME,0);
 
 addbang((method)ginger_bang);
 addmess((method)ginger_set, "set", A_GIMME, 0);
 addmess((method)ginger_reset, "reset", 0);
 addmess((method)ginger_nx, "x", A_FLOAT, 0);
 addmess((method)ginger_ny, "y", A_FLOAT, 0);
 addmess((method)ginger_seed, "z", A_FLOAT, 0);
 addmess((method)ginger_seed, "seed", A_FLOAT, 0);
 addmess((method)ginger_om, "om", A_LONG, 0);
 addmess((method)ginger_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-ginger  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void ginger_assist(ginger *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"A-Chaos Lib"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x ginger     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y ginger      e             c                h                   o"); }
}


