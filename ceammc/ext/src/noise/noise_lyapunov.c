/*

————————————————————————————————————————————————————————————————
a-lyapunov - © andré sier 2004
	
inspired by Paul Bourke, implementation by André Sier	
		
lyapunov random attractors

Paul Bourke's words http://astronomy.swin.edu.au/%7Epbourke/fractals/lyapunov/

On average 98% of the random selections of (an, bn) result in an infinite series. 
This is so common because of the range (-2 <= a,b <= 2) for each of the parameters 
a and b, the number of infinite cases will reduce greatly with a smaller range. 
About 1% were point attractors, and about 0.5% were periodic basins of attraction.


xn+1 = a0 + a1 xn + a2 xn2 + a3 xn yn + a4 yn + a5 yn2 
 yn+1 = b0 + b1 xn + b2 xn2 + b3 xn yn + b4 yn + b5 yn2 



References

 Berge, P., Pomeau, Y., Vidal, C.,
 Order Within Chaos, Wiley, New York, 1984.

 Crutchfield, J., Farmer, J., Packard, N.
 Chaos, Scientific American, 1986, 255, 46-47

 Das, A., Das, Pritha, Roy, A
 Applicability of Lyapunov Exponent in EEG data analysis. Complexity International, draft manuscript.

 Devaney, R.
 An Introduction to Choatic Dynamical Systems, Addison-Wesley, 1989

 Feigenbaum, M.,
 Universal behavior in Nonlinear Systems, Los Alamos Science, 1981

 Peitgen, H., Jurgens, H., Saupe, D
 Lyapunov exponents and chaotic attractors in Choas and fractals - new frontiers of science. Springer, new York, 1992.
————————————————————————————————————————————————————————————————
*/

#include "ext.h"
#include "ext_common.h"

//#include <math.h>



typedef struct	_lyapunov
{
	t_object 	        c_ob;			
	void 		        *c_out, *c_out2;		// outlets	
	double 				a[6], b[6], nx, ny;
	double 				ainit[6], binit[6], nxinit, nyinit;
	Boolean om;
} lyapunov;



void *lyapunov_new (Symbol *msg, short argc, Atom *argv);
void lyapunov_calc (lyapunov *x); 
void lyapunov_bang (lyapunov *x); 
void lyapunov_set (lyapunov *x, Symbol *msg, short argc, Atom *argv);  
void lyapunov_reset(lyapunov *x, Symbol *msg, short argc, Atom *argv);
void lyapunov_info(lyapunov *x, Symbol *msg, short argc, Atom *argv);

void lyapunov_a (lyapunov *x, Symbol *msg, short argc, Atom *argv);
void lyapunov_b (lyapunov *x, Symbol *msg, short argc, Atom *argv);
void lyapunov_nx (lyapunov *x, double max);
void lyapunov_ny (lyapunov *x, double max);
void lyapunov_om (lyapunov *x, long max);

void lyapunov_assist(lyapunov *x, void *b, long m, long a, char *s);
void *lyapunov_class;




void *lyapunov_new (Symbol *msg, short argc, Atom *argv) 
{
 lyapunov *x;
 int i;
 
 x=(lyapunov *)newobject(lyapunov_class);

 x->c_out2=floatout(x);
 x->c_out=floatout(x);

 //init calculated at random (of course with a max patch:)
	x->a[0] = -1.4f;
	x->ainit[0] = -1.4f;
	x->a[1] = 0.7f;
	x->ainit[1] = 0.7f;
	x->a[2] = -0.16f;
	x->ainit[2] = -0.16f;
	x->a[3] = -1.21f;
	x->ainit[3] =  -1.21f;
	x->a[4] = 1.11f;
	x->ainit[4] = 1.11f;
	x->a[5] = -1.46f;
	x->ainit[5] = -1.46f;

	x->b[0] = -0.19f;
	x->binit[0] = -0.19f;
	x->b[1] = 0.04f;
	x->binit[1] = 0.04f;
	x->b[2] = 1.59f;
	x->binit[2] = 1.59f;
	x->b[3] = 1.3f;
	x->binit[3] = 1.3f;
	x->b[4] = 1.68f;
	x->binit[4] = 1.68f;
	x->b[5] = 1.76f;
	x->binit[5] = 1.76f;
	
	x->nx = 0.0f;
	x->nxinit = 0.0f;
	x->ny = 0.0f;
	x->nyinit = 0.0f;

	x->om = 0;
	
	if (argc)
	 lyapunov_set(x, msg, argc, argv);
 	return(x);	
}
void lyapunov_om(lyapunov *x, long n)     { x->om = (n>0);}

void lyapunov_set (lyapunov *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	if (argc) {

		if (argc>13) {
			if (argv[13].a_type == A_LONG)
				x->nyinit = (double) argv[13].a_w.w_long;
			else if (argv[13].a_type == A_FLOAT)
				x->nyinit = (double) argv[13].a_w.w_float;
			x->ny = x->nyinit;
		}
		if (argc>12) {
			if (argv[12].a_type == A_LONG)
				x->nxinit = (double) argv[12].a_w.w_long;
			else if (argv[12].a_type == A_FLOAT)
				x->nxinit = (double) argv[12].a_w.w_float;
			x->nx = x->nxinit;
		}

		if (argc>11) {
			if (argv[11].a_type == A_LONG)
				x->binit[5] = (double) argv[11].a_w.w_long;
			else if (argv[11].a_type == A_FLOAT)
				x->binit[5] = (double) argv[11].a_w.w_float;
			x->b[5] = x->binit[5];
		}

		if (argc>10) {
			if (argv[10].a_type == A_LONG)
				x->binit[4] = (double) argv[10].a_w.w_long;
			else if (argv[10].a_type == A_FLOAT)
				x->binit[4] = (double) argv[10].a_w.w_float;
			x->b[4] = x->binit[4];
		}

		if (argc>9) {
			if (argv[9].a_type == A_LONG)
				x->binit[3] = (double) argv[9].a_w.w_long;
			else if (argv[9].a_type == A_FLOAT)
				x->binit[3] = (double) argv[9].a_w.w_float;
			x->b[3] = x->binit[3];
		}

		if (argc>8) {
			if (argv[8].a_type == A_LONG)
				x->binit[2] = (double) argv[8].a_w.w_long;
			else if (argv[8].a_type == A_FLOAT)
				x->binit[2] = (double) argv[8].a_w.w_float;
			x->b[2] = x->binit[2];
		}

		if (argc>7) {
			if (argv[7].a_type == A_LONG)
				x->binit[1] = (double) argv[7].a_w.w_long;
			else if (argv[7].a_type == A_FLOAT)
				x->binit[1] = (double) argv[7].a_w.w_float;
			x->b[1] = x->binit[1];
		}

		if (argc>6) {
			if (argv[6].a_type == A_LONG)
				x->binit[0] = (double) argv[6].a_w.w_long;
			else if (argv[6].a_type == A_FLOAT)
				x->binit[0] = (double) argv[6].a_w.w_float;
			x->b[0] = x->binit[0];
		}


		if (argc>5) {
			if (argv[5].a_type == A_LONG)
				x->ainit[5] = (double) argv[5].a_w.w_long;
			else if (argv[5].a_type == A_FLOAT)
				x->ainit[5] = (double) argv[5].a_w.w_float;
			x->ainit[5] = x->ainit[5];
		}

		if (argc>4) {
			if (argv[4].a_type == A_LONG)
				x->ainit[4] = (double) argv[4].a_w.w_long;
			else if (argv[4].a_type == A_FLOAT)
				x->ainit[4] = (double) argv[4].a_w.w_float;
			x->a[4] = x->ainit[4];
		}

		if (argc>3) {
			if (argv[3].a_type == A_LONG)
				x->ainit[3] = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->ainit[3] = (double) argv[3].a_w.w_float;
			x->a[3] = x->ainit[3];
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->ainit[2] = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->ainit[2] = (double) argv[2].a_w.w_float;
			x->a[2] = x->ainit[2];
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->ainit[1] = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->ainit[1] = (double) argv[1].a_w.w_float;
			x->a[1] = x->ainit[1];
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->ainit[0] = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->ainit[0] = (double) argv[0].a_w.w_float;
			x->a[0] = x->ainit[0];
		}

	} //end if args
}

void  lyapunov_calc (lyapunov *x)
{
	double nx,ny;
	double a[6], b[6];
	
	a[0] = x->a[0];
	a[1] = x->a[1];
	a[2] = x->a[2];
	a[3] = x->a[3];
	a[4] = x->a[4];
	a[5] = x->a[5];
	b[5] = x->b[5];
	b[4] = x->b[4];
	b[3] = x->b[3];
	b[2] = x->b[2];
	b[1] = x->b[1];
	b[0] = x->b[0];
	nx = x->nx;
	ny = x->ny;

	// maybe lyapunov attractor : quadratic eq

	x->nx = a[0] + a[1]*nx + a[2]*nx*nx + a[3]*nx*ny + a[4]*ny + a[5]*ny*ny;
	x->ny = b[0] + b[1]*nx + b[2]*nx*nx + b[3]*nx*ny + b[4]*ny + b[5]*ny*ny;
}

void lyapunov_bang (lyapunov *x)  //important, first output values then calc next
{	
	outlet_float (x->c_out2, x->nx);
	outlet_float (x->c_out, x->ny);
	lyapunov_calc(x);	
}

void lyapunov_a (lyapunov *x, Symbol *msg, short argc, Atom *argv) 
{
		if (argc>5) {
			if (argv[5].a_type == A_LONG)
				x->ainit[5] = (double) argv[5].a_w.w_long;
			else if (argv[5].a_type == A_FLOAT)
				x->ainit[5] = (double) argv[5].a_w.w_float;
			x->ainit[5] = x->ainit[5];
		}

		if (argc>4) {
			if (argv[4].a_type == A_LONG)
				x->ainit[4] = (double) argv[4].a_w.w_long;
			else if (argv[4].a_type == A_FLOAT)
				x->ainit[4] = (double) argv[4].a_w.w_float;
			x->a[4] = x->ainit[4];
		}

		if (argc>3) {
			if (argv[3].a_type == A_LONG)
				x->ainit[3] = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->ainit[3] = (double) argv[3].a_w.w_float;
			x->a[3] = x->ainit[3];
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->ainit[2] = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->ainit[2] = (double) argv[2].a_w.w_float;
			x->a[2] = x->ainit[2];
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->ainit[1] = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->ainit[1] = (double) argv[1].a_w.w_float;
			x->a[1] = x->ainit[1];
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->ainit[0] = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->ainit[0] = (double) argv[0].a_w.w_float;
			x->a[0] = x->ainit[0];
		}

	if (x->om) lyapunov_bang(x);

} 

void lyapunov_b (lyapunov *x, Symbol *msg, short argc, Atom *argv) 
{
		if (argc>5) {
			if (argv[5].a_type == A_LONG)
				x->binit[5] = (double) argv[5].a_w.w_long;
			else if (argv[5].a_type == A_FLOAT)
				x->binit[5] = (double) argv[5].a_w.w_float;
			x->binit[5] = x->binit[5];
		}

		if (argc>4) {
			if (argv[4].a_type == A_LONG)
				x->binit[4] = (double) argv[4].a_w.w_long;
			else if (argv[4].a_type == A_FLOAT)
				x->binit[4] = (double) argv[4].a_w.w_float;
			x->b[4] = x->binit[4];
		}

		if (argc>3) {
			if (argv[3].a_type == A_LONG)
				x->binit[3] = (double) argv[3].a_w.w_long;
			else if (argv[3].a_type == A_FLOAT)
				x->binit[3] = (double) argv[3].a_w.w_float;
			x->b[3] = x->binit[3];
		}

		if (argc>2) {
			if (argv[2].a_type == A_LONG)
				x->binit[2] = (double) argv[2].a_w.w_long;
			else if (argv[2].a_type == A_FLOAT)
				x->binit[2] = (double) argv[2].a_w.w_float;
			x->b[2] = x->binit[2];
		}

		if (argc>1) {
			if (argv[1].a_type == A_LONG)
				x->binit[1] = (double) argv[1].a_w.w_long;
			else if (argv[1].a_type == A_FLOAT)
				x->binit[1] = (double) argv[1].a_w.w_float;
			x->b[1] = x->binit[1];
		}

		if (argc>0) {
			if (argv[0].a_type == A_LONG)
				x->binit[0] = (double) argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				x->binit[0] = (double) argv[0].a_w.w_float;
			x->b[0] = x->binit[0];
		}

	if (x->om) lyapunov_bang(x);

} 

void lyapunov_nx (lyapunov *x, double max)
{
	x->nx = max;
//	x->nxinit = max;
	if (x->om) lyapunov_bang(x);
}
void lyapunov_ny (lyapunov *x, double max)
{
	x->ny = max;
//	x->nyinit = max;
	if (x->om) lyapunov_bang(x);
}

void lyapunov_reset (lyapunov *x, Symbol *msg, short argc, Atom *argv)
{
	short i;
	
	for (i=0; i<6; i++)
		x->a[i] = x->ainit[i];
	for (i=0; i<6; i++)
		x->b[i] = x->binit[i];

	x->nx = x->nxinit;
	x->ny = x->nyinit;
}

void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&lyapunov_class,(method)lyapunov_new,0L,(short)sizeof(lyapunov),0L,
 A_GIMME,0);
 
 addbang((method)lyapunov_bang);
 addmess((method)lyapunov_reset, "reset", A_GIMME, 0);
 addmess((method)lyapunov_set, "set", A_GIMME, 0);
 addmess((method)lyapunov_a, "a", A_GIMME, 0);
 addmess((method)lyapunov_b, "b", A_GIMME, 0);
 addmess((method)lyapunov_nx, "x", A_DEFFLOAT, 0);
 addmess((method)lyapunov_ny, "y", A_DEFFLOAT, 0);
 addmess((method)lyapunov_om, "om", A_DEFLONG, 0);
 addmess((method)lyapunov_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-lyapunov  " __DATE__" "__TIME__"                                   ©   a n d r é s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void lyapunov_assist(lyapunov *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"echo e   c  h   o"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x lyapunov     e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  lyapunov     e             c                h                   o"); }
}


