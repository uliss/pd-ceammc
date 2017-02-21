/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - lorenze © andrŽ sier 20030914
		
		lorenze chaos by euler method
		faster / no accuracy
		

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"

//#include "math.h"


#define lorx(x, y, a) 		(a) * ((y) - (x));		
#define lory(x, y, z, a) 	((x) * ((r) - (z))) - (y);
#define lorz(x, y, z, c) 	((x) * (y)) - ((z) * (c));


typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		// value
	void 		        *c_out2;		// value
	void 		        *c_out3;		// value
	
	double		a, r, c, nx, ny, nz,dt; //eq variables
	double		ainit, rinit, cinit, nxinit, nyinit, nzinit,dtinit; //init eq variables

	Boolean om;
} lorenze;


void *lorenze_new (Symbol *msg, short argc, Atom *argv);
void lorenze_bang (lorenze *x);

//user variables
void lorenze_a (lorenze *x, double echo);
void lorenze_r (lorenze *x, double echo);
void lorenze_c (lorenze *x, double echo);
void lorenze_nx (lorenze *x, double echo);
void lorenze_ny (lorenze *x, double echo);
void lorenze_nz (lorenze *x, double echo);
void lorenze_dt (lorenze *x, double echo);
void lorenze_param (lorenze *x, double a, double reynolds, double beta, double dt);

//int, double -> dt
void lorenze_echo (lorenze *x, double echo);
void lorenze_int(lorenze *x, int n);
void lorenze_float(lorenze *x, double n);

//calc
void lorenze_calc (lorenze *x);
void lorenze_reset (lorenze *x, Symbol *msg, short argc, Atom *argv); 
void lorenze_set (lorenze *x, Symbol *msg, short argc, Atom *argv) ; 

void lorenze_assist(lorenze *x, void *b, long m, long a, char *s);
void *lorenze_class;


void *lorenze_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 lorenze *x;
	 int i;
	 
	 x=(lorenze *)newobject(lorenze_class);

	 x->c_out3=floatout(x);
	 x->c_out2=floatout(x);
	 x->c_out=floatout(x);

	 //classic lorenze
		 x->a=10.0;
		 x->r=28.0; 
		 x->c= 2.67; 
		 x->nx=0.1; 
		 x->ny=0.1; 
		 x->nz=0.1;
		 x->dt=0.01;

		 x->ainit=10.0;
		 x->rinit=28.0; 
		 x->cinit= 2.67; 
		 x->nxinit=0.1; 
		 x->nyinit=0.1; 
		 x->nzinit=0.1;
		 x->dtinit=0.01;
		 
		 x->om = 0;

	lorenze_set(x, msg, argc, argv);

	 return(x);	
}

void lorenze_om(lorenze *x, int n)     { x->om = (n>0);}


void lorenze_set (lorenze *x, Symbol *msg, short ac, Atom *av) //input the args 
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
				x->cinit = (double)av[5].a_w.w_long;
			else if (av[5].a_type == A_FLOAT)
				x->cinit = av[5].a_w.w_float;
			x->c = x->cinit;
		}
		if (ac > 4) {
			if (av[4].a_type == A_LONG)
				x->rinit = (double)av[4].a_w.w_long;
			else if (av[4].a_type == A_FLOAT)
				x->rinit = av[4].a_w.w_float;
			x->r = x->rinit;
		}
		if (ac > 3) {
			if (av[3].a_type == A_LONG)
				x->ainit = (double)av[3].a_w.w_long;
			else if (av[3].a_type == A_FLOAT)
				x->ainit = av[3].a_w.w_float;
			x->a = x->ainit;
		}
		if (ac > 2) {
			if (av[2].a_type == A_LONG)
				x->nzinit = (double)av[2].a_w.w_long;
			else if (av[2].a_type == A_FLOAT)
				x->nzinit = av[2].a_w.w_float;
			x->nz = x->nzinit;
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

void lorenze_reset (lorenze *x, Symbol *msg, short argc, Atom *argv)
{
	x->nx = x->nxinit;
	x->ny = x->nyinit;
	x->nz = x->nzinit;
	x->a = x->ainit;
	x->r = x->rinit;
	x->c = x->cinit;
	x->dt = x->dtinit;

}



void  lorenze_calc (lorenze *x)
{
	double	a, r, c, dt, x0, y0, z0;
	double	dx, dy, dz;
	
	//local copies
	a = x->a;
	r = x->r;
	c = x->c;
	dt = x->dt;
	x0 = x->nx;
	y0 = x->ny;
	z0 = x->nz;


	dx = ((y0*a)-(x0*a))*dt;
	dy = ((x0*r)-y0-(x0*z0))*dt;
	dz = ((x0*y0)-(z0*c))*dt;
	
	x->nx += dx;
	x->ny += dy;
	x->nz += dz;
}

void lorenze_echo (lorenze *x, double echo)
{
 	x->dt = echo;
// 	lorenze_bang(x); //prevent increase in speed if undesired
}

void lorenze_int(lorenze *x, int n)     { lorenze_echo(x, n*0.01);}
void lorenze_float(lorenze *x, double n) { lorenze_echo(x, n);}



void lorenze_a (lorenze *x, double echo)
{
	x->a = (double)echo;
//	x->ainit = (double)echo;
	if (x->om) lorenze_bang(x);
}


void lorenze_r (lorenze *x, double echo)
{
	x->r = (double)echo;
//	x->rinit = (double)echo;
	if (x->om) lorenze_bang(x);
}

void lorenze_c (lorenze *x, double echo)
{
	x->c = (double)echo;
//	x->cinit = (double)echo;
	if (x->om) lorenze_bang(x);
}

void lorenze_nx (lorenze *x, double echo)
{
	x->nx = (double)echo;
//	x->nxinit = (double)echo;
	if (x->om) lorenze_bang(x);
}

void lorenze_ny (lorenze *x, double echo)
{
	x->ny = (double)echo;
//	x->nyinit = (double)echo;
	if (x->om) lorenze_bang(x);
}

void lorenze_nz (lorenze *x, double echo)
{
	x->nz = (double)echo;
//	x->nzinit = (double)echo;
	if (x->om) lorenze_bang(x);
}

void lorenze_dt (lorenze *x, double echo)
{
	x->dt = (double)echo;
//	x->dtinit = (double)echo;
	if (x->om) lorenze_bang(x);
}


void lorenze_bang (lorenze *x)
{

	outlet_float(x->c_out3, x->nz); 
	outlet_float(x->c_out2, x->ny); 
	outlet_float(x->c_out, x->nx); 

    lorenze_calc(x);   //next
    
}



void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&lorenze_class,(method)lorenze_new,0L,(short)sizeof(lorenze),0L,
 A_GIMME,0);

 addbang((method)lorenze_bang);
 addmess((method)lorenze_set,"set", A_GIMME, 0);
 addmess((method)lorenze_reset,"reset", A_GIMME, 0);

 addmess((method)lorenze_dt,"dt", A_DEFFLOAT, 0);
 addmess((method)lorenze_a,"a", A_DEFFLOAT, 0);
 addmess((method)lorenze_r,"r", A_DEFFLOAT, 0);
 addmess((method)lorenze_c,"c", A_DEFFLOAT, 0);
 addmess((method)lorenze_nx,"x", A_DEFFLOAT, 0);
 addmess((method)lorenze_ny,"y", A_DEFFLOAT, 0);
 addmess((method)lorenze_nz,"z", A_DEFFLOAT, 0);
 addint((method)lorenze_int);
 addfloat((method)lorenze_float);
 addmess((method)lorenze_om,"om", A_DEFLONG, 0);

 addmess((method)lorenze_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-lorenz.e  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void lorenze_assist(lorenze *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float), ·, ¶ª"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  l o r e n t z   e         c            h              o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) z  l o r e n t z   e         c            h              o"); }
//    else if (m==2&&a==1) { sprintf(s,"(lisy)       e             c                h                   o"); }
}


