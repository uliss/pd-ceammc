/*
Ikeda attractor

 zn+1 = a + b zn exp[ i k - i p / (1 + | zn2 | ) ]

where z is a complex number

real number formula by euler returns:

xn+1 = a + k*( x0*cos(b-(p/(1+x0*x0+y0*y0))) - y0*sin(b-(p/(1*x0*x0+y0*y0))) )
yn+1 =     k*( x0*sin(b-(p/(1+x0*x0+y0*y0))) - y0*cos(b-(p/(1*x0*x0+y0*y0))) )


 a = 0.85, b = 0.9, k = 0.4, p = 7.7
 a = 1.0, b = 0.9, k = 0.4, p = 6.0
*/

#include "ext.h"
#include "ext_common.h"

#include "math.h"



typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		// value
	void 		        *c_out2;		// value
	
	double		a, b, k, p,nx, ny; //eq variables
	double		ainit, binit, kinit, pinit, nxinit, nyinit; //init eq variables

	Boolean om;
} ikeda;


void *ikeda_new (Symbol *msg, short argc, Atom *argv);
void ikeda_bang (ikeda *x);

//user variables
void ikeda_a (ikeda *x, double echo);
void ikeda_b (ikeda *x, double echo);
void ikeda_k (ikeda *x, double echo);
void ikeda_nx (ikeda *x, double echo);
void ikeda_ny (ikeda *x, double echo);
void ikeda_p (ikeda *x, double echo);

//calc
void ikeda_calc (ikeda *x);
void ikeda_reset (ikeda *x, Symbol *msg, short argc, Atom *argv); 
void ikeda_set (ikeda *x, Symbol *msg, short argc, Atom *argv) ; 

void ikeda_assist(ikeda *x, void *b, long m, long a, char *s);
void *ikeda_class;


void *ikeda_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 ikeda *x;
	 int i;
	 
	 x=(ikeda *)newobject(ikeda_class);

	 x->c_out2=floatout(x);
	 x->c_out=floatout(x);

	 //classic ikeda  a = 0.85, b = 0.9, k = 0.4, p = 7.7

		 x->a=0.85;
		 x->b=0.9; 
		 x->k=0.4;
		 x->p=7.7; 
		 x->nx=0; 
		 x->ny=0; 

		 x->ainit=0.85;
		 x->binit=0.9; 
		 x->kinit=0.4;
		 x->pinit=7.7; 
		 x->nxinit=0; 
		 x->nyinit=0; 
		 
		 x->om = 0;

	ikeda_set(x, msg, argc, argv);

	 return(x);	
}

void ikeda_om(ikeda *x, int n)     { x->om = (n>0);}


void ikeda_set (ikeda *x, Symbol *msg, short ac, Atom *av) //input the args 
{
	if (ac) {

		if (ac > 5) {
			if (av[5].a_type == A_LONG)
				x->pinit = (double)av[5].a_w.w_long;
			else if (av[5].a_type == A_FLOAT)
				x->pinit = av[5].a_w.w_float;
			x->p = x->pinit;
		}
		if (ac > 4) {
			if (av[4].a_type == A_LONG)
				x->kinit = (double)av[4].a_w.w_long;
			else if (av[4].a_type == A_FLOAT)
				x->kinit = av[4].a_w.w_float;
			x->k = x->kinit;
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


	
	} // end if args
}

void ikeda_reset (ikeda *x, Symbol *msg, short argc, Atom *argv)
{
	x->nx = x->nxinit;
	x->ny = x->nyinit;
	x->a = x->ainit;
	x->b = x->binit;
	x->k = x->kinit;
	x->p = x->pinit;

}



void  ikeda_calc (ikeda *x)
{
	double	a, b, k, p, x0, y0;
	double  sinx, cosx, temp;
	
	//local copies
	a = x->a;
	b = x->b;
	k = x->k;
	p = x->p;
	x0 = x->nx;
	y0 = x->ny;


	temp = (p/(1+x0*x0+y0*y0));
	cosx = (cos(b-temp));
	sinx = (sin(b-temp));
/*
xn+1 = a + k*( x0*cos(b-(p/(1+x0*x0+y0*y0))) - y0*sin(b-(p/(1*x0*x0+y0*y0))) )
yn+1 =     k*( x0*sin(b-(p/(1+x0*x0+y0*y0))) - y0*cos(b-(p/(1*x0*x0+y0*y0))) )
*/
	x->nx = a + k*(x0*cosx)-y0*sinx;
	x->ny = k*sinx -y0*cosx;
}




void ikeda_a (ikeda *x, double echo)
{
	x->a = (double)echo;
//	x->ainit = (double)echo;
	if (x->om) ikeda_bang(x);
}


void ikeda_b (ikeda *x, double echo)
{
	x->b = (double)echo;
//	x->rinit = (double)echo;
	if (x->om) ikeda_bang(x);
}

void ikeda_k (ikeda *x, double echo)
{
	x->k = (double)echo;
//	x->cinit = (double)echo;
	if (x->om) ikeda_bang(x);
}

void ikeda_nx (ikeda *x, double echo)
{
	x->nx = (double)echo;
//	x->nxinit = (double)echo;
	if (x->om) ikeda_bang(x);
}

void ikeda_ny (ikeda *x, double echo)
{
	x->ny = (double)echo;
//	x->nyinit = (double)echo;
	if (x->om) ikeda_bang(x);
}

void ikeda_p (ikeda *x, double echo)
{
	x->p = (double)echo;
//	x->nzinit = (double)echo;
	if (x->om) ikeda_bang(x);
}



void ikeda_bang (ikeda *x)
{

	outlet_float(x->c_out2, x->ny); 
	outlet_float(x->c_out, x->nx); 

    ikeda_calc(x);   //next
    
}



void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&ikeda_class,(method)ikeda_new,0L,(short)sizeof(ikeda),0L,
 A_GIMME,0);

 addbang((method)ikeda_bang);
 addmess((method)ikeda_set,"set", A_GIMME, 0);
 addmess((method)ikeda_reset,"reset", A_GIMME, 0);

 addmess((method)ikeda_nx,"x", A_DEFFLOAT, 0);
 addmess((method)ikeda_ny,"y", A_DEFFLOAT, 0);
 addmess((method)ikeda_a,"a", A_DEFFLOAT, 0);
 addmess((method)ikeda_b,"b", A_DEFFLOAT, 0);
 addmess((method)ikeda_k,"c", A_DEFFLOAT, 0);
 addmess((method)ikeda_p,"p", A_DEFFLOAT, 0);
 addmess((method)ikeda_om,"om", A_DEFLONG, 0);

 addmess((method)ikeda_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-ikeda  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void ikeda_assist(ikeda *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, set, "); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  ikeda   e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  ikeda   e         c            h              o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) z  ikeda   e         c            h              o"); }
//    else if (m==2&&a==1) { sprintf(s,"(lisy)       e             c                h                   o"); }
}


