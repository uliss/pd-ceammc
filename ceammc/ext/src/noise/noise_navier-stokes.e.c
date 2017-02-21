/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - navierstokese © andrŽ sier 20030914
		
a port from RD's navier-stokese

his INFO!
/* chaos collection - 1996, Richard Dudas, IRCAM */
/* updated summer 96 for CodeWarrior PPC/68k */
/* navier-stokese equation solved using heuns' method */
// optimized code and removed 68k-specific junk Oct 02 -Rd

/* 
info: 

A model obtained by a suitable five-mode truncation 
of the Navier-stokese equations for a two-dimensional 
incompressible fluid on a torus:

dx1= -2*x1 + 4*x2*x3 + 4*x4*x5
dx2= -9*x2 + 3*x1*x3
dx3= -5*x3 - 7*x1*x2 + reyn
dx4= -5*x4 - x1*x5
dx5= -x5 - 3*x1*x4

Where <reyn> is the Reynolds number,
and exibits  an interesting variety of different behaviors for different 
ranges of <reyn>. For the critical values of <reyn>, the most interesting
feature is the stochastic behavior observed when  R1 < <reyn> < R2.
With 28.73 < R1 > 29.0 and R2 ± = 33.43. 

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"





typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out[5];		// value
	
	double		a, b, c, d, e, r, dt; //eq variables
	double		ainit, binit, cinit, dinit, einit, rinit, dtinit; //eq variables

	Boolean om;
} navierstokese;


void *navierstokese_new (Symbol *msg, short argc, Atom *argv);
void navierstokese_bang (navierstokese *x);

//user variables
void navierstokese_a (navierstokese *x, double echo);
void navierstokese_r (navierstokese *x, double echo);
void navierstokese_c (navierstokese *x, double echo);
void navierstokese_dt (navierstokese *x, double echo);

//calc
void navierstokese_calc (navierstokese *x);
void navierstokese_reset (navierstokese *x, Symbol *msg, short argc, Atom *argv); 
void navierstokese_set (navierstokese *x, Symbol *msg, short argc, Atom *argv) ; 

void navierstokese_assist(navierstokese *x, void *b, long m, long a, char *s);
void *navierstokese_class;


void *navierstokese_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 navierstokese *x;
	 int i;
	 
	 x=(navierstokese *)newobject(navierstokese_class);

	 x->c_out[4]=floatout(x);
	 x->c_out[3]=floatout(x);
	 x->c_out[2]=floatout(x);
	 x->c_out[1]=floatout(x);
	 x->c_out[0]=floatout(x);

	 //classic navierstokese
		x->a = 1.0;
		x->b = 1.0;
		x->c = 1.0;
		x->d = 1.0;
		x->e = 1.0;
		x->ainit = 1.0;
		x->binit = 1.0;
		x->cinit = 1.0;
		x->dinit = 1.0;
		x->einit = 1.0;

		x->r = 28.0;
		x->rinit = 28.0;
		x->dt = 0.01;
		x->dtinit = 0.01;

		 
		 x->om = 0;

	navierstokese_set(x, msg, argc, argv);

	 return(x);	
}

void navierstokese_om(navierstokese *x, int n)     { x->om = (n>0);}


void navierstokese_set (navierstokese *x, Symbol *msg, short ac, Atom *av) //input the args 
{
	if (ac) {

		if (ac > 6) {
			if (av[6].a_type == A_LONG)
				x->dtinit = (double)av[6].a_w.w_long;
			else if (av[6].a_type == A_FLOAT)
				x->dtinit = av[6].a_w.w_float;
			x->dt = x->dtinit;
		}
		if (ac > 5) {
			if (av[5].a_type == A_LONG)
				x->rinit = (double)av[5].a_w.w_long;
			else if (av[5].a_type == A_FLOAT)
				x->rinit = av[5].a_w.w_float;
			x->r = x->rinit;
		}
		if (ac > 4) {
			if (av[4].a_type == A_LONG)
				x->einit = (double)av[4].a_w.w_long;
			else if (av[4].a_type == A_FLOAT)
				x->einit = av[4].a_w.w_float;
			x->e = x->einit;
		}
		if (ac > 3) {
			if (av[3].a_type == A_LONG)
				x->dinit = (double)av[3].a_w.w_long;
			else if (av[3].a_type == A_FLOAT)
				x->dinit = av[3].a_w.w_float;
			x->d = x->dinit;
		}
		if (ac > 2) {
			if (av[2].a_type == A_LONG)
				x->cinit = (double)av[2].a_w.w_long;
			else if (av[2].a_type == A_FLOAT)
				x->cinit = av[2].a_w.w_float;
			x->c = x->cinit;
		}
		if (ac > 1) {
			if (av[1].a_type == A_LONG)
				x->binit = (double)av[1].a_w.w_long;
			else if (av[1].a_type == A_FLOAT)
				x->binit = av[1].a_w.w_float;
			x->b = x->binit;
		}	
		if (ac > 0) {
			if (av[0].a_type == A_LONG)
				x->ainit = (double)av[0].a_w.w_long;
			else if (av[0].a_type == A_FLOAT)
				x->ainit = av[0].a_w.w_float;
			x->a = x->ainit;
		}


	
	} // end if args
}

void navierstokese_reset (navierstokese *x, Symbol *msg, short argc, Atom *argv)
{
	x->b = x->binit;
	x->d = x->dinit;
	x->e = x->einit;
	x->a = x->ainit;
	x->r = x->rinit;
	x->c = x->cinit;
	x->dt = x->dtinit;

}

/*==========================================================================*/
#define nava(a, b, c, d, e)		((-2*(a)) + (4*(b)*(c)) + (4*(d)*(e)))
#define navb(a, b, c)			((-9*(b)) + (3*(a)*(c)))
#define navc(a, b, c, r)		((-5*(c)) + (-7*(a)*(b)) + (r))
#define navd(a, d, e)			((-5*(d)) - ((a)*(e)))
#define nave(a, d, e)			((-3*(a)*(d)) - (e))


void  navierstokese_calc (navierstokese *x)
{
	double a, b, c, d, e, r, dt, dt2;
	double da, db, dc, dd, de;
		
	//local copies
	e = x->e;
	d = x->d;
	c = x->c;
	dt = x->dt;
	dt2 = dt/2.;
	b = x->b;
	a = x->a;
	r = x->r;

	da=((a * -2) + (4 * b * c) + (4 * d * e))*dt;
	db=((b * -9) + (3 * a * c))*dt;
	dc=((c * -5) + (-7 * a * b) + r)*dt;
	dd=((d * -5) - (a * e))*dt;
	de=((a * d * -3) - e)*dt;
	
		
	x->a += da;
	x->b += db;
	x->c += dc;
	x->d += dd;
	x->e += de;
}



void navierstokese_a (navierstokese *x, double echo)
{
	x->a = (double)echo;
//	x->ainit = (double)echo;
	if (x->om) navierstokese_bang(x);
}


void navierstokese_r (navierstokese *x, double echo)
{
	x->r = (double)echo;
//	x->rinit = (double)echo;
	if (x->om) navierstokese_bang(x);
}

void navierstokese_b (navierstokese *x, double echo)
{
	x->b = (double)echo;
//	x->binit = (double)echo;
	if (x->om) navierstokese_bang(x);
}

void navierstokese_c (navierstokese *x, double echo)
{
	x->c = (double)echo;
//	x->cinit = (double)echo;
	if (x->om) navierstokese_bang(x);
}

void navierstokese_d (navierstokese *x, double echo)
{
	x->d = (double)echo;
//	x->nxinit = (double)echo;
	if (x->om) navierstokese_bang(x);
}


void navierstokese_dt (navierstokese *x, double echo)
{
	x->dt = (double)echo;
//	x->dtinit = (double)echo;
	if (x->om) navierstokese_bang(x);
}


void navierstokese_bang (navierstokese *x) //important, output _ten_ calc
{

	outlet_float(x->c_out[4], x->e); 
	outlet_float(x->c_out[3], x->d); 
	outlet_float(x->c_out[2], x->c); 
	outlet_float(x->c_out[1], x->b); 
	outlet_float(x->c_out[0], x->a); 

    navierstokese_calc(x);   //next
    
}



void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&navierstokese_class,(method)navierstokese_new,0L,(short)sizeof(navierstokese),0L,
 A_GIMME,0);

 addbang((method)navierstokese_bang);
 addmess((method)navierstokese_set,"set", A_GIMME, 0);
 addmess((method)navierstokese_reset,"reset", A_GIMME, 0);

 addmess((method)navierstokese_dt,"dt", A_DEFFLOAT, 0);
 addmess((method)navierstokese_a,"a", A_DEFFLOAT, 0);
 addmess((method)navierstokese_r,"r", A_DEFFLOAT, 0);
 addmess((method)navierstokese_c,"c", A_DEFFLOAT, 0);
 addmess((method)navierstokese_b,"b", A_DEFFLOAT, 0);
 addmess((method)navierstokese_c,"c", A_DEFFLOAT, 0);
 addmess((method)navierstokese_d,"d", A_DEFFLOAT, 0);
 addmess((method)navierstokese_om,"om", A_DEFLONG, 0);

 addmess((method)navierstokese_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-navier-stokes.e  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void navierstokese_assist(navierstokese *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"ping, (int), (float), ·, ¶ª"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) z  l o r e n t z   e         c            h              o"); }
//    else if (m==2&&a==1) { sprintf(s,"(lisy)       e             c                h                   o"); }
}


