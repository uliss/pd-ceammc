/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - navierstokes © andrŽ sier 20030914
		
a port from RD's navier-stokes

his INFO!
/* chaos collection - 1996, Richard Dudas, IRCAM */
/* updated summer 96 for CodeWarrior PPC/68k */
/* navier-stokes equation solved using heuns' method */
// optimized code and removed 68k-specific junk Oct 02 -Rd

/* 
info: 

A model obtained by a suitable five-mode truncation 
of the Navier-Stokes equations for a two-dimensional 
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
} navierstokes;


void *navierstokes_new (Symbol *msg, short argc, Atom *argv);
void navierstokes_bang (navierstokes *x);

//user variables
void navierstokes_a (navierstokes *x, double echo);
void navierstokes_r (navierstokes *x, double echo);
void navierstokes_c (navierstokes *x, double echo);
void navierstokes_dt (navierstokes *x, double echo);

//calc
void navierstokes_calc (navierstokes *x);
void navierstokes_reset (navierstokes *x, Symbol *msg, short argc, Atom *argv); 
void navierstokes_set (navierstokes *x, Symbol *msg, short argc, Atom *argv) ; 

void navierstokes_assist(navierstokes *x, void *b, long m, long a, char *s);
void *navierstokes_class;


void *navierstokes_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 navierstokes *x;
	 int i;
	 
	 x=(navierstokes *)newobject(navierstokes_class);

	 x->c_out[4]=floatout(x);
	 x->c_out[3]=floatout(x);
	 x->c_out[2]=floatout(x);
	 x->c_out[1]=floatout(x);
	 x->c_out[0]=floatout(x);

	 //classic navierstokes
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

	navierstokes_set(x, msg, argc, argv);

	 return(x);	
}

void navierstokes_om(navierstokes *x, int n)     { x->om = (n>0);}


void navierstokes_set (navierstokes *x, Symbol *msg, short ac, Atom *av) //input the args 
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

void navierstokes_reset (navierstokes *x, Symbol *msg, short argc, Atom *argv)
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


void  navierstokes_calc (navierstokes *x)
{
	double a, b, c, d, e, r, dt, dt2;
	double afa, bfb, cfc, dfd, efe;
	double fa, fb, fc, fd, fe;
	double ffa, ffb, ffc, ffd, ffe;
	
	//local copies
	e = x->e;
	d = x->d;
	c = x->c;
	dt = x->dt;
	dt2 = dt/2.;
	b = x->b;
	a = x->a;
	r = x->r;

	fa=nava(a, b, c, d, e);
	fb=navb(a, b, c);
	fc=navc(a, b, c, r);
	fd=navd(a, d, e);
	fe=nave(a, d, e);
	
	afa = a +(dt*fa);
	bfb = b +(dt*fb);
	cfc = c +(dt*fc);
	dfd = d +(dt*fd);
	efe = e +(dt*fe);
	
	ffa = nava(afa,bfb,cfc,dfd,efe);
	ffb = navb(afa,bfb,cfc);
	ffc = navc(afa,bfb,cfc,r);
	ffd = navd(afa,dfd,efe);
	ffe = nave(afa,dfd,efe);
	
	x->a += (dt2*(fa+ffa));
	x->b += (dt2*(fb+ffb));
	x->c += (dt2*(fc+ffc));
	x->d += (dt2*(fd+ffd));
	x->e += (dt2*(fe+ffe));
}



void navierstokes_a (navierstokes *x, double echo)
{
	x->a = (double)echo;
//	x->ainit = (double)echo;
	if (x->om) navierstokes_bang(x);
}


void navierstokes_r (navierstokes *x, double echo)
{
	x->r = (double)echo;
//	x->rinit = (double)echo;
	if (x->om) navierstokes_bang(x);
}

void navierstokes_b (navierstokes *x, double echo)
{
	x->b = (double)echo;
//	x->binit = (double)echo;
	if (x->om) navierstokes_bang(x);
}

void navierstokes_c (navierstokes *x, double echo)
{
	x->c = (double)echo;
//	x->cinit = (double)echo;
	if (x->om) navierstokes_bang(x);
}

void navierstokes_d (navierstokes *x, double echo)
{
	x->d = (double)echo;
//	x->nxinit = (double)echo;
	if (x->om) navierstokes_bang(x);
}


void navierstokes_dt (navierstokes *x, double echo)
{
	x->dt = (double)echo;
//	x->dtinit = (double)echo;
	if (x->om) navierstokes_bang(x);
}


void navierstokes_bang (navierstokes *x) //important, output _ten_ calc
{

	outlet_float(x->c_out[4], x->e); 
	outlet_float(x->c_out[3], x->d); 
	outlet_float(x->c_out[2], x->c); 
	outlet_float(x->c_out[1], x->b); 
	outlet_float(x->c_out[0], x->a); 

    navierstokes_calc(x);   //next
    
}



void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&navierstokes_class,(method)navierstokes_new,0L,(short)sizeof(navierstokes),0L,
 A_GIMME,0);

 addbang((method)navierstokes_bang);
 addmess((method)navierstokes_set,"set", A_GIMME, 0);
 addmess((method)navierstokes_reset,"reset", A_GIMME, 0);

 addmess((method)navierstokes_dt,"dt", A_DEFFLOAT, 0);
 addmess((method)navierstokes_a,"a", A_DEFFLOAT, 0);
 addmess((method)navierstokes_r,"r", A_DEFFLOAT, 0);
 addmess((method)navierstokes_c,"c", A_DEFFLOAT, 0);
 addmess((method)navierstokes_b,"b", A_DEFFLOAT, 0);
 addmess((method)navierstokes_c,"c", A_DEFFLOAT, 0);
 addmess((method)navierstokes_d,"d", A_DEFFLOAT, 0);
 addmess((method)navierstokes_om,"om", A_DEFLONG, 0);

 addmess((method)navierstokes_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-navier-stokes  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void navierstokes_assist(navierstokes *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"ping, (int), (float), ·, ¶ª"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) z  l o r e n t z   e         c            h              o"); }
//    else if (m==2&&a==1) { sprintf(s,"(lisy)       e             c                h                   o"); }
}


